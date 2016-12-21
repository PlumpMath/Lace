#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "glad.h"
#include <SDL.h>
#define HUNTER_ENABLED
#include "hunter.h"
#include "hotrod.h"
#include "files.h"
#define GB_MATH_IMPLEMENTATION
#include "gb_math.h"

#define OK(error) if((error) != 0) { fprintf(stderr, "Error: %s\n", SDL_GetError()); exit(1); }

int handle_events();

float *tri = NULL;
int tri_count = 6;
GLuint vbo;

void load_model() {
    if(tri) {
        free(tri);
    }
    tri = malloc(sizeof(float) * 6 * 512); // max nr of tris is 512
    tri_count = 0;
    
    char *data = file_read_all("data.txt");
    char *linep = data;
    while(1) {
        // Find end of line
        char *i = strchr(linep, '\n'); 
        if(i == NULL) {
            break;
        }

        int line_length = i - linep;

        // Create a substring with only the current line
        char *line = malloc(line_length);
        strncpy(line, linep, line_length);
        
        float x, y, z, r, g, b;
        int read = sscanf(line, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);

        // Only create a triangle if all five data points could be read
        if(read == 6) {
            printf("Read (%+.2f, %+.2f, %+.2f, %+.2f, %+.2f, %+.2f)\n", x, y, z, r, g, b);
            tri[tri_count * 5 + 0] = x;
            tri[tri_count * 5 + 1] = y;
            tri[tri_count * 5 + 2] = z;
            tri[tri_count * 5 + 3] = r;
            tri[tri_count * 5 + 4] = g;
            tri[tri_count * 5 + 5] = b;
            tri_count++;
        } else {
            //printf("Skipping line: %s\n", line);
        }

        linep += line_length + 1;
        free(line);
    }

    free(data);
}

void check_shader(GLuint shader, const char *shader_name) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("Error in %s: %s\n", shader_name, info_log);
    }
}

GLuint program;

void load_shader_program() {
    const char *vert_src = file_read_all("shader.vert");
    const char *frag_src = file_read_all("shader.frag");
    
    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vert_src, NULL);
    glCompileShader(vs);
    check_shader(vs, "vs");
  
    GLuint fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &frag_src, NULL);
    glCompileShader(fs);
    check_shader(fs, "fs");

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        GLchar info_log[512];
        glGetProgramInfoLog(program, 512, NULL, info_log);
        printf("Linker error: %s\n", info_log);
    }
}

void print_mat4(const gbMat4 *m) {
    for(int i = 0; i < 4; i++) {
        printf("%f %f %f %f\n", m->col[i].x, m->col[i].y, m->col[i].z, m->col[i].w);
    }
}

gbMat4 transform;

int main() {
    load_model();
    
    OK(SDL_Init(SDL_INIT_EVERYTHING));

    SDL_Window *win;
    SDL_Renderer *rend;
    int window_flags = SDL_WINDOW_OPENGL;
    OK(SDL_CreateWindowAndRenderer(512, 512, window_flags, &win, &rend));

    // Must set this before creating the context
    // On OSX it seems like we don't need to set SDL_GL_CONTEXT_MAJOR_VERSION
    // and SDL_GL_CONTEXT_MINOR_VERSION ..?! Only SDL_GL_CONTEXT_PROFILE_CORE.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GLContext glcontext = SDL_GL_CreateContext(win);
    
    if(!gladLoadGL()) {
        printf("Something went wrong when loading glad.\n");
        exit(-1);
    }
    
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));
    
    /* int major_version, minor_version; */
    /* SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version); */
    /* SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version); */
    /* printf("GL version according to SDL..? %d.%d\n", major_version, minor_version); */

    // GL Actual Work
    load_shader_program();
    glUseProgram(program);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
  
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tri_count * 6, tri, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    gbMat4 id;
    gb_mat4_identity(&id);
    transform = id;

    GLint transformLoc = glGetUniformLocation(program, "transform");
    assert(transformLoc > -1);

    GLint projectionLoc = glGetUniformLocation(program, "projection");
    assert(projectionLoc > -1);

    GLint camLoc = glGetUniformLocation(program, "camera");
    assert(camLoc > -1);

    gbMat4 projection;
    gbMat4 camera;

    gbVec3 eye = { 0, 0, -2 };
    gbVec3 centre = { 0, 0, 0 };
    gbVec3 up = { 0, 1, 0 };
    
    gb_mat4_look_at(&camera, eye, centre, up);

    glEnable(GL_DEPTH_TEST);

    // RENDER LOOP
    while(!handle_events()) {
        HOTROD(flop, &projection);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform.e);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.e);
        glUniformMatrix4fv(camLoc, 1, GL_FALSE, camera.e);
        glDrawArrays(GL_TRIANGLES, 0, tri_count);
        SDL_GL_SwapWindow(win);
        SDL_Delay(30);
    }

    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(win);
    SDL_Quit();

    free(tri);
    hunter_print_allocations();
}

int handle_events() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            return 1;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                printf("Reloading shader program.\n");
                load_shader_program();
                glUseProgram(program);
                printf("Reloading model.\n");
                load_model();
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tri_count * 5, tri, GL_STATIC_DRAW);
            }
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                return 1;
            }
            else if(event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                HOTROD(on_enter);
                //print_mat4(&transform);
            }
            break;
        }
    }
    return 0;
}
