#pragma once
#include "aabb.h"
#include <settings.h>
#include <algorithm>

namespace vkGeometry {
    struct BVHNode {
        AABB boundingBox;
        BVHNode* left;
        BVHNode* right;
        std::vector<Triangle> triangles;

        bool isLeaf() const {
            return left == nullptr && right == nullptr;
        }
    };

    struct GPU_BVHNode {
        AABB boundingBox;
        int leftChildIndex;  // -1 if leaf
        int rightChildIndex; // -1 if leaf
        int firstTriangleIndex;
        int triangleCount;
        int nextNodeIndex;
    };

    float CalculateSAH(const AABB& leftBox, const AABB& rightBox, int leftCount, int rightCount);

    // Funkcja do znalezienia najlepszego podzia³u
    std::pair<int, float> FindBestSplit(const std::vector<Triangle>& triangles, const AABB& nodeBox);

    // Funkcja do budowania BVH
    BVHNode* BuildBVH(std::vector<Triangle>& triangles, int depth = 0);

    int FlattenBVH(BVHNode* node, std::vector<GPU_BVHNode>& flatNodes, std::vector<Triangle>& flatTriangles, int& nextNodeIndex);
}