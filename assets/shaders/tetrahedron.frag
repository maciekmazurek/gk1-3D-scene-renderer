#version 330 core

in vec3 vColor;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

void main()
{
    vec3 lightPos = vec3(3.0, 3.0, 3.0);
    vec3 lightColor = vec3(1.0);
    vec3 viewPos = vec3(0.0, 0.0, 5.0);

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFragPos);
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 diffuse = diff * vColor * lightColor;
    vec3 specular = spec * lightColor;

    vec3 color = diffuse + specular;
    FragColor = vec4(color, 1.0);
}