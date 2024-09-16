#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;

uniform vec3 objectColor;
uniform vec3 viewPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // ambient, diffuse and specular
    vec3 lighting = (ambient + diffuse + specular) * objectColor;

    // texture
    vec4 texColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);

    // texture with lighting
    vec3 result = lighting * texColor.rgb;

    FragColor = vec4(result, 1.0);
} 
