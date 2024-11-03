#version 450

layout (location = 0) in vec2 UV;
layout (location = 1) flat in int textureId;


layout (location = 0) out vec4 outColor;


void main() 
{
	outColor = vec4(1.0f,0.3f,0.95f,1.0f);
}