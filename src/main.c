#include "graphics.h"
#include <stdio.h>

vec3 pointLights[] = {
    { 0.7f,  0.2f,  2.0f},
    { 2.3f, -3.3f, -4.0f},
    {-4.0f,  2.0f, -12.0f},
    { 0.0f,  0.0f, -3.0f}
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
    unsigned int shaderInstance =
        cg_shader_create("res/shaders/vertex/vs-instance.glsl", "res/shaders/fragment/fs-texture.glsl");

    stbi_set_flip_vertically_on_load(TRUE);

    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    //glEnable(GL_STENCIL_TEST);
    //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    Model grass, sand, water;
    cg_model_create(&grass, "res/models/wfc/grass/grass.obj");
    cg_model_create(&sand, "res/models/wfc/sand/sand.obj");
    cg_model_create(&water, "res/models/wfc/water/water.obj");

    unsigned int instances = 100;
    mat4 modelMatrices[instances];
    srand(glfwGetTime());
    for (unsigned int i = 0; i < instances; i++) {
        mat4 model;
        glm_mat4_identity(model);
        glm_translate(model, (vec3) {rand() % 10, rand() % 10, rand() % 10});

        glm_mat4_copy(model, modelMatrices[i]);
    }

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, instances * sizeof(mat4), &modelMatrices[0], GL_STATIC_DRAW);

    cg_model_instance_setup(&sand);

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
            //glStencilMask(0x00);
            //glEnable(GL_DEPTH_TEST);

            cg_shader_use(shaderInstance);
            {
                cg_shader_uniform_matrix4fv(shaderInstance, "projection", &projection);
                cg_shader_uniform_matrix4fv(shaderInstance, "view", &view);

                cg_shader_uniform3f(shaderInstance, "dirLight.direction", -0.2f, -1.0f, -0.3f);
                cg_shader_uniform3f(shaderInstance, "dirLight.ambient", 0.05f, 0.05f, 0.05f);
                cg_shader_uniform3f(shaderInstance, "dirLight.diffuse", 0.4f, 0.4f, 0.4f);
                cg_shader_uniform3f(shaderInstance, "dirLight.specular", 0.5f, 0.5f, 0.5f);
                cg_shader_uniform1f(shaderInstance, "material.shininess", 32.0f);  // Adjust shininess

                cg_shader_light_pointLights(shaderInstance, pointLights, 4);

                cg_shader_uniform3f(shaderInstance, "spotLight.position", camera.pos[0], camera.pos[1], camera.pos[2]);
                cg_shader_uniform3f(shaderInstance, "spotLight.direction", camera.front[0], camera.front[1], camera.front[2]);
                cg_shader_uniform3f(shaderInstance, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
                cg_shader_uniform3f(shaderInstance, "spotLight.diffuse", 1.0f, 1.0f, 1.0f);
                cg_shader_uniform3f(shaderInstance, "spotLight.specular", 1.0f, 1.0f, 1.0f);
                cg_shader_uniform1f(shaderInstance, "spotLight.constant", 1.0f);
                cg_shader_uniform1f(shaderInstance, "spotLight.linear", 0.09f);
                cg_shader_uniform1f(shaderInstance, "spotLight.quadratic", 0.032f);
                cg_shader_uniform1f(shaderInstance, "spotLight.cutOff", cos(glm_rad(12.5f)));
                cg_shader_uniform1f(shaderInstance, "spotLight.outerCutOff", cos(glm_rad(17.5f)));
            }

            cg_model_instance_draw(&sand, shaderInstance, instances);

            cg_shader_use(shaderId);
            {
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
                cg_shader_uniform1f(shaderId, "spotLight.outerCutOff", cos(glm_rad(17.5f)));
            }

            // 1st. render pass, draw objects as normal, writing to the stencil buffer
            //glStencilFunc(GL_ALWAYS, 1, 0xFF);
            //glStencilMask(0xFF);

            // render the loaded model
            mat4 model;
            glm_mat4_identity(model);
            cg_shader_uniform_matrix4fv(shaderId, "model", &model);
            cg_model_draw(&grass, shaderId);

            // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
            // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing
            // the objects' size differences, making it look like borders.
            //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            //glStencilMask(0x00);
            //glDisable(GL_DEPTH_TEST);

            //cg_shader_use(shaderLight);
            //cg_shader_uniform_matrix4fv(shaderId, "projection", &projection);
            //cg_shader_uniform_matrix4fv(shaderId, "view", &view);
            //cg_shader_uniform3f(shaderLight, "color", 0.5f, 0.6f, 0.90f);

            //glm_mat4_identity(model);
            //glm_scale(model, (vec3) {1.1f, 1.1f, 1.1f});
            //cg_shader_uniform_matrix4fv(shaderLight, "model", &model);
            //cg_model_draw(&grass, shaderLight, 0);

            //glStencilMask(0xFF);
            //glStencilFunc(GL_ALWAYS, 0, 0xFF);

        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &buffer);
    cg_shader_destroy(shaderId);
    cg_shader_destroy(shaderLight);
    cg_shader_destroy(shaderInstance);

    cg_model_destroy(&grass);
    cg_model_destroy(&water);
    cg_model_destroy(&sand);

    glfwTerminate();
    return 0;
}
