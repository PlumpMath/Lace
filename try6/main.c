#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glad/glad.h>
#include <SDL.h>
#include "hotrod.h"

#define OK(error) if((error) != 0) { fprintf(stderr, "Error: %s\n", SDL_GetError()); exit(1); }

int handle_events();

float tri[] = { -0.8,  0.6,    1, 0, 0,
                0.7,  0.0,    1, 1, 1,
                -0.8, -0.6,    1, 1, 0, };

const char *vertex_shader_src = ""
    "#version 330 core\n"
    "layout (location = 0) in vec2 pos;"
    "layout (location = 1) in vec3 col;"
    "out vec3 passed_color;"
    "void main() {"
    "  passed_color = col;"
    "  gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);"
    "}";

const char *fragment_shader_src = ""
    "#version 330 core\n"
    "in vec3 passed_color;"
    "out vec4 color;"
    "void main() {"
    "  color = vec4(passed_color.rgb * vec3(0.0, 1.0, 2.0), 1.0);"
    "}";

void check_shader(GLuint shader, const char *shader_name) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLchar info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("Error in %s: %s\n", shader_name, info_log);
    }
}

int main() {
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
    glClearColor(0.95, 0.9, 0.95, 1.0);
  
    GLuint vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader_src, NULL);
    glCompileShader(vs);
    check_shader(vs, "vs");
  
    GLuint fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader_src, NULL);
    glCompileShader(fs);
    check_shader(fs, "fs");

    GLuint program;
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

    glUseProgram(program);
  
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
  
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    while(!handle_events()) {
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
        case SDL_KEYDOWN:
            if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                return 1;
            }
            break;
        }
    }
    return 0;
}
