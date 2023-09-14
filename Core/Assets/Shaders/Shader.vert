#version 460

layout (set = 0, binding = 0) uniform Transforms
{
	mat4 ViewProj;
} transforms;

layout (push_constant) uniform PushConstants
{
	layout (offset = 0) mat4 ModelMatrix;
};

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTexCoord;

layout (location = 0) out vec3 fPosition;
layout (location = 1) out vec3 fNormals;
layout (location = 2) out vec2 fTexCoord;


void main()
{
	gl_Position = transforms.ViewProj * ModelMatrix * vec4(vPos, 1.0);
	fPosition = vec3(ModelMatrix * vec4(vPos, 1.0));
	fNormals = mat3(transpose(inverse(ModelMatrix))) * vNormals;
	fTexCoord = vTexCoord;
}
