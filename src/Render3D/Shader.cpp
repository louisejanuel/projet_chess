#include "Shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Ouverture des fichiers sources
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    // Vérification de la lecture
    if (!vShaderFile.is_open() || !fShaderFile.is_open()) {
        std::cout << "ERREUR CRITIQUE : Impossible de trouver les shaders." << std::endl;
        return; // Stoppe l'exécution pour éviter un plantage
    }

    std::stringstream vShaderStream;
    std::stringstream fShaderStream;
    
    // Extraction du texte
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    
    // Fermeture des fichiers
    vShaderFile.close();
    fShaderFile.close();

    // Conversion en chaînes de caractères
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compilation du Vertex Shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);

    // Compilation du Fragment Shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    // Création et liaison du programme principal
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // Nettoyage des shaders intermédiaires inutiles
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}