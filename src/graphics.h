#ifndef GRAPHICS_H 
#define GRAPHICS_H 

#define GLFW_INCLUDE_NONE
#include "glad/gl.h"
#include "stb_image.h"

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

// Camera
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
} Camera;

typedef struct {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int size;
} Mesh;

// control
GLFWwindow* cg_control_window_create(Camera* camera, int width, int height, const char* title);
void cg_control_camera_create(Camera* camera, float speed);
void cg_control_camera_move(GLFWwindow* window, float deltaTime);

// Shader
unsigned int cg_shader_create(const char* vertexPath, const char* fragmentPath);
void cg_shader_use(unsigned int programId);
void cg_shader_uniform1i(unsigned int programId, const char* programName, int samplerId);
void cg_shader_uniform3f(unsigned int programId, const char* name, float x, float y, float z);
void cg_shader_uniform_matrix4fv(unsigned int programId, const char* name, mat4* projection);
void cg_shader_destroy(unsigned int programId);

// File
// Returns int size, and first parameter src code
size_t cg_file_read(char** src, const char* srcPath);
int cg_file_load_scene(const char* filePath);

// Texture
void cg_texture_create(unsigned int* textureIds, int size);
void cg_texture_load(const char* texturePath);
void cg_texture_bind(unsigned int* textureId, unsigned int index);
void cg_texture_use(unsigned int* textureId, unsigned int index);

#endif // GRAPHICS_H 
