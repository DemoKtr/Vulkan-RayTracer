#include "View/vkVertexData/aabb.h"

vkGeometry::AABB vkGeometry::CalculateBoundingBox(const std::vector<Triangle>& triangles)
{
    AABB box;
    box.minBounds = glm::vec3(std::numeric_limits<float>::max());
    box.maxBounds = glm::vec3(std::numeric_limits<float>::lowest());

    for (const auto& triangle : triangles) {
        // Funkcje anonimowe (lambdy) do obliczania min i max wektorów
        auto minVec3 = [](const glm::vec3& a, const glm::vec3& b) {
            return glm::vec3(glm::min(a.x, b.x), glm::min(a.y, b.y), glm::min(a.z, b.z));
            };

        auto maxVec3 = [](const glm::vec3& a, const glm::vec3& b) {
            return glm::vec3(glm::max(a.x, b.x), glm::max(a.y, b.y), glm::max(a.z, b.z));
            };

        // Obliczamy minBounds
        box.minBounds = minVec3(box.minBounds, triangle.v0.Position);
        box.minBounds = minVec3(box.minBounds, triangle.v1.Position);
        box.minBounds = minVec3(box.minBounds, triangle.v2.Position);

        // Obliczamy maxBounds
        box.maxBounds = maxVec3(box.maxBounds, triangle.v0.Position);
        box.maxBounds = maxVec3(box.maxBounds, triangle.v1.Position);
        box.maxBounds = maxVec3(box.maxBounds, triangle.v2.Position);
    }
    return box;

}

vkGeometry::AABB vkGeometry::AABB::Union(const AABB& box1, const AABB& box2)
{
    AABB result;
    result.minBounds = glm::min(box1.minBounds, box2.minBounds);
    result.maxBounds = glm::max(box1.maxBounds, box2.maxBounds);
    return result;
}
