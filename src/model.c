#include "graphics.h"

typedef struct aiScene aiScene;
typedef struct aiMesh aiMesh;
typedef struct aiNode aiNode;
typedef struct aiFace aiFace;
typedef struct aiMaterial aiMaterial;
typedef struct aiString aiString;
typedef enum aiTextureType aiTextureType;

static void cg_model_load_material_textures(Model* model, Texture** textures, aiMaterial* mat, aiTextureType type, char* typeName) {
    for (unsigned int i = 0; i < aiGetMaterialTextureCount(mat, type); i++) {
        aiString str;
        aiGetMaterialTexture(mat, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);
        int skip = 0;
        for (unsigned int j = 0; j < arrlen(model->texturesLoaded); j++) {
            if (strcmp(model->texturesLoaded[j].path, str.data) == 0) {
                arrput(*textures, model->texturesLoaded[j]);
                skip = 1;
                break;
            }
        }
        if (!skip) {
            Texture texture;

            size_t strLen = str.length;
            size_t directoryLen = model->directoryLength;
            char filePath[128];
            strncpy(filePath, model->directory, directoryLen);
            filePath[directoryLen++] = '/';
            int i;
            for (i = 0; i < strLen; i++) {
                filePath[i + directoryLen] = str.data[i];
            }
            filePath[i + directoryLen] = '\0';
            
            unsigned int textureId = cg_texture_load(filePath);

            texture.id = textureId;
            texture.type = typeName;
            strncpy(texture.path, str.data, str.length + 1);
            arrput(*textures, texture);
            arrput(model->texturesLoaded, texture);
        }
    }
}

static void cg_model_process_mesh(Model* model, Mesh* m, aiMesh* mesh, const aiScene* scene) {
    Vertex* vertices = NULL;
    unsigned int* indices = NULL;
    Texture* textures = NULL;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vec3 vector = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        glm_vec3_copy(vector, vertex.position);
        if (mesh->mNormals != NULL && mesh->mNumVertices > 0) { // mesh->HasNormals()
            vector[0] = mesh->mNormals[i].x;
            vector[1] = mesh->mNormals[i].y;
            vector[2] = mesh->mNormals[i].z;
            glm_vec3_copy(vector, vertex.normal);
        }
        if (mesh->mTextureCoords[0]) {
            vec2 texCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            glm_vec2_copy(texCoords, vertex.texCoords);
        } else {
            vec2 texCoords = {0, 0};
            glm_vec2_copy(texCoords, vertex.texCoords);
        }
        arrput(vertices, vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            arrput(indices, face.mIndices[j]);
        }
    }
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    Texture *diffuseMaps = NULL;
    Texture *specularMaps = NULL;
    Texture *normalMaps = NULL;
    cg_model_load_material_textures(model, &diffuseMaps, material, aiTextureType_DIFFUSE, "diffuse");
    for (unsigned int i = 0; i < arrlen(diffuseMaps); i++) {
        arrput(textures, diffuseMaps[i]);
    }
    cg_model_load_material_textures(model, &specularMaps, material, aiTextureType_SPECULAR, "specular");
    for (unsigned int i = 0; i < arrlen(specularMaps); i++) {
        arrput(textures, specularMaps[i]);
    }
    cg_model_load_material_textures(model, &normalMaps, material, aiTextureType_HEIGHT, "normal");
    for (unsigned int i = 0; i < arrlen(normalMaps); i++) {
        arrput(textures, normalMaps[i]);
    }
    arrfree(diffuseMaps);
    arrfree(specularMaps);
    arrfree(normalMaps);

    //printf("should be printing path %s\n", textures[0].path);
    cg_mesh_create(m, vertices, indices, textures);
    arrfree(vertices);
    arrfree(indices);
    arrfree(textures);
}

static void cg_model_process_node(Model* model, aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh m;
        cg_model_process_mesh(model, &m, mesh, scene);
        arrput(model->meshes, m);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        cg_model_process_node(model, node->mChildren[i], scene);
    }
}

void cg_model_load(Model* model, const char* filePath) {
    const aiScene* scene = aiImportFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
   if (!scene) {
       fprintf(stderr, "Failed to open file using assimp: %s\n", aiGetErrorString());
   }
   const char *slash = strrchr(filePath, '/');
   if (slash == NULL) {
       model->directory[0] = '\0';
       model->directoryLength = 0;
   } else {
       size_t len = slash - filePath;
       strncpy(model->directory, filePath, len);
       model->directory[len] = '\0';
       model->directoryLength = len;
   }

   cg_model_process_node(model, scene->mRootNode, scene);
   aiReleaseImport(scene);
}

void cg_model_create(Model* model, const char* filePath) {
    model->texturesLoaded = NULL;
    model->meshes = NULL;
    cg_model_load(model, filePath);
}

void cg_model_draw(Model* model, unsigned int shaderId, unsigned int numInstances) {
    for (unsigned int i = 0; i < arrlen(model->meshes); i++) {
        cg_mesh_draw(&model->meshes[i], shaderId, numInstances);
    }
}

void cg_model_destroy(Model* model) {
    for (int i = 0; i < arrlen(model->meshes); i++) {
        cg_mesh_destroy(&model->meshes[i]);
    }
    arrfree(model->meshes);
    arrfree(model->texturesLoaded);
}
