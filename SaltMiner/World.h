#pragma once

#include "Tile.h"
#include "Vector2d.h"
#include "ImageProcessor.h"

// draws tiles onto the game image
class World {
public:
    inline static constexpr int columns{7};    // default 7
    inline static constexpr int rows{8};       // default 8
    inline static constexpr int width{Tile::width * columns};
    inline static constexpr int height{Tile::height * rows};
    inline static constexpr int area{width * height};
    inline static constexpr int widthRGB{width * 3};
    inline static constexpr int heightRGB{height * 3};
    inline static constexpr int areaRGB{widthRGB * heightRGB};

private:
    const Tile m_tile{};
    const ImageProcessor imageProcessor{};
    uint8_t* m_template{};
    uint8_t* m_worldImage{};

    void drawTile(
        const Tile::Type type,
        const Vector2d& pos,
        const bool hasAlpha=false
    ) { drawTile(m_worldImage, type, pos, hasAlpha); }

    void drawTile(
        uint8_t* worldImage,
        const Tile::Type type,
        const Vector2d& pos,
        const bool hasAlpha=false
    );

public:
    World(): 
        m_template{new uint8_t[areaRGB]}, 
        m_worldImage{new uint8_t[areaRGB]}
    {
        for (int x{0}; x < columns; ++x) {
            for (int y{0}; y < rows; ++y) {
                drawTile(m_template, m_tile.dirt, {x, y});
            }
        }
        PLOGD << "Constructed empty template.";

        clear();
    }

    const uint8_t* image() const { return m_worldImage; }

    const std::string_view compressedImage() const { 
        return imageProcessor.compress(m_worldImage, width, height);
    }

    void drawGuy(Vector2d pos) {
        drawTile(m_tile.guy, pos, true);
    }

    void clear() { std::memcpy(m_worldImage, m_template, areaRGB); }
};
