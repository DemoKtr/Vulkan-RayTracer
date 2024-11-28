#pragma once
#include "Scene/ECS/component.h"
#include"Scene/ECS/scripts/scriptLoader.h"

class ScriptComponent : public Component {
    std::shared_ptr<scripts::Script> script;

public:

    ~ScriptComponent();

    explicit ScriptComponent(const std::string& libraryPath)
        : script(std::make_shared<scripts::Script>(libraryPath)) {}

    void executeOnStart() {
        if (script) script->execute_onStart_function();
    }

    void executeOnUpdate(float deltaTime) {
        if (script) script->execute_onUpdate_function(deltaTime);
    }

    std::shared_ptr<scripts::Script> getScript() const {
        return script;
    }

private:
    


};