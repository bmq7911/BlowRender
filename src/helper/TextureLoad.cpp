#include "TextureLoad.h"
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb-image/stb_image.h"

namespace helper {
    std::shared_ptr<gpc::Texture2d> TextureLoad::makeTexture2D(const char * strPath) const {
        using byte = unsigned char;
        std::ifstream fs(strPath, std::ios::binary);
        if (!fs) {
            return nullptr;
        }
        fs.seekg(0, std::ios::end);
        auto size = fs.tellg();
        byte *pData = (byte*)malloc( size * sizeof(byte));
        fs.seekg(0);
        fs.read((char*)pData, size);
        fs.close();
        int32_t width;
        int32_t height;
        int channels;
        stbi_uc* pimage = stbi_load_from_memory((const stbi_uc*)pData, size, (int*)&width, (int*)&height, &channels, STBI_rgb_alpha);

        if (nullptr == pimage) {
            return nullptr;
        }
        std::shared_ptr<gpc::Texture2d> texture = std::make_shared<gpc::Texture2d>(width, height);
        texture->m_image.copyIn( pimage, sizeof(byte)*4 * width * height);
        return texture;
    }
    
}