#version 450


layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexTexCoord;



layout (push_constant) uniform constants {
	mat4 projection;
	mat4 view;
}ProjectionData;


layout(location = 0) out vec2 TexCoord;


void main() {
	gl_Position = ProjectionData.projection * ProjectionData.view * mat4(1.0f) * vertexPosition;
	TexCoord = vertexTexCoord.xy;

}