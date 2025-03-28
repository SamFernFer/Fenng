#include <Resources.hpp>
#include <utils/FileUtils.hpp>

#include <glad/glad.h>

namespace fs = std::filesystem;

namespace Fennton::Resources {
    static fs::path resPath;
    static fs::path exePath;
    static fs::path rootPath;

    static std::uint32_t quadVAO = 0;

    void init() {
        exePath = fs::path(Fennton::FileUtils::getExecutablePath());
        // The root is 3 levels above the executable's path.
        // NOTE: it considers the "release" or "debug" directory.
        rootPath = exePath.parent_path().parent_path().parent_path();
        resPath = rootPath / "resources";
    }
    std::filesystem::path getExePath() {
        return exePath;
    }
    std::filesystem::path getRootPath() {
        return rootPath;
    }
    std::filesystem::path getPath(std::filesystem::path const& path) {
        return resPath / path;
    }
    std::filesystem::path getPath() {
        return resPath;
    }
    void initQuad() {
        // Creates the framebuffer's quad.
        float quadVertices[] = {
            // positions  texCoords
            -1.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };

        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);

        std::uint32_t quadVBO;
        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(
            GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW
        );

        // Vertex positions.
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Texture coordinates.
        glVertexAttribPointer(
            1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))
        );
        glEnableVertexAttribArray(1);
    }
    std::uint32_t getQuadVAO() {
        return quadVAO;
    }
}