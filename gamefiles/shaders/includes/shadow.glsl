float shadowCalc(vec3 posLightSpace, sampler2D depthMap, vec3 N, DirectionalLight directionalLight) {
    vec3 projCoords = posLightSpace * 0.5 + 0.5;
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.005 * (1.0 - dot(N, directionalLight.direction)), 0.0005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    return shadow;
}