#include "graphics.h"

static void cg_texture_parameter() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void cg_texture_create(Texture* tex, const char* texturePath) {

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    glGenTextures(1, &tex->id);

    if (data == NULL) {
        fprintf(stderr, "Error loading in texture: %s\n", texturePath);
    } else {
        GLenum glTypeColor = 0;
        if (nrChannels == 1) {
            glTypeColor = GL_RED;
        } else if (nrChannels == 3) {
            glTypeColor = GL_RGB;
        } else if (nrChannels == 4) {
            glTypeColor = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, tex->id);
        glTexImage2D(GL_TEXTURE_2D, 0, glTypeColor, width, height, 0, glTypeColor, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glTypeColor == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glTypeColor == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    stbi_image_free(data);
}

void cg_texture_attachment_create(Texture* tex) {
    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, CG_SCREEN_X, CG_SCREEN_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
}

void cg_texture_destroy(Texture* tex) {
    glDeleteBuffers(1, &tex->id);
}
