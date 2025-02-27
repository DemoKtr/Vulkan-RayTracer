#version 450 core

layout(location = 0) out vec2 outUV;

vec2 positions[6] = vec2[](
    vec2(-1.0, -1.0), // pierwszy trójkąt: lewy dolny
    vec2( 1.0, -1.0), // pierwszy trójkąt: prawy dolny
    vec2(-1.0,  1.0), // pierwszy trójkąt: lewy górny

    vec2(-1.0,  1.0), // drugi trójkąt: lewy górny
    vec2( 1.0, -1.0), // drugi trójkąt: prawy dolny
    vec2( 1.0,  1.0)  // drugi trójkąt: prawy górny
);

vec2 uvs[6] = vec2[](
    vec2(0.0, 0.0), // odpowiada lewemu dolnemu pikselowi
    vec2(1.0, 0.0), // odpowiada prawemu dolnemu pikselowi
    vec2(0.0, 1.0), // odpowiada lewemu górnemu pikselowi

    vec2(0.0, 1.0), // odpowiada lewemu górnemu pikselowi
    vec2(1.0, 0.0), // odpowiada prawemu dolnemu pikselowi
    vec2(1.0, 1.0)  // odpowiada prawemu górnemu pikselowi
);

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    outUV = uvs[gl_VertexIndex];
}
