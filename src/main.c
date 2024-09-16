#include "graphics.h"

#include <stdio.h>

float vertices[] = {
    // xyz, texture, normal
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
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

vec3 pointLightPositions[] = {
    { 0.7f,  0.2f,  2.0f},
    { 2.3f, -3.3f, -4.0f},
    {-4.0f,  2.0f, -12.0f},
    { 0.0f,  0.0f, -3.0f}
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
    cg_texture_load("res/images/container.png");
    cg_texture_bind(textureId, 1);
    cg_texture_load("res/images/container_specular.png");

    cg_shader_use(shaderId);
    cg_shader_uniform1i(shaderId, "material.diffuse", 0);
    cg_shader_uniform1i(shaderId, "material.specular", 1);

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
        mat4 projection;
        glm_perspective(glm_rad(camera.zoom), (float) CG_SCREEN_X / (float) CG_SCREEN_Y, 0.1f, 100.0f, projection);

        {
            cg_shader_use(shaderId);
            cg_shader_uniform1f(shaderId, "material.shininess", 32.0f);
            cg_shader_uniform3f(shaderId, "viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);

            cg_shader_uniform_matrix4fv(shaderId, "view", &view);
            cg_shader_uniform_matrix4fv(shaderId, "projection", &projection);
            cg_shader_uniform3f(shaderId, "objectColor", 1.0f, 1.0f, 1.0f);

            cg_shader_uniform3f(shaderId, "dirLight.direction", -0.2f, -1.0f, -0.3f);
            cg_shader_uniform3f(shaderId, "dirLight.ambient", 0.05f, 0.05f, 0.05f);
            cg_shader_uniform3f(shaderId, "dirLight.diffuse", 0.4f, 0.4f, 0.4f);
            cg_shader_uniform3f(shaderId, "dirLight.specular", 0.5f, 0.5f, 0.5f);

            cg_shader_uniform3f(shaderId, "pointLights[0].position", pointLightPositions[0][0], pointLightPositions[0][1], pointLightPositions[0][2]);
            cg_shader_uniform3f(shaderId, "pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
            cg_shader_uniform3f(shaderId, "pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
            cg_shader_uniform3f(shaderId, "pointLights[0].specular", 1.0f, 1.0f, 1.0f);
            cg_shader_uniform1f(shaderId, "pointLights[0].constant", 1.0f);
            cg_shader_uniform1f(shaderId, "pointLights[0].linear", 0.09f);
            cg_shader_uniform1f(shaderId, "pointLights[0].quadratic", 0.032f);

            cg_shader_uniform3f(shaderId, "pointLights[1].position", pointLightPositions[1][0], pointLightPositions[1][1], pointLightPositions[1][2]);
            cg_shader_uniform3f(shaderId, "pointLights[1].ambient", 0.2f, 0.2f, 0.2f);
            cg_shader_uniform3f(shaderId, "pointLights[1].diffuse", 0.5f, 0.5f, 0.5f);
            cg_shader_uniform3f(shaderId, "pointLights[1].specular", 1.0f, 1.0f, 1.0f);
            cg_shader_uniform1f(shaderId, "pointLights[1].constant", 1.0f);
            cg_shader_uniform1f(shaderId, "pointLights[1].linear", 0.09f);
            cg_shader_uniform1f(shaderId, "pointLights[1].quadratic", 0.032f);

            cg_shader_uniform3f(shaderId, "pointLights[2].position", pointLightPositions[2][0], pointLightPositions[2][1], pointLightPositions[2][2]);
            cg_shader_uniform3f(shaderId, "pointLIghts[2].ambient", 0.2f, 0.2f, 0.2f);
            cg_shader_uniform3f(shaderId, "pointLIghts[2].diffuse", 0.5f, 0.5f, 0.5f);
            cg_shader_uniform3f(shaderId, "pointLIghts[2].specular", 1.0f, 1.0f, 1.0f);
            cg_shader_uniform1f(shaderId, "pointLights[2].constant", 1.0f);
            cg_shader_uniform1f(shaderId, "pointLights[2].linear", 0.09f);
            cg_shader_uniform1f(shaderId, "pointLights[2].quadratic", 0.032f);

            cg_shader_uniform3f(shaderId, "pointLights[3].position", pointLightPositions[3][0], pointLightPositions[3][1], pointLightPositions[3][2]);
            cg_shader_uniform3f(shaderId, "pointLights[3].ambient", 0.2f, 0.2f, 0.2f);
            cg_shader_uniform3f(shaderId, "pointLights[3].diffuse", 0.5f, 0.5f, 0.5f);
            cg_shader_uniform3f(shaderId, "pointLights[3].specular", 1.0f, 1.0f, 1.0f);
            cg_shader_uniform1f(shaderId, "pointLights[3].constant", 1.0f);
            cg_shader_uniform1f(shaderId, "pointLights[3].linear", 0.09f);
            cg_shader_uniform1f(shaderId, "pointLights[3].quadratic", 0.032f);

            cg_shader_uniform3f(shaderId, "spotLight.position", camera.pos[0], camera.pos[1], camera.pos[2]);
            cg_shader_uniform3f(shaderId, "spotLight.direction", camera.front[0], camera.front[1], camera.front[2]);
            cg_shader_uniform3f(shaderId, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
            cg_shader_uniform3f(shaderId, "spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            cg_shader_uniform3f(shaderId, "spotLight.specular", 1.0f, 1.0f, 1.0f);
            cg_shader_uniform1f(shaderId, "spotLight.constant", 1.0f);
            cg_shader_uniform1f(shaderId, "spotLight.linear", 0.09f);
            cg_shader_uniform1f(shaderId, "spotLight.quadratic", 0.032f);
            cg_shader_uniform1f(shaderId, "spotLight.cutOff", cos(glm_rad(12.5f)));
            cg_shader_uniform1f(shaderId, "spotLight.outerCutOff", cos(glm_rad(15.0f)));     
            cg_shader_uniform1f(shaderId, "spotLight.cutOff", cos(glm_rad(12.5f)));
            cg_shader_uniform1f(shaderId, "spotLight.outerCutOff", cos(glm_rad(17.5f)));

            cg_texture_use(textureId, 0);
            cg_texture_use(textureId, 1);
            cg_texture_use(textureId, 2);

            glBindVertexArray(VAO[0]);
            for(unsigned int i = 0; i < 10; i++)
            {
                mat4 model = GLM_MAT4_IDENTITY;
                glm_translate_make(model, cubePositions[i]);
                float angle = 20.0f * i;
                glm_rotate(model, glm_rad(angle), (vec3) {1.0f, 0.3f, 0.5f});
                glm_rotate(model, (float)glfwGetTime(), (vec3) {0.5f, 1.0f, 0.0f});
                cg_shader_uniform_matrix4fv(shaderId, "model", &model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        {
            cg_shader_use(shaderLight);

            cg_shader_uniform_matrix4fv(shaderLight, "projection", &projection);
            cg_shader_uniform_matrix4fv(shaderLight, "view", &view);

            glBindVertexArray(VAO[1]);
            for (unsigned int i = 0; i < 4; i++) {
                mat4 model = GLM_MAT4_IDENTITY;
                glm_translate_make(model, pointLightPositions[i]);
                glm_scale(model, (vec3) {0.2f, 0.2f, 0.2f});
                cg_shader_uniform_matrix4fv(shaderLight, "model", &model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
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
