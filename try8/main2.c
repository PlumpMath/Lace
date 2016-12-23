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
    float data[] = {
        -0.5, 0.5,
        0.5, 0.5,
        0.0, -0.5
    };

    // The vertex buffer object maps C array of floats to a shader input variables
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // More data to render
    float data2[] = {
        -0.35, 0.5,
        0.75, 0.75,
        0.0, -0.5
    };
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao2;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);
    
    GLuint vbo2;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data2), data2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    int run = 1;
    while(run) {         
        // Render Loop
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glBindVertexArray(vao2);
        glDrawArrays(GL_LINE_STRIP, 0, 3);

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
