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
    for (size_t i = 1; i < boids->size; i+=2, j++) {
        glm_mat4_identity(modelBody[j]);
        glm_translate(modelBody[j], boids->boid[i].position);

        cg_boid_angle_update(&boids->boid[i], rotation, forward);
        glm_mat4_mul(modelBody[j], rotation, modelBody[j]);


        glm_mat4_identity(modelTail[j]);
        glm_translate(modelTail[j], boids->boid[i].position);

        cg_boid_angle_update(&boids->boid[i], rotation, forward);
        glm_mat4_mul(modelTail[j], rotation, modelTail[j]);
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

void cg_starfish_positions(mat4* models, size_t size) {
    float radius = 5.0f;
    for (size_t i = 0; i < size; i++) {
        mat4 model;
        glm_mat4_identity(model);
        glm_mat4_scale(model, 20);
        vec3 position = { ( (rand() % 100) - 50.0f) / 20, (-52.0f) / 20, ( (rand() % 100) - 50.0f ) / 20};
        glm_translate(model, position);
        glm_mat4_copy(model, models[i]);
    }
}
