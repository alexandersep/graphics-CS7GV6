#include "graphics.h"

static void cg_boid_initialize(Boid* b) {
    float x = (rand() % 100) - 50;
    float y = (rand() % 100) - 50;
    float z = (rand() % 100) - 50;
    glm_vec3_copy((vec3){x,-y,-z}, b->position);

    x = (rand() % 50) - 25;
    y = (rand() % 50) - 25;
    z = (rand() % 20) - 10;
    glm_vec3_copy((vec3){x, y, z}, b->velocity);

    b->time = ((float)rand() / RAND_MAX) * 2.0f * GLM_PI;
    b->maxForce = 1.0f;
    b->maxSpeed = 0.025f;
    glm_quat_identity(b->orientation);
}

void cg_boids_create(Boids* boids, size_t size) {
    boids->boid = malloc(sizeof(Boid) * size);
    for (int i = 0; i < size; i++) {
        cg_boid_initialize(&boids->boid[i]);
    }
    boids->size = size;
}

// Boids try to swim towards the perceived centre of mass of neighbouring boids
// Cohesion
void cg_boids_rule1(Boids* b, size_t j, vec3 v) {
    float perception = 5.0f;
    vec3 steering = {0.0f, 0.0f, 0.0f};
    size_t total = 0;
    for (size_t i = 0 ; i < b->size; i++) {
        if (i != j) {
            float distance = glm_vec3_distance(
                    b->boid[j].position, 
                    b->boid[i].position
            );
            if (distance < perception) {
                glm_vec3_add(steering, b->boid[i].position, steering);
                total++;
            }
        }
    }
    if (total > 0) {
        glm_vec3_divs(steering, total, steering);
        glm_vec3_sub(steering, b->boid[j].position, steering);
        glm_vec3_normalize(steering);
        glm_vec3_scale(steering, b->boid[j].maxSpeed, steering);
        glm_vec3_sub(steering, b->boid[j].velocity, steering);

        float steeringMagnitude = glm_vec3_norm(steering);
        if (steeringMagnitude > b->boid[j].maxForce) {
            glm_vec3_normalize(steering);
            glm_vec3_scale(steering, b->boid[j].maxForce, steering);
        }
    }

    glm_vec3_copy(steering, v);
}

// Boids try to keep a small distance away from other objects (including boids)
// Seperation
void cg_boids_rule2(Boids* b, size_t j, vec3 v) {
    float perception = 15.0f;
    vec3 steering = {0.0f, 0.0f, 0.0f};
    size_t total = 0;
    for (size_t i = 0 ; i < b->size; i++) {
        if (i != j) {
            float distance = glm_vec3_distance(
                    b->boid[j].position, 
                    b->boid[i].position
            );
            if (distance < perception) {
                vec3 diff;
                glm_vec3_sub(b->boid[j].position, b->boid[i].position, diff);
                glm_vec3_divs(diff, distance*distance, diff);
                glm_vec3_add(steering, diff, steering);
                total++;
            }
        }
    }
    if (total > 0) {
        glm_vec3_divs(steering, total, steering);
        glm_vec3_normalize(steering);
        glm_vec3_scale(steering, b->boid[j].maxSpeed, steering);
        glm_vec3_sub(steering, b->boid[j].velocity, steering);

        float steeringMagnitude = glm_vec3_norm(steering);
        if (steeringMagnitude > b->boid[j].maxForce) {
            glm_vec3_normalize(steering);
            glm_vec3_scale(steering, b->boid[j].maxForce, steering);
        }
    }

    glm_vec3_copy(steering, v);
}

// Boids try to match velocity with near boids
// Alignment
void cg_boids_rule3(Boids* b, size_t j , vec3 v) {
    float perception = 10.0f;
    vec3 steering = {0.0f, 0.0f, 0.0f};
    size_t total = 0;
    for (size_t i = 0 ; i < b->size; i++) {
        if (i != j) {
            float distance = glm_vec3_distance(
                    b->boid[j].position, 
                    b->boid[i].position
            );
            if (distance < perception) {
                glm_vec3_add(steering, b->boid[i].velocity, steering);
                total++;
            }
        }
    }
    if (total > 0) {
        glm_vec3_divs(steering, total, steering);
        glm_vec3_normalize(steering);
        glm_vec3_scale(steering, b->boid[j].maxSpeed, steering);
        glm_vec3_sub(steering, b->boid[j].velocity, steering);

        float steeringMagnitude = glm_vec3_norm(steering);
        if (steeringMagnitude > b->boid[j].maxForce) {
            glm_vec3_normalize(steering);
            glm_vec3_scale(steering, b->boid[j].maxForce, steering);
        }
    }

    glm_vec3_copy(steering, v);
}

void cg_boid_position_edge(Boid* b) {
    int distance = 45;

    // X-axis limits
    if (b->position[0] > distance) {
        b->position[0] = distance;
        b->velocity[0] *= -1;  // Reverse velocity in x direction
    } else if (b->position[0] < -distance) {
        b->position[0] = -distance;
        b->velocity[0] *= -1;
    }

    // Y-axis limits
    if (b->position[1] > distance) {
        b->position[1] = distance;
        b->velocity[1] *= -1;  // Reverse velocity in y direction
    } else if (b->position[1] < -distance) {
        b->position[1] = -distance;
        b->velocity[1] *= -1;
    }

    // Z-axis limits
    if (b->position[2] > distance) {
        b->position[2] = distance;
        b->velocity[2] *= -1;  // Reverse velocity in z direction
    } else if (b->position[2] < -distance) {
        b->position[2] = -distance;
        b->velocity[2] *= -1;
    }
}

void cg_boid_angle_update(Boid* b, mat4 model, vec3 forward) {
    vec3 normVelocity;
    glm_vec3_normalize_to(b->velocity, normVelocity);

    vec3 rotation;
    glm_vec3_cross(forward, normVelocity, rotation);
    glm_vec3_normalize(rotation);

    float dot = glm_vec3_dot(forward, normVelocity);
    float angle = acos(dot);

    versor quaternion; // unit vector of norm one, x, y, z, w (unlike glm)
    glm_quatv(quaternion, angle, rotation);

    float slerp = 0.01f;
    versor interpolation;
    glm_quat_slerp(b->orientation, quaternion, slerp, interpolation);

    glm_quat_mat4(interpolation, model);
    glm_quat_copy(interpolation, b->orientation);
}


void cg_boids_destroy(Boids* boids) {
    free(boids->boid);
}

void cg_boid_velocity_limit(Boid* b) {
    float vlim = b->maxSpeed;
    float magnitude = glm_vec3_norm(b->velocity);
    if (magnitude > vlim) {
        glm_vec3_scale(b->velocity, vlim / magnitude, b->velocity);
    }
}
