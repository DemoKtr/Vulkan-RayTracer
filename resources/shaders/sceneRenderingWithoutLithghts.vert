#version 450


layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 tangent;
layout(location = 2) in vec4 vertexTexCoord;


layout(set = 0, binding = 0) uniform UBO {
	mat4 view;
	vec4 camPos;
} cameraData;

struct SBO {
    mat4 model;
	uvec4 textureID; // textureID przechowywane w jednym z elementów
};

layout(std430,set = 0, binding = 1) readonly buffer StorageBuffer {
    SBO sbo[];
} ObjectData;


layout (push_constant) uniform constants {
	mat4 projection;
}ProjectionData;


layout (location = 0) out vec2 TexCoord;
layout (location = 1) flat out uint textureIndex;


void main() {
	gl_Position =  ProjectionData.projection * cameraData.view*ObjectData.sbo[gl_InstanceIndex].model * vertexPosition;
	TexCoord = vertexTexCoord.xy;
	textureIndex = ObjectData.sbo[gl_InstanceIndex].textureID.x;
}