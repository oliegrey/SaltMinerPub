#pragma once

#include <stdint.h>
#include <string>
#include <plog/Log.h>

#include <turbojpeg.h>

#include "Vector2d.h"

class ImageProcessor {
private:
    tjhandle m_compressor{tjInitCompress()};

public:
    const std::string_view compress(uint8_t* img, Vector2d size) const {
        std::cout << size;
        unsigned char* buffer{nullptr};
        long unsigned int dataSize = 0;
        if (!tjCompress2(
                m_compressor,
                img, size.x, 0, size.y, // image data and dimensions        
                TJPF_RGB,               // pixel format
                &buffer, &dataSize,     // address for result and size   
                TJSAMP_444, 85,         // chrominance and quality
                TJFLAG_ACCURATEDCT))    // accurate or fast

        { return {reinterpret_cast<char*>(buffer), dataSize}; }

        else { PLOGD << "Compression failed."; return ""; }
    }
};
