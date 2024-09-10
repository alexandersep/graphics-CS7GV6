#include "graphics.h"

#define LOG_SIZE 512

static unsigned int cg_shaderid_create(int glTypeShader) {
    unsigned int shaderId;
    shaderId = glCreateShader(glTypeShader);
    return shaderId;
}

static void cg_shaderid_check(unsigned int shaderId, const char* shaderSrc) {
    int success;
    char infoLog[LOG_SIZE];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, LOG_SIZE, NULL, infoLog);
        fprintf(stderr, "Error: shader compilation failed: %s\n", shaderSrc);
    }
}

static void cg_shader_compile(int shaderId, const char* shaderSrc) {
    glShaderSource(shaderId, 1, &shaderSrc, NULL);
    glCompileShader(shaderId);
    cg_shaderid_check(shaderId, shaderSrc);
}

static unsigned int cg_program_create() {
    unsigned int programId;
    programId = glCreateProgram();
    return programId;
}

static void cg_program_check(unsigned int programId) {
    int success;
    char infoLog[512];
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        fprintf(stderr, "Error: shader linking failed\n");
    }
}

// Note: Deletes vertex and fragmentshader
static void cg_program_link(unsigned int programId, int vertexShaderId, int fragmentShaderId) {
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    cg_program_check(programId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

unsigned int cg_shader_create(const char* vertexPath, const char* fragmentPath) {
    char* vertexShaderSrc;
    char* fragmentShaderSrc;
    cg_read_file(&vertexShaderSrc, vertexPath);
    cg_read_file(&fragmentShaderSrc, fragmentPath);

    unsigned int vertexShader = cg_shaderid_create(GL_VERTEX_SHADER);
    cg_shader_compile(vertexShader, vertexShaderSrc);

    unsigned int fragmentShader = cg_shaderid_create(GL_FRAGMENT_SHADER);
    cg_shader_compile(fragmentShader, fragmentShaderSrc);

    unsigned int programId = cg_program_create();
    cg_program_link(programId, vertexShader, fragmentShader);

    return programId;
}

void cg_shader_destroy(unsigned int programId) {
    glDeleteProgram(programId);
}
