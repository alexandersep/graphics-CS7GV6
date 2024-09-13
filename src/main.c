#include "graphics.h"

#include <stdio.h>

float vertices[] = {
    // xyz                // texture   // normal
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
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
    Camera camera;
    cg_control_camera_create(&camera, 10.0f);
    GLFWwindow* window = cg_control_window_create(&camera, CG_SCREEN_X, CG_SCREEN_Y, "Window");
    if (window == NULL) {
        fprintf(stderr, "Failed to open a window\n");
        glfwTerminate();
        return 1;
    }

    unsigned int shaderId =
        cg_shader_create("res/shaders/vertex/vs-texture.glsl", "res/shaders/fragment/fs-texture.glsl");

    unsigned int shaderLight =
        cg_shader_create("res/shaders/vertex/vs-light.glsl", "res/shaders/fragment/fs-light.glsl");
    //cg_file_load_scene("res/models/cube/cube.obj");

    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindVertexArray(VAO[1]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    stbi_set_flip_vertically_on_load(TRUE);

    unsigned int textureId[2];
    cg_texture_create(textureId, 2);
    cg_texture_bind(textureId, 0);
    cg_texture_load("res/images/wall.jpg");
    cg_texture_bind(textureId, 1);
    cg_texture_load("res/images/awesomeface.png");

    cg_shader_use(shaderId);
    cg_shader_uniform1i(shaderId, "texture1", 0);
    cg_shader_uniform1i(shaderId, "texture2", 1);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        cg_control_camera_move(window, deltaTime);

        glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view;
        vec3 center;
        glm_vec3_add(camera.pos, camera.front, center);
        glm_lookat(camera.pos, center, camera.up, view);
        mat4 projection = GLM_MAT4_IDENTITY;
        glm_perspective(glm_rad(camera.zoom), (float) CG_SCREEN_X / (float) CG_SCREEN_Y, 0.1f, 100.0f, projection);

        {
            cg_shader_use(shaderId);
            cg_shader_uniform_matrix4fv(shaderId, "view", &view);
            cg_shader_uniform_matrix4fv(shaderId, "projection", &projection);
            cg_shader_uniform3f(shaderId, "objectColor", 1.0f, 0.5f, 0.31f);
            cg_shader_uniform3f(shaderId, "lightColor", 1.0f, 1.0f, 1.0f);
            vec3 lightPos = {1.2f, 1.0f, 2.0f};
            cg_shader_uniform3f(shaderId, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);

            cg_shader_uniform3f(shaderId, "material.ambient", 1.0f, 0.5f, 0.31f);
            cg_shader_uniform3f(shaderId, "material.diffuse", 1.0f, 0.5f, 0.31f);
            cg_shader_uniform3f(shaderId, "material.specular", 0.5f, 0.5f, 0.5f);
            cg_shader_uniform1f(shaderId, "material.shininess", 32.0f);

            cg_shader_uniform3f(shaderId, "light.specular", 1.0f, 1.0f, 1.0f);

            float time = glfwGetTime();

            vec3 lightColor;
            lightColor[0] = sin(time * 2.0f);
            lightColor[1] = sin(time * 0.7f);
            lightColor[2] = sin(time * 1.3f);

            vec3 diffuseColor;
            glm_vec3_mul(lightColor, (vec3) {0.5f, 0.5f, 0.5f}, diffuseColor);
            vec3 ambientColor;
            glm_vec3_mul(diffuseColor, (vec3) {0.2f, 0.2f, 0.2f}, ambientColor);

            cg_shader_uniform3f(shaderId, "light.ambient", ambientColor[0], ambientColor[1], ambientColor[2]);
            cg_shader_uniform3f(shaderId, "light.diffuse", diffuseColor[0], diffuseColor[1], diffuseColor[2]);

            cg_texture_use(textureId, 0);
            cg_texture_use(textureId, 1);

            glBindVertexArray(VAO[0]);
            for(unsigned int i = 0; i < 10; i++)
            {
                mat4 model = GLM_MAT4_IDENTITY;
                glm_translate_make(model, cubePositions[i]);
                float angle = 20.0f * i;
                //glm_rotate(model, glm_rad(angle), (vec3) {1.0f, 0.3f, 0.5f});
                //glm_rotate(model, (float)glfwGetTime(), (vec3) {0.5f, 1.0f, 0.0f});
                cg_shader_uniform_matrix4fv(shaderId, "model", &model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        {
            cg_shader_use(shaderLight);

            cg_shader_uniform_matrix4fv(shaderLight, "view", &view);
            cg_shader_uniform_matrix4fv(shaderLight, "projection", &projection);

            glBindVertexArray(VAO[1]);
            vec3 lightPos = {1.2f, 1.0f, 2.0f};
            mat4 model = GLM_MAT4_IDENTITY;
            glm_translate_make(model, lightPos);
            glm_scale(model, (vec3) {0.2f, 0.2f, 0.2f});
            cg_shader_uniform_matrix4fv(shaderLight, "model", &model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(1, &VBO);
    cg_shader_destroy(shaderId);
    cg_shader_destroy(shaderLight);
    glfwTerminate();
    return 0;
}
