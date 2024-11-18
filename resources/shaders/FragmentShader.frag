#version 450

layout (location = 0) in vec2 UV;
layout (location = 1) flat in int textureId;

layout(set=1,binding=0) uniform sampler2DArray textureAtlas;

layout (location = 0) out vec4 outColor;


void main() 
{
	vec3 color = texture(textureAtlas, vec3(UV.st, textureId)).rgb;
	outColor = vec4(color,1.0f);
}