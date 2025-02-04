#version 430 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D usedTexture;

void main() {
    vec4 color = texture(
        usedTexture,
        vec2(TexCoords.x, TexCoords.y)
    );
    if (color.w < 0.1) discard;
    FragColor = color;
}