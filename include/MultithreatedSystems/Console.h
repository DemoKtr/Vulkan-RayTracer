#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <mutex>
#include <imgui.h>
#include <functional>
namespace console {
    // Globalne zmienne do przechowywania wiadomoœci w konsoli
    extern std::vector<std::pair<std::string, ImVec4>> consoleMessages;
    extern std::mutex consoleMutex;

    // Funkcja dodawania wiadomoœci do konsoli
    void AddConsoleMessage(const std::string& message, const ImVec4& color);


    // Klasa do przechwytywania strumieni
    class ImGuiConsoleBuffer : public std::streambuf {
    public:
        ImGuiConsoleBuffer(std::ostream& targetStream, ImVec4 textColor);
            

        ~ImGuiConsoleBuffer();

        void setCallback(std::function<void(const std::string&, const ImVec4&)> callback);
    protected:
        // Funkcja odpowiadaj¹ca za zapis danych
        virtual int overflow(int c) override;

    private:
        std::ostream& targetStream;
        std::streambuf* originalBuffer;
        std::string currentLine;
        ImVec4 color;
        std::function<void(const std::string&, const ImVec4&)> callback;
    };
}