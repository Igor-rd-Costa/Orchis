#version 460

layout (set = 0, binding = 0) uniform Transforms
{
	mat4 ViewProj;
	mat4 Model;
} transforms;


layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTexCoord;


layout (location = 0) out vec3 fNormals;
layout (location = 1) out vec2 fTexCoord;


void main()
{
	gl_Position = transforms.ViewProj * transforms.Model * vec4(vPos, 1.0);
	fTexCoord = vTexCoord;
	fNormals = vNormals;
}
