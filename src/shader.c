#include "graphics.h"
#include <string.h>

#define LOG_SIZE 512

static unsigned int cg_shaderid_create(int glTypeShader) {
    unsigned int shaderId;
    shaderId = glCreateShader(glTypeShader);
    return shaderId;
}

static void cg_shaderid_check(unsigned int shaderId, const char* shaderSrc, const char* shaderPath) {
    int success;
    char infoLog[LOG_SIZE];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderId, LOG_SIZE, NULL, infoLog);
        fprintf(stderr, "Error: shader compilation failed in file: %s\n", shaderPath);
    }
}

static void cg_shader_compile(int shaderId, const char* shaderSrc, const char* shaderPath) {
    glShaderSource(shaderId, 1, &shaderSrc, NULL);
    glCompileShader(shaderId);
    cg_shaderid_check(shaderId, shaderSrc, shaderPath);
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
    cg_file_read(&vertexShaderSrc, vertexPath);
    cg_file_read(&fragmentShaderSrc, fragmentPath);

    unsigned int vertexShader = cg_shaderid_create(GL_VERTEX_SHADER);
    cg_shader_compile(vertexShader, vertexShaderSrc, vertexPath);

    unsigned int fragmentShader = cg_shaderid_create(GL_FRAGMENT_SHADER);
    cg_shader_compile(fragmentShader, fragmentShaderSrc, fragmentPath);

    unsigned int programId = cg_program_create();
    cg_program_link(programId, vertexShader, fragmentShader);

    return programId;
}

void cg_shader_light_pointLights(unsigned int programId, vec3* pointLights, unsigned int size) {
    char name[50];
    for (unsigned int i = 0; i < size; i++) {
        snprintf(name, sizeof(name), "pointLights[%d].position", i);
        cg_shader_uniform3f(programId, name, pointLights[i][0], pointLights[i][1], pointLights[i][2]);
        snprintf(name, sizeof(name), "pointLights[%d].ambient", i);
        cg_shader_uniform3f(programId, name, 0.2f, 0.2f, 0.2f);
        snprintf(name, sizeof(name), "pointLights[%d].diffuse", i);
        cg_shader_uniform3f(programId, name, 0.5f, 0.5f, 0.5f);
        snprintf(name, sizeof(name), "pointLights[%d].specular", i);
        cg_shader_uniform3f(programId, name, 1.0f, 1.0f, 1.0f);
        snprintf(name, sizeof(name), "pointLights[%d].constant", i);
        cg_shader_uniform1f(programId, name, 1.0f);
        snprintf(name, sizeof(name), "pointLights[%d].linear", i);
        cg_shader_uniform1f(programId, name, 0.09f);
        snprintf(name, sizeof(name), "pointLights[%d].quadratic", i);
        cg_shader_uniform1f(programId, name, 0.032f);
    }
}

void cg_shader_use(unsigned int programId) {
    glUseProgram(programId);
}

void cg_shader_uniform1i(unsigned int programId, const char* name, int x) {
    glUniform1i(glGetUniformLocation(programId, name), x);
}

void cg_shader_uniform_matrix4fv(unsigned int programId, const char* name, mat4* projection) {
    glUniformMatrix4fv(glGetUniformLocation(programId, name), 1, GL_FALSE, projection[0][0]);
}

void cg_shader_uniform1f(unsigned int programId, const char* name, float x) {
    int res = glGetUniformLocation(programId, name);
    if (res == -1) {
        fprintf(stdout, "res uniform1f %d\n", res);
    }
    glUniform1f(res, x);
}

void cg_shader_uniform2f(unsigned int programId, const char* name, float x, float y) {
    glUniform2f(glGetUniformLocation(programId, name), x, y);
}

void cg_shader_uniform3f(unsigned int programId, const char* name, float x, float y, float z) {
    int res = glGetUniformLocation(programId, name);
    if (res == -1) {
        fprintf(stdout, "res uniform3f %d\n", res);
    }
    glUniform3f(res, x, y, z);
}

void cg_shader_destroy(unsigned int programId) {
    glDeleteProgram(programId);
}
