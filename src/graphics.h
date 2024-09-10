#ifndef GRAPHICS_H 
#define GRAPHICS_H 

#define GLFW_INCLUDE_NONE
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#define GRAPHICS_SCREEN_X 400 
#define GRAPHICS_SCREEN_Y 400

// Types
typedef char bool;

#define TRUE GL_TRUE
#define FALSE GL_FALSE

#define GREEN 0x0000FF
#define BLUE 0x00FF00
#define RED 0xFF0000

// control
// Window
GLFWwindow* cg_window_create(int width, int height, const char* title);

// Callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Shader
unsigned int cg_shader_create(const char* vertexPath, const char* fragmentPath);
void cg_shader_destroy(unsigned int programId);

// File
// Returns int size, and first parameter src code
size_t cg_read_file(char** shaderSrc, const char* shaderPath);

#endif // GRAPHICS_H 
