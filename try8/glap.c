#include "glap.h"
#include "files.h"

/* #define GB_MATH_IMPLEMENTATION */
/* #include "gb_math.h" */

static SDL_Window *win;
static SDL_Renderer *rend;
static SDL_GLContext glcontext;

Glap glap_start() {
    OK(SDL_Init(SDL_INIT_EVERYTHING));

    int window_flags = SDL_WINDOW_OPENGL;
    OK(SDL_CreateWindowAndRenderer(512, 512, window_flags, &win, &rend));

    // Must set this before creating the context
    // On OSX it seems like we don't need to set SDL_GL_CONTEXT_MAJOR_VERSION
    // and SDL_GL_CONTEXT_MINOR_VERSION ..?! Only SDL_GL_CONTEXT_PROFILE_CORE.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    glcontext = SDL_GL_CreateContext(win);
    
    if(!gladLoadGL()) {
        printf("Something went wrong when loading glad.\n");
        exit(-1);
    }
    
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    Glap glap = {
        .window = win
    };

    return glap;
}

void glap_stop() {
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(win);
    SDL_Quit();
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

GLuint load_shader_program() {
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

    GLuint program = glCreateProgram();
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

    return program;
}

/* void print_mat4(const gbMat4 *m) { */
/*     for(int i = 0; i < 4; i++) { */
/*         printf("%f %f %f %f\n", m->col[i].x, m->col[i].y, m->col[i].z, m->col[i].w); */
/*     } */
/* } */
