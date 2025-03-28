#include <Texture.hpp>

#include <utils/Misc.hpp>
#include <ImageLoad.h>

#include <glad/glad.h>

#include <stdexcept>
#include <utility>

namespace fs = std::filesystem;

namespace Fennton {
    void Texture::ApplyWrapping(
        std::uint32_t axis,
        Texture::WrappingMode mode
    ) {
        std::uint32_t _mode;
        // Gets the OpenGL enum from the wrapping mode enum value.
        switch (mode) {
            case WrappingMode::Repeat:
            default:
                _mode = GL_REPEAT;
                break;
            case WrappingMode::MirroredRepeat:
                _mode = GL_MIRRORED_REPEAT;
                break;
            case WrappingMode::ClampToEdge:
                _mode = GL_CLAMP_TO_EDGE;
                break;
            case WrappingMode::ClampToBorder:
                _mode = GL_CLAMP_TO_BORDER;
                break;
        }
        glTexParameteri(GL_TEXTURE_2D, axis, _mode);
    }
    void Texture::ApplyMagPixelFilter(Texture::Filter filter) {
        switch (filter) {
            case Filter::Nearest:
            default:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;
            case Filter::Linear:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
        }
    }
    void Texture::ApplyMinFilter(
        Texture::Filter pixelFilter,
        Texture::Filter mipmapFilter
    ) {
        // Helper macro for getting the filter enum.
        #define FENNTON_SET_FILTER(pixelFilter, mipmapFilter)\
            glTexParameteri(\
                GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,\
                GL_##pixelFilter##_MIPMAP_##mipmapFilter\
            );
        switch (pixelFilter) {
            case Filter::Nearest:
            default:
                switch (mipmapFilter) {
                    case Filter::None:
                    default:
                        glTexParameteri(
                            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST
                        );
                        break;
                    case Filter::Nearest:
                        FENNTON_SET_FILTER(NEAREST, NEAREST);
                        break;
                    case Filter::Linear:
                        FENNTON_SET_FILTER(NEAREST, LINEAR);
                        break;
                }
                break;
            case Filter::Linear:
                switch (mipmapFilter) {
                    case Filter::None:
                    default:
                        glTexParameteri(
                            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR
                        );
                        break;
                    case Filter::Nearest:
                        FENNTON_SET_FILTER(LINEAR, NEAREST);
                        break;
                    case Filter::Linear:
                        FENNTON_SET_FILTER(LINEAR, LINEAR);
                        break;
                }
                break;
        }
        #undef FENNTON_FILTER_ENUM
    }
    Texture::Texture(
        const fs::path& path,
        Texture::WrappingMode wrappingS,
        Texture::WrappingMode wrappingT,
        Texture::Filter magPixelFilter,
        Texture::Filter minPixelFilter,
        Texture::Filter mipmapFilter
    ) {
        if (!fs::is_regular_file(path)) {
            throw std::runtime_error(std::format(
                "[Filesystem] {} is not a file.", Fennton::quote(path.generic_string())
            ));
        }
        std::uint8_t* _data = stbi_load(
            path.generic_string().c_str(), &width, &height, &channelCount, 0
        );

        if (!_data) {
            throw std::runtime_error(std::format(
                "[Image Reading] Could not read image {} from disk.",
                Fennton::quote(path.generic_string())
            ));
        }

        glGenTextures(1, &id);
        Bind();

        // Sets the filters.
        SetMagPixelFilter(magPixelFilter);
        SetMinPixelFilter(minPixelFilter);
        SetMipmapFilter(mipmapFilter);

        // Sets the wrapping mode for each axis.
        SetWrappingS(wrappingS);
        SetWrappingT(wrappingT);

        // Chooses the texture's format based on the channel count.
        switch (channelCount) {
            case 4:
                format = GL_RGBA;
                colourFormat = Format::cRGBA;
                break;
            case 3:
                format = GL_RGB;
                colourFormat = Format::cRGB;
                break;
            case 2:
                format = GL_RG;
                colourFormat = Format::cRG;
                break;
            case 1:
            default:
                format = GL_RED;
                colourFormat = Format::Red;
        }

        glTexImage2D(
            GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, _data
        );
        if (mipmapFilter != Filter::None) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        // Frees the image data.
        stbi_image_free(_data);
    }
    Texture::Texture(
        std::int32_t width,
        std::int32_t height,
        std::uint8_t* data,
        Texture::Format colourFormat,
        WrappingMode wrappingS,
        WrappingMode wrappingT,
        Filter magPixelFilter,
        Filter minPixelFilter,
        Filter mipmapFilter
    ) : width(width), height(height), colourFormat(colourFormat) {
        glGenTextures(1, &id);
        Bind();

        // Sets the filters.
        SetMagPixelFilter(magPixelFilter);
        SetMinPixelFilter(minPixelFilter);
        SetMipmapFilter(mipmapFilter);

        // Sets the wrapping mode for each axis.
        SetWrappingS(wrappingS);
        SetWrappingT(wrappingT);

        // Converts the Format enum value to an OpenGL pixel data format.
        switch (colourFormat) {
            case Format::cRGBA:
                format = GL_RGBA;
                channelCount = 4;
                break;
            case Format::cRGB:
                format = GL_RGB;
                channelCount = 3;
                break;
            case Format::cRG:
                format = GL_RG;
                channelCount = 2;
                break;
            case Format::Red:
            default:
                format = GL_RED;
                channelCount = 1;
        }

        glTexImage2D(
            GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
            data? data : NULL // Just in case nullptr and NULL are not the same.
        );

        if (mipmapFilter != Filter::None) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        // Unbind();
    }
    Texture::~Texture() {
        // Deletes the texture.
        if (id) glDeleteTextures(1, &id);
    }
    void Texture::Bind() const {
        glBindTexture(GL_TEXTURE_2D, id);
    }
    void Texture::BindTo(std::uint32_t unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        Bind();
    }
    void Texture::Unbind(std::uint32_t unit) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    std::int32_t Texture::GetWidth() const {
        return width;
    }
    std::int32_t Texture::GetHeight() const {
        return height;
    }
    std::int32_t Texture::GetChannelCount() const {
        return channelCount;
    }
    std::uint32_t Texture::GetID() const {
        return id;
    }
    Texture::Format Texture::GetColourFormat() const {
        return colourFormat;
    }
    Texture::WrappingMode Texture::GetWrappingS() const {
        return wrappingS;
    }
    Texture::WrappingMode Texture::GetWrappingT() const {
        return wrappingT;
    }
    void Texture::SetWrappingS(
        Texture::WrappingMode mode
    ) {
        wrappingS = mode;
        ApplyWrapping(GL_TEXTURE_WRAP_S, mode);
    }
    void Texture::SetWrappingT(
        Texture::WrappingMode mode
    ) {
        wrappingT = mode;
        ApplyWrapping(GL_TEXTURE_WRAP_T, mode);
    }
    Texture::Filter Texture::GetMagPixelFilter() const {
        return magPixelFilter;
    }
    Texture::Filter Texture::GetMinPixelFilter() const {
        return minPixelFilter;
    }
    Texture::Filter Texture::GetMipmapFilter() const {
        return mipmapFilter;
    }
    void Texture::SetMagPixelFilter(Texture::Filter filter) {
        magPixelFilter = filter;
        ApplyMagPixelFilter(filter);
    }
    void Texture::SetMinPixelFilter(Texture::Filter filter) {
        minPixelFilter = filter;
        ApplyMinFilter(filter, mipmapFilter);
    }
    void Texture::SetMipmapFilter(Texture::Filter filter) {
        mipmapFilter = filter;
        ApplyMinFilter(minPixelFilter, filter);
    }
    void Texture::SetData(std::uint8_t* data) {
        Bind();
        glTexSubImage2D(
            GL_TEXTURE_2D, 0,
            0, 0,
            width, height, format,
            GL_UNSIGNED_BYTE, data
        );
    }
    void Texture::SetData(
        std::uint8_t* data,
        glm::ivec2 const& position,
        glm::ivec2 const& size,
        std::int32_t level
    ) {
        Bind();
        glTexSubImage2D(
            GL_TEXTURE_2D,
            level,
            position.x, position.y,
            size.x, size.y,
            format,
            GL_UNSIGNED_BYTE, data
        );
    }
    Framebuffer::Framebuffer(
        Texture&& texture
    ) : texture(std::move(texture)) {
        glGenFramebuffers(1, &id);
        Bind();

        // Attaches the texture to the framebuffer.
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture.GetID(), 0
        );
        // Creates the renderbuffer for the depth and stencil attachments.
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(
            GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
            this->texture.GetWidth(), this->texture.GetHeight()
        );
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        // Attaches the renderbuffer to the framebuffer.
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo
        );
        // Checks if the framebuffer is complete.
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("[Framebuffer] Framebuffer is not complete.");
        }
        // Unbinds the framebuffer.
        Unbind();
    }
    Framebuffer::~Framebuffer() {
        // Binds the framebuffer before detaching its attachments.
        Bind();
        // Detaches the texture from the framebuffer before deleting it.
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0
        );
        // Detaches the renderbuffer from the framebuffer before deleting it.
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0
        );
        // Unbinds the framebuffer.
        Unbind();
        // Deletes the renderbuffer.
        if (rbo) glDeleteRenderbuffers(1, &rbo);
        // Deletes the framebuffer.
        if (id) glDeleteFramebuffers(1, &id);
    }
    void Framebuffer::Bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
    }
    void Framebuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    std::uint32_t Framebuffer::GetID() const {
        return id;
    }
    const Texture& Framebuffer::GetTexture() const {
        return texture;
    }
}