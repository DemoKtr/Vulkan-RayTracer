#version 450
layout (location = 0) in vec3 aPos;

layout(set = 0, binding = 0) uniform UBO {
	mat4 view;
	vec4 camPos;
} cameraData;


layout (push_constant) uniform constants {
	mat4 projection;
}ProjectionData;

layout(location = 0) out vec3 TexCoords;

void main() {
	TexCoords = aPos;
	vec4 pos = ProjectionData.projection * cameraData.view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}