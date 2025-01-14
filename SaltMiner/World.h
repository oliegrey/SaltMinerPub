#pragma once

#include "Tile.h"
#include "Vector2d.h"
#include "ImageProcessor.h"

#include <vector>

class Player;

// draws tiles onto and handles the game image
class World {
public:
    inline static constexpr int channels{3};

    inline static constexpr Vector2d dimensionsGrid{7, 8};  // default 7, 8
    inline static constexpr Vector2d dimensions{dimensionsGrid * Tile::dimensions};
    inline static constexpr Vector2d dimensionsRGB{dimensions * channels};

    inline static constexpr int area{dimensions.area()};
    inline static constexpr int areaRGB{area * channels};

private:
    const Tile m_tile{};
    const ImageProcessor imageProcessor{};
    inline static uint8_t* m_template{new uint8_t[areaRGB]};
    uint8_t* m_worldImage{};

    inline static int globalSeed{}; // increments per connection
    uint32_t m_seed{};

    std::vector<Player*> m_connections{}; // element 0 is always owner

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
    // for dummy use
    World() = default;

    World(Player* owner):
        m_worldImage{new uint8_t[areaRGB]}, m_connections{owner} 
    { clear(); }

    ~World() { delete[] m_worldImage; }

    const uint32_t getSeed() const { return m_seed; }

    void setSeed(uint32_t seed) { m_seed = seed; }

    void drawTemplate();

    const uint8_t* image() const { return m_worldImage; }

    const std::string_view compressedImage() const { 
        return imageProcessor.compress(m_worldImage , dimensions);
    }

    void drawGuy(Vector2d pos) { drawTile(m_tile.guy, pos, true); }

    void clear() { std::memcpy(m_worldImage, m_template, areaRGB); }

    Tile tile() { return m_tile; }

    const Player* getOwner() const { return m_connections[0]; }

    auto getConnection(Player* player) {
        return std::find(m_connections.begin(), m_connections.end(), player);
    }

    bool isValidConnection(auto it) { return it == m_connections.end(); }

    void disconnect(Player* player);

    void clearConnections();

    void connect(Player* player);

    bool isOwner(Player* player) { return m_connections[0] == player; }
};
