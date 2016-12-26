#include "glap.h"
#include "files.h"

Glap glap_start() {
    OK(SDL_Init(SDL_INIT_EVERYTHING));

    SDL_Window *win;
    SDL_Renderer *rend;

    int window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    OK(SDL_CreateWindowAndRenderer(512, 512, window_flags, &win, &rend));

    // Must set this before creating the context
    // On OSX it seems like we don't need to set SDL_GL_CONTEXT_MAJOR_VERSION
    // and SDL_GL_CONTEXT_MINOR_VERSION ..?! Only SDL_GL_CONTEXT_PROFILE_CORE.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    
    if(!gladLoadGL()) {
        printf("Something went wrong when loading glad.\n");
        exit(-1);
    }
    
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    Glap glap = {
        .window = win,
        .renderer = rend,
        .context = ctx
    };

    return glap;
}

void glap_stop(Glap glap) {
    SDL_GL_DeleteContext(glap.context);
    SDL_DestroyWindow(glap.window);
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

GLuint load_shader_program(const char *vshader_name, const char *fshader_name) {
    const char *vert_src = file_read_all(vshader_name);
    const char *frag_src = file_read_all(fshader_name);
    
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
