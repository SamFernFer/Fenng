#include <Texture.hpp>

namespace Fennton {
    Texture::Texture(Texture&& other) {
        this->width = other.width;
		this->height = other.height;
		this->channelCount = other.channelCount;
		this->id = other.id;
		this->colourFormat = other.colourFormat;
		this->wrappingS = other.wrappingS;
		this->wrappingT = other.wrappingT;
		this->magPixelFilter = other.magPixelFilter;
		this->minPixelFilter = other.minPixelFilter;
		this->mipmapFilter = other.mipmapFilter;
        other.Reset();
    }
    void Texture::Reset() {
        this->width = {};
		this->height = {};
		this->channelCount = {};
		this->id = {};
		this->colourFormat = {};
		this->wrappingS = {};
		this->wrappingT = {};
		this->magPixelFilter = {};
		this->minPixelFilter = {};
		this->mipmapFilter = {};
    }
}