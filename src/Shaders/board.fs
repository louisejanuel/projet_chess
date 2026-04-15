#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

// Variables envoyées depuis App.cpp et Board.hpp
uniform vec3 baseColor;
uniform vec3 lightColor;     // La couleur de la chaine de Markov
uniform vec3 lightPos;       // La lumière fixe en hauteur
uniform vec3 mobileLightPos; // La lumière dirigée par tes boutons
uniform int isWhiteTurn;     // 1 = Blanc, 0 = Noir

void main()
{
    vec3 norm = normalize(Normal);
    vec3 result = vec3(0.0);

    if (isWhiteTurn == 1) 
    {
        // 1. Lumière Fixe (Ambiance Jour)
        vec3 lightDir1 = normalize(lightPos - FragPos);
        float diff1 = max(dot(norm, lightDir1), 0.0);
        vec3 diffuse1 = diff1 * vec3(0.8, 0.8, 0.8); // Lumière blanche douce

        // 2. Lumière Mobile (Tes boutons !)
        vec3 lightDir2 = normalize(mobileLightPos - FragPos);
        float diff2 = max(dot(norm, lightDir2), 0.0);
        
        // On calcule une "atténuation" pour qu'elle fasse l'effet d'une lampe qui éclaire une zone
        float distance = length(mobileLightPos - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
        
        // Lumière mobile avec une teinte chaude (Orange/Jaune)
        vec3 diffuse2 = diff2 * vec3(1.0, 0.6, 0.2) * attenuation; 

        // On mélange le tout
        vec3 ambient = 0.3 * baseColor; // Beaucoup de lumière ambiante
        result = ambient + (diffuse1 + diffuse2) * baseColor;
    } 
    else 
    {

        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        
        // On utilise ta couleur de Markov pour colorer l'ambiance !
        vec3 diffuse = diff * lightColor; 

        vec3 ambient = 0.1 * baseColor; // Très peu de lumière ambiante (Nuit)
        result = ambient + diffuse * baseColor;
    }

    FragColor = vec4(result, 1.0);
}