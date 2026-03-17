#include "ObjLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>

std::vector<float> ObjLoader::load(const std::string& path) {
    std::vector<float> vertexData;
    
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "ERREUR OBJ : Impossible d'ouvrir le fichier " << path << std::endl;
        return vertexData;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } else if (type == "vt") {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        } else if (type == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } else if (type == "f") {
            // Lecture des faces (triangles)
            for (int i = 0; i < 3; ++i) {
                std::string vertexStr;
                iss >> vertexStr;
                
                // On remplace les '/' par des espaces
                std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
                std::istringstream viss(vertexStr);
                
                unsigned int vIdx = 0, uvIdx = 0, nIdx = 0;
                viss >> vIdx >> uvIdx >> nIdx;

                // En OBJ, les indices commencent à 1, mais en C++ les tableaux commencent à 0 !
                glm::vec3 vertex = temp_vertices[vIdx - 1];
                glm::vec2 uv = (uvIdx > 0 && uvIdx <= temp_uvs.size()) ? temp_uvs[uvIdx - 1] : glm::vec2(0.0f);
                glm::vec3 normal = (nIdx > 0 && nIdx <= temp_normals.size()) ? temp_normals[nIdx - 1] : glm::vec3(0.0f);

                // On aplatit tout ça pour OpenGL (8 floats par sommet)
                vertexData.push_back(vertex.x);
                vertexData.push_back(vertex.y);
                vertexData.push_back(vertex.z);
                vertexData.push_back(normal.x);
                vertexData.push_back(normal.y);
                vertexData.push_back(normal.z);
                vertexData.push_back(uv.x);
                vertexData.push_back(uv.y);
            }
        }
    }
    
    std::cout << "[SUCCES] Modele 3D charge : " << path << " (" << (vertexData.size() / 8) / 3 << " triangles)" << std::endl;
    return vertexData;
}