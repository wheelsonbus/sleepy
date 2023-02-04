#version 450
#pragma shader_stage(vertex)

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = vec4(1.0, 1.0, 1.0, 1.0 - gl_Position.z);
}