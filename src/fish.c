#include "graphics.h"

void cg_fish_create(Fish* f) {
    cg_model_create(&f->body, "res/models/fish/body/body.obj");
    cg_model_create(&f->tail, "res/models/fish/tail/tail.obj");

    unsigned int buffers[2];
    glGenBuffers(2, buffers);
    f->bufferBody = buffers[0];
    f->bufferTail = buffers[1];
}

void cg_fish_model_draw(Fish* f, Boid* b, unsigned int shaderId) {
    mat4 model;
    glm_mat4_identity(model);

    glm_translate(model, b->position);

    mat4 rotation;
    vec3 forward = {1.0f, 0.0f, 0.0f};  // The fish model faces positive x axis
    cg_boid_angle_update(b, rotation, forward);
    glm_mat4_mul(model, rotation, model);

    glm_scale(model, (vec3){0.4f, 0.4f, 0.4f});

    cg_shader_uniform_matrix4fv(shaderId, "model", model);

    cg_model_draw(&f->body, shaderId);
    cg_model_draw(&f->tail, shaderId);
}

void cg_fish_models_instance_draw(Fish* fish, unsigned int shaderId, Boids* boids) {
    size_t size = (boids->size / 2) - 1;
    mat4 modelBody[size];
    mat4 modelTail[size];
    vec3 forward = {1.0f, 0.0f, 0.0f};  // The fish model faces positive x axis
    mat4 rotation;
    size_t j = 0;

    float degrees = 2.0f;
    for (size_t i = 1; i < boids->size; i+=2, j++) {
        glm_mat4_identity(modelBody[j]);
        glm_translate(modelBody[j], boids->boid[i].position);

        cg_boid_angle_update(&boids->boid[i], rotation, forward);
        glm_mat4_mul(modelBody[j], rotation, modelBody[j]);

        float time = glfwGetTime();
        float angle = degrees * sin(time + boids->boid[i].time);

        glm_mat4_identity(modelTail[j]);
        glm_mat4_mul(modelBody[j], modelTail[j], modelTail[j]);
        glm_rotate(modelTail[j], glm_rad(angle), (vec3) {0.0f, 1.0f, 0.0f});
    }

    glBindBuffer(GL_ARRAY_BUFFER, fish->bufferBody);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(mat4), &modelBody[0], GL_DYNAMIC_DRAW);
    cg_model_instance_setup(&fish->body);
    cg_model_instance_draw(&fish->body, shaderId, size);

    glBindBuffer(GL_ARRAY_BUFFER, fish->bufferTail);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(mat4), &modelTail[0], GL_DYNAMIC_DRAW);
    cg_model_instance_setup(&fish->tail);
    cg_model_instance_draw(&fish->tail, shaderId, size);
}

void cg_fish_destroy(Fish* f) {
    cg_model_destroy(&f->body);
    cg_model_destroy(&f->tail);

    glDeleteBuffers(2, &f->bufferBody); // body is the start of buffer of length 2
}
