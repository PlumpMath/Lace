#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "glap.h"
#include "hotrod.h"
#include "files.h"

#define GB_MATH_IMPLEMENTATION
#include "gb_math.h"

GLuint program;

int main() {    
    Glap glap = glap_start();
   
    // GL Actual Work
    program = load_shader_program("shader.vert", "shader.frag");
    glUseProgram(program);

    // All programs need at least one Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Data to render
    int n = 20;
    float step = 2.0 / (float)n;
    int vert_count = n * n;
    int vert_buffer_size = sizeof(float) * vert_count * 2;
    float *data = malloc(vert_buffer_size);

    int i = 0;
    for(float y = -1; y <= 1; y += step) {
        for(float x = -1; x <= 1; x += step) {
            data[i++] = x + step/2;
            data[i++] = y + step/2;
        }
    }

    // The vertex buffer object maps C array of floats to a shader input variables
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_buffer_size, data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glPointSize(5.0);
    
    int run = 1;
    while(run) {         
        // Render Loop
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, vert_count);     

        // Show!
        SDL_GL_SwapWindow(glap.window);
        SDL_Delay(30);

        int error = glGetError();
        if(error) {
            printf("GL Error: %d\n", error);
            exit(1);
        }
        
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                run = 0;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    printf("Reloading shader program.\n");
                    program = load_shader_program("shader.vert", "shader.frag");
                    glUseProgram(program);
                }
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    run = 0;
                }
                else if(event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                
                }
                break;
            }
        }
    }

    glap_stop(glap);
}
