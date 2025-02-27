#include <iostream>

extern "C" __declspec(dllexport) void onStart() {
    std::cout << "Script started!" << std::endl;
}

extern "C" __declspec(dllexport) void onUpdate(float deltaTime) {
    std::cout << "Updating script, deltaTime: " << deltaTime << std::endl;
}