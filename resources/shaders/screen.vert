#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

// Scale for handling a mismatch between the aspect ratio of the window and the texture.
uniform vec2 scale;

void main() {
    vec2 _pos = aPos * scale;
    gl_Position = vec4(_pos, 0.0f, 1.0f);
    texCoords = aTexCoords;
}