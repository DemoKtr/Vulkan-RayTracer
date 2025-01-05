#version 450

layout (location = 0) out vec2 outUV;
layout (location = 1) flat out uint textureIndex;
layout (location = 2) out vec2 outLocalUV;

struct SBO {
    vec4 PosSize; // Zawiera: x, y (pozycja) oraz z, w (szerokość, wysokość)
	vec4 UVBounds; // Minimalne i maksymalne UV w atlasie tekstur (minU, minV, maxU, maxV)
	uvec4 textureID; // textureID przechowywane w jednym z elementów
};

layout(std430, set = 0, binding = 0) readonly buffer StorageBuffer {
    SBO sbo[];
} ObjectData;

layout (push_constant) uniform constants {
	vec2 projection;
}ProjectionData;

void main() 
{

    int vertexID = int(gl_VertexIndex);
	vec2 localUV = vec2((vertexID << 1) & 2, vertexID & 2); // (0,0), (1,0), (0,1), (1,1)
	outLocalUV = localUV;
	 localUV.y = 1.0 - localUV.y;
    outUV = mix(ObjectData.sbo[gl_InstanceIndex].UVBounds.xy, ObjectData.sbo[gl_InstanceIndex].UVBounds.zw, localUV);

   

    vec2 screenPos = (ObjectData.sbo[gl_InstanceIndex].PosSize.xy + localUV * ObjectData.sbo[gl_InstanceIndex].PosSize.zw) / ProjectionData.projection * 2.0 - 1.0;
    //screenPos.y = -screenPos.y; // Odwrócenie osi Y (OpenGL ma oś Y skierowaną w górę, a ekrany w dół).
	textureIndex = ObjectData.sbo[gl_InstanceIndex].textureID.x;
	
    gl_Position = vec4(screenPos, 0.0, 1.0);	
}
