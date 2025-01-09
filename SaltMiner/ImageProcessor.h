#pragma once

#include <stdint.h>
#include <string>
#include <plog/Log.h>

#include <turbojpeg.h>

class ImageProcessor {
private:
    tjhandle m_compressor{tjInitCompress()};

public:
    const std::string_view compress(
        uint8_t* img, int width, int height
    ) const {
        unsigned char* buffer{nullptr};
        long unsigned int size = 0;
        if (!tjCompress2(
                m_compressor,
                img, width, 0, height,    // image data and dimensions        
                TJPF_RGB,                 // pixel format
                &buffer, &size,           // address for result and size   
                TJSAMP_444, 85,           // chrominance and quality
                TJFLAG_ACCURATEDCT))      // accurate or fast

        { return {reinterpret_cast<char*>(buffer), size}; }

        else { PLOGD << "Compression failed."; return ""; }
    }
};
