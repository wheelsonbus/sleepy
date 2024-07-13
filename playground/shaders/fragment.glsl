#version 450
#pragma shader_stage(fragment)

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTextureCoordinates;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D textureSampler;

void main() {
    outColor = texture(textureSampler, inTextureCoordinates);
}