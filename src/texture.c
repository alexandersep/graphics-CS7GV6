#include "graphics.h"
#include <string.h>

void cg_texture_create(unsigned int* textureId, int size) {
    glGenTextures(size, textureId);
}

static void cg_texture_parameter() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void cg_texture_load(const char* texturePath) {
    cg_texture_parameter();
    char* c = (char *)texturePath;
    for (; *c != '.'; c++); 
    c++;
    char extension[4];
    int i = 0;
    for (i = 0; *c != '\0'; c++) {
        extension[i] = *c;
        i++;
    }
    extension[i] = '\0';

    int glTypeColor = 0;
    if (strcmp(extension, "png") == 0) {
        glTypeColor = GL_RGBA;
    } else if (strcmp(extension, "jpg") == 0) {
        glTypeColor = GL_RGB;
    }

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data == NULL) {
        fprintf(stderr, "Error loading in texture: %s\n", texturePath);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, glTypeColor, width, height, 0, glTypeColor, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void cg_texture_bind(unsigned int* textureId, unsigned int index) {
    glBindTexture(GL_TEXTURE_2D, textureId[index]);
}

void cg_texture_use(unsigned int* textureId, unsigned int index) {
    int texture = GL_TEXTURE0 + index;
    if (index > 30) {
        fprintf(stderr, "Error exceeding allowable textures, Number of textures is %d", index + 1);
    }
    glActiveTexture(texture);
    cg_texture_bind(textureId, index);
}
