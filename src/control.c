#include <stdio.h>
#include "graphics.h"

GLFWwindow* graphics_create_window(int width, int height) {
    if (glfwInit() == false) {
        int error = glfwGetError(NULL);
        fprintf(stderr, "GLFW error: %d\n", error);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "Window", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to intialise window\n");
        return NULL;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);

    // Initialize GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        int error = glfwGetError(NULL);
        fprintf(stderr, "Failed to intialise GLAD: %d\n", error);
        return NULL;
    }

    return window;
}

bool graphics_is_key_pressed(GLFWwindow* window, int key) {
    int state = glfwGetKey(window, key);    
    if (state == GLFW_PRESS) {
        return true;
    }
    return false;
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
} 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch(key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_A:
            if (action == GLFW_PRESS) {
                printf("KEY A\n");
            }
            break;
        case GLFW_KEY_B:
            if (action == GLFW_PRESS) {
                printf("KEY B\n");
            }
            break;
        case GLFW_KEY_C:
            if (action == GLFW_PRESS) {
                printf("KEY C\n");
            }
            break;
    }
}
