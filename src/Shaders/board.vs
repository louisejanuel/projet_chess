#version 330 core
layout (location = 0) in vec3 aPos;    // Position du sommet
layout (location = 1) in vec3 aNormal; // Normale pour l'éclairage
layout (location = 2) in vec2 aTexCoords; // Coordonnées de texture

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;      // Matrice de la case
uniform mat4 view;       // Matrice de la caméra
uniform mat4 projection; // Matrice de perspective

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}