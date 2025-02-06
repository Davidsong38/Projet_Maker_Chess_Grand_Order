#version 430 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D usedTexture;
uniform vec3 filterColor;
uniform vec4 defaultColor;

void main() {
    vec4 color = texture(
        usedTexture,
        vec2(TexCoords.x, TexCoords.y)
    );
    color.x += filterColor.x;
    color.y += filterColor.y;
    color.z += filterColor.z;
    if (color.w < 0.1) {
        color = defaultColor;
    }
    FragColor = color;
}