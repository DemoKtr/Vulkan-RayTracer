#include "MultithreatedSystems/Console.h"


void console::AddConsoleMessage(const std::string& message, const ImVec4& color) {
	std::lock_guard<std::mutex> lock(consoleMutex);
	consoleMessages.emplace_back(message, color);
}

console::ImGuiConsoleBuffer::ImGuiConsoleBuffer(std::ostream& targetStream, ImVec4 textColor) : targetStream(targetStream), color(textColor), callback(nullptr) {
	// Przechowaj oryginalny bufor i ustaw nowy
	originalBuffer = targetStream.rdbuf(this);
}

console::ImGuiConsoleBuffer::~ImGuiConsoleBuffer()
{
	// Przywróæ oryginalny bufor po zakoñczeniu
	targetStream.rdbuf(originalBuffer);
}

void console::ImGuiConsoleBuffer::setCallback(std::function<void(const std::string&, const ImVec4&)> callback){
	this->callback = callback;
}

int console::ImGuiConsoleBuffer::overflow(int c) {
    if (c == EOF) {
        return EOF;
    }
    if (callback) {
        currentLine += static_cast<char>(c);
        if (c == '\n') {
            callback(currentLine, color); // Wywo³anie callbacku
            currentLine.clear();
        }
    }
    return c;
}
namespace console {
    std::vector<std::pair<std::string, ImVec4>> consoleMessages; // Definicja
    std::mutex consoleMutex; // Definicja
}