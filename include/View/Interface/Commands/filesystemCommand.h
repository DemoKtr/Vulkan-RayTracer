#pragma once
#include "View/Interface/Commands/command.h"

/*
namespace vkCommand {
    class FileSystemCommand : public Command {
    private:
        std::filesystem::path source;
        std::filesystem::path destination;
        bool isMove; // flaga, by rozró¿niæ kopiowanie od przenoszenia

    public:
        FileSystemCommand(const std::filesystem::path& src, const std::filesystem::path& dest, bool move)
            : source(src), destination(dest), isMove(move) {}

        void execute() override {
            if (isMove) {
                std::filesystem::rename(source, destination);
            }
            else {
                std::filesystem::copy(source, destination, std::filesystem::copy_options::overwrite_existing);
            }
        }

        void undo() override {
            if (isMove) {
                std::filesystem::rename(destination, source); // Cofniêcie przeniesienia
            }
            else {
                std::filesystem::remove(destination); // Usuniêcie skopiowanego pliku
            }
        }
    };
}
*/