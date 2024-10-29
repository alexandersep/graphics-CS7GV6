#include "graphics.h"

float skybox_vertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

static void cg_cubemap_init(CubeMap* cubeMap) {
    glGenVertexArrays(1, &cubeMap->VAO);
    glGenBuffers(1, &cubeMap->VBO);
    glBindVertexArray(cubeMap->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeMap->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void cg_cubemap_create(CubeMap* cubeMap) {
    cg_cubemap_init(cubeMap);
    const char* faces[] = { 
        "res/textures/skybox-water/right.png", 
        "res/textures/skybox-water/left.png", 
        "res/textures/skybox-water/top.png", 
        "res/textures/skybox-water/bottom.png", 
        "res/textures/skybox-water/front.png", 
        "res/textures/skybox-water/back.png", 
    };

    glGenTextures(1, &cubeMap->textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->textureId);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++) {
        unsigned char *data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
        if (data == NULL) {
            fprintf(stderr, "Error loading in texture: %s\n", faces[i]);
        } else {
            GLenum glTypeColor = 0;
            if (nrChannels == 1) {
                glTypeColor = GL_RED;
            } else if (nrChannels == 3) {
                glTypeColor = GL_RGB;
            } else if (nrChannels == 4) {
                glTypeColor = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, glTypeColor, width, height, 0, glTypeColor, GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void cg_cubemap_draw(CubeMap* cubeMap) {
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(cubeMap->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->textureId);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void cg_cubemap_model_draw(CubeMap* cubeMap, Model* model, unsigned int shaderId) {
    for (unsigned int i = 0; i < arrlen(model->meshes); i++) {
        glBindVertexArray(model->meshes[i].VAO);
        cg_mesh_texture_bind(&model->meshes[i], shaderId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->textureId);
        glDrawElements(GL_TRIANGLES, (unsigned int) arrlen(model->meshes[i].indices), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void cg_cubemap_destroy(CubeMap* cubeMap) {
    glDeleteBuffers(1, &cubeMap->VAO);
    glDeleteBuffers(1, &cubeMap->VBO);
}
