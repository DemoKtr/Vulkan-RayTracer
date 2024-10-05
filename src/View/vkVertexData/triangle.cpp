#include "View/vkVertexData/triangle.h"

std::pair<std::vector<vkGeometry::Triangle>, std::vector<vkGeometry::Triangle>> vkGeometry::SplitTriangles(const std::vector<Triangle>& triangles, int axis, float splitPos)
{
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
