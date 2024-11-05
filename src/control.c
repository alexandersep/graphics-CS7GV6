#include "graphics.h"

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glfwSetWindowPos(window, width, height);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Cameras* cameras = glfwGetWindowUserPointer(window);
    Camera* camera = &cameras->camera[cameras->focus];

    camera->zoom -= yoffset;
    if (camera->zoom < 1.0f) {
        camera->zoom = 1.0f;
    }
    if (camera->zoom > 45.0f) {
        camera->zoom = 45.0f;
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Cameras* cameras = glfwGetWindowUserPointer(window);

    static int isPolyMode = 0;
    if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
        if (!isPolyMode) {
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            isPolyMode = 1;
        } else {
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            isPolyMode = 0;
        }
    }
    if (key == GLFW_KEY_C && action == GLFW_RELEASE) {
        Camera* previousCamera = &cameras->camera[cameras->focus];
        if (cameras->focus == 0) {
            cameras->focus = 1;
        } else { // if (cameraFocus == 1) 
            cameras->focus = 0;
        }
        Camera* camera = &cameras->camera[cameras->focus];
        //camera->angle = previousCamera->angle; // don't copy angles of previous camera
        camera->mouse = previousCamera->mouse;
        glm_vec3_copy(previousCamera->front, camera->front);
        glm_vec3_copy(previousCamera->up, camera->up);
        glm_vec3_copy(previousCamera->right, camera->right);
        glm_vec3_copy(previousCamera->worldup, camera->worldup);
        cg_control_angle_update(&camera->angle, camera->front, camera->worldup, camera->up, camera->right);
    }
}

void cg_control_camera_move(GLFWwindow* window, float deltaTime) {
    Cameras* cameras = glfwGetWindowUserPointer(window);
    Camera* camera = &cameras->camera[cameras->focus];

    float velocity = camera->speed * deltaTime;
    if (glfwGetKey(window, CLOSE1) || glfwGetKey(window, CLOSE2) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, TRUE);
    }
    // Check if each movement key is pressed
    if (glfwGetKey(window, FORWARD) == GLFW_PRESS) {
        glm_vec3_muladds(camera->front, velocity, camera->pos);
    }
    if (glfwGetKey(window, BACKWARD) == GLFW_PRESS) {
        glm_vec3_muladds(camera->front, -velocity, camera->pos);
    }
    if (glfwGetKey(window, LEFT) == GLFW_PRESS) {
        vec3 cross;
        glm_vec3_cross(camera->front, camera->up, cross);
        glm_vec3_normalize(cross);
        glm_vec3_muladds(cross, -velocity, camera->pos);
    }
    if (glfwGetKey(window, RIGHT) == GLFW_PRESS) {
        vec3 cross;
        glm_vec3_cross(camera->front, camera->up, cross);
        glm_vec3_normalize(cross);
        glm_vec3_muladds(cross, velocity, camera->pos);
    }
    if (glfwGetKey(window, UP) == GLFW_PRESS) {
        glm_vec3_muladds(camera->worldup, velocity, camera->pos);
    }
    if (glfwGetKey(window, DOWN) == GLFW_PRESS) {
        glm_vec3_muladds(camera->worldup, -velocity, camera->pos);
    }
}

void cg_control_angle_update(EulerAngle* angle, vec3 front, vec3 worldup, vec3 up, vec3 right) {
    front[0] = cos(glm_rad(angle->yaw)) * cos(glm_rad(angle->pitch));
    front[1] = sin(glm_rad(angle->pitch));
    front[2] = sin(glm_rad(angle->yaw)) * cos(glm_rad(angle->pitch));
    glm_normalize(front); // normalize front vector

    glm_vec3_cross(front, worldup, right); // cross product between the worlds up and right vector insert into right vector
    glm_normalize(right); // normalize the result

    glm_vec3_cross(right, front, up); // cross product between right and front, and that becomes up vector
    glm_normalize(up); // normalize the result
}

static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    Cameras* cameras = glfwGetWindowUserPointer(window); // extract cameras from GLFW
    Camera* camera = &cameras->camera[cameras->focus];

    float xpos = xposIn;
    float ypos = yposIn;
    if (!camera->mouse.focus) { // If the camera 1 is enabled
        camera->mouse.xpos = xpos;
        camera->mouse.ypos = ypos;
        camera->mouse.focus = 1;
    }

    // set sensitivity of camera
    float xoffset = xpos - camera->mouse.xpos; 
    float yoffset = camera->mouse.ypos - ypos;
    camera->mouse.xpos = xpos;
    camera->mouse.ypos = ypos;

    xoffset *= camera->mouse.sensitivity; 
    yoffset *= camera->mouse.sensitivity;

    // update yaw, pitch. roll is not needed for camera
    camera->angle.yaw += xoffset;
    camera->angle.pitch += yoffset;

    if (camera->angle.pitch > 89.0f) {
        camera->angle.pitch = 89.0f;
    }
    if (camera->angle.pitch < -89.0f) {
        camera->angle.pitch = -89.0f;
    }
    cg_control_angle_update(&camera->angle, camera->front, camera->worldup, camera->up, camera->right);
}

GLFWwindow* cg_control_window_create(int width, int height, const char* title) {
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
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        int error = glfwGetError(NULL);
        fprintf(stderr, "Failed to intialise GLAD: %d\n", error);
        return NULL;
    }

    return window;
}

void cg_control_camera_create(Camera* camera, float speed, vec3 pos) {
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
    cg_control_angle_update(&camera->angle, camera->front, camera->worldup, camera->up, camera->right);
}
