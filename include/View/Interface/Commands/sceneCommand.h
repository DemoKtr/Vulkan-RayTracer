#pragma once
#include <Scene/sceneObject.h>
#include "View/Interface/Commands/command.h"

namespace vkCommand {
    class sceneCommand : public Command {
    private:
        SceneObject obj;
        SceneObject s;

        bool isAddOperation;

    public:
        sceneCommand(SceneObject obj, bool addOp);

        void execute() override {
           
        }

        void undo() override {
            
        }
    };
}
