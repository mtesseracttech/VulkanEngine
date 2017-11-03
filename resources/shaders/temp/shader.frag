#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D inTexSampler;

layout(location = 0) in vec3 inFragColor;
layout(location = 1) in vec2 inFragTexCoord;
layout(location = 2) in float inDiffuseIntensity;

layout(location = 0) out vec4 outColor;

const vec3 diffuseColor = vec3(0.8,0.8,0.8);
const vec3 directionalLightColor = vec3(1,1,1);
const vec3 globalAmbient = vec3(0.2,0.2,0.2);

void main()
{
	vec3 ambientTerm = texture(inTexSampler, inFragTexCoord).rgb * diffuseColor;
    vec3 diffuseTerm = inDiffuseIntensity * directionalLightColor * diffuseColor;
	vec3 specularTerm = vec3(0,0,0);
    outColor = vec4 (ambientTerm + diffuseTerm + specularTerm, 1);

    //outColor = vec4(inFragColor * texture(inTexSampler, inFragTexCoord).rgb, 1.0);
}
