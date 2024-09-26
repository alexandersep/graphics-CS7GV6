#include "graphics.h"

#include <stddef.h>

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

void cg_mesh_instance_setup(Mesh* mesh) {
    // Instance
    glBindVertexArray(mesh->VAO);

    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)0);
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(vec4)));
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(2 * sizeof(vec4)));
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(3 * sizeof(vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

static void cg_mesh_init(Mesh* mesh, Vertex* vertices, unsigned int* indices, Texture* textures) {
    mesh->vertices = NULL;
    mesh->indices = NULL;
    mesh->textures = NULL;
    for (unsigned int i = 0; i < arrlen(vertices); i++) {
        arrput(mesh->vertices, vertices[i]);
    }
    for (unsigned int i = 0; i < arrlen(indices); i++) {
        arrput(mesh->indices, indices[i]);
    }
    for (unsigned int i = 0; i < arrlen(textures); i++) {
        arrput(mesh->textures, textures[i]);
    }
}

void cg_mesh_create(Mesh* mesh, Vertex* vertices, unsigned int* indices, Texture* textures) {
    cg_mesh_init(mesh, vertices, indices, textures);
    cg_mesh_setup(mesh);
}

static void cg_mesh_texture_bind(Mesh* mesh, unsigned int shaderId) {
    unsigned int diffuse = 1;
    unsigned int specular = 1;
    unsigned int normal = 1;
    for (unsigned int i = 0; i < arrlen(mesh->textures); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        int number;
        char* type = mesh->textures[i].type;
        if (strcmp(type, "diffuse") == 0) {
            number = diffuse++;
        } else if (strcmp(type, "specular") == 0) {
            number = specular++;
        } else if (strcmp(type, "normal") == 0) {
            number = normal++;
        }

        char* name = NULL;
        char* tmp = "material.";
        for (char* c = tmp; *c != '\0'; c++) {
            arrput(name, *c);
        }
        for (char* c = type; *c != '\0'; c++) {
            arrput(name, *c);
        }
        //cg_tool_itoa(name, number);
        arrput(name, '\0');

        cg_shader_uniform1i(shaderId, name, i);
        arrfree(name);

        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
}

void cg_mesh_draw(Mesh* mesh, unsigned int shaderId) {
    cg_mesh_texture_bind(mesh, shaderId);
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, (unsigned int) arrlen(mesh->indices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void cg_mesh_instance_draw(Mesh* mesh, unsigned int shaderId, unsigned int numInstances) {
    cg_mesh_texture_bind(mesh, shaderId);
    glBindVertexArray(mesh->VAO);
    glDrawElementsInstanced(GL_TRIANGLES, (unsigned int) arrlen(mesh->indices), GL_UNSIGNED_INT, 0, numInstances);
    glBindVertexArray(0);
}

void cg_mesh_destroy(Mesh* mesh) {
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);
    arrfree(mesh->vertices);    
    arrfree(mesh->indices);    
    arrfree(mesh->textures);    
}
