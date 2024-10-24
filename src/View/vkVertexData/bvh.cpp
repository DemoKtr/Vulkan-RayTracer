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
        // Sortuj tr�jk�ty wzd�u� danej osi
        std::vector<Triangle> sortedTriangles = triangles;
        std::sort(sortedTriangles.begin(), sortedTriangles.end(), [axis](const Triangle& a, const Triangle& b) {
            return a.Center()[axis] < b.Center()[axis];
            });

        // Spr�buj r�ne punkty podzia�u
        for (size_t i = 1; i < sortedTriangles.size(); ++i) {
            auto [leftTriangles, rightTriangles] = SplitTriangles(sortedTriangles, axis, sortedTriangles[i].Center()[axis]);

            // Oblicz AABB dla lewej i prawej strony
            AABB leftBox = CalculateBoundingBox(leftTriangles);
            AABB rightBox = CalculateBoundingBox(rightTriangles);

            // Oblicz koszt SAH
            float cost = CalculateSAH(leftBox, rightBox, leftTriangles.size(), rightTriangles.size());

            // Sprawd�, czy jest to najlepszy koszt
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

    // Je�li mamy ma�o tr�jk�t�w lub osi�gn�li�my maksymaln� g��boko��, stw�rz li��
    if (triangles.size() <= vkSettings::MIN_TRIANGLES || depth >= vkSettings::MAX_DEPTH) {
        BVHNode* leaf = new BVHNode();
        leaf->boundingBox = nodeBox;
        leaf->triangles = triangles;
        leaf->left = leaf->right = nullptr;
        return leaf;
    }

    // Znajd� najlepszy podzia�
    auto [axis, splitPos] = FindBestSplit(triangles, nodeBox);

    if (axis == -1) {
        // Je�li nie znaleziono dobrego podzia�u, utw�rz li��
        BVHNode* leaf = new BVHNode();
        leaf->boundingBox = nodeBox;
        leaf->triangles = triangles;
        leaf->left = leaf->right = nullptr;
        return leaf;
    }

    // Podziel tr�jk�ty na dwie grupy
    auto [leftTriangles, rightTriangles] = SplitTriangles(triangles, axis, splitPos);

    // Rekurencyjnie buduj lew� i praw� stron� drzewa
    BVHNode* node = new BVHNode();
    node->boundingBox = nodeBox;
    node->left = BuildBVH(leftTriangles, depth + 1);
    node->right = BuildBVH(rightTriangles, depth + 1);

    return node;
}

int vkGeometry::FlattenBVH(BVHNode* node, std::vector<GPU_BVHNode>& flatNodes, std::vector<Triangle>& flatTriangles, int& nextNodeIndex)
{
    // Tworzymy nowy w�ze� dla struktury liniowej
    GPU_BVHNode gpuNode;
    gpuNode.boundingBox = node->boundingBox;
    // Zapisujemy indeks tego w�z�a
    int currentIndex = flatNodes.size();
    if (node->isLeaf()) {
        // To jest li��, ustaw indeksy tr�jk�t�w
        gpuNode.leftChildIndex = -1;
        gpuNode.rightChildIndex = -1;
        gpuNode.firstTriangleIndex = flatTriangles.size(); // Indeks pierwszego tr�jk�ta w tablicy
        gpuNode.triangleCount = node->triangles.size();

        // Dodaj tr�jk�ty z li�cia do p�askiej tablicy tr�jk�t�w
        flatTriangles.insert(flatTriangles.end(), node->triangles.begin(), node->triangles.end());



        // Dodajemy bie��cy w�ze� do p�askiej tablicy w�z��w
        flatNodes.push_back(gpuNode);

        // Ustawiamy nextNodeIndex, poniewa� to jest li��
        flatNodes[currentIndex].nextNodeIndex = nextNodeIndex;  // Nast�pny w�ze� do odwiedzenia po przetworzeniu li�cia

        // Zwracamy indeks bie��cego w�z�a
        return currentIndex;
    }
    else {
        // To jest w�ze� wewn�trzny, rekurencyjnie sp�aszczamy lewe i prawe poddrzewo
        gpuNode.firstTriangleIndex = -1;  // Niepotrzebne w w�z�ach wewn�trznych
        gpuNode.triangleCount = 0;        // Niepotrzebne w w�z�ach wewn�trznych

        int savedNextNodeIndex = nextNodeIndex;
        // Zapisujemy indeks tego w�z�a
        nextNodeIndex = currentIndex + 1;

        // Rezerwujemy miejsce dla bie��cego w�z�a
        flatNodes.push_back(gpuNode);  // Dodajemy bie��cy w�ze�, zostanie uzupe�niony p�niej

        // Przetwarzamy lewe dziecko
        int leftChildIndex = FlattenBVH(node->left, flatNodes, flatTriangles, nextNodeIndex);

        // Ustawiamy nextNodeIndex dla prawego dziecka, czyli na bie��cy w�ze� (przetworzenie prawego poddrzewa powinno nas wr�ci� tu)
        int rightChildIndex = FlattenBVH(node->right, flatNodes, flatTriangles, nextNodeIndex);

        // Uzupe�niamy indeksy dzieci
        flatNodes[currentIndex].leftChildIndex = leftChildIndex;
        flatNodes[currentIndex].rightChildIndex = rightChildIndex;

        // Po przetworzeniu prawego dziecka, ustawiamy nextNodeIndex na nast�pny w�ze� po ca�ym poddrzewie
        flatNodes[currentIndex].nextNodeIndex = nextNodeIndex;

        // Zwracamy indeks bie��cego w�z�a
        return currentIndex;
    }
}

