#pragma once
#include "View/vkVertexData/vertex.h"


namespace vkGeometry {
    struct Triangle {
        // Wierzcho³ki trójk¹ta
        vkGeometry::Vertex v0, v1, v2;

        // Oblicz œrodek trójk¹ta
        glm::vec3 Center() const {
            return (v0.Position + v1.Position + v2.Position) / 3.0f;
        }
    };

    std::pair<std::vector<Triangle>, std::vector<Triangle>> SplitTriangles(const std::vector<Triangle>& triangles, int axis, float splitPos) {
        std::vector<Triangle> leftTriangles;
        std::vector<Triangle> rightTriangles;

        for (const auto& triangle : triangles) {
            glm::vec3 center = triangle.Center();
            if (center[axis] < splitPos) {
                leftTriangles.push_back(triangle);
            }
            else {
                rightTriangles.push_back(triangle);
            }
        }

        return { leftTriangles, rightTriangles };
    }


}
