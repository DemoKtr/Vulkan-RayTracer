#include "View/vkVertexData/aabb.h"

vkGeometry::AABB vkGeometry::CalculateBoundingBox(const std::vector<Triangle>& triangles)
{
    AABB box;
    box.minBounds = glm::vec3(std::numeric_limits<float>::max());
    box.maxBounds = glm::vec3(std::numeric_limits<float>::lowest());

    for (const auto& triangle : triangles) {
        box.minBounds = glm::min(box.minBounds, glm::min(glm::min(triangle.v0.Position, triangle.v1.Position), triangle.v2.Position));
        box.maxBounds = glm::max(box.maxBounds, glm::max(glm::max(triangle.v0.Position, triangle.v1.Position), triangle.v2.Position));
    }
    return box;
}
