#include "graphics.h"
#include <cglm/affine.h>

void cg_fish_create(Fish* f) {
    cg_model_create(&f->body, "res/models/fish/body/body.obj");
    cg_model_create(&f->tail, "res/models/fish/tail/tail.obj");
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

void cg_fish_destroy(Fish* f) {
    cg_model_destroy(&f->body);
    cg_model_destroy(&f->tail);
}
