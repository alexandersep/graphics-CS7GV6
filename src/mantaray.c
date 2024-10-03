#include "graphics.h"

void cg_mantaray_create(Mantaray* m) {
    cg_model_create(&m->body, "res/models/man/body/body.obj");
    cg_model_create(&m->leftHand, "res/models/man/left-hand/left-hand.obj");
    cg_model_create(&m->rightHand, "res/models/man/right-hand/right-hand.obj");
}

static void cg_mantaray_models_draw(Mantaray* m, unsigned int shaderId, vec3 position, float angle) {
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3) {position[0], position[1], position[2]});
    cg_shader_uniform_matrix4fv(shaderId, "model", &model);
    cg_model_draw(&m->body, shaderId);

    glm_mat4_identity(model);
    glm_translate(model, (vec3) {position[0], position[1], position[2]});
    glm_rotate(model, glm_rad(angle), (vec3) {0.0f, 0.0f, 1.0f});
    cg_shader_uniform_matrix4fv(shaderId, "model", &model);
    cg_model_draw(&m->leftHand, shaderId);

    glm_mat4_identity(model);
    glm_translate(model, (vec3) {position[0], position[1], position[2]});
    glm_rotate(model, glm_rad(angle), (vec3) {0.0f, 0.0f, -1.0f});
    cg_shader_uniform_matrix4fv(shaderId, "model", &model);
    cg_model_draw(&m->rightHand, shaderId);
}

void cg_mantaray_boids_draw(Mantaray* m, Boids* b, unsigned int shaderId) {
    vec3 v1, v2, v3, v4;
    for (size_t i = 0; i < b->size; i++) {
        cg_boid_position_edge(&b->boid[i]);

        const float time = glfwGetTime();
        const float degrees = 10.0f;
        float angle = degrees * sin(time + b->boid[i].time);
        cg_mantaray_models_draw(m, shaderId, b->boid[i].position, angle);

        cg_boids_rule1(b, i, v1);
        cg_boids_rule2(b, i, v2); 
        cg_boids_rule3(b, i, v3);

        glm_vec3_add(b->boid[i].velocity, v1, b->boid[i].velocity);
        glm_vec3_add(b->boid[i].velocity, v2, b->boid[i].velocity);
        glm_vec3_add(b->boid[i].velocity, v3, b->boid[i].velocity);

        //cg_boid_position_bound(&b->boid[i], v4);
        //glm_vec3_add(b->boid[i].velocity, v4, b->boid[i].velocity);

        cg_boid_velocity_limit(&b->boid[i]);
        glm_vec3_add(b->boid[i].position, b->boid[i].velocity, b->boid[i].position);
    }
}

void cg_mantaray_destroy(Mantaray* m) {
    cg_model_destroy(&m->body);
    cg_model_destroy(&m->leftHand);
    cg_model_destroy(&m->rightHand);
}
