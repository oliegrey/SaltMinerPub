#include <cassert>

#include <plog/Log.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Random.h"
#include "World.h"

Tile::Tile() {
    /* load all tiles into memory. */
    int _;
    for (size_t i{0}; i < m_tileName.size(); ++i) {
        std::string path{
            "../SaltMiner/images/tiles/" + m_tileName[i] + ".png"
        };
        m_tiles[i] = stbi_load(path.c_str(), &_, &_, &_, 3);

        if (m_tiles[i]) { PLOGD << "Success loading " << path; }
        else { PLOGD << "Failure loading " << path; }
    }
    PLOGD << "Finished loading tiles into memory.";
}

// draws a tile onto an image
void World::drawTile(
    uint8_t* worldImage,
    const Tile::Type type,
    const Vector2d& pos,
    const bool hasAlpha
) {
    if (!worldImage) {
        throw std::invalid_argument("Error: World is nullptr!");
    }
    else if (type > Tile::maxType) {
        throw std::invalid_argument("Error: type doesn't exist!");
    }

    const int indexOffset{
        pos.x * m_tile.widthRGB +
        pos.y * width * m_tile.heightRGB
    };
    const uint8_t* tileImage{m_tile.getTile(type)};

    if (!tileImage) {
        throw std::invalid_argument("Error: failed to get tile!");
    }

    for (int y{0}; y < m_tile.height; ++y) {

        if (hasAlpha) {
            for (int x{0}; x < m_tile.widthRGB; x += 3) {

                const uint8_t* px = &tileImage[y * m_tile.widthRGB + x];

                if (px[0] == 255 && px[1] == 0 && px[2] == 255)
                { continue; }

                else {
                    std::memcpy(
                        &worldImage[y * widthRGB + indexOffset + x],
                        px, sizeof(uint8_t) * 3
                    );      // need to increase copying amount here
                }
            }
        }
        else {
            std::memcpy(
                &worldImage[y * widthRGB + indexOffset],
                &tileImage[y * m_tile.widthRGB],
                sizeof(uint8_t) * m_tile.widthRGB
            );
        }
    }
}