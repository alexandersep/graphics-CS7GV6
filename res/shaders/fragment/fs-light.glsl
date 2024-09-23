#version 460 core
out vec4 FragColor;

uniform vec3 color;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    FragColor = vec4(color, 1.0);
}
