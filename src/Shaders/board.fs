#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;

// --- VARIABLES GLOBALES ---
uniform int isWhiteTurn;  
uniform vec3 baseColor;      
uniform vec3 lightColor;     // Couleur Markov
uniform vec3 lightPos;       
uniform vec3 mobileLightPos; 

void main()
{
    vec3 norm = normalize(Normal);
    vec3 result = vec3(0.0);

    if (isWhiteTurn == 1) 
    {
        // Lumière fixe claire
        vec3 lightDir1 = normalize(lightPos - FragPos);
        float diff1 = max(dot(norm, lightDir1), 0.0);
        vec3 diffuse1 = diff1 * vec3(0.8, 0.8, 0.8); 

        // Lumière boutons
        vec3 lightDir2 = normalize(mobileLightPos - FragPos);
        float diff2 = max(dot(norm, lightDir2), 0.0);
        
        // Calcul distance lumière
        float distance = length(mobileLightPos - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
        
        // Lumière boutons atténuation
        vec3 diffuse2 = diff2 * vec3(1.0, 0.6, 0.2) * attenuation; 

        // Forte luminosité ambiante
        vec3 ambient = 0.3 * baseColor; 
        result = ambient + (diffuse1 + diffuse2) * baseColor;
    } 
    else 
    {
        // Lumière fixe sombre
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        
        // Couleur Markov
        vec3 diffuse = diff * lightColor; 

        // Faible luminosité ambiante
        vec3 ambient = 0.1 * baseColor; 
        result = ambient + diffuse * baseColor;
    }

    FragColor = vec4(result, 1.0);
}