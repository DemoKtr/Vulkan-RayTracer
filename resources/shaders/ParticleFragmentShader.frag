#version 460

layout (location = 0) in VertexInput {
 // vec4 color;
  vec2 uv;
  flat uint textureId;
} vertexInput;

layout(location = 0) out vec4 outFragColor;
 
layout(set=1,binding=0) uniform sampler2DArray textureAtlas;

void main()
{
	if (vertexInput.uv.x > 1.0 || vertexInput.uv.y > 1.0) {
        discard;
    }
	

	vec4 color = texture(textureAtlas, vec3(vertexInput.uv.st, vertexInput.textureId));
	outFragColor = color;

}