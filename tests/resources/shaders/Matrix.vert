#version 330 core
layout (location = 0) in vec3 aPos;
// Vertex colours.
layout (location = 1) in vec3 aColour;
// Texture coordinates.
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexPos;
out vec3 vertexColour;
out vec2 texCoord;

uniform mat4 model, view, proj;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexPos = aPos;
    vertexColour = aColour;
    texCoord = aTexCoord;
}