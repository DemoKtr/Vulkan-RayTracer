#version 450

layout (location = 0) in vec2 UV;


layout(set=0,binding=0) uniform sampler2D text;

layout (location = 0) out vec4 outColor;


void main() 
{
	vec3 color = texture(text, UV.st).rgb;
	outColor = vec4(color,1.0f);
}