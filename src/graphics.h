#ifndef GRAPHICS_H 
#define GRAPHICS_H 

#include <stdalign.h>
#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "stb_image.h"
#include "stb_ds.h"

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define CG_SCREEN_X 1920
#define CG_SCREEN_Y 1200

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
    vec3 pos;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldup;
    float speed;
    float zoom;
    Mouse mouse;
    EulerAngle angle;
    float deltaTime;
} Camera;

typedef struct {
    Camera* camera; // a list of cameras
    size_t focus;
    size_t size;
} Cameras;

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

typedef struct {
    Model body;
    Model tail;

    unsigned int bufferBody;
    unsigned int bufferTail;
} Fish;

typedef struct {
    Model body;
    size_t size;
    unsigned int bufferBody;
} GenericModel;

typedef struct {
    Model body;
    Model leftHandOne;
    Model rightHandOne;

    Model leftHandTwo;
    Model rightHandTwo;

    unsigned int bufferBody;
    unsigned int bufferLeftHand;
    unsigned int bufferRightHand;
} Mantaray;

typedef struct {
    vec3 position;
    vec3 velocity;
    float time;
    float maxSpeed;
    float maxForce;
    versor orientation;
} Boid;

typedef struct {
    Boid* boid;
    size_t size;
} Boids;

typedef struct {
    unsigned int textureId;
    unsigned int VAO;
    unsigned int VBO;
} CubeMap;

typedef struct {
    unsigned int fbo;
} FrameBuffer;

typedef struct {
    unsigned int rbo;
} RenderBuffer;

typedef struct {
    unsigned int vao;
    unsigned int vbo;
} Quad;

typedef struct {
    vec3* arr;
    unsigned int vao;
    unsigned int vbo;
} Transparent;

// control
GLFWwindow* cg_control_window_create(int width, int height, const char* title);
void cg_control_camera_create(Camera* camera, float speed, vec3 pos);
void cg_control_camera_move(GLFWwindow* window, float deltaTime);
void cg_control_angle_update(EulerAngle* angle, vec3 front, vec3 worldup, vec3 right, vec3 up);

// Shader
unsigned int cg_shader_create(const char* vertexPath, const char* fragmentPath);
void cg_shader_use(unsigned int programId);
void cg_shader_uniform1i(unsigned int programId, const char* name, int x);
void cg_shader_uniform2i(unsigned int programId, const char* name, int x, int y);
void cg_shader_uniform3i(unsigned int programId, const char* name, int x, int y, int z);
void cg_shader_uniform1f(unsigned int programId, const char* name, float x);
void cg_shader_uniform2f(unsigned int programId, const char* name, float x, float y);
void cg_shader_uniform3f(unsigned int programId, const char* name, float x, float y, float z);
void cg_shader_uniform_matrix4fv(unsigned int programId, const char* name, mat4 projection);
void cg_shader_light_pointLights(unsigned int programId, vec3* points, unsigned int size);
void cg_shader_destroy(unsigned int programId);

// File
// Returns int size, and first parameter src code
size_t cg_file_read(char** src, const char* srcPath);
int cg_file_load_scene(const char* filePath);

// Texture
void cg_texture_create(Texture* tex, const char* texturePath);
void cg_texture_attachment_create(Texture* tex);
void cg_texture_destroy(Texture* tex);

// Mesh
void cg_mesh_create(Mesh* mesh, Vertex* vertices, unsigned int* indices, Texture* textures);
void cg_mesh_texture_bind(Mesh* mesh, unsigned int shaderId);
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

// Boids
void cg_boids_create(Boids* boids, size_t size);
void cg_boids_destroy(Boids* boids);
void cg_boids_initialize(Boids* b, size_t size);
void cg_boids_rule1(Boids* b, size_t j, vec3 v);
void cg_boids_rule2(Boids* b, size_t j, vec3 v);
void cg_boids_rule3(Boids* b, size_t j , vec3 v);
// Boid
void cg_boid_velocity_limit(Boid* b);
void cg_boid_position_bound(Boid* b, vec3 v);
void cg_boid_position_edge(Boid* b);
void cg_boid_angle_update(Boid* b, mat4 model, vec3 forward);

// Mantray
void cg_mantaray_position_set(Mantaray* m, vec3 position);
void cg_mantaray_create(Mantaray* m);
void cg_mantaray_boids_draw(Mantaray* m, Fish* fish, Boids* b, unsigned int shaderId);
void cg_mantaray_destroy(Mantaray* mantray);

// Fish
void cg_fish_create(Fish* f);
void cg_fish_model_draw(Fish* f, Boid* b, unsigned int shaderId);
void cg_fish_models_instance_draw(Fish* fish, unsigned int shaderId, Boids* b);
void cg_fish_destroy(Fish* f);

// Generic Model
void cg_generic_model_create(GenericModel* g, const char* filePath, mat4* models, size_t size);
void cg_generic_model_instance_draw(GenericModel* g, unsigned int shaderId, mat4* models, size_t size);
void cg_generic_model_positions(mat4* models, size_t size);
void cg_generic_model_destroy(GenericModel* g);

// CubeMap
void cg_cubemap_create(CubeMap* cubeMap);
void cg_cubemap_draw(CubeMap* cubeMap);
void cg_cubemap_model_draw(CubeMap* cubeMap, Model* model, unsigned int shaderId);
void cg_cubemap_destroy(CubeMap* cubeMap);

// FrameBuffer
void cg_framebuffer_create(FrameBuffer* f);
void cg_framebuffer_texture_attach(FrameBuffer* f, Texture* tex);
void cg_framebuffer_bind(FrameBuffer* f);
void cg_framebuffer_default_bind();
void cg_framebuffer_destroy(FrameBuffer* f);

// RenderBuffer
void cg_renderbuffer_create(RenderBuffer* r);
void cg_renderbuffer_bind(RenderBuffer* r);
void cg_renderbuffer_destroy(RenderBuffer* r);

// Screen
// Quad
void cg_screen_quad_create(Quad* q);
void cg_screen_quad_destroy(Quad* q);
void cg_screen_quad_draw(Quad* q, Texture* tex);

// Transparent
void cg_transparent_create(Transparent* t);
void cg_transparent_locations(Transparent* t, vec3* locations);
void cg_transparent_draw(Transparent* t, unsigned int shaderId, Texture* tex, mat4* models, size_t size);
void cg_transparent_positions(mat4* models, size_t size);
void cg_transparent_destroy(Transparent* t);

#endif // GRAPHICS_H 
