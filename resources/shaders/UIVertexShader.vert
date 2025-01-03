#version 450

layout (location = 0) out vec2 outUV;

struct SBO {
    vec4 PosSize; // Zawiera: x, y (pozycja) oraz z, w (szerokość, wysokość)
};

layout(std430, set = 0, binding = 0) readonly buffer StorageBuffer {
    SBO sbo[];
} ObjectData;

void main() 
{
     // Indeks wierzchołka (0, 1, 2).
    int vertexID = int(gl_VertexIndex);
	outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
    // Wierzchołki trójkąta w lokalnych współrzędnych prostokąta (0..1).
    vec2 vertices[3] = vec2[3](
        vec2(0.0, 0.0), // Lewy górny róg.
        vec2(1.0, 0.0), // Prawy górny róg.
        vec2(0.0, 1.0)  // Lewy dolny róg.
    );

    // Przeliczenie pozycji w pikselach na współrzędne [-1, 1].
    vec2 screenPos = (ObjectData.sbo[gl_InstanceIndex].PosSize.xy + outUV * ObjectData.sbo[gl_InstanceIndex].PosSize.zw) / vec2(1920.0f,1080.f) * 2.0 - 1.0;
    screenPos.y = -screenPos.y; // Odwrócenie osi Y (OpenGL ma oś Y skierowaną w górę, a ekrany w dół).

    // Ustawienie pozycji w przestrzeni klipu.
    gl_Position = vec4(screenPos, 0.0, 1.0);

    // Przekazanie współrzędnych UV.
    
	
}
