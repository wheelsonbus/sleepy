#version 450
#pragma shader_stage(fragment)

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTextureCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(inTextureCoord.x, inTextureCoord.y, 0.5f, 1.0f);
}