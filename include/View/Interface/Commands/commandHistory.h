#pragma once
#include <stack>
#include <memory>
#include "View/Interface/Commands/command.h"
namespace vkCommand {
    class CommandHistory {
    private:
        std::stack<std::unique_ptr<Command>> undoStack;
        std::stack<std::unique_ptr<Command>> redoStack;

    public:
        void executeCommand(std::unique_ptr<Command> command) {
            command->execute();
            undoStack.push(std::move(command));
            while (!redoStack.empty()) redoStack.pop(); // Usuniêcie redo po nowej akcji
        }

        void undo() {
            if (!undoStack.empty()) {
                auto command = std::move(undoStack.top());
                undoStack.pop();
                command->undo();
                redoStack.push(std::move(command));
            }
        }

        void redo() {
            if (!redoStack.empty()) {
                auto command = std::move(redoStack.top());
                redoStack.pop();
                command->execute();
                undoStack.push(std::move(command));
            }
        }
    };
}