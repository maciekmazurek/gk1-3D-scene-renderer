#version 330 core

in vec3 vColor;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

// Uniforms dla światła punktowego
uniform vec3 u_lightPos;
uniform vec3 u_lightColor;
uniform vec3 u_viewPos;

// Uniforms dla reflektora
uniform vec3 u_spotlightPos;
uniform vec3 u_spotlightDir;    // Kierunek reflektora
uniform vec3 u_spotlightColor;
uniform float u_spotlightAngle; // Kąt otwarcia reflektora

vec3 calculatePointLight(vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(u_lightPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    vec3 diffuse = diff * vColor * u_lightColor;
    vec3 specular = spec * u_lightColor;
    
    return diffuse + specular;
}

vec3 calculateSpotlight(vec3 norm, vec3 viewDir)
{
    vec3 lightDir = normalize(u_spotlightPos - vFragPos);
    vec3 spotDir = normalize(u_spotlightDir);
    
    // Oblicz kąt między kierunkiem reflektora a wektorem do piksela
    float theta = dot(-lightDir, spotDir);  // Minus, bo patrzysz w kierunku -lightDir
    
    // Jeśli pixel jest poza stożkiem reflektora - bez oświetlenia
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
    
    // Oświetlenie ze źródła punktowego
    vec3 color = calculatePointLight(norm, viewDir);
    
    // Oświetlenie z reflektora
    color += calculateSpotlight(norm, viewDir);
    
    FragColor = vec4(color, 1.0);
}