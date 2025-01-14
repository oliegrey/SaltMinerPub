#pragma once

#include <iostream>
#include <array>
#include <string>

#include <plog/Log.h>

#include "Vector2d.h"

/* Arranges tiles to create representations of objects in the world */
class Tile {
public:
    inline static constexpr int channels{3};

    inline static constexpr Vector2d dimensions{50, 50};
    inline static constexpr Vector2d dimensionsRGB{dimensions * channels};

    inline static constexpr int area{dimensions.area()};
    inline static constexpr int areaRGB{area * channels};

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
        return m_tileName[type];
    }

    uint8_t* getTile(Type type) const {
        return m_tiles[type];
    }
};
