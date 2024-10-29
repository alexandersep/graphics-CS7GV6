#include "graphics.h"

float quad_vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,   1.0f,  0.0f, 1.0f,
    -1.0f,  -1.0f,  0.0f, 0.0f,
     1.0f,  -1.0f,  1.0f, 0.0f,

    -1.0f,   1.0f,  0.0f, 1.0f,
     1.0f,  -1.0f,  1.0f, 0.0f,
     1.0f,   1.0f,  1.0f, 1.0f
};

void cg_screen_quad_create(Quad* q) {
    glGenVertexArrays(1, &q->vao);
    glGenBuffers(1, &q->vbo);
    glBindVertexArray(q->vao);
    glBindBuffer(GL_ARRAY_BUFFER, q->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void cg_screen_quad_draw(Quad* q, Texture* tex) {
    glBindVertexArray(q->vao);
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void cg_screen_quad_destroy(Quad* q) {
    glDeleteBuffers(1, &q->vao);
    glDeleteBuffers(1, &q->vbo);
}
