#version 330 core
out vec4 fragColour;

in vec2 texCoords;

uniform sampler2D screenTex;
uniform sampler2D opacityTex;
uniform sampler2D lightTex;

void main() {
    fragColour = texture(screenTex, texCoords);
}