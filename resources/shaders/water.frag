#version 460

layout (location = 0) in VertexInput {

  vec2 uv;

} vertexInput;

layout(location = 0) out vec4 outFragColor;
 
layout(set=1,binding=0) uniform sampler2DArray textureAtlas;

void main()
{
	if (vertexInput.uv.x > 1.0 || vertexInput.uv.y > 1.0) {
        discard;
    }
	

	vec4 color = vec4(0,50,200,255);
	outFragColor = color;

}