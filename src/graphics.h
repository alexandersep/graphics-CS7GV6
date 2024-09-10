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
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#define GREEN 0x0000FF
#define BLUE 0x00FF00
#define RED 0xFF0000

// control.h
// Window
GLFWwindow* graphics_create_window(int width, int height);
bool graphics_is_key_pressed(GLFWwindow* window, int key);

// Callbacks
// framebuffer for opengl
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Error
void error_callback(int error, const char* description);
// Keyboard
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif // GRAPHICS_H 
