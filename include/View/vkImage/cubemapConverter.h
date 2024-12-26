#pragma once
#include "config.h"
#include "View/vkImage/bitmap.h"
#include "config.h"
#include <algorithm> // std::clamp
#include <cmath>

#define _USE_MATH_DEFINES

#define CUBEMAP_NUM_FACES 6


#define CUBE_MAP_INDEX_NEG_X 0
#define CUBE_MAP_INDEX_POS_X 1
#define CUBE_MAP_INDEX_POS_Y 2
#define CUBE_MAP_INDEX_NEG_Y 3
#define CUBE_MAP_INDEX_NEG_Z 4
#define CUBE_MAP_INDEX_POS_Z 5
constexpr double PI = 3.14159265358979323846;

namespace vkImage {


	glm::vec3 FaceCoordsToXYZ(int x, int y, int FaceID, int FaceSize);
	void ConvertEquirectangularImageToCubemap(const Bitmap& b, std::vector<Bitmap>& Cubemap);
}