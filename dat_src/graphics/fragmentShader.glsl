#version 430 core
#extension GL_ARB_bindless_texture: require

in float texfId;
in vec4 color;
in vec2 texCoords;

out vec4 fragColor;

layout(std430, binding = 0) readonly buffer ssbo1 {
	sampler2D textureHandles[];
} tBuffers;

void main() {
    fragColor = color;

    int texId = int(texfId); //convert dem texture ids :D

    if (texId >= 0) {
        fragColor = texture(tBuffers.textureHandles[texId], texCoords);
    }
}