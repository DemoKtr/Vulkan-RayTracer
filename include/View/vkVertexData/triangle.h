#pragma once
#include "View/vkVertexData/vertex.h"


namespace vkGeometry {
    struct Triangle {
        // Wierzcho�ki tr�jk�ta
        vkGeometry::Vertex v0, v1, v2;


        // Oblicz �rodek tr�jk�ta
        glm::vec3 Center() const {
            return (v0.Position + v1.Position + v2.Position) / 3.0f;
        }
    };

    std::pair<std::vector<Triangle>, std::vector<Triangle>> SplitTriangles(const std::vector<Triangle>& triangles, int axis, float splitPos);


}

/*

std::vector<Triangle> vkMesh::ObjMesh::getTriangles() const {
    std::vector<Triangle> triangles;

    // Przechodzimy przez wszystkie indeksy
    for (size_t i = 0; i < indices.size(); i += 3) {
        // Tworzymy tr�jk�t z 3 kolejnych indeks�w
        Triangle triangle;

        triangle.v0.position = vertices[indices[i]];     // Pierwszy wierzcho�ek
        triangle.v1.position = vertices[indices[i + 1]]; // Drugi wierzcho�ek
        triangle.v2.position = vertices[indices[i + 2]]; // Trzeci wierzcho�ek

        triangle.v0.TexCoords = vertices[indices[i]];     // Pierwszy wierzcho�ek
        triangle.v1.TexCoords = vertices[indices[i + 1]]; // Drugi wierzcho�ek
        triangle.v2.TexCoords = vertices[indices[i + 2]]; // Trzeci wierzcho�ek

        // Dodaj tr�jk�t do listy tr�jk�t�w
        triangles.push_back(triangle);
    }

    return triangles;
}
*/