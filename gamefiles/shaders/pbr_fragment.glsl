#version 330 core
out vec4 FragColor;
in vec2 texCoord;
in vec3 worldPos;
in vec3 worldNormal;

// material parameters
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// lights
struct PointLight {
    vec3 position;
    vec3 color;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

uniform PointLight pointLights[4];
uniform DirectionalLight directionalLight;

uniform vec3 camPosW;

#include "debug.glsl"
#include "pbr.glsl"

// New function to calculate per-light contribution

void main()
{		
    vec3 N = normalize(worldNormal);
    vec3 V = normalize(camPosW - worldPos);

    // Calculate F0 based on metalness.
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    // point lights
    for(int i = 0; i < 4; ++i) 
    {
        vec3 L = normalize(pointLights[i].position - worldPos);
        float distance = length(pointLights[i].position - worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLights[i].color * attenuation;
        Lo += CalcLight(L, radiance, F0, N, V, albedo, metallic, roughness);
    }   

    // directional light
    vec3 L = normalize(-directionalLight.direction);
    vec3 radiance = directionalLight.color;
    Lo += CalcLight(L, radiance, F0, N, V, albedo, metallic, roughness);
    
    // ambient lighting (to be replaced later with IBL)
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // Gamma correction
    color = pow(color, vec3(1.0 / 2.2)); 

    FragColor = vec4(color, 1.0);

    if(zero){
        FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    }
}