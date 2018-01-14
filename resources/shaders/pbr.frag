#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) out vec4 outFragColor;

void main() {
	outFragColor = vec4(0,0,0,1);
}
