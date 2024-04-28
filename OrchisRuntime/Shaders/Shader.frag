#version 460

layout (set = 0, binding = 1) uniform Material
{
	float Ambient;
	float Diffuse;
	float Specular;
	float Shininess;
} material;

layout (set = 0, binding = 2) uniform Light
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
} light;

layout (push_constant) uniform Model
{
	layout (offset = 64) vec3 CameraPosition;
	layout (offset = 80) uint TextureIndex;
};

layout (location = 0) out vec4 Color;

layout (location = 0) in vec3 fPosition;
layout (location = 1) in vec3 fNormals;
layout (location = 2) in vec2 fTexCoord;

layout (set = 1, binding = 0) uniform sampler2D TexSampler[1];

void main()
{
	vec3 normal = normalize(fNormals);
	vec3 lightDir = normalize(light.Position - fPosition);
	vec3 viewDir = normalize(CameraPosition - fPosition);
	vec3 reflectDir = reflect(-lightDir, normal);

	vec3 Ambient = material.Ambient * light.Ambient;
	vec3 Diffuse = material.Diffuse * max(dot(lightDir, normal), 0.0) * light.Diffuse;
	vec3 Specular = material.Specular * pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess) * light.Specular;

	//Color = texture(TexSampler[TextureIndex], fTexCoord) * vec4(Ambient + Diffuse + Specular, 1.0);
	Color = vec4(1.0, 0.0, 0.0, 1.0) * vec4(Ambient + Diffuse + Specular, 1.0);
}
