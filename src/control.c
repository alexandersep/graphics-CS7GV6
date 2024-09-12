#include "graphics.h"

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Camera* camera = glfwGetWindowUserPointer(window);
    camera->zoom -= yoffset;
    if (camera->zoom < 1.0f) {
        camera->zoom = 1.0f;
    }
    if (camera->zoom > 45.0f) {
        camera->zoom = 45.0f;
    }
}

void cg_control_camera_move(GLFWwindow* window, float deltaTime) {
    Camera* camera = glfwGetWindowUserPointer(window);

    float velocity = camera->speed * deltaTime;
    // Check if each movement key is pressed
    if (glfwGetKey(window, CG_FORWARD) == GLFW_PRESS) {
        glm_vec3_muladds(camera->front, velocity, camera->pos);
    }
    if (glfwGetKey(window, CG_BACKWARD) == GLFW_PRESS) {
        glm_vec3_muladds(camera->front, -velocity, camera->pos);
    }
    if (glfwGetKey(window, CG_LEFT) == GLFW_PRESS) {
        vec3 cross;
        glm_vec3_cross(camera->front, camera->up, cross);
        glm_vec3_normalize(cross);
        glm_vec3_muladds(cross, -velocity, camera->pos);
    }
    if (glfwGetKey(window, CG_RIGHT) == GLFW_PRESS) {
        vec3 cross;
        glm_vec3_cross(camera->front, camera->up, cross);
        glm_vec3_normalize(cross);
        glm_vec3_muladds(cross, velocity, camera->pos);
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

static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    Camera* camera = glfwGetWindowUserPointer(window);
    float xpos = xposIn;
    float ypos = yposIn;
    if (!camera->mouse.focus) {
        camera->mouse.xpos = xpos;
        camera->mouse.ypos = ypos;
        camera->mouse.focus = 1;
    }

    float xoffset = xpos - camera->mouse.xpos;
    float yoffset = camera->mouse.ypos - ypos;
    camera->mouse.xpos = xpos;
    camera->mouse.ypos = ypos;

    xoffset *= camera->mouse.sensitivity;
    yoffset *= camera->mouse.sensitivity;

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
    camera->mouse.xpos = CG_SCREEN_X / 2.0;
    camera->mouse.ypos = CG_SCREEN_Y / 2.0;
    camera->mouse.focus = 0;
    camera->mouse.sensitivity = 0.1f;
    cg_control_camera_update(camera);
}
