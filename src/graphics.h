#ifndef GRAPHICS_H 
#define GRAPHICS_H 

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "stb_image.h"
#include "stb_ds.h"

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define CG_SCREEN_X 400
#define CG_SCREEN_Y 400

// Types
#define TRUE GL_TRUE
#define FALSE GL_FALSE

#define GREEN 0x0000FF
#define BLUE 0x00FF00
#define RED 0xFF0000

enum KeyMap {
    CLOSE1 = GLFW_KEY_ESCAPE,
    CLOSE2 = GLFW_KEY_CAPS_LOCK,
    FORWARD = GLFW_KEY_W,
    BACKWARD = GLFW_KEY_S,
    LEFT = GLFW_KEY_A,
    RIGHT = GLFW_KEY_D,
    UP = GLFW_KEY_SPACE,
    DOWN = GLFW_KEY_LEFT_SHIFT,
};

// Camera
typedef struct {
    float yaw;
    float pitch;
    float roll;
} EulerAngle;

typedef struct {
    float xpos;
    float ypos;
    float sensitivity;
    bool focus;
} Mouse;

typedef struct {
    float speed;
    float zoom;
    EulerAngle angle;
    Mouse mouse;
    vec3 pos;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldup;
    float deltaTime;
} Camera;

typedef struct {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
} Vertex;

typedef struct {
    unsigned int id;
    char* type;
    char path[255];
} Texture;

// Mesh
typedef struct {
    Vertex* vertices;
    unsigned int* indices;
    Texture* textures;
    unsigned int VAO, VBO, EBO;
} Mesh;

typedef struct {
    Mesh* meshes;
    Texture* texturesLoaded;
    char directory[255];
    size_t directoryLength;
} Model;

// Cube
typedef struct {
    vec3 vertices[2][3]; // 1 triangle
    unsigned int VAO, VBO, EBO;
} Cube;

// control
GLFWwindow* cg_control_window_create(Camera* camera, int width, int height, const char* title);
void cg_control_camera_create(Camera* camera, float speed);
void cg_control_camera_move(GLFWwindow* window, float deltaTime);

// Shader
unsigned int cg_shader_create(const char* vertexPath, const char* fragmentPath);
void cg_shader_use(unsigned int programId);
void cg_shader_uniform1i(unsigned int programId, const char* name, int x);
void cg_shader_uniform2i(unsigned int programId, const char* name, int x, int y);
void cg_shader_uniform3i(unsigned int programId, const char* name, int x, int y, int z);
void cg_shader_uniform1f(unsigned int programId, const char* name, float x);
void cg_shader_uniform2f(unsigned int programId, const char* name, float x, float y);
void cg_shader_uniform3f(unsigned int programId, const char* name, float x, float y, float z);
void cg_shader_uniform_matrix4fv(unsigned int programId, const char* name, mat4* projection);
void cg_shader_light_pointLights(unsigned int programId, vec3* points, unsigned int size);
void cg_shader_destroy(unsigned int programId);

// File
// Returns int size, and first parameter src code
size_t cg_file_read(char** src, const char* srcPath);
int cg_file_load_scene(const char* filePath);

// Texture
unsigned int cg_texture_load(const char* texturePath);

// Mesh
void cg_mesh_create(Mesh* mesh, Vertex* vertices, unsigned int* indices, Texture* textures);
void cg_mesh_draw(Mesh* mesh, unsigned int shaderId);
void cg_mesh_instance_draw(Mesh* mesh, unsigned int shaderId, unsigned int numInstances);
void cg_mesh_instance_setup(Mesh* mesh);
void cg_mesh_destroy(Mesh* mesh);

// Mesh
void cg_model_create(Model* model, const char* filePath);
void cg_model_load(Model* model, const char* filePath);
void cg_model_draw(Model* model, unsigned int shaderId);
void cg_model_instance_setup(Model* model);
void cg_model_instance_draw(Model* model, unsigned int shaderId, unsigned int numInstances);
void cg_model_destroy(Model* model);

// Tool
void cg_tool_itoa(char* str, int n);

// cube
void cg_cube_create(Cube* cube);

#endif // GRAPHICS_H 
