#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;

layout (binding = 0) uniform UBO
{
	mat4 model;
	mat4 view;
	mat4 projection;
	float lodBias;
} ubo;

layout (location = 0) out vec3 outPos;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out float outLodBias;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;
layout (location = 5) out mat4 outInvModelView;
layout (location = 9) out vec2 outUv;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPos.xyz, 1.0);

	outUv = inUv;

	outPos = vec3(ubo.view * ubo.model * vec4(inPos, 1.0));
	outNormal = mat3(ubo.view * ubo.model) * inNormal;
	outLodBias = ubo.lodBias;

	outInvModelView = inverse(ubo.view);

	vec3 lightPos = vec3(0.0f, -5.0f, 5.0f);
	outLightVec = lightPos.xyz - outPos.xyz;
	outViewVec = -outPos.xyz;
}
