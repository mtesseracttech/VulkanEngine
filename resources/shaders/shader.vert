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

//Data out:
layout(location = 0) out vec3 outFragColor;
layout(location = 1) out vec2 outFragTexCoord;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    vec3 modelPos = vec3(ubo.model[3]);
    vec3 stretchDir = inPosition - modelPos;

    vec3 offsetScale = vec3(
        sin(M_PI*2 / 3 * 1 + (ubo.time * 10) + inPosition.x),
        sin(M_PI*2 / 3 * 2 + (ubo.time * 10) + inPosition.y),
        sin(M_PI*2 / 3 * 3 + (ubo.time * 10) + inPosition.z)
    );

    vec3 finalPos = inPosition + (stretchDir * (0.1 *sin(ubo.time*10))) * offsetScale;

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(finalPos, 1.0);
    outFragColor = inColor;
    outFragTexCoord = inTexCoord;
}
