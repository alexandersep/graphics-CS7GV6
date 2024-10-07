#include "graphics.h"

void cg_mantaray_create(Mantaray* m) {
    cg_model_create(&m->body, "res/models/man/body/body.obj");
    cg_model_create(&m->leftHand, "res/models/man/left-hand/left-hand.obj");
    cg_model_create(&m->rightHand, "res/models/man/right-hand/right-hand.obj");
}

static void cg_mantaray_models_draw(Mantaray* m, unsigned int shaderId, Boid* b, float angle) {
    mat4 model;
    glm_mat4_identity(model);

    glm_translate(model, b->position);

    mat4 rotation;
    vec3 forward = {0.0f, 0.0f, 1.0f};  // mantaray faces the positive z axis (right handed system opengl)
    cg_boid_angle_update(b, rotation, forward);
    glm_mat4_mul(model, rotation, model);

    cg_shader_uniform_matrix4fv(shaderId, "model", model);
    cg_model_draw(&m->body, shaderId);

    Model models[] = {m->leftHand, m->rightHand};
    int zRotation = 1.0f;
    for (size_t i = 0; i < 2; i++) {
        glm_mat4_identity(model);
        glm_translate(model, b->position);
        cg_boid_angle_update(b, rotation, forward);
        glm_mat4_mul(model, rotation, model);
        glm_rotate(model, glm_rad(angle), (vec3) {0.0f, 0.0f, zRotation});
        cg_shader_uniform_matrix4fv(shaderId, "model", model);
        cg_model_draw(&models[i], shaderId);
        zRotation = -zRotation;
    }
}

void cg_mantaray_boids_draw(Mantaray* m, Fish* fish, Boids* b, unsigned int shaderId) {
    vec3 v1, v2, v3;
    for (size_t i = 0; i < b->size; i++) {
        cg_boid_position_edge(&b->boid[i]);

        if (i % 2 == 0) { // every second boid is a fish
            const float time = glfwGetTime();
            const float degrees = 10.0f;
            float angle = degrees * sin(time + b->boid[i].time);
            cg_mantaray_models_draw(m, shaderId, &b->boid[i], angle);
        } else {
            cg_fish_model_draw(fish, &b->boid[i], shaderId);
        }

        cg_boids_rule1(b, i, v1);
        cg_boids_rule2(b, i, v2); 
        cg_boids_rule3(b, i, v3);

        glm_vec3_add(b->boid[i].velocity, v1, b->boid[i].velocity);
        glm_vec3_add(b->boid[i].velocity, v2, b->boid[i].velocity);
        glm_vec3_add(b->boid[i].velocity, v3, b->boid[i].velocity);

        cg_boid_velocity_limit(&b->boid[i]);
        glm_vec3_add(b->boid[i].position, b->boid[i].velocity, b->boid[i].position);
    }
}

void cg_mantaray_destroy(Mantaray* m) {
    cg_model_destroy(&m->body);
    cg_model_destroy(&m->leftHand);
    cg_model_destroy(&m->rightHand);
}
