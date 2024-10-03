#pragma once
#include "config.h"
#include "triangle.h"

namespace vkGeometry {
    struct AABB {
        glm::vec3 minBounds, maxBounds;

        // Funkcja do obliczenia powierzchni AABB
        float SurfaceArea() const {
            glm::vec3 extent = maxBounds - minBounds;
            return 2.0f * (extent.x * extent.y + extent.x * extent.z + extent.y * extent.z);
        }

        // Funkcja do z³¹czenia dwóch AABB w jeden
        static AABB Union(const AABB& box1, const AABB& box2) {
            AABB result;
            result.minBounds = glm::min(box1.minBounds, box2.minBounds);
            result.maxBounds = glm::max(box1.maxBounds, box2.maxBounds);
            return result;
        }
    };


    // Funkcja do obliczenia AABB dla zestawu trójk¹tów
    AABB CalculateBoundingBox(const std::vector<Triangle>& triangles) {
        AABB box;
        box.minBounds = glm::vec3(std::numeric_limits<float>::max());
        box.maxBounds = glm::vec3(std::numeric_limits<float>::lowest());

        for (const auto& triangle : triangles) {
            box.minBounds = glm::min(box.minBounds, glm::min(glm::min(triangle.v0.Position, triangle.v1.Position), triangle.v2.Position));
            box.maxBounds = glm::max(box.maxBounds, glm::max(glm::max(triangle.v0.Position, triangle.v1.Position), triangle.v2.Position));
        }
        return box;
    }

}