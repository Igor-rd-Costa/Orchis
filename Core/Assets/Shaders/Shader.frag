#version 460

layout (set = 0, binding = 1) uniform Colors 
{
	vec3 ColorTint;
} colors;

layout (location = 0) out vec4 Color;

layout (location = 0) in vec3 fNormals;
layout (location = 1) in vec2 fTexCoord;

layout (set = 1, binding = 0) uniform sampler2D TexSampler;

void main()
{
	Color = texture(TexSampler, fTexCoord);
}
