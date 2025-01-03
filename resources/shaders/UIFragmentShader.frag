#version 450

layout (location = 0) in vec2 outUV;


layout (location = 0) out vec4 outColor;

void main() {
// Jeśli fragment wychodzi poza obszar prostokąta, odrzuć go.
    if (outUV.x > 1.0 || outUV.y > 1.0) {
        discard;
    }
	outColor = vec4(0,0,0,1.0f);
}