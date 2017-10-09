#version 450
#extension GL_ARB_separate_shader_objects : enable

#define M_PI 3.1415926535897932384626433832795

//Data in:
layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
    float time;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

//Data out:
layout(location = 0) out vec3 outFragColor;
layout(location = 1) out vec2 outFragTexCoord;
layout(location = 2) out float outDiffuseIntensity;

const vec3 directionalLightVector = vec3(2, 3, -5);

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
	//make sure normal is in same space as light direction. You should be able to explain the 0.
    vec3 worldNormal = vec3 (ubo.model * vec4 (inNormal, 0));
    //take the dot of the direction from surface to light with the world surface normal
    outDiffuseIntensity = max (0, dot(-normalize(directionalLightVector), normalize (worldNormal)));
    outFragColor = inColor;
    outFragTexCoord = inTexCoord;
}
