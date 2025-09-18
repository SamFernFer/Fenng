#version 330 core
out vec4 FragColour;

in vec3 vertexPos;
in vec3 vertexColour;

uniform vec2 lightPos;
uniform float lightIntensity;
uniform float lightRadius;

void main() {
    vec3 _col = vec3(0.75, 0.5, 0.25) * vertexColour;
    vec2 _delta = lightPos - vec2(vertexPos);
    // Linear attenuation.
    float _att = lightRadius - dot(_delta, _delta);
    // Divides by the radius to normalise it.
    _att /= lightRadius;
    // Calculates the lighting.
    _col *= _att * lightIntensity;
    // Outputs the fragment's colour.
    FragColour = vec4(_col, 1.0);
}