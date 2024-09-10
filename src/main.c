#include <stdio.h>
#include "graphics.h"

int main() {
    GLFWwindow* window = graphics_create_window(GRAPHICS_SCREEN_X, GRAPHICS_SCREEN_Y);
    if (window == NULL) {
        fprintf(stderr, "Failed to open a window\n");
        glfwTerminate();
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glClearColor((float) RED / 255, (float) GREEN / 255, (float) BLUE / 255, (float) 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
