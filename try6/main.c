#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "glad.h"
#include <SDL.h>
#include "hotrod.h"
#include "files.h"

#define OK(error) if((error) != 0) { fprintf(stderr, "Error: %s\n", SDL_GetError()); exit(1); }

int handle_events();

float *tri = NULL;
int tri_count = 6;

void load_model() {
    if(tri) {
        free(tri);
    }
    tri = malloc(sizeof(float) * 5 * 512); // max nr of tris is 512
    tri_count = 0;
    
    char *data = file_read_all("data.txt");
    char *line = data;
    while(1) {
        float x, y, r, g, b;
        int read = sscanf(line, "%f %f %f %f %f", &x, &y, &r, &g, &b);
        if(read == 5) {
            //printf("Read (%+.2f, %+.2f, %+.2f, %+.2f, %+.2f)\n", x, y, r, g, b);
            tri[tri_count * 5 + 0] = x;
            tri[tri_count * 5 + 1] = y;
            tri[tri_count * 5 + 2] = r;
            tri[tri_count * 5 + 3] = g;
            tri[tri_count * 5 + 4] = b;
            tri_count++;
        }
        // Find end of line
        char *i = strchr(line, '\n'); 
        if(i == NULL) {
            break;
        } else {
            int line_length = i - line;
            line += line_length + 1;
        }
    }
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

int main() {
    load_model();
    for(int i = 0; i < tri_count; i++) {
        float x = tri[i * 5 + 0];
        float y = tri[i * 5 + 1];
        float r = tri[i * 5 + 2];
        float g = tri[i * 5 + 3];
        float b = tri[i * 5 + 4];
    }
    
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
  
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tri_count * 5, tri, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    //hotrod_extra_includes = (char*[]) { , NULL };
    
    while(!handle_events()) {
        HOTROD(flop);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, tri_count);
        SDL_GL_SwapWindow(win);
        SDL_Delay(30);
    }

    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(win);
    SDL_Quit();
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
            }
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                return 1;
            }
            else if(event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                
            }
            break;
        }
    }
    return 0;
}
