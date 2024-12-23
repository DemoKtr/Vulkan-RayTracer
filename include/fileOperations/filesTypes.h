#pragma once
#include <vector>
#include <string>


namespace fileOperations {

    class FileType {
    public:
        enum Type {
            Text,      // Plik tekstowy
            Image,     // Plik graficzny
            Audio,     // Plik dŸwiêkowy
            Video,     // Plik wideo
            Binary,    // Plik binarny
            Script,    // Typ pliku skryptu (np. .cpp, .h)
            Model,    // Typ pliku skryptu (np. .cpp, .h)
            Prefab,    // Typ pliku skryptu (np. .cpp, .h)
            Unknown    // Nieznany typ pliku
        };

        static const char* toString(Type type) {
            switch (type) {
            case Text:   return "Text";
            case Image:  return "Image";
            case Audio:  return "Audio";
            case Video:  return "Video";
            case Binary: return "Binary";
            case Script: return "Script";
            case Model: return  "Model";
            case Prefab: return  "Prefab";
            case Unknown:return "Unknown";
            default:     return "Invalid Type";
            }
        }

        // Funkcja, która zwraca rozszerzenie pliku na podstawie typu "Script"
        static std::vector<std::string> getExtensions(Type type) {

            switch (type) {
            case Text:   return { ".txt" };
            case Image:  return { ".jpg", ".png" };
            case Audio:  return { ".mp3" };
            case Video:  return { ".mp4" };
            case Binary: return { "" };
            case Script: return { ".cpp", ".h" };
            case Model: return { ".obj", ".fbx" };
            case Prefab: return { ".prefab" };
            case Unknown:return { ".unknown" };
            default:     return { "invalid Type" };
            }
        }
    };

}