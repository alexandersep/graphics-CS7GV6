#include "graphics.h"

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    camera->zoom -= yoffset;
    if (camera->zoom < 1.0f) {
        camera->zoom = 1.0f;
    }
    if (camera->zoom > 45.0f) {
        camera->zoom = 45.0f;
    }
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float cg_control_time_get_delta() {
    return deltaTime;
}

float cg_control_time_get_last_frame() {
    return lastFrame;
}

void cg_control_time_set_delta(float dt) {
    deltaTime = dt;
}

void cg_control_time_set_last_frame(float lf) {
    lastFrame = lf;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);

    float velocity = camera->speed * deltaTime;
    switch(key) {
        case CG_CLOSE: {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        } break;
        case CG_FORWARD: {
            glm_vec3_muladds(camera->front, velocity, camera->pos);
        } break;
        case CG_BACKWARD: {
            glm_vec3_muladds(camera->front, -velocity, camera->pos);
        } break;
        case CG_LEFT: {
            vec3 cross;
            glm_vec3_cross(camera->front, camera->up, cross);
            glm_vec3_normalize(cross);
            glm_vec3_muladds(cross, -velocity, camera->pos);
        } break;
        case CG_RIGHT: {
            vec3 cross;
            glm_vec3_cross(camera->front, camera->up, cross);
            glm_vec3_normalize(cross);
            glm_vec3_muladds(cross, velocity, camera->pos);
        } break;
    }
}

static void cg_control_camera_update(Camera* camera) {
    vec3 front;
    front[0] = cos(glm_rad(camera->angle.yaw)) * cos(glm_rad(camera->angle.pitch));
    front[1] = sin(glm_rad(camera->angle.pitch));
    front[2] = sin(glm_rad(camera->angle.yaw)) * cos(glm_rad(camera->angle.pitch));
    glm_normalize_to(front, camera->front);

    glm_vec3_cross(front, camera->worldup, camera->right);
    glm_normalize(camera->right);

    glm_vec3_cross(camera->right, camera->front, camera->up);
    glm_normalize(camera->up);
}

static void mouse_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    xoffset *= camera->sensitivity;
    yoffset *= camera->sensitivity;

    camera->angle.yaw += xoffset;
    camera->angle.pitch += yoffset;

    if (camera->angle.pitch > 89.0f) {
        camera->angle.pitch = 89.0f;
    }
    if (camera->angle.pitch < -89.0f) {
        camera->angle.pitch = -89.0f;
    }
    cg_control_camera_update(camera);
}

GLFWwindow* cg_control_window_create(Camera* camera, int width, int height, const char* title) {
    if (glfwInit() == FALSE) {
        int error = glfwGetError(NULL);
        fprintf(stderr, "GLFW error: %d\n", error);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to intialise window\n");
        return NULL;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetErrorCallback(error_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowUserPointer(window, camera);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        int error = glfwGetError(NULL);
        fprintf(stderr, "Failed to intialise GLAD: %d\n", error);
        return NULL;
    }

    return window;
}

void cg_control_camera_create(Camera* camera, float speed) {
    vec3 pos = {0.0f, 0.0f, 3.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    vec3 front = {0.0f, 0.0f, -1.0f};
    glm_vec3_copy(pos, camera->pos);
    glm_vec3_copy(front, camera->front);
    glm_vec3_copy(up, camera->worldup);
    camera->speed = speed;
    camera->zoom = 45.0f;
    camera->angle.pitch = 0.0f;
    camera->angle.yaw = -90.0f;
    cg_control_camera_update(camera);
}
