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

float height_at(float x, float z) {
    return sinf(x * 5) * 0.1 + x * cos(x * 100 + z * 10) * 0.3 + sinf(z * 5) * 0.2;
    /* if(x > 0.1 && z > 0.1) { */
    /*     return 0.7; */
    /* } */
    /* else if(x < -0.2) { */
    /*     return 0.1; */
    /* } */
    /* else { */
    /*     return 0.3; */
    /* } */
}

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
    #define MAX_VERT_COUNT 99999
    Vert *verts = malloc(sizeof(Vert) * MAX_VERT_COUNT);
    int vert_count = 0;

    float w = 0.1;
    for(float z = -0.8; z <= 0.8 - w; z += w) {
        for(float x = -0.8; x <= 0.8 - w; x += w) {

            float y0 = height_at(x, z);
            float y1 = height_at(x + w, z);
            float y2 = height_at(x, z + w);
            float y3 = height_at(x + w, z + w);
            
            verts[vert_count++] =  (Vert) { x, y0, z, };
            verts[vert_count++] =  (Vert) { x + w, y1, z };
            verts[vert_count++] =  (Vert) { x, y2, z + w };

            verts[vert_count++] =  (Vert) { x + w, y1, z };
            verts[vert_count++] =  (Vert) { x + w, y3, z + w };
            verts[vert_count++] =  (Vert) { x, y2, z + w };

            assert(vert_count < MAX_VERT_COUNT);
        }
    }

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vert_count * sizeof(Vert), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    gbVec3 eye = { 0, 1.0, 1.5 };
    
    gbMat4 camera;
    gb_mat4_look_at(&camera, eye, (gbVec3) { 0, 0, 0}, (gbVec3) { 0, 1, 0 });
    
    gbMat4 perspective;
    gb_mat4_infinite_perspective(&perspective, 45, 1.0, 0.001);

    GLint combinedTransformLoc = glGetUniformLocation(program, "combinedTransform");
    assert(combinedTransformLoc > -1);

    glEnable(GL_DEPTH_TEST);

    int run = 1;
    while(run) {
        float t = SDL_GetTicks() / 1000.0f;
        
        // Draw
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gbMat4 objTransform;
        gb_mat4_rotate(&objTransform, (gbVec3) { 0.0, 1.0, 0.0 }, t);

        gbMat4 combinedTransform; // persp * cam * obj
        gb_mat4_mul(&combinedTransform, &camera, &objTransform);
        gb_mat4_mul(&combinedTransform, &perspective, &combinedTransform);
        
        glUniformMatrix4fv(combinedTransformLoc, 1, GL_FALSE, combinedTransform.e);
        
        glDrawArrays(GL_TRIANGLES, 0, vert_count);

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
