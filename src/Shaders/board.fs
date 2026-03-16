#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 baseColor;           
uniform int state;                 
uniform vec3 lightPos;             
uniform vec3 lightColor;           

void main() {
    // Éclairage Ambient simple
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Éclairage Diffus
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Couleur
    vec3 result = (ambient + diffuse) * baseColor;

    // Halo de sélection
    if (state == 1) { 
        result += vec3(0.2, 0.2, 0.0); 
    } else if (state == 2) { 
        result += vec3(0.0, 0.4, 0.0); 
    }

    FragColor = vec4(result, 1.0);
}