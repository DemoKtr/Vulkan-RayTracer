#pragma once
#include "config.h"

namespace vkSettings {
	static float zNear = 0.1f;
	static float zFar = 64.0f;
	static float lightFOV = 100.0f;
	static bool firstMouse = true;
	static float lastX = { 0.0f };
	static float lastY = { 0.0f };


	//editor
	static float leftPanelWidth = 200.0f;
	static float rightPanelWidth = 200.0f;
	static float bottomPanelHeight = 200.0f;
	static float menuHeight = 0.0f;

	static bool resizingLeft = false;
	static bool resizingRight = false;
	static bool resizingBottom = false;



    static int MIN_TRIANGLES = 2; // Minimalna liczba trójk¹tów w wêŸle BVH
    static int MAX_DEPTH = 32;     // Maksymalna g³êbokoœæ drzewa BVH
    static int thumbWidth = 512;
    static int thumbHeigh = 512;
    


}