#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

//UBO is on binding 0
layout (binding = 1) uniform samplerCube samplerColor;

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in float inLodBias;
layout (location = 3) in vec3 inViewVec;
layout (location = 4) in vec3 inLightVec;
layout (location = 5) in mat4 inInvModelView;

layout (location = 0) out vec4 outFragColor;

void main()
{
    //Getting local reflection
	vec3 localReflection = reflect(normalize(inPos), normalize(inNormal));

	localReflection = vec3(inInvModelView * vec4(localReflection, 1.0)); //Scalar may have to be 0.0
	localReflection.x *= -1.0;

    //Getting sample color
	vec4 color = texture(samplerColor, localReflection, inLodBias);

    //Normalize Inputs
	vec3 surfaceNormal = normalize(inNormal);
	vec3 surfaceLight = normalize(inLightVec);
	vec3 viewDirection = normalize(inViewVec);

	vec3 reflectionVec = reflect(-surfaceLight, surfaceNormal);
	vec3 ambient = vec3(0.5) * color.rgb;
	vec3 diffuse = max(dot(surfaceNormal, surfaceLight), 0.0) * vec3(1.0);
	vec3 specular = pow(max(dot(reflectionVec, viewDirection), 0.0), 16.0) * vec3(0.5);
	outFragColor = vec4(ambient + diffuse * color.rgb + specular, 1.0);
}
