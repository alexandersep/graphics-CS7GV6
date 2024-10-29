#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform float time;

void main() {             
    vec4 adjust_color = vec4(0.5, 0.5, 0.5, 1.0);

    float freq = 50.0;
    float amp = 0.01;

    float wavex = sin(TexCoords.y * freq + time) * amp;
    float wavey = cos(TexCoords.x * freq + time) * amp;

    vec2 distortion = TexCoords + vec2(wavex, wavey);

    vec4 texColor = texture(texture1, distortion) * adjust_color;
    if (texColor.a < 0.1) {
        discard;
    }
    FragColor = texColor;
}
