#pragma once
#include <vector>
#include <string>
#include <cmath>

#include <stdexcept>
#include <ft2build.h>

#include FT_FREETYPE_H
#include <map>
#include "fstream"



namespace fileOperations {
    
    struct GlyphInfo {
        float x, y;           // Pozycja w atlasie (tekstura SDF)
        float width, height;  // Rozmiar znaku
        float advanceX;       // Przesuniêcie po renderowaniu
        float offsetX, offsetY; // Przesuniêcie wzglêdem linii bazowej
    };

    // Funkcja do generowania atlasu SDF
    std::vector<unsigned char> generateSDFAtlas(const std::string& fontPath, int textureSize, int glyphSize, std::map<char, GlyphInfo>& glyphMap);
}


