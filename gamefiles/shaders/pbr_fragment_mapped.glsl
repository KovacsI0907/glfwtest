#version 330 core
out vec4 FragColor;
in vec2 texCoord;
in vec3 worldPos;
in vec3 worldNormal;
in vec3 worldTangent;
in vec3 posLightSpace;

// material parameters

// mapped material parameters
uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D aoMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

struct PointLight {
    vec3 position;
    vec3 color;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

#include "debug.glsl"
#include "pbr.glsl"
#include "shadow.glsl"

uniform PointLight pointLights[4];
uniform DirectionalLight directionalLight;

uniform vec3 camPosW;

void main()
{	
    // calculate mapped normal
    vec3 cotangent = cross(worldTangent, worldNormal);
    vec3 mapValue = texture(normalMap, texCoord).xyz * 2.0 - 1.0;
    vec3 mappedNormal = worldTangent * mapValue.x + cotangent * mapValue.y + worldNormal * mapValue.z;
    vec3 N = normalize(mappedNormal);
    vec3 V = normalize(camPosW - worldPos);

    // gather material data from textures
    vec3 albedo = texture(albedoMap, texCoord).rgb;
    float metallic = texture(metallicMap, texCoord).r;
    float roughness = texture(roughnessMap, texCoord).r;
    float ao = texture(aoMap, texCoord).r;

    // calculate reflectance at normal incidence; if dielectric, use F0 of 0.04
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

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
    float shadow = shadowCalc(posLightSpace, depthMap, N, directionalLight);
    vec3 L = normalize(-directionalLight.direction);
    vec3 radiance = directionalLight.color;
    Lo += CalcLight(L, radiance, F0, N, V, albedo, metallic, roughness) * (1.0-shadow);
    
    // ambient lighting (to be replaced later with environment lighting)
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);

    if(zero){
        FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    }
}