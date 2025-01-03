#pragma once

#include "triangle.h"

namespace vkGeometry {
    struct AABB {
        glm::vec3 minBounds, maxBounds;

        // Funkcja do obliczenia powierzchni AABB
        float SurfaceArea() const {
            glm::vec3 extent = maxBounds - minBounds;
            return 2.0f * (extent.x * extent.y + extent.x * extent.z + extent.y * extent.z);
        }

        // Funkcja do z��czenia dw�ch AABB w jeden
        static AABB Union(const AABB& box1, const AABB& box2);
    };


    // Funkcja do obliczenia AABB dla zestawu tr�jk�t�w
    AABB CalculateBoundingBox(const std::vector<Triangle>& triangles);

}  