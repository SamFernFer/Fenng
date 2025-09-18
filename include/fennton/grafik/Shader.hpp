#include <glm/glm.hpp>

#include <string>
#include <filesystem>
#include <cstdint>

namespace Fennton::Grafik {
    enum class ShaderType {
        Vertex,
        Fragment
    };
    class Shader;
    // An OpenGL shader stage.
    class Stage {
        friend class Shader;
    public:
        enum class Type {
            None, Geometry, Vertex, Fragment, Compute
        };
        using enum Type;
    private:
        std::string infoLog = {};
        Type type = None;
        std::uint32_t id = 0;

        // Creates the internal shader object with the specified type.
        void Create(Type type);
        // Sets or replaces the shader stage's source code.
        void SetSource(std::string_view src);
        // Compiles the shader stage. Stores the info log, if there is any, 
        // even if there are no errors. Throws on error.
        void Compile();
        // Moves the fields from the other Stage object into this instance and resets the 
        // fields of the other instance to their default values.
        void MoveFrom(Stage&& other);
        // Resets all fields.
        void Reset();
        // Returns the shader stage's ID.
        std::uint32_t GetID() const;
    public:
        // Constructs a null shader stage.
        Stage() = default;
        Stage(Stage const&) = delete;
        Stage(Stage&&);
        Stage& operator=(Stage const&) = delete;
        Stage& operator=(Stage&&);
        ~Stage();
        // Creates a shader stage of the specified type and with the specified source code 
        // and compiles it. This function may throw or error.
        static Stage build(Type type, std::string_view src);
        // Creates a shader stage of the specified type and whose source code is the 
        // contents of the file in the specified path and compiles it. This function may 
        // throw or error.
        static Stage buildFromFile(Type type, std::filesystem::path const& path);
        // Deletes the shader stage if its ID is not zero.
        void Destroy();
    };
    // An OpenGL shader program.
    class Shader {
    private:
        std::string infoLog = {};
        std::uint32_t id = 0;
        // Moves the fields from the other Shader object into this instance and resets the 
        // fields of the other instance to their default values.
        void MoveFrom(Shader&& other);
        // Resets all fields.
        void Reset();
    public:
        Shader() = default;
        Shader(Shader const&) = delete;
        Shader(Shader&&);
        Shader& operator=(Shader const&) = delete;
        Shader& operator=(Shader&&);
        ~Shader();
        // Creates a shader program without any attached stage.
        static Shader create();
        // Deletes the shader program if its ID is not zero.
        void Destroy();
        // Attaches the shader stage to the program.
        void Attach(Stage const& stage);
        // Links the shader program. Stores the info log, if there is any, 
        // even if there are no errors. Throws on error.
        void Link();
        // Binds the shader program.
        void Use();
        // The following `Set` functions all attempt to set the uniform named `name` and throw 
        // if it cannot be found.
        void Set(std::string const& name, float value);
        void Set(std::string const& name, glm::vec2 value);
        void Set(std::string const& name, glm::vec3 value);
        void Set(std::string const& name, glm::vec4 value);
        void Set(std::string const& name, bool value);

        // The follow `TrySet` function all attempt to set the uniform named `name` and 
        // return true on success and false on failure. It is undefined what happens if 
        // the uniform's type is different from that of the `value` argument.
        bool TrySet(std::string const& name, float value);
        bool TrySet(std::string const& name, glm::vec2 value);
        bool TrySet(std::string const& name, glm::vec3 value);
        bool TrySet(std::string const& name, glm::vec4 value);
        bool TrySet(std::string const& name, bool value);
    };
}