#include "fileOperations/fontLoader.h"

std::vector<unsigned char> fileOperations::generateSDFAtlas(const std::string& fontPath, int textureSize, int glyphSize, std::map<char, GlyphInfo>& glyphMap) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        throw std::runtime_error("Could not initialize FreeType.");
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        throw std::runtime_error("Failed to load font.");
    }

    FT_Set_Pixel_Sizes(face, 0, glyphSize);

    int atlasWidth = textureSize;
    int atlasHeight = textureSize;
    std::vector<unsigned char> atlasData(atlasWidth * atlasHeight, 0);

    int offsetX = 0;
    int offsetY = 0;
    int maxRowHeight = 0;

    for (char c = 32; c < 127; ++c) { // Przetwarzanie znaków od 32 do 126 (ASCII)
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            throw std::runtime_error("Failed to load character.");
        }

        FT_Bitmap& bitmap = face->glyph->bitmap;

        // Sprawdzanie, czy znak mieœci siê w aktualnym wierszu
        if (offsetX + bitmap.width >= atlasWidth) {
            offsetX = 0;
            offsetY += maxRowHeight + 1;
            maxRowHeight = 0;
        }

        if (offsetY + bitmap.rows >= atlasHeight) {
            throw std::runtime_error("Atlas texture size is too small.");
        }

        // Kopiowanie danych bitmapy do atlasu
        for (int y = 0; y < bitmap.rows; ++y) {
            for (int x = 0; x < bitmap.width; ++x) {
                int atlasIndex = (offsetY + y) * atlasWidth + (offsetX + x);
                atlasData[atlasIndex] = bitmap.buffer[y * bitmap.width + x];
            }
        }

        // Zapisywanie informacji o znaku
        glyphMap[c] = {
            static_cast<float>(offsetX) / atlasWidth,
            static_cast<float>(offsetY) / atlasHeight,
            static_cast<float>(bitmap.width) / atlasWidth,
            static_cast<float>(bitmap.rows) / atlasHeight,
            static_cast<float>(face->glyph->advance.x >> 6),
            static_cast<float>(face->glyph->bitmap_left),
            static_cast<float>(face->glyph->bitmap_top)
        };

        offsetX += bitmap.width + 1;
        maxRowHeight = std::max(maxRowHeight, static_cast<int>(bitmap.rows));
    }

    /*
    // Zapisz atlas (np. jako plik binarny lub teksturê)
    std::ofstream outFile(outputPath, std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(atlasData.data()), atlasData.size());
    outFile.close();
    */
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return atlasData;
}
