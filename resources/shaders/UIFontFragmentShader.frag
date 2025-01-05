#version 450

layout(set=1,binding=0) uniform sampler2DArray sfdAtlas;

layout (location = 0) in vec2 outUV;
layout (location = 1) flat in uint textureId;
layout (location = 2) in vec2 outLocalUV;
layout (location = 0) out vec4 outColor;

void main() {
// Jeśli fragment wychodzi poza obszar prostokąta, odrzuć go.
	if (outLocalUV.x > 1.0 || outLocalUV.y > 1.0 || outLocalUV.x < 0.0 || outLocalUV.y < 0.0) {
        discard;
    }
	float sdfValue = texture(sfdAtlas, vec3(outUV.st, 0)).r;

    float alpha = smoothstep(0.4, 0.5, sdfValue);

    // Kolor tekstu (np. biały z przezroczystością)
    outColor = vec4(0.0f, 0.0f, 1.0f, alpha);

}