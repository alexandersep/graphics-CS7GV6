#include "graphics.h"

//void cg_mesh_create(Mesh* mesh, Vertex* vertices, unsigned int* indices, Texture* textures) {

/*
static void cg_mesh_setup(Mesh* mesh) {
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);
  
    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

    glBufferData(GL_ARRAY_BUFFER, arrlen(mesh->vertices) * sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlen(mesh->indices) * sizeof(unsigned int), 
                 &mesh->indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
*/

static void cg_cube_setup(Cube* cube) {
    glGenVertexArrays(1, &cube->VAO);
    glGenBuffers(1, &cube->VBO);
    glGenBuffers(1, &cube->EBO);
  
    // bind VAO
    glBindVertexArray(cube->VAO);

    // copy our vertices in an OpenGL buffer 
    glBindBuffer(GL_ARRAY_BUFFER, cube->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube->vertices), cube->vertices, GL_STATIC_DRAW);  

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

}

void cg_cube_create(Cube* cube) {
    /*
    cube->vertices[0] = -0.5;
    cube->vertices[1] = -0.5;
    cube->vertices[2] = 0.0;

    cube->vertices[3] = -0.5;
    cube->vertices[4] = 0.5;
    cube->vertices[5] = 0.0;

    cube->vertices[6] = 0.5;
    cube->vertices[7] = -0.5;
    cube->vertices[8] = 0.0;
    */
    glm_vec3_copy((vec3) { 0.5f, -0.5f, 0.0f},  cube->vertices[0][0]);
    glm_vec3_copy((vec3) {-0.5f,  0.5f, 0.0f},  cube->vertices[0][1]);
    glm_vec3_copy((vec3) {-0.5f, -0.5f, 0.0f},  cube->vertices[0][2]);

    glm_vec3_copy((vec3) {-0.5f,  0.5f, 0.0f},  cube->vertices[1][0]);
    glm_vec3_copy((vec3) { 0.5f, -0.5f, 0.0f},  cube->vertices[1][1]);
    glm_vec3_copy((vec3) { 0.5f,  0.5f, 0.0f},  cube->vertices[1][2]);

    cg_cube_setup(cube);
}
