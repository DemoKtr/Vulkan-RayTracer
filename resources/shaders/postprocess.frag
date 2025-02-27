/*
#version 450 core

// Wejście/wyjście
layout(location = 0) in vec2 outUV;
layout(location = 0) out vec4 FragColor;

// Tekstury z poprzednich etapów renderowania
layout(set = 0, binding = 0) uniform sampler2D uSceneColor;  // Kolor sceny
layout(set = 0, binding = 1) uniform sampler2D uSceneDepth;  // Bufor głębokości

// Uniformy kamery, macierzy oraz światła
layout(set = 0, binding = 2) uniform UBO {
    vec3 uCameraPos;
    mat4 uInvProjection;
    mat4 uInvView;
    vec3 uLightPos;
    vec3 uLightColor;
    float uNear; // Near plane (kamery)
    float uFar;  // Far plane (kamery)
} UBOData;

// Parametry mgły (na stałe w shaderze; w praktyce możesz je ustawiać z aplikacji)
float uFogDensity  = 0.02;             // Gęstość mgły (zmniejszona, by uniknąć przepaleń)
float uScattering  = 1.0;              // Współczynnik rozpraszania
float uAnisotropy  = 0.8;              // Henyey-Greenstein, -1..1; unikaj ±1
vec3  uFogColor    = vec3(0.7, 0.7, 0.8); // Kolor mgły

// Parametry raymarchingu
const float MAX_DISTANCE = 100.0;
const float STEP_SIZE    = 0.1;
const int   MAX_STEPS    = int(MAX_DISTANCE / STEP_SIZE);

// Funkcja do określenia bazy gęstości mgły w zależności od pozycji
float GetFogDensity(vec3 worldPos)
{
    // Przykładowo mgła zależna od wysokości (opcjonalnie):
    // return exp(-0.05 * worldPos.y);
    return 1.0; // Skalowane przez uFogDensity dalej
}

void main()
{
    // 1. Rekonstrukcja kierunku promienia (ray direction)
    vec2 ndc     = outUV; //* 2.0 - 1.0;             // Przestrzeń NDC: [-1..1]
    vec4 clipPos = vec4(ndc, 1.0, 1.0);           // Używamy z = 1.0 (far plane)
    vec4 viewPos = UBOData.uInvProjection * clipPos;
    viewPos /= viewPos.w;
    vec4 worldPos4 = UBOData.uInvView * viewPos;
    vec3 rayDir    = normalize(worldPos4.xyz - UBOData.uCameraPos);

    // 2. Konwersja głębokości nieliniowej na liniową (jeśli depth jest w [0..1])
    float sceneDepth = texture(uSceneDepth, outUV).r;
    float ndDepth    = sceneDepth * 2.0 - 1.0; // z NDC
    float linearDepth = (2.0 * UBOData.uNear * UBOData.uFar) /
                        (UBOData.uFar + UBOData.uNear - ndDepth * (UBOData.uFar - UBOData.uNear));

    // Maksymalny dystans, do którego liczymy mgłę
    float maxMarchDistance = min(MAX_DISTANCE, linearDepth);

    // 3. Raymarching przez objętość mgły
    vec3  fogAccum       = vec3(0.0);  // Zsumowane światło rozproszone
    float transmittance  = 1.0;        // Ile światła (sceny) jeszcze „dociera” do oka
    float t              = 0.0;

    // Zabezpieczenie przed g=±1
    float g = clamp(uAnisotropy, -0.9999, 0.9999);

    for (int i = 0; i < MAX_STEPS && t < maxMarchDistance; i++)
    {
        vec3 samplePos = UBOData.uCameraPos + rayDir * t;

        // Lokalne zagęszczenie mgły (np. zależne od wysokości)
        float density = GetFogDensity(samplePos) * uFogDensity;

        // Beer-Lambert na małym odcinku STEP_SIZE
        float delta             = density * STEP_SIZE;
        float stepTransmittance = exp(-delta);

        // Henyey-Greenstein: obliczenie fazy rozpraszania
        vec3 lightDir = normalize(UBOData.uLightPos - samplePos);
        float mu      = dot(lightDir, rayDir); // kąt pomiędzy światłem a kierunkiem patrzenia
        mu            = clamp(mu, -1.0, 1.0);  // zabezpieczenie numeryczne

        float denom = 1.0 + g*g - 2.0*g*mu;
        denom       = max(denom, 1e-5);        // unikamy dzielenia przez zero
        float phase = (1.0 - g*g) / pow(denom, 1.5);

        // Proste osłabienie światła w funkcji odległości
        float lightDistance = length(UBOData.uLightPos - samplePos);
        float lightAtten    = 1.0 / (1.0 + 0.1 * lightDistance * lightDistance);

        // Akumulacja światła rozproszonego (in-scattering)
        fogAccum += transmittance
                    * uScattering
                    * density
                    * phase
                    * uFogColor
                    * STEP_SIZE
                    * lightAtten;

        // Aktualizacja transmisji (ile „zostaje” z koloru sceny)
        transmittance *= stepTransmittance;

        // Przesuwamy się dalej wzdłuż promienia
        t += STEP_SIZE;

        // Jeśli transmisja jest znikoma, można przerwać iterację
        if (transmittance < 0.01)
            break;
    }

    // 4. Pobieramy kolor sceny i mieszamy go z mgłą
    vec3 sceneColor = texture(uSceneColor, outUV).rgb;

    // Fizycznie bliższa poprawności formuła:
    // finalColor = (kolorSceny * transmisja) + (światło rozproszone)
    vec3 finalColor = sceneColor * transmittance + fogAccum;

    // W razie potrzeby: tone mapping / korekcja gamma

    FragColor = vec4(finalColor, 1.0);
}

*/

#version 450 core

// Wejście/wyjście
layout(location = 0) in vec2 outUV;
layout(location = 0) out vec4 FragColor;



void main()
{
   

    FragColor = vec4(0.0,0.0,1.0, 1.0);
}

