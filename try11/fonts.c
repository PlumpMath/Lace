#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "glap.h"
#include "files.h"
#include <libgen.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define GB_MATH_IMPLEMENTATION
#include "gb_math.h"

GLuint program;

int main(int argc, char **argv) {
    Glap glap = glap_start();
   
    // GL Actual Work
    char *base_path = dirname(argv[0]);
    char *vspath, *fspath;
    asprintf(&vspath, "%s/shader.vert", base_path);
    asprintf(&fspath, "%s/shader.frag", base_path);
    program = load_shader_program(vspath, fspath);
    glUseProgram(program);
    printf("Shader programs loaded.\n");

    // WITHOUT THIS BEFORE LOADING FONT TEXTURES, YOU'RE SCREWED
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Freetype
    FT_Library ftlib;
    OK(FT_Init_FreeType(&ftlib));

    FT_Face face;
    OK(FT_New_Face(ftlib, "./Hasklig.otf", 0, &face));

    FT_Set_Pixel_Sizes(face, 0, 50);

    OK(FT_Load_Char(face, 'I', FT_LOAD_RENDER));

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // GL settings
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float left = 0.0;
    float top = 50;
    float vertices[] = {
        left, top, 0.0, 0.0,
        left, left, 0.0, 1.0,
        top, left, 1.0, 1.0,

        left, top, 0.0, 0.0,
        top, left, 1.0, 1.0,
        top, top, 1.0, 0.0,
    };
    
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    GLint textColorUniformLoc = glGetUniformLocation(program, "textColor");
    assert(textColorUniformLoc > -1);

    GLint transformUniformLoc = glGetUniformLocation(program, "projection");
    assert(transformUniformLoc > -1);

    gbMat4 orto;
    gb_mat4_identity(&orto);
    gb_mat4_ortho2d(&orto, 0, 800, 0, 800);
    //gb_mat4_ortho3d(&orto, 0, 0, 10, 10, 0, 100);

    int run = 1;
    while(run) {
        float t = SDL_GetTicks() / 1000.0f;

        // Draw
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform3f(textColorUniformLoc, 1.0, 1.0, 1.0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUniformMatrix4fv(transformUniformLoc, 1, GL_FALSE, orto.e);
        
        //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
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
                    program = load_shader_program(vspath, fspath);
                    glUseProgram(program);
                }
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    run = 0;
                }
                break;
            }
        }
    }

    glap_stop(glap);
}
