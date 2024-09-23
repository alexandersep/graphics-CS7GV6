#include "graphics.h"
#include <stdio.h>

vec3 pointLights[] = {
    { 0.7f,  0.2f,  4.0f},
    { 2.3f, -3.3f, -4.0f},
    { 5.0f,  5.0f, 0.0f},
    { 0.0f,  0.0f, -3.0f},
};

/*
typedef struct {
    Model backpack;
    Cube cube;
    Camera camera;
} Scene;
*/

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

    stbi_set_flip_vertically_on_load(TRUE);

    glEnable(GL_DEPTH_TEST);

    Model grass, sand, water;
    cg_model_create(&grass, "res/models/wfc/grass/grass.obj");
    cg_model_create(&sand, "res/models/wfc/sand/sand.obj");
    cg_model_create(&water, "res/models/wfc/water/water.obj");

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
            cg_shader_uniform_matrix4fv(shaderId, "projection", &projection);
            cg_shader_uniform_matrix4fv(shaderId, "view", &view);

            cg_shader_uniform3f(shaderId, "dirLight.direction", -0.2f, -1.0f, -0.3f);
            cg_shader_uniform3f(shaderId, "dirLight.ambient", 0.05f, 0.05f, 0.05f);
            cg_shader_uniform3f(shaderId, "dirLight.diffuse", 0.4f, 0.4f, 0.4f);
            cg_shader_uniform3f(shaderId, "dirLight.specular", 0.5f, 0.5f, 0.5f);
            cg_shader_uniform1f(shaderId, "material.shininess", 32.0f);  // Adjust shininess

            cg_shader_light_pointLights(shaderId, pointLights, 4);

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

            wfc_draw(&sand, shaderId);

            // render the loaded model
            mat4 model = GLM_MAT4_IDENTITY;
            //glm_mat4_identity(model);
            //glm_translate(model, (vec3) {2.0f, 0.0f, 0.0f});
            //cg_shader_uniform_matrix4fv(shaderId, "model", &model);
            //cg_model_draw(&grass, shaderId);

            glm_mat4_identity(model);
            glm_translate(model, (vec3) {4.0f, 0.0f, 0.0f});
            cg_shader_uniform_matrix4fv(shaderId, "model", &model);
            cg_model_draw(&sand, shaderId);

            glm_mat4_identity(model);
            glm_translate(model, (vec3) {6.0f, 0.0f, 0.0f});
            cg_shader_uniform_matrix4fv(shaderId, "model", &model);
            cg_model_draw(&water, shaderId);

        }
        {
            cg_shader_use(shaderLight);
            cg_shader_uniform_matrix4fv(shaderLight, "projection", &projection);
            cg_shader_uniform_matrix4fv(shaderLight, "view", &view);

            mat4 model;
            for (int i = 0; i < 4; i++) {
                cg_shader_uniform3f(shaderLight, "color", 0.2f, 1.0f, 0.3f);

                glm_mat4_identity(model);
                glm_translate(model, (vec3) {pointLights[i][0], pointLights[i][1], pointLights[i][2]});
                cg_shader_uniform_matrix4fv(shaderId, "model", &model);
                cg_model_draw(&water, shaderId);
            }

        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cg_shader_destroy(shaderId);
    cg_shader_destroy(shaderLight);

    cg_model_destroy(&grass);
    cg_model_destroy(&water);
    cg_model_destroy(&sand);

    glfwTerminate();
    return 0;
}
