#version 430 core
#extension GL_ARB_bindless_texture: require

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texPos;
layout(location = 2) in vec4 modColor;
layout(location = 3) in float texID;

out float texfId;
out vec4 color;
out vec2 texCoords;

uniform mat4 proj_mat;

void main() {
    gl_Position = proj_mat * vec4(pos.xy, 0.0, 1.0);
    //gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    color = modColor;
    texfId = texID;
    texCoords = texPos;
}