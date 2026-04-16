#pragma once
#include <vector>
#include <string>

// Sommets et indices de réutilisation
struct MeshData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

class ObjLoader {
public:
    // OBJ to données 3D optimisées
    static MeshData load(const std::string& path);
};