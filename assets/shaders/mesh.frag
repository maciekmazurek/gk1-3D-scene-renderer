#version 330 core

in vec3 vColor;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

// Uniforms dla pierwszego światła punktowego
uniform vec3 u_lightPos1;
uniform vec3 u_lightColor1;

// Uniforms dla drugiego światła punktowego
uniform vec3 u_lightPos2;
uniform vec3 u_lightColor2;

// Uniforms dla reflektora
uniform vec3 u_spotlightPos;
uniform vec3 u_spotlightDir;
uniform vec3 u_spotlightColor;
uniform float u_spotlightAngle;

uniform vec3 u_viewPos;

vec3 calculatePointLight(vec3 lightPos, vec3 lightColor, vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(lightPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    vec3 diffuse = diff * vColor * lightColor;
    vec3 specular = spec * lightColor;
    
    return diffuse + specular;
}

vec3 calculateSpotlight(vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(u_spotlightPos - vFragPos);
    vec3 spotDir = normalize(u_spotlightDir);
    
    float theta = dot(-lightDir, spotDir);
    
    if (theta < u_spotlightAngle)
        return vec3(0.0);
    
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    vec3 diffuse = diff * vColor * u_spotlightColor;
    vec3 specular = spec * u_spotlightColor;
    
    return diffuse + specular;
}

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(u_viewPos - vFragPos);
    
    // Oświetlenie z pierwszego światła punktowego
    vec3 color = calculatePointLight(u_lightPos1, u_lightColor1, norm, viewDir);
    
    // Oświetlenie z drugiego światła punktowego
    color += calculatePointLight(u_lightPos2, u_lightColor2, norm, viewDir);
    
    // Oświetlenie z reflektora
    color += calculateSpotlight(norm, viewDir);
    
    FragColor = vec4(color, 1.0);
}