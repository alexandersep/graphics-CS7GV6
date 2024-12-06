#include "graphics.h"

void cg_mantaray_create(Mantaray* m) {
    cg_model_create(&m->body, "res/models/man/body/body.obj");

    cg_model_create(&m->leftHandOne, "res/models/man/left-hand/one/one.obj");
    cg_model_create(&m->rightHandOne, "res/models/man/right-hand/one/one.obj");

    cg_model_create(&m->leftHandTwo, "res/models/man/left-hand/two/two.obj");
    cg_model_create(&m->rightHandTwo, "res/models/man/right-hand/two/two.obj");

    unsigned int buffers[3];
    glGenBuffers(3, buffers);
    m->bufferBody = buffers[0];
    m->bufferLeftHand = buffers[1];
    m->bufferRightHand = buffers[2];
}
static void cg_mantaray_models_instance_draw(Mantaray* m, unsigned int shaderId, Boids* boids) {
    vec3 forward = {0.0f, 0.0f, 1.0f};  // mantaray faces the positive z axis (right handed system opengl)

    size_t size = (boids->size / 2) + 1;
    mat4 modelBody[size];

    mat4 modelLeftHandOne[size];
    mat4 modelRightHandOne[size];

    mat4 modelLeftHandTwo[size];
    mat4 modelRightHandTwo[size];

    for (size_t i = 0, j = 0; i < boids->size; i+=2, j++) {
        glm_mat4_identity(modelBody[j]); // identity matrix

        glm_translate(modelBody[j], boids->boid[i].position); // translate boid to specific position

        mat4 rotation; // rotation matrix
        cg_boid_angle_update(&boids->boid[i], rotation, forward); // make boid face forward
        glm_mat4_mul(modelBody[j], rotation, modelBody[j]); // rotation * body to rotate body
        
        float time = glfwGetTime(); // timer to flap the hands of the Manta Ray
        float degrees = 10.0f;
        float angle = degrees * sin(time + boids->boid[i].time); // sin wave to give a constant flapping

        glm_mat4_identity(modelLeftHandOne[j]); // identity matrix
        glm_mat4_mul(modelBody[j], modelLeftHandOne[j], modelLeftHandOne[j]); // hierarchical animation (body * left hand 1)
        glm_rotate(modelLeftHandOne[j], glm_rad(angle), (vec3) {0.0f, 0.0f, 1.0f});

        glm_mat4_identity(modelLeftHandTwo[j]); // identity
        glm_mat4_mul(modelLeftHandOne[j], modelLeftHandTwo[j], modelLeftHandTwo[j]); // hiearchical animation (left hand 1 * left hand 2)
        glm_rotate(modelLeftHandTwo[j], glm_rad(angle / 10), (vec3) {0.0f, 0.0f, 1.0f});

        glm_mat4_identity(modelRightHandOne[j]);

        glm_mat4_mul(modelBody[j], modelRightHandOne[j], modelRightHandOne[j]);
        glm_rotate(modelRightHandOne[j], glm_rad(angle), (vec3) {0.0f, 0.0f, -1.0f});

        glm_mat4_identity(modelRightHandTwo[j]);
        glm_mat4_mul(modelRightHandOne[j], modelRightHandTwo[j], modelRightHandTwo[j]);
        glm_rotate(modelRightHandTwo[j], glm_rad(angle / 10), (vec3) {0.0f, 0.0f, -1.0f});
   }

    glBindBuffer(GL_ARRAY_BUFFER, m->bufferBody);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(mat4), &modelBody[0], GL_DYNAMIC_DRAW);
    cg_model_instance_setup(&m->body);
    cg_model_instance_draw(&m->body, shaderId, size);

    glBindBuffer(GL_ARRAY_BUFFER, m->bufferLeftHand);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(mat4), &modelLeftHandOne[0], GL_DYNAMIC_DRAW);
    cg_model_instance_setup(&m->leftHandOne);
    cg_model_instance_draw(&m->leftHandOne, shaderId, size);

    glBindBuffer(GL_ARRAY_BUFFER, m->bufferLeftHand);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(mat4), &modelLeftHandTwo[0], GL_DYNAMIC_DRAW);
    cg_model_instance_setup(&m->leftHandTwo);
    cg_model_instance_draw(&m->leftHandTwo, shaderId, size);

    glBindBuffer(GL_ARRAY_BUFFER, m->bufferRightHand);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(mat4), &modelRightHandOne[0], GL_DYNAMIC_DRAW);
    cg_model_instance_setup(&m->rightHandOne);
    cg_model_instance_draw(&m->rightHandOne, shaderId, size);

    glBindBuffer(GL_ARRAY_BUFFER, m->bufferRightHand);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(mat4), &modelRightHandTwo[0], GL_DYNAMIC_DRAW);
    cg_model_instance_setup(&m->rightHandTwo);
    cg_model_instance_draw(&m->rightHandTwo, shaderId, size);
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

    Model models[] = {m->leftHandOne, m->rightHandOne};
    float zRotation = 1.0f;
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

        cg_boids_rule1(b, i, v1);
        cg_boids_rule2(b, i, v2); 
        cg_boids_rule3(b, i, v3);

        glm_vec3_add(b->boid[i].velocity, v1, b->boid[i].velocity);
        glm_vec3_add(b->boid[i].velocity, v2, b->boid[i].velocity);
        glm_vec3_add(b->boid[i].velocity, v3, b->boid[i].velocity);

        cg_boid_velocity_limit(&b->boid[i]);
        glm_vec3_add(b->boid[i].position, b->boid[i].velocity, b->boid[i].position);
    }

    cg_mantaray_models_instance_draw(m, shaderId, b);
    cg_fish_models_instance_draw(fish, shaderId, b);
}

void cg_mantaray_destroy(Mantaray* m) {
    cg_model_destroy(&m->body);

    cg_model_destroy(&m->leftHandOne);
    cg_model_destroy(&m->rightHandOne);

    cg_model_destroy(&m->leftHandTwo);
    cg_model_destroy(&m->rightHandTwo);

    glDeleteBuffers(3, &m->bufferBody); // body is the start of buffer of length 3
}
