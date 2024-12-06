#include "graphics.h"
#include <cglm/affine-pre.h>

/*
typedef struct {
    Model backpack;
    Cube cube;
    Camera camera;
} Scene;
*/

int main() {
    srand(time(NULL));
    Camera camera[2]; // Two cameras
    {
        vec3 cameraOne = {0.0f, 0.0f, 3.0f}; // In ocean
        cg_control_camera_create(&camera[0], 10.0f, cameraOne);

        vec3 cameraTwo = {0.0f, 90.0f, 0.0f}; // outside ocean
        cg_control_camera_create(&camera[1], 10.0f, cameraTwo);
    }

    camera[1].angle.pitch = -89.0f;

    GLFWwindow* window = cg_control_window_create(CG_SCREEN_X, CG_SCREEN_Y, "Window");
    Cameras cameras;
    cameras.camera = camera;
    cameras.size = 2;
    cameras.focus = 0;
    glfwSetWindowUserPointer(window, &cameras); // default to first person view camera
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
    unsigned int shaderMarines =
        cg_shader_create("res/shaders/vertex/vs-instance.glsl", "res/shaders/fragment/fs-texture.glsl");
    unsigned int shaderSkyBox =
        cg_shader_create("res/shaders/vertex/vs-skybox.glsl", "res/shaders/fragment/fs-skybox.glsl");
    unsigned int shaderCubeSkyBox =
        cg_shader_create("res/shaders/vertex/vs-cube.glsl", "res/shaders/fragment/fs-cube.glsl");
    unsigned int shaderFrameBuffer =
        cg_shader_create("res/shaders/vertex/vs-framebuffer.glsl", "res/shaders/fragment/fs-framebuffer.glsl");
    unsigned int shaderBlending =
        cg_shader_create("res/shaders/vertex/vs-blending.glsl", "res/shaders/fragment/fs-blending.glsl");

    Texture textureGrass;
    cg_texture_create(&textureGrass, "res/textures/grass/grass.png");

    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    //glEnable(GL_STENCIL_TEST);
    //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    Model grass, water, sandFloor;
    cg_model_create(&grass, "res/models/wfc/grass/grass.obj");
    cg_model_create(&water, "res/models/wfc/water/water.obj");
    cg_model_create(&sandFloor, "res/models/wfc/sand-floor/sand.obj");

    GenericModel starfish;
    size_t size = 50;
    mat4 models[size];
    cg_generic_model_positions(models, size);
    cg_generic_model_create(&starfish, "res/models/starfish/starfish.obj", models, size);

    Boids boids;
    cg_boids_create(&boids, 51);

    Mantaray mantaray;
    cg_mantaray_create(&mantaray);

    Fish fish;
    cg_fish_create(&fish);

    unsigned int instances = 100;
    mat4 modelMatrices[instances];
    for (unsigned int i = 0; i < instances; i++) {
        mat4 model;
        glm_mat4_identity(model);
        glm_translate(model, (vec3) {(float) (rand() % 10), (float) (rand() % 10), (float) (rand() % 10)});

        glm_mat4_copy(model, modelMatrices[i]);
    }

    CubeMap skybox;
    cg_cubemap_create(&skybox);

    Transparent transGrass;
    size_t grassSize = 100;
    cg_transparent_create(&transGrass);
    mat4 transGrass_models[grassSize];
    cg_transparent_positions(transGrass_models, grassSize);

    Quad quad;
    cg_screen_quad_create(&quad);

    FrameBuffer framebuffer;
    cg_framebuffer_create(&framebuffer);

    Texture textureAttachment;
    cg_texture_attachment_create(&textureAttachment);
    cg_framebuffer_texture_attach(&framebuffer, &textureAttachment);

    RenderBuffer renderbuffer;
    cg_renderbuffer_create(&renderbuffer);
    cg_renderbuffer_bind(&renderbuffer);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    vec3 pointLights[20];
    for (int i = 0; i < 20; i++) {
        float x = ((float)rand() / RAND_MAX) * 45.0f - 23.5f;
        float y = ((float)rand() / RAND_MAX) * 20.0f - 10.0f;
        float z = ((float)rand() / RAND_MAX) * 40.0f - 20.0f;
        glm_vec3_copy((vec3){x, y, z}, pointLights[i]);
    }

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        cg_control_camera_move(window, deltaTime);

        cg_framebuffer_bind(&framebuffer);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.14f, 0.14f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view;
        vec3 center;
        glm_vec3_add(cameras.camera[cameras.focus].pos, cameras.camera[cameras.focus].front, center);
        glm_lookat(cameras.camera[cameras.focus].pos, center, cameras.camera[cameras.focus].up, view);
        mat4 projection;
        glm_perspective(glm_rad(cameras.camera[cameras.focus].zoom), (float) CG_SCREEN_X / (float) CG_SCREEN_Y, 0.1f, 1000.0f, projection);

        {
            //glStencilMask(0x00);
            //glEnable(GL_DEPTH_TEST);

            cg_shader_use(shaderId);
            {
                cg_shader_uniform_matrix4fv(shaderId, "projection", projection);
                cg_shader_uniform_matrix4fv(shaderId, "view", view);

                cg_shader_uniform3f(shaderId, "dirLight.direction", -0.2f, -1.0f, -0.3f);
                cg_shader_uniform3f(shaderId, "dirLight.ambient", 0.15f, 0.15f, 0.15f);
                cg_shader_uniform3f(shaderId, "dirLight.diffuse", 0.4f, 0.4f, 0.4f);
                cg_shader_uniform3f(shaderId, "dirLight.specular", 0.5f, 0.5f, 0.5f);
                cg_shader_uniform1f(shaderId, "material.shininess", 32.0f);  // Adjust shininess

                cg_shader_light_pointLights(shaderId, pointLights, 20);

                cg_shader_uniform3f(shaderId, "spotLight.position", cameras.camera[cameras.focus].pos[0], cameras.camera[cameras.focus].pos[1], cameras.camera[cameras.focus].pos[2]);
                cg_shader_uniform3f(shaderId, "spotLight.direction", cameras.camera[cameras.focus].front[0], cameras.camera[cameras.focus].front[1], cameras.camera[cameras.focus].front[2]);
                cg_shader_uniform3f(shaderId, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
                cg_shader_uniform3f(shaderId, "spotLight.diffuse", 1.0f, 1.0f, 1.0f);
                cg_shader_uniform3f(shaderId, "spotLight.specular", 1.0f, 1.0f, 1.0f);
                cg_shader_uniform1f(shaderId, "spotLight.constant", 1.0f);
                cg_shader_uniform1f(shaderId, "spotLight.linear", 0.09f);
                cg_shader_uniform1f(shaderId, "spotLight.quadratic", 0.032f);
                cg_shader_uniform1f(shaderId, "spotLight.cutOff", cos(glm_rad(12.5f)));
                cg_shader_uniform1f(shaderId, "spotLight.outerCutOff", cos(glm_rad(17.5f)));

                cg_shader_uniform3f(shaderId, "CameraPos", cameras.camera[cameras.focus].pos[0], cameras.camera[cameras.focus].pos[1], cameras.camera[cameras.focus].pos[2]);
            }

            // 1st. render pass, draw objects as normal, writing to the stencil buffer
            //glStencilFunc(GL_ALWAYS, 1, 0xFF);
            //glStencilMask(0xFF);

            // render the loaded model
            mat4 model;

            glm_mat4_identity(model);
            glm_mat4_scale(model, 51);
            cg_shader_uniform_matrix4fv(shaderId, "model", model);
            cg_model_draw(&grass, shaderId);
            cg_model_draw(&sandFloor, shaderId);

            cg_shader_use(shaderMarines);
            {
                cg_shader_uniform_matrix4fv(shaderMarines, "projection", projection);
                cg_shader_uniform_matrix4fv(shaderMarines, "view", view);

                cg_shader_uniform3f(shaderMarines, "dirLight.direction", -0.2f, -1.0f, -0.3f);
                cg_shader_uniform3f(shaderMarines, "dirLight.ambient", 0.15f, 0.15f, 0.15f);
                cg_shader_uniform3f(shaderMarines, "dirLight.diffuse", 0.4f, 0.4f, 0.4f);
                cg_shader_uniform3f(shaderMarines, "dirLight.specular", 0.5f, 0.5f, 0.5f);
                cg_shader_uniform1f(shaderMarines, "material.shininess", 32.0f);  // Adjust shininess

                cg_shader_light_pointLights(shaderMarines, pointLights, 20);

                cg_shader_uniform3f(shaderMarines, "spotLight.position", cameras.camera[cameras.focus].pos[0], cameras.camera[cameras.focus].pos[1], cameras.camera[cameras.focus].pos[2]);
                cg_shader_uniform3f(shaderMarines, "spotLight.direction", cameras.camera[cameras.focus].front[0], cameras.camera[cameras.focus].front[1], cameras.camera[cameras.focus].front[2]);
                cg_shader_uniform3f(shaderMarines, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
                cg_shader_uniform3f(shaderMarines, "spotLight.diffuse", 1.0f, 1.0f, 1.0f);
                cg_shader_uniform3f(shaderMarines, "spotLight.specular", 1.0f, 1.0f, 1.0f);
                cg_shader_uniform1f(shaderMarines, "spotLight.constant", 1.0f);
                cg_shader_uniform1f(shaderMarines, "spotLight.linear", 0.09f);
                cg_shader_uniform1f(shaderMarines, "spotLight.quadratic", 0.032f);
                cg_shader_uniform1f(shaderMarines, "spotLight.cutOff", cos(glm_rad(12.5f)));
                cg_shader_uniform1f(shaderMarines, "spotLight.outerCutOff", cos(glm_rad(17.5f)));

                cg_shader_uniform3f(shaderMarines, "CameraPos", cameras.camera[cameras.focus].pos[0], cameras.camera[cameras.focus].pos[1], cameras.camera[cameras.focus].pos[2]);
            }

            cg_mantaray_boids_draw(&mantaray, &fish, &boids, shaderMarines);
            cg_generic_model_instance_draw(&starfish, shaderMarines, models, size);

            cg_shader_use(shaderBlending); 
            {
                cg_shader_uniform_matrix4fv(shaderBlending, "projection", projection);
                cg_shader_uniform_matrix4fv(shaderBlending, "view", view);
                cg_shader_uniform1i(shaderBlending, "texture1", 0);

                cg_shader_uniform1f(shaderBlending, "time", currentFrame);
            }
            cg_transparent_draw(&transGrass, shaderBlending, &textureGrass, transGrass_models, grassSize);

            //glm_mat4_identity(model);
            //glm_scale(model, (vec3){0.1f, 0.1f, 0.25f});
            //cg_shader_uniform_matrix4fv(shaderId, "model", model);
            //cg_model_draw(&fish, shaderId);

            //cg_mantaray_draw(&mantaray, shaderId);
            //cg_mantaray_position_set(&mantaray, (vec3){0.0f, 10.0f, 10.0f});
            //cg_mantaray_draw(&mantaray, shaderId);

            // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
            // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing
            // the objects' size differences, making it look like borders.
            //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            //glStencilMask(0x00);
            //glDisable(GL_DEPTH_TEST);

            // Commented out to not display the light blocks as the seemlesness dissapears 
            /*
            cg_shader_use(shaderLight);
            cg_shader_uniform_matrix4fv(shaderLight, "projection", projection);
            cg_shader_uniform_matrix4fv(shaderLight, "view", view);
            cg_shader_uniform3f(shaderLight, "color", 0.5f, 0.6f, 0.90f);

            glm_mat4_identity(model);
            for (int i = 0; i < 20; i++) {
                glm_translate(model, pointLights[i]);
                glm_mat4_scale(model, 50);
                cg_shader_uniform_matrix4fv(shaderLight, "model", model);
                cg_model_draw(&grass, shaderLight);
            }
            */

            //glm_mat4_identity(model);
            //glm_scale(model, (vec3) {1.1f, 1.1f, 1.1f});
            //cg_shader_uniform_matrix4fv(shaderLight, "model", &model);
            //cg_model_draw(&grass, shaderLight, 0);

            //glStencilMask(0xFF);
            //glStencilFunc(GL_ALWAYS, 0, 0xFF);
            cg_shader_use(shaderCubeSkyBox);
            {
                cg_shader_uniform_matrix4fv(shaderCubeSkyBox, "projection", projection);
                cg_shader_uniform_matrix4fv(shaderCubeSkyBox, "view", view);

                glm_mat4_identity(model);
                glm_mat4_scale(model, 52);
                cg_shader_uniform_matrix4fv(shaderCubeSkyBox, "model", model);

                cg_shader_uniform3f(shaderCubeSkyBox, "cameraPos", 
                        cameras.camera[cameras.focus].pos[0],
                        cameras.camera[cameras.focus].pos[0],
                        cameras.camera[cameras.focus].pos[0]);
            }
            cg_cubemap_model_draw(&skybox, &grass, shaderCubeSkyBox);

            cg_shader_use(shaderSkyBox); 
            {
                cg_shader_uniform_matrix4fv(shaderSkyBox, "projection", projection);
                view[3][0] = 0.0f;
                view[3][1] = 0.0f;
                view[3][2] = 0.0f;
                cg_shader_uniform_matrix4fv(shaderSkyBox, "view", view);
                cg_shader_uniform1f(shaderSkyBox, "time", currentFrame);
            }
            cg_cubemap_draw(&skybox);


            cg_framebuffer_default_bind();
            glDisable(GL_DEPTH_TEST);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            cg_shader_use(shaderFrameBuffer); 
            cg_screen_quad_draw(&quad, &textureAttachment);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cg_shader_destroy(shaderId);
    cg_shader_destroy(shaderLight);
    cg_shader_destroy(shaderInstance);
    cg_shader_destroy(shaderMarines);
    cg_shader_destroy(shaderSkyBox);
    cg_shader_destroy(shaderCubeSkyBox);
    cg_shader_destroy(shaderFrameBuffer);
    cg_shader_destroy(shaderBlending);

    cg_texture_destroy(&textureGrass);

    cg_cubemap_destroy(&skybox);
    cg_transparent_destroy(&transGrass);

    cg_model_destroy(&grass);
    cg_model_destroy(&water);
    cg_model_destroy(&sandFloor);

    cg_generic_model_destroy(&starfish);

    cg_fish_destroy(&fish);
    cg_mantaray_destroy(&mantaray);

    cg_boids_destroy(&boids);

    cg_screen_quad_destroy(&quad);

    // Framebuffer specific
    cg_framebuffer_destroy(&framebuffer);
    cg_texture_destroy(&textureAttachment);
    cg_renderbuffer_destroy(&renderbuffer);

    glfwTerminate();
    return 0;
}
