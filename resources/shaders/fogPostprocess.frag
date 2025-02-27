#version 450 core

layout (location = 0) in vec2 outUV;
layout (location = 0) out vec4 FragColor;

// Tekstury z poprzednich etapów renderowania
layout (set = 0, binding = 0) uniform sampler2D uSceneColor;    // Renderowana scena (color buffer)
layout (set = 1, binding = 0) uniform sampler3D uVolume;          // Bufor mgły/objętości

// Uniformy kamery oraz macierze potrzebne do rekonstrukcji pozycji w świecie
layout( set = 0, binding = 1) uniform UBO {
    mat4 uInvViewProj;  // 64 bajty
    vec4 uCamPos;        // +16 bajtów
    vec4 uBoxMin;        // +16 bajtów
    vec4 uBoxMax;        // +16 bajtów
    float uTime;         // +4 bajty
	float _padding[3];
    // w std140 może się jeszcze pojawić padding do 16 bajtów
} UBOData;

const int MAX_STEPS = 128;
const float STEP_SIZE = 0.02;
const vec3 FOG_COLOR = vec3(0.8, 0.8, 0.8);

struct Ray {
    vec3 origin;
    vec3 dir;
};

bool intersectBox(vec3 minB, vec3 maxB, Ray ray, out float tmin, out float tmax) {
    vec3 invDir = 1.0 / ray.dir;
    vec3 t0s = (minB - ray.origin) * invDir;
    vec3 t1s = (maxB - ray.origin) * invDir;
    vec3 tsmaller = min(t0s, t1s);
    vec3 tbigger  = max(t0s, t1s);
    tmin = max(tsmaller.x, max(tsmaller.y, tsmaller.z));
    tmax = min(tbigger.x, min(tbigger.y, tbigger.z));
    return tmax > tmin;
}

void main() {
     // Obliczamy pozycje na near i far plane w NDC [-1,1]
    vec2 ndc = outUV * 2 -1;
	vec4 ndcNear = vec4(ndc, 0, 1.0); 
	vec4 ndcFar  = vec4(ndc, 1.0, 1.0);  

    // Transformacja do przestrzeni świata
    vec4 worldNear = UBOData.uInvViewProj * ndcNear;
    worldNear /= worldNear.w;
    vec4 worldFar = UBOData.uInvViewProj * ndcFar;
    worldFar /= worldFar.w;

    // Budujemy promień
    Ray ray;
    ray.origin = UBOData.uCamPos.xyz;
    ray.dir = normalize(worldFar.xyz - worldNear.xyz);

    float tmin, tmax;
    if(!intersectBox(UBOData.uBoxMin.xyz, UBOData.uBoxMax.xyz, ray, tmin, tmax)){
        
		vec4 sceneColor = texture(uSceneColor, outUV);
		FragColor = sceneColor ;
        return;
    }

    // Jeśli kamera jest wewnątrz mgły, oblicz tmin jako odległość do najbliższej ściany.
    bool inside = all(greaterThanEqual(UBOData.uCamPos.xyz, UBOData.uBoxMin.xyz)) && all(lessThanEqual(UBOData.uCamPos.xyz, UBOData.uBoxMax.xyz));
     if (inside) {
         vec3 camToMin = UBOData.uCamPos.xyz - UBOData.uBoxMin.xyz;
		vec3 camToMax = UBOData.uBoxMax.xyz - UBOData.uCamPos.xyz;
		vec3 distances = min(camToMin, camToMax);
		tmin = min(distances.x, min(distances.y, distances.z));
		tmin = max(tmin, 0.0); // Rozpocznij od najbliższej ściany
    } else {
        if(tmin < 0.0) tmin = 0.0;
    }

    vec4 color = vec4(0.0);
    float t = tmin;
    for (int i = 0; i < MAX_STEPS; i++){
        if(t > tmax) break;

        vec3 pos = ray.origin + ray.dir * t;
        // Obliczamy uvw do próbkowania tekstury
        vec3 uvw = (pos - UBOData.uBoxMin.xyz) / (UBOData.uBoxMax.xyz - UBOData.uBoxMin.xyz);

        // Dodajemy per-woxelowe, losowe przesunięcie bazujące na pozycji i czasie.
        vec3 randomOffset = vec3(
            sin(uvw.x * 10.0 + UBOData.uTime),
            cos(uvw.y * 10.0 + UBOData.uTime + 2.0),
            -sin(uvw.z * 10.0 + UBOData.uTime + 4.0)
        ) * 0.05;
        uvw += randomOffset;
		

        // Opcjonalny falloff: mgła jest wyraźniejsza w centrum
        float falloff = 1.0 - smoothstep(0.4, 0.6, length(uvw - vec3(0.5)));
        float density = texture(uVolume, uvw).r * falloff;
        float alpha = density * 0.03;
        float oneMinusA = 1.0 - color.a;
        color.rgb += FOG_COLOR * alpha * oneMinusA;
        color.a   += alpha * oneMinusA;

        if(color.a > 0.95){
            color.a = 1.0;
            break;
        }
        t += STEP_SIZE;
    }
    vec4 sceneColor = texture(uSceneColor, outUV);
	FragColor = mix(sceneColor, vec4(FOG_COLOR, color.a), color.a);
}
