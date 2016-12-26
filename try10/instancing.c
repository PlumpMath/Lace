#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "glap.h"
#include "files.h"
#include "array.h"

#define GB_MATH_IMPLEMENTATION
#include "gb_math.h"

GLuint program;

typedef struct {
    float x, y, z;
} Vert;

int main() {    
    Glap glap = glap_start();
   
    // GL Actual Work
    program = load_shader_program("shader.vert", "shader.frag");
    glUseProgram(program);

    // All programs need at least one Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Data
    Vert verts[] = {
        // front
        (Vert) { -0.5, -0.5, 0.5 },
        (Vert) { 0.5, -0.5, 0.5 },
        (Vert) { 0.5, 0.5, 0.5 },
        (Vert) { -0.5, 0.5, 0.5 },
        // back
        (Vert) { -0.5, -0.5, -0.5 },
        (Vert) { 0.5, -0.5, -0.5 },
        (Vert) { 0.5, 0.5, -0.5 },
        (Vert) { -0.5, 0.5, -0.5 },
    };
    int vert_count = sizeof(verts) / sizeof(Vert);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_count * sizeof(Vert), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Element indexes
    int indexes[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Right face
        5, 1, 2,
        6, 5, 2,
        // Left face
        0, 3, 4,
        4, 3, 7,
        // Back face
        4, 5, 6,
        4, 6, 7,
        // Top face
        2, 3, 6,
        3, 6, 7
    };
    
    GLuint elementsBuffer;
    glGenBuffers(1, &elementsBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    gbVec3 eye = { 0, 10.0, 15.0 };

    GLint cameraPerspLoc = glGetUniformLocation(program, "cameraPersp");
    assert(cameraPerspLoc > -1);

    GLint transformLoc = glGetUniformLocation(program, "transform");
    assert(transformLoc > -1);

    glEnable(GL_DEPTH_TEST);

    int run = 1;
    while(run) {
        float t = SDL_GetTicks() / 1000.0f;

        eye = (gbVec3) { eye.x, eye.y + t * 0.01, eye.z + t * 0.005 };
        
        // Draw
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        gbMat4 camera;
        gb_mat4_look_at(&camera, eye, (gbVec3) { 0, 0, 0}, (gbVec3) { 0, 1, 0 });
    
        gbMat4 perspective;
        gb_mat4_perspective(&perspective, 45, 1.0, 0.001, 10000);

        gbMat4 objTransform;
        gb_mat4_rotate(&objTransform, (gbVec3) { 0.0, 1.0, 0.0 }, t);

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, objTransform.e);

        gbMat4 cameraPersp; // persp * cam
        gb_mat4_mul(&cameraPersp, &perspective, &camera);
        
        glUniformMatrix4fv(cameraPerspLoc, 1, GL_FALSE, cameraPersp.e);
        
        //glDrawArrays(GL_TRIANGLES, 0, vert_count);
        glDrawElementsInstanced(GL_TRIANGLES, sizeof(indexes) / sizeof(int), GL_UNSIGNED_INT, 0, 10000);

        // Swap frame buffer
        SDL_GL_SwapWindow(glap.window);

        // Check for error
        int error = glGetError();
        if(error) {
            printf("GL Error: %d\n", error);
            exit(1);
        }
        
        SDL_Delay(30);
        
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                run = 0;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    //printf("Reloading shader program.\n");
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
