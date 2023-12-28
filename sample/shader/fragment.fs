#version 430 core

struct MaterialType {
    uint tex_index[5];
    vec4 tex_offset[5];
    vec4 tex_mult[5];
};

layout (std430) buffer Material
{
    MaterialType material[];
};

uniform sampler2DArray albedo;

in vec4 vPos;
in vec2 vUV;
in vec4 vColor;
flat in int vMaterialId;

out vec4 fColor;

// vec3 CalcPointLight(PointLight light, Material material, vec3 fragPos, vec3 viewDir)
// {
//     vec3 diffuse = texture(diffuse_0, vec3(vUV, material.texture.diffuse_element_index)).rgb;
//     vec3 normal = texture(normal_0, vec3(vUV, material.texture.normal_element_index)).rgb;
//     vec3 specular = texture(specular_0, vec3(vUV, material.texture.specular_element_index)).rgb;

//     normal = normalize(normal * 2.0 - 1.0);

//     vec3 lightDir = normalize(light.position.rgb - fragPos);
//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float shininess = 16;
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
//     // attenuation
//     float distance    = length(light.position.rgb - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + 
//   			     light.quadratic * (distance * distance));    
//     // combine results
//     vec3 ambient_light  = light.ambient.rgb * diffuse;
//     vec3 diffuse_light  = light.diffuse.rgb * diff * diffuse;
//     vec3 specular_light = light.specular.rgb * spec * specular;
//     ambient_light  *= attenuation;
//     diffuse_light  *= attenuation;
//     specular_light *= attenuation;
//     return (ambient_light + diffuse_light + specular_light);
// } 

void main(){
    // Material material = materials[vMaterialId];
    // fColor = vec4(CalcPointLight(lights[0], material, vPos.rgb, camera.direction.rgb), 1);
    fColor = vColor;
} 