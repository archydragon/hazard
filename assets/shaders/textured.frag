#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

vec3 lightPos = vec3(-1, 4, -3);

uniform sampler2D texture_diffuse;

void main()
{
    // ambient
    vec3 ambient = 0.2 * texture(texture_diffuse, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = 1 * diff * texture(texture_diffuse, TexCoords).rgb;

    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}
