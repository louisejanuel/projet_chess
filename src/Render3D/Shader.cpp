#include "Shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // Code source des shaders depuis les fichiers
    std::string   vertexCode;
    std::string   fragmentCode;
    std::ifstream vShaderFile, fShaderFile;

    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    if (!vShaderFile.is_open() || !fShaderFile.is_open())
    {
        std::cout << "ERREUR CRITIQUE : Impossible de trouver les shaders aux chemins :" << std::endl;
        std::cout << vertexPath << " et " << fragmentPath << std::endl;
        std::cout << "Verifie les chemins dans ton main.cpp !" << std::endl;
    }

    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();
    vertexCode   = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compiler les shaders
    GLuint vertex, fragment;
    int    success;
    char   infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Créer le programme Shader
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // Nettoyage
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}