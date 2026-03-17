#pragma once
#include <vector>
#include <string>

class ObjLoader {
public:
    // Lit un fichier OBJ et retourne un tableau de floats prêt pour OpenGL
    // Format de sortie : [px, py, pz, nx, ny, nz, u, v] pour chaque sommet
    static std::vector<float> load(const std::string& path);
};