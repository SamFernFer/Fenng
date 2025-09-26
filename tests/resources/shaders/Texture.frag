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
        _att /= lightSqrRadius;
        // Multiplies by the light's intensity.
        _att *= lightIntensity;
        // Outputs the fragment's colour.
        FragColour = vec4(vec3(1.0, 0.75, 0.5) * _att, 1.0);
    } else {
        FragColour = vec4(vec3(0.0), 1.0);
    }
}