#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D inTexSampler;

layout(location = 0) in vec3 inFragColor;
layout(location = 1) in vec2 inFragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
  outColor = vec4(inFragColor * texture(inTexSampler, inFragTexCoord).rgb, 1.0);
}
