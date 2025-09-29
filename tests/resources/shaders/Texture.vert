#version 330 core
layout (location = 0) in vec3 aPos;
// Skipping the vertex colours.

// Texture coordinates.
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexPos;
out vec2 texCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexPos = aPos;
    texCoord = aTexCoord;
}