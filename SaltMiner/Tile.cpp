#include "stb_image.h"

#include "Tile.h"

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