#version 450

layout (location = 0) out vec2 outUV;

struct SBO {
    vec4 PosSize; // Zawiera: x, y (pozycja) oraz z, w (szerokość, wysokość)
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
	outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    

    vec2 screenPos = (ObjectData.sbo[gl_InstanceIndex].PosSize.xy + outUV * ObjectData.sbo[gl_InstanceIndex].PosSize.zw) / ProjectionData.projection * 2.0 - 1.0;
    screenPos.y = -screenPos.y; // Odwrócenie osi Y (OpenGL ma oś Y skierowaną w górę, a ekrany w dół).


    gl_Position = vec4(screenPos, 0.0, 1.0);


    
	
}
