#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform float time;

void main() {    
    float theta = time * 0.01;

    // https://en.wikipedia.org/wiki/Rotation_matrix
    mat3 rotation_x = mat3( // roll
        1, 0, 0,
        0, cos(theta), -sin(theta),
        0, sin(theta), cos(theta)
    );

    mat3 rotation_y = mat3( // pitch
        cos(theta), 0, sin(theta),
        0, 1, 0,
        -sin(theta), 0, cos(theta)
    );

    mat3 rotation_z = mat3( // yaw
        cos(theta), -sin(theta), 0,
        sin(theta), cos(theta), 0,
        0, 0, 1
    );
    
    mat3 rotation = rotation_z * rotation_y * rotation_x;

    vec3 resTexCoords = rotation * TexCoords;
    FragColor = texture(skybox, resTexCoords);
}
