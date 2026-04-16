#include "ObjLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <glm/glm.hpp>

namespace {
    // Extrait et compile 
    void processFaceLine(std::istringstream& iss,
                         const std::vector<glm::vec3>& temp_vertices,
                         const std::vector<glm::vec2>& temp_uvs,
                         const std::vector<glm::vec3>& temp_normals,
                         std::map<std::string, unsigned int>& vertexHistory,
                         unsigned int& uniqueVertexCount,
                         MeshData& mesh) {
        
        // Stocke les indices des sommets 
        std::vector<unsigned int> faceIndices;
        std::string vertexStrOriginal;

        // Boucle sur chaque point 
        while (iss >> vertexStrOriginal) {
            const std::string& key = vertexStrOriginal;

            // Vérifie si sommet déjà traité
            if (vertexHistory.find(key) != vertexHistory.end()) {
                faceIndices.push_back(vertexHistory[key]);
            } else {
                std::string vertexStr = vertexStrOriginal;
                std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
                std::istringstream viss(vertexStr);

                unsigned int vIdx = 0;
                unsigned int uvIdx = 0;
                unsigned int nIdx = 0;

                viss >> vIdx >> uvIdx >> nIdx;

                // Récupération des données géométriques
                glm::vec3 vertex = temp_vertices[vIdx - 1];
                glm::vec2 uv = (uvIdx > 0 && uvIdx <= temp_uvs.size()) ? temp_uvs[uvIdx - 1] : glm::vec2(0.0f);
                glm::vec3 normal = (nIdx > 0 && nIdx <= temp_normals.size()) ? temp_normals[nIdx - 1] : glm::vec3(0.0f);

                // Valeurs dans le tableau
                mesh.vertices.push_back(vertex.x);
                mesh.vertices.push_back(vertex.y);
                mesh.vertices.push_back(vertex.z);
                mesh.vertices.push_back(normal.x);
                mesh.vertices.push_back(normal.y);
                mesh.vertices.push_back(normal.z);

                // Sommet pour éviter les doublons
                vertexHistory[key] = uniqueVertexCount;
                faceIndices.push_back(uniqueVertexCount);
                uniqueVertexCount++;
            }
        }

        // Triangulation
        if (faceIndices.size() >= 3) {
            for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
                mesh.indices.push_back(faceIndices[0]);
                mesh.indices.push_back(faceIndices[i]);
                mesh.indices.push_back(faceIndices[i+1]);
            }
        }
    }
}

MeshData ObjLoader::load(const std::string& path) {
    MeshData mesh;
    
    // Conteneurs temporaires pour la lecture initiale
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    // Dictionnaire des sommets uniques
    std::map<std::string, unsigned int> vertexHistory;
    unsigned int uniqueVertexCount = 0;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "ERREUR OBJ : Impossible d'ouvrir le fichier " << path << std::endl;
        return mesh;
    }

    std::string line;
    // Lecture du fichier ligne par ligne
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        // Traitement selon l'id de la ligne
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
            processFaceLine(iss, temp_vertices, temp_uvs, temp_normals, vertexHistory, uniqueVertexCount, mesh);
        }
    }
    
    // std::cout << "[SUCCES EBO] " << path << " (Sommets uniques: " << uniqueVertexCount 
    //           << " | Triangles: " << mesh.indices.size() / 3 << ")" << std::endl;
              
    return mesh;
}