#pragma once

#include <iostream>
#include <array>
#include <string>

#include <plog/Log.h>

/* Arranges tiles to create representations of objects in the world */
class Tile {
public:
    inline static constexpr int width{50};
    inline static constexpr int height{50};
    inline static constexpr int widthRGB{width * 3};
    inline static constexpr int heightRGB{width * 3};
    inline static constexpr int area{width * height};
    inline static constexpr int areaRGB{area * 3};

    enum Type {
        dirt,
        stone,
        sky,
        dug,
        guy,
        maxType
    };

private:
    std::array<std::string, maxType> m_tileName{
        "dirt",
        "stone",
        "sky",
        "dug",
        "guy"
    };

    std::array<uint8_t*, maxType> m_tiles{};

public:
    Tile();

    const std::string_view getTileName(Type type) const {
        return m_tileName[type];  // A constexpr method
    }

    const uint8_t* getTile(Type type) const {
        return m_tiles[type];  // This needs to be constexpr as well
    }
};
