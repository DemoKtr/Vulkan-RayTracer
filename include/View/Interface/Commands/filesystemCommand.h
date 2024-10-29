#pragma once
#include "View/Interface/Commands/command.h"

/*
namespace vkCommand {
    class FileSystemCommand : public Command {
    private:
        std::filesystem::path source;
        std::filesystem::path destination;
        bool isMove; // flaga, by rozr�ni� kopiowanie od przenoszenia

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
                std::filesystem::rename(destination, source); // Cofni�cie przeniesienia
            }
            else {
                std::filesystem::remove(destination); // Usuni�cie skopiowanego pliku
            }
        }
    };
}
*/