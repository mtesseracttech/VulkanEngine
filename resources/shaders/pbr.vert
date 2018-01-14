#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

void main() {
	gl_Position = vec4(inPos.xyz, 1.0);
}
