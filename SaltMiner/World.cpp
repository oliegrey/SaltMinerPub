#include <cassert>
#include <vector>

#include <plog/Log.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Random.h"
#include "World.h"
#include "Player.h"

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
        pos.x * m_tile.dimensionsRGB.x +
        pos.y * m_tile.dimensionsRGB.y * dimensions.x
    };

    uint8_t* tileImage{m_tile.getTile(type)};

    if (!tileImage) {
        throw std::invalid_argument("Error: failed to get tile!");
    }

    for (int y{0}; y < m_tile.dimensions.y; ++y) {

        if (hasAlpha) {
            for (int x{0}; x < m_tile.dimensionsRGB.x; x += channels) {

                const uint8_t* tilePx = &tileImage[y * m_tile.dimensionsRGB.x + x];

                if (tilePx[0] == 255 && tilePx[1] == 0 && tilePx[2] == 255)
                { continue; }

                else {
                    std::memcpy(
                        &worldImage[y * dimensionsRGB.x + indexOffset + x],
                        tilePx, sizeof(uint8_t) * channels
                    );      // need to increase copying amount here
                }
            }
        }
        else {
            std::memcpy(
                &worldImage[y * dimensionsRGB.x + indexOffset],
                &tileImage[y * m_tile.dimensionsRGB.x],
                sizeof(uint8_t) * m_tile.dimensionsRGB.x
            );
        }
    }
}

void World::drawTemplate() {
    PLOGD << "Drawing template...";
    for (int x{0}; x < dimensionsGrid.x; ++x) {
        for (int y{0}; y < dimensionsGrid.y; ++y) {
            drawTile(m_template, m_tile.dirt, {x, y});
        }
    }
    PLOGD << "Template drawn.";
}

// if first connection, make owner?
void World::connect(Player* player) {
}

void World::clearConnections() {
    for (Player* player : m_connections) { disconnect(player); }
}

void World::disconnect(Player* player) {
    if (player->getWorld() == this) { player->setWorld(nullptr); }
    
    auto connection{getConnection(player)};
    if (isValidConnection(connection)) { m_connections.erase(connection); }
}