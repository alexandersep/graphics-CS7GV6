#ifndef GRAPHICS_H 
#define GRAPHICS_H 

#define GLFW_INCLUDE_NONE
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "cglm/cglm.h"
#include "stb_image.h"

#define CG_SCREEN_X 400
#define CG_SCREEN_Y 400

// Types
#define TRUE GL_TRUE
#define FALSE GL_FALSE

#define GREEN 0x0000FF
#define BLUE 0x00FF00
#define RED 0xFF0000

enum KeyMap {
    CG_CLOSE = GLFW_KEY_ESCAPE,
    CG_FORWARD = GLFW_KEY_W,
    CG_BACKWARD = GLFW_KEY_S,
    CG_LEFT = GLFW_KEY_A,
    CG_RIGHT = GLFW_KEY_D
};

typedef struct {
    float yaw;
    float pitch;
    float roll;
} EulerAngle;

// Camera
typedef struct {
    float speed;
    float zoom;
    float sensitivity;
    EulerAngle angle;
    vec3 pos;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldup;
} Camera;

// control
GLFWwindow* cg_control_window_create(Camera* camera, int width, int height, const char* title);
void cg_control_camera_create(Camera* camera, float speed);
float cg_control_time_get_delta();
float cg_control_time_get_last_frame();
void cg_control_time_set_delta(float dt);
void cg_control_time_set_last_frame(float lf);

// Shader
unsigned int cg_shader_create(const char* vertexPath, const char* fragmentPath);
void cg_shader_use(unsigned int programId);
void cg_shader_uniform1i(unsigned int programId, const char* programName, int samplerId);
void cg_shader_destroy(unsigned int programId);

// File
// Returns int size, and first parameter src code
size_t cg_read_file(char** shaderSrc, const char* shaderPath);

// Texture
void cg_texture_create(unsigned int* textureIds, int size);
void cg_texture_load(const char* texturePath);
void cg_texture_use(unsigned int textureId);

#endif // GRAPHICS_H 
