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
    AABB CalculateBoundingBox(const std::vector<Triangle>& triangles);

}