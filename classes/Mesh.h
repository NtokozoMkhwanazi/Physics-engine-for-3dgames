#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

class Mesh {
public:
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices; // Only used for indexed drawing
    unsigned int texture;

    // Non-indexed constructor
    Mesh(const std::vector<float>& verts)
        : vertices(verts) 
    {
        setupMeshNonIndexed();
    }

    // Indexed constructor
    Mesh(const std::vector<float>& verts, const std::vector<unsigned int>& inds)
        : vertices(verts), indices(inds) 
    {
        setupMeshIndexed();
    }

    void Draw(bool useIndexed = false) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        if (useIndexed) {
            // Indexed drawing
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        } else {
            // Non-indexed drawing
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 8); // 8 floats per vertex
        }

        glBindVertexArray(0);
    }

    void cleanUp() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        if (!indices.empty())
            glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &texture);
    }

private:
    void setupMeshNonIndexed() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        setupVertexAttributes();
        glBindVertexArray(0);
    }

    void setupMeshIndexed() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        setupVertexAttributes();
        glBindVertexArray(0);
    }

    void setupVertexAttributes() {
        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // TexCoords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
};

