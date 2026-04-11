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
            
            std::vector<std::vector<float>> faceVertices;
            std::string vertexStr;
            
            while (iss >> vertexStr) {
                std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
                std::istringstream viss(vertexStr);
                
                unsigned int vIdx = 0, uvIdx = 0, nIdx = 0;
                viss >> vIdx >> uvIdx >> nIdx;

                glm::vec3 vertex = temp_vertices[vIdx - 1];
                glm::vec2 uv = (uvIdx > 0 && uvIdx <= temp_uvs.size()) ? temp_uvs[uvIdx - 1] : glm::vec2(0.0f);
                glm::vec3 normal = (nIdx > 0 && nIdx <= temp_normals.size()) ? temp_normals[nIdx - 1] : glm::vec3(0.0f);

                std::vector<float> vertexProps = {
                    vertex.x, vertex.y, vertex.z,
                    normal.x, normal.y, normal.z,
                    uv.x, uv.y
                };
                faceVertices.push_back(vertexProps);
            }

            // Triangulation, si 3 points triangle,  si 4 points 2 triangles
            for (size_t i = 1; i < faceVertices.size() - 1; ++i) {
                vertexData.insert(vertexData.end(), faceVertices[0].begin(), faceVertices[0].end());
                vertexData.insert(vertexData.end(), faceVertices[i].begin(), faceVertices[i].end());
                vertexData.insert(vertexData.end(), faceVertices[i+1].begin(), faceVertices[i+1].end());
            }
        }
    }
    
    std::cout << "[SUCCES] Modele 3D charge : " << path << " (" << (vertexData.size() / 8) / 3 << " triangles)" << std::endl;
    return vertexData;
}