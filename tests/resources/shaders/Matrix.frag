#version 330 core
out vec4 FragColour;

in vec3 vertexPos;
in vec3 vertexColour;
in vec2 texCoord;

uniform vec2 lightPos;
uniform float ambientIntensity;
uniform float lightIntensity;
uniform float lightSqrRadius;

uniform sampler2D tex1;

void main() {
    vec2 _delta = lightPos - vec2(vertexPos);
    // Linear attenuation.
    float _att = lightSqrRadius - dot(_delta, _delta);
    // Only calculates the attenuation for pixels which are affected by the light.
    if (_att > 0.0) {
        // Divides by the radius to normalise it.
        _att /= lightSqrRadius;
        // Multiplies by the light's intensity.
        _att *= lightIntensity;
    } else {
        _att = 0.0;
    }
    // Adds the ambient contribution.
    _att += ambientIntensity;
    // Outputs the fragment's colour.
    FragColour = vec4(texture(tex1, texCoord).xyz * _att, 1.0);
}