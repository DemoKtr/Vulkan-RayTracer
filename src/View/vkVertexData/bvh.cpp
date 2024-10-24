#include "View/vkVertexData/bvh.h"

float vkGeometry::CalculateSAH(const AABB& leftBox, const AABB& rightBox, int leftCount, int rightCount)
{
    float leftArea = leftBox.SurfaceArea();
    float rightArea = rightBox.SurfaceArea();
    float totalArea = AABB::Union(leftBox, rightBox).SurfaceArea();

    return (leftArea / totalArea) * leftCount + (rightArea / totalArea) * rightCount;
}

std::pair<int, float> vkGeometry::FindBestSplit(const std::vector<Triangle>& triangles, const AABB& nodeBox)
{
    int bestAxis = -1;
    float bestSplit = 0.0f;
    float bestCost = std::numeric_limits<float>::max();

    // Przeszukaj wszystkie osie
    for (int axis = 0; axis < 3; ++axis) {
        // Sortuj trójk¹ty wzd³u¿ danej osi
        std::vector<Triangle> sortedTriangles = triangles;
        std::sort(sortedTriangles.begin(), sortedTriangles.end(), [axis](const Triangle& a, const Triangle& b) {
            return a.Center()[axis] < b.Center()[axis];
            });

        // Spróbuj ró¿ne punkty podzia³u
        for (size_t i = 1; i < sortedTriangles.size(); ++i) {
            auto [leftTriangles, rightTriangles] = SplitTriangles(sortedTriangles, axis, sortedTriangles[i].Center()[axis]);

            // Oblicz AABB dla lewej i prawej strony
            AABB leftBox = CalculateBoundingBox(leftTriangles);
            AABB rightBox = CalculateBoundingBox(rightTriangles);

            // Oblicz koszt SAH
            float cost = CalculateSAH(leftBox, rightBox, leftTriangles.size(), rightTriangles.size());

            // SprawdŸ, czy jest to najlepszy koszt
            if (cost < bestCost) {
                bestCost = cost;
                bestAxis = axis;
                bestSplit = sortedTriangles[i].Center()[axis];
            }
        }
    }

    return { bestAxis, bestSplit };
}

vkGeometry::BVHNode* vkGeometry::BuildBVH(std::vector<Triangle>& triangles, int depth)
{
    AABB nodeBox = CalculateBoundingBox(triangles);

    // Jeœli mamy ma³o trójk¹tów lub osi¹gnêliœmy maksymaln¹ g³êbokoœæ, stwórz liœæ
    if (triangles.size() <= vkSettings::MIN_TRIANGLES || depth >= vkSettings::MAX_DEPTH) {
        BVHNode* leaf = new BVHNode();
        leaf->boundingBox = nodeBox;
        leaf->triangles = triangles;
        leaf->left = leaf->right = nullptr;
        return leaf;
    }

    // ZnajdŸ najlepszy podzia³
    auto [axis, splitPos] = FindBestSplit(triangles, nodeBox);

    if (axis == -1) {
        // Jeœli nie znaleziono dobrego podzia³u, utwórz liœæ
        BVHNode* leaf = new BVHNode();
        leaf->boundingBox = nodeBox;
        leaf->triangles = triangles;
        leaf->left = leaf->right = nullptr;
        return leaf;
    }

    // Podziel trójk¹ty na dwie grupy
    auto [leftTriangles, rightTriangles] = SplitTriangles(triangles, axis, splitPos);

    // Rekurencyjnie buduj lew¹ i praw¹ stronê drzewa
    BVHNode* node = new BVHNode();
    node->boundingBox = nodeBox;
    node->left = BuildBVH(leftTriangles, depth + 1);
    node->right = BuildBVH(rightTriangles, depth + 1);

    return node;
}

int vkGeometry::FlattenBVH(BVHNode* node, std::vector<GPU_BVHNode>& flatNodes, std::vector<Triangle>& flatTriangles, int& nextNodeIndex)
{
    // Tworzymy nowy wêze³ dla struktury liniowej
    GPU_BVHNode gpuNode;
    gpuNode.boundingBox = node->boundingBox;
    // Zapisujemy indeks tego wêz³a
    int currentIndex = flatNodes.size();
    if (node->isLeaf()) {
        // To jest liœæ, ustaw indeksy trójk¹tów
        gpuNode.leftChildIndex = -1;
        gpuNode.rightChildIndex = -1;
        gpuNode.firstTriangleIndex = flatTriangles.size(); // Indeks pierwszego trójk¹ta w tablicy
        gpuNode.triangleCount = node->triangles.size();

        // Dodaj trójk¹ty z liœcia do p³askiej tablicy trójk¹tów
        flatTriangles.insert(flatTriangles.end(), node->triangles.begin(), node->triangles.end());



        // Dodajemy bie¿¹cy wêze³ do p³askiej tablicy wêz³ów
        flatNodes.push_back(gpuNode);

        // Ustawiamy nextNodeIndex, poniewa¿ to jest liœæ
        flatNodes[currentIndex].nextNodeIndex = nextNodeIndex;  // Nastêpny wêze³ do odwiedzenia po przetworzeniu liœcia

        // Zwracamy indeks bie¿¹cego wêz³a
        return currentIndex;
    }
    else {
        // To jest wêze³ wewnêtrzny, rekurencyjnie sp³aszczamy lewe i prawe poddrzewo
        gpuNode.firstTriangleIndex = -1;  // Niepotrzebne w wêz³ach wewnêtrznych
        gpuNode.triangleCount = 0;        // Niepotrzebne w wêz³ach wewnêtrznych

        int savedNextNodeIndex = nextNodeIndex;
        // Zapisujemy indeks tego wêz³a
        nextNodeIndex = currentIndex + 1;

        // Rezerwujemy miejsce dla bie¿¹cego wêz³a
        flatNodes.push_back(gpuNode);  // Dodajemy bie¿¹cy wêze³, zostanie uzupe³niony póŸniej

        // Przetwarzamy lewe dziecko
        int leftChildIndex = FlattenBVH(node->left, flatNodes, flatTriangles, nextNodeIndex);

        // Ustawiamy nextNodeIndex dla prawego dziecka, czyli na bie¿¹cy wêze³ (przetworzenie prawego poddrzewa powinno nas wróciæ tu)
        int rightChildIndex = FlattenBVH(node->right, flatNodes, flatTriangles, nextNodeIndex);

        // Uzupe³niamy indeksy dzieci
        flatNodes[currentIndex].leftChildIndex = leftChildIndex;
        flatNodes[currentIndex].rightChildIndex = rightChildIndex;

        // Po przetworzeniu prawego dziecka, ustawiamy nextNodeIndex na nastêpny wêze³ po ca³ym poddrzewie
        flatNodes[currentIndex].nextNodeIndex = nextNodeIndex;

        // Zwracamy indeks bie¿¹cego wêz³a
        return currentIndex;
    }
}

