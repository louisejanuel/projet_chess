#pragma once
#include <vector>
#include <array>
#include "ObjLoader.hpp"
#include "OpenGL.hpp"

class Model {
public:
    GLuint  VAO = 0;
    GLuint  VBO = 0;
    GLuint  EBO = 0;
    GLsizei vertexCount = 0;
    GLsizei indexCount = 0;

    Model() = default;

    // Libération propre des buffers GPU
    ~Model() {
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (EBO != 0) glDeleteBuffers(1, &EBO);
    }

    // Blocage de la copie
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    // Move Constructor
    Model(Model&& other) noexcept : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), vertexCount(other.vertexCount), indexCount(other.indexCount) {
        other.VAO = 0; other.VBO = 0; other.EBO = 0;
        other.vertexCount = 0; other.indexCount = 0;
    }

    // Move Assignment
    Model& operator=(Model&& other) noexcept {
        if (this != &other) {
            if (VAO != 0) glDeleteVertexArrays(1, &VAO);
            if (VBO != 0) glDeleteBuffers(1, &VBO);
            if (EBO != 0) glDeleteBuffers(1, &EBO);
            
            VAO = other.VAO; VBO = other.VBO; EBO = other.EBO;
            vertexCount = other.vertexCount; indexCount = other.indexCount;
            
            other.VAO = 0; other.VBO = 0; other.EBO = 0;
            other.vertexCount = 0; other.indexCount = 0;
        }
        return *this;
    }

    // Cube pour plateau 
    void setupCube() {
        vertexCount = 36;
        indexCount  = 0;

        // Tableau optimisé 
        constexpr std::array<float, 216> vertices = {
            // Face arrière
            -0.5f, -0.1f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.1f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.1f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.1f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.1f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.1f, -0.5f,  0.0f,  0.0f, -1.0f,
            // Face avant
            -0.5f, -0.1f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.1f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.1f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.1f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.1f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.1f,  0.5f,  0.0f,  0.0f,  1.0f,
            // Face gauche
            -0.5f,  0.1f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.1f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.1f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.1f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.1f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.1f,  0.5f, -1.0f,  0.0f,  0.0f,
            // Face droite
             0.5f,  0.1f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.1f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.1f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.1f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.1f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.1f,  0.5f,  1.0f,  0.0f,  0.0f,
            // Face dessous
            -0.5f, -0.1f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.1f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.1f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.1f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.1f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.1f, -0.5f,  0.0f, -1.0f,  0.0f,
            // Face dessus
            -0.5f,  0.1f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.1f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.1f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.1f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.1f, -0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.1f,  0.5f,  0.0f,  1.0f,  0.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Position (x, y, z)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        
        // Normales (nx, ny, nz)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // Modèle from OBJ
    void setupFromData(const MeshData& mesh) {
        indexCount  = static_cast<GLsizei>(mesh.indices.size());
        vertexCount = static_cast<GLsizei>(mesh.vertices.size() / 6); // On divise par 6 au lieu de 8 !

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        
        // Normales
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    // Affichage de l'objet sur la scène
    void draw() const {
        glBindVertexArray(VAO);

        if (indexCount > 0) {
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }

        glBindVertexArray(0);
    }
};