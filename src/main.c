#include "graphics.h"

#include <stdio.h>

float vertices[] = {
    // x y z            // colors         // textures
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // 0
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // 1
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,  // 2
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // 3
};

unsigned int indices[] = {
    0, 1, 3,
    0, 2, 3
};

int main() {
    GLFWwindow* window = cg_window_create(GRAPHICS_SCREEN_X, GRAPHICS_SCREEN_Y, "Window");
    if (window == NULL) {
        fprintf(stderr, "Failed to open a window\n");
        glfwTerminate();
        return 1;
    }

    unsigned int shaderId =
        cg_shader_create("res/shaders/vertex/vs-texture.glsl", "res/shaders/fragment/fs-texture.glsl");

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    stbi_set_flip_vertically_on_load(TRUE);

    unsigned int textureId[2];
    glGenTextures(2, textureId);
    glBindTexture(GL_TEXTURE_2D, textureId[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    {
        int width, height, nrChannels;
        const char* textureFile = "res/images/wall.jpg";
        unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
        if (data == NULL) {
            fprintf(stderr, "Error loading in texture: %s\n", textureFile);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    glBindTexture(GL_TEXTURE_2D, textureId[1]);
    {
        int width, height, nrChannels;
        const char* textureFile = "res/images/awesomeface.png";
        unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
        if (data == NULL) {
            fprintf(stderr, "Error loading in texture: %s\n", textureFile);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    glUseProgram(shaderId);
    glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderId, "texture2"), 1);

    while (!glfwWindowShouldClose(window)) {

        //glClearColor((float) RED / 255, (float) GREEN / 255, (float) BLUE / 255, (float) 1.0f);
        glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureId[1]);

        glUseProgram(shaderId);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0); // no need to unbind it every time

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    cg_shader_destroy(shaderId);
    glfwTerminate();
    return 0;
}
