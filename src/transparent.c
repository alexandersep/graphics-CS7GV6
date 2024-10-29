#include "graphics.h"
#include <cglm/affine-pre.h>

float transparent_vertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.05f,  0.05f,
        0.0f, -0.5f,  0.0f,  0.05f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.05f,  0.05f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.05f
};

void cg_transparent_create(Transparent* t) {
    glGenVertexArrays(1, &t->vao);
    glGenBuffers(1, &t->vbo);
    glBindVertexArray(t->vao);
    glBindBuffer(GL_ARRAY_BUFFER, t->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparent_vertices), transparent_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void cg_transparent_draw(Transparent* t, unsigned int shaderId, Texture* tex, mat4* models, size_t size) {
    mat4 model;
    glBindVertexArray(t->vao);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    vec3 offset = {0.5f, 0.0f, 0.0f};
    for (size_t i = 0; i < size; i++) {
        cg_shader_uniform_matrix4fv(shaderId, "model", models[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        mat4 rotated_model;
        glm_mat4_copy(models[i], rotated_model);

        glm_rotate_y(rotated_model, glm_rad(90.0f), rotated_model);
        glm_translate(rotated_model, (vec3){ -offset[0], 0, 0.5});
        //glm_translate(rotated_model, offset);
        cg_shader_uniform_matrix4fv(shaderId, "model", rotated_model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void cg_transparent_positions(mat4* models, size_t size) {
    float radius = 5.0f;
    float scaling = 10;
    for (size_t i = 0; i < size; i++) {
        mat4 model;
        glm_mat4_identity(model);

        vec3 position = { ( (rand() % 90) - 45.0f) , -46.0f, ( (rand() % 90) - 45.0f ) };
        glm_translate(model, position);

        glm_scale(model, (vec3){scaling, scaling, scaling});

        glm_mat4_copy(model, models[i]);
    }
}

void cg_transparent_destroy(Transparent* t) {
    glDeleteBuffers(1, &t->vao);
    glDeleteBuffers(1, &t->vbo);
}
