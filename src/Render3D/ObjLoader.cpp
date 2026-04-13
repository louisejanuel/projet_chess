#include "ObjLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <glm/glm.hpp>

MeshData ObjLoader::load(const std::string& path) {
    MeshData mesh;
    
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::map<std::string, unsigned int> vertexHistory;
    unsigned int uniqueVertexCount = 0;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "ERREUR OBJ : Impossible d'ouvrir le fichier " << path << std::endl;
        return mesh;
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
            std::vector<unsigned int> faceIndices;
            std::string vertexStrOriginal;
            
            while (iss >> vertexStrOriginal) {
                std::string key = vertexStrOriginal;
                
                // Si on a DÉJÀ vu ce point, on récupère juste son indice !
                if (vertexHistory.find(key) != vertexHistory.end()) {
                    faceIndices.push_back(vertexHistory[key]);
                } else {
                    // Si c'est un point INCONNU, on le découpe et on le rajoute
                    std::string vertexStr = vertexStrOriginal;
                    std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
                    std::istringstream viss(vertexStr);
                    
                    unsigned int vIdx = 0, uvIdx = 0, nIdx = 0;
                    viss >> vIdx >> uvIdx >> nIdx;

                    glm::vec3 vertex = temp_vertices[vIdx - 1];
                    glm::vec2 uv = (uvIdx > 0 && uvIdx <= temp_uvs.size()) ? temp_uvs[uvIdx - 1] : glm::vec2(0.0f);
                    glm::vec3 normal = (nIdx > 0 && nIdx <= temp_normals.size()) ? temp_normals[nIdx - 1] : glm::vec3(0.0f);

                    mesh.vertices.push_back(vertex.x);
                    mesh.vertices.push_back(vertex.y);
                    mesh.vertices.push_back(vertex.z);
                    mesh.vertices.push_back(normal.x);
                    mesh.vertices.push_back(normal.y);
                    mesh.vertices.push_back(normal.z);
                    mesh.vertices.push_back(uv.x);
                    mesh.vertices.push_back(uv.y);

                    vertexHistory[key] = uniqueVertexCount;
                    faceIndices.push_back(uniqueVertexCount);
                    uniqueVertexCount++;
                }
            }

            // On crée les triangles en utilisant uniquement les INIDICES
            if (faceIndices.size() >= 3) {
                for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
                    mesh.indices.push_back(faceIndices[0]);
                    mesh.indices.push_back(faceIndices[i]);
                    mesh.indices.push_back(faceIndices[i+1]);
                }
            }
        }
    }
    
    std::cout << "[SUCCES EBO] " << path << " (Sommets uniques: " << uniqueVertexCount 
              << " | Triangles: " << mesh.indices.size() / 3 << ")" << std::endl;
    return mesh;
}