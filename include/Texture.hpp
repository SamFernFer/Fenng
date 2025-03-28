#ifndef FENNTON_TEXTURE_HPP
#define FENNTON_TEXTURE_HPP

#include <glm/glm.hpp>
#include <cstdint>
#include <filesystem>

namespace Fennton {
    class Texture {
    public:
        enum class WrappingMode {
            Repeat,
            MirroredRepeat,
            ClampToEdge,
            ClampToBorder
        };
        enum class Filter {
            None, Nearest, Linear
        };
        enum class Format {
            cRGBA,
            cRGB,
            cRG,
            Red
        };
    private:
        std::int32_t width, height, channelCount;
        std::uint32_t id;
        // The OpenGL pixel data format.
        std::uint32_t format;
        Format colourFormat;
        WrappingMode wrappingS, wrappingT;
        Filter magPixelFilter, minPixelFilter, mipmapFilter;

        // Resets all fields.
        void Reset();
        // Applies the wrapping mode for the specified axis.
        void ApplyWrapping(std::uint32_t axis, WrappingMode mode);
        // Applies the magnification filter. Bind must be called first.
        void ApplyMagPixelFilter(Filter filter);
        // Appies the minification filter. Bind must be called first.
        void ApplyMinFilter(Filter pixelFilter, Filter mipmapFilter);
    public:
        Texture(
            const std::filesystem::path& path,
            WrappingMode wrappingS = WrappingMode::Repeat,
            WrappingMode wrappingT = WrappingMode::Repeat,
            Filter magPixelFilter = Filter::Nearest,
            Filter minPixelFilter = Filter::Nearest,
            Filter mipmapFilter = Filter::Nearest
        );
        Texture(
            std::int32_t width,
            std::int32_t height,
            std::uint8_t* data,
            Format colourFormat,
            WrappingMode wrappingS = WrappingMode::Repeat,
            WrappingMode wrappingT = WrappingMode::Repeat,
            Filter magPixelFilter = Filter::Nearest,
            Filter minPixelFilter = Filter::Nearest,
            Filter mipmapFilter = Filter::None
        );
        Texture(const Texture& other) = delete;
        Texture(Texture&& other);
        ~Texture();
        // Binds the texture for the next texture operations.
        void Bind() const;
        // Binds the texture to the specified unit.
        void BindTo(std::uint32_t unit) const;
        // Unbinds the texture unit.
        static void Unbind(std::uint32_t unit);
        // Returns the texture's width.
        std::int32_t GetWidth() const;
        // Returns the texture's height.
        std::int32_t GetHeight() const;
        // Returns the texture's channel count.
        std::int32_t GetChannelCount() const;
        // Returns the texture's ID.
        std::uint32_t GetID() const;
        Format GetColourFormat() const;
        // Gets the wrapping for the S axis.
        WrappingMode GetWrappingS() const;
        // Gets the wrapping for the T axis.
        WrappingMode GetWrappingT() const;
        // Sets the wrapping for the S axis. Bind must be called first.
        void SetWrappingS(WrappingMode mode);
        // Sets the wrapping for the T axis. Bind must be called first.
        void SetWrappingT(WrappingMode mode);
        // Gets the magnification pixel filter.
        Filter GetMagPixelFilter() const;
        // Gets the minification pixel filter.
        Filter GetMinPixelFilter() const;
        // Gets the mipmap pixel filter.
        Filter GetMipmapFilter() const;
        // Sets the magnification pixel filter. Bind must be called first.
        void SetMagPixelFilter(Filter filter);
        // Sets the minification pixel filter. Bind must be called first.
        void SetMinPixelFilter(Filter filter);
        // Sets the mipmap pixel filter. Bind must be called first.
        void SetMipmapFilter(Filter filter);
        // Updates all the texture's data in mipmap lavel 0.
        void SetData(std::uint8_t* data);
        // Updates the texture's data in the specified region at the specified mipmap lavel.
        void SetData(
            std::uint8_t* data, // The data to use.
            glm::ivec2 const& position, // The position of the region.
            glm::ivec2 const& size, // The size of the region.
            std::int32_t level = 0 // Which mipmap level to update.
        );
    };

    class Framebuffer {
        std::uint32_t id, rbo;
        const Texture texture;
    public:
        // Creates a framebuffer with texture as the colour attachment. The texture gets 
        // moved to inside the framebuffer class. The framebuffer's depth and stencil 
        // renderbuffer are created with the texture's dimensions.
        Framebuffer(
            Texture&& texture
        );
        ~Framebuffer();
        // Binds the framebuffer for the next framebuffer operations.
        void Bind() const;
        // Sets the current framebuffer to 0 (the window's framebuffer).
        static void Unbind();
        // Returns the framebuffer's ID.
        std::uint32_t GetID() const;
        // Returns the framebuffer's internal texture.
        const Texture& GetTexture() const;
    };
}
#endif