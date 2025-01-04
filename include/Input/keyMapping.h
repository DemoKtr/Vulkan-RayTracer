#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>
#include "Input/keyboardInput.h"
#include "mouseInput.h"
namespace input{
	extern std::unordered_map<int, KeyboardInput> keyMapping;
	extern std::unordered_map<int, MouseInput> mouseMapping;


}