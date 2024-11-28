#pragma once


#ifndef SCRIPT_BASE_H
#define SCRIPT_BASE_H
#include <string>

class IScript {
public:
    virtual ~IScript() = default;
    virtual void onStart() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onDestroy() = 0;
};

#endif // SCRIPT_BASE_H