#pragma once
#include <vector>
#include <string>

struct MeshData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

class ObjLoader {
public:
    static MeshData load(const std::string& path);
};