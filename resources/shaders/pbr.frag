#version 450

layout (location = 0) in vec3 inWorldPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

layout (binding = 0) uniform UBO
{
	mat4 projection;
	mat4 model;
	mat4 view;
	vec3 camPos;
} ubo;

layout (location = 0) out vec4 outColor;

layout (binding = 1) uniform UBOShared {
	vec4 lights[4];
} uboParams;

layout(push_constant) uniform PushConsts {
	//layout(offset = 0) vec3 color;
	layout(offset = 12) float r; //Starts at 12 because of UBO
	layout(offset = 16) float g;
	layout(offset = 20) float b;
	layout(offset = 24) float roughness;
	layout(offset = 28) float metallic;
} material;

const float PI = 3.14159265359;

vec3 getColor(){
	return vec3(material.r, material.g, material.b);
}


//Normal distribution
float normalDistribution(float dotNH, float roughness)
{
	float alpha = roughness * roughness;
	float alpha2 = alpha * alpha;
	float denom = dotNH * dotNH * (alpha2 - 1.0) + 1.0;
	return (alpha2)/(PI * denom*denom);
}

float G_SchlicksmithGGX(float dotNL, float dotNV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;
	float GL = dotNL / (dotNL * (1.0 - k) + k);
	float GV = dotNV / (dotNV * (1.0 - k) + k);
	return GL * GV;
}

//Fresnel calculation
//CosTheta is the dot product at which the camera looks at the surface of an object
vec3 fresnel(float cosTheta, float metallic)
{
	vec3 F0 = mix(vec3(0.04), getColor(), metallic);
	vec3 F = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
	return F;
}

//Bidirectional reflectance distribution
vec3 brdf(vec3 L, vec3 V, vec3 N, float metallic, float roughness)
{
	//total direction of both light and camera
	vec3 H = normalize (V + L);
	float dotNV = clamp(dot(N, V), 0.0, 1.0);
	float dotNL = clamp(dot(N, L), 0.0, 1.0);
	float dotLH = clamp(dot(L, H), 0.0, 1.0);
	float dotNH = clamp(dot(N, H), 0.0, 1.0);

	// Light color fixed
	vec3 lightColor = vec3(1.0);

	vec3 color = vec3(0.0);

	if (dotNL > 0.0)
	{
		float roughness = max(0.05, roughness);
		// D = Normal distribution (Distribution of the microfacets)
		float D = normalDistribution(dotNH, roughness);
		// G = Geometric shadowing term (Microfacets shadowing)
		float G = G_SchlicksmithGGX(dotNL, dotNV, roughness);
		// F = Fresnel factor (Reflectance depending on angle of incidence)
		vec3 F = fresnel(dotNV, metallic);

		vec3 spec = D * F * G / (4.0 * dotNL * dotNV);

		color += spec * dotNL * lightColor;
	}

	return color;
}

void main()
{
	vec3 N = normalize(inNormal);
	//Normalized Camera Direction
	vec3 V = normalize(ubo.camPos - inWorldPos);

	// Specular contribution
	vec3 Lo = vec3(0.0);
	for (int i = 0; i < uboParams.lights.length(); i++) {
	    //In world light direction
		vec3 L = normalize(uboParams.lights[i].xyz - inWorldPos);
		Lo += brdf(L, V, N, material.metallic, material.roughness);
	};

	// Combine with ambient
	vec3 color = getColor() * 0.02;
	color += Lo;

	// Gamma correct
	color = pow(color, vec3(0.4545));

	outColor = vec4(color, 1.0);
}