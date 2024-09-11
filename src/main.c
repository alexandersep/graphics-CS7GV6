#include "graphics.h"

#include <stdio.h>

float vertices[] = {
    // xyz               // texture
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

vec3 cubePositions[] = {
    { 0.0f,  0.0f,  0.0f},
    { 2.0f,  5.0f, -15.0f},
    {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.0f, -12.3f},
    { 2.4f, -0.4f, -3.5f},
    {-1.7f,  3.0f, -7.5f},
    { 1.3f, -2.0f, -2.5f},
    { 1.5f,  2.0f, -2.5f},
    { 1.5f,  0.2f, -1.5f},
    {-1.3f,  1.0f, -1.5f}
};

int main() {
    GLFWwindow* window = cg_window_create(CG_SCREEN_X, CG_SCREEN_Y, "Window");
    if (window == NULL) {
        fprintf(stderr, "Failed to open a window\n");
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    unsigned int shaderId =
        cg_shader_create("res/shaders/vertex/vs-texture.glsl", "res/shaders/fragment/fs-texture.glsl");

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    stbi_set_flip_vertically_on_load(TRUE);

    unsigned int textureId[2];
    cg_texture_create(textureId, 2);
    cg_texture_use(textureId[0]);
    cg_texture_load("res/images/wall.jpg");
    cg_texture_use(textureId[1]);
    cg_texture_load("res/images/awesomeface.png");

    cg_shader_use(shaderId);
    cg_shader_uniform1i(shaderId, "texture1", 0);
    cg_shader_uniform1i(shaderId, "texture2", 1);

    unsigned int transformLoc = glGetUniformLocation(shaderId, "transform");
    while (!glfwWindowShouldClose(window)) {
        //glClearColor((float) RED / 255, (float) GREEN / 255, (float) BLUE / 255, (float) 1.0f);
        glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view = GLM_MAT4_IDENTITY;
        mat4 projection = GLM_MAT4_IDENTITY;
        glm_translate_make(view, (vec3) {0.0f, 0.0f, -3.0f});
        glm_perspective(glm_rad(45.0f), (float)CG_SCREEN_X / (float) CG_SCREEN_Y, 0.1f, 100.0f, projection);

        int modelLoc = glGetUniformLocation(shaderId, "model");
        int viewLoc = glGetUniformLocation(shaderId, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, &projection[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureId[1]);

        glUseProgram(shaderId);

        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            mat4 model = GLM_MAT4_IDENTITY;
            glm_translate_make(model, cubePositions[i]);
            float angle = 20.0f * i;
            glm_rotate(model, glm_rad(angle), (vec3) {1.0f, 0.3f, 0.5f});
            glm_rotate(model, (float)glfwGetTime(), (vec3) {0.5f, 1.0f, 0.0f});
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *) model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glBindVertexArray(0); // no need to unbind it every time

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    cg_shader_destroy(shaderId);
    glfwTerminate();
    return 0;
}
