#version 330 core
out vec4 FragColour;

in vec3 vertexPos;

uniform vec2 lightPos;
uniform float lightIntensity;
uniform float lightSqrRadius;

void main() {
    vec2 _delta = lightPos - vec2(vertexPos);
    // Linear attenuation.
    float _att = lightSqrRadius - dot(_delta, _delta);
    // Only calculates the lighting for pixels which are affected by the light.
    if (_att > 0) {
        // Divides by the radius to normalise it.
        _att /= lightRadius;
        // Multiplies by the light's intensity.
        _att *= lightIntensity;
        _col = vec3() * _att;
        // Outputs the fragment's colour.
        FragColour = vec4(_col, 1.0);
    } else {
        FragColour = vec3(0.0);
    }
}