#version 430 core

struct CameraType {
    vec3 position;
    vec3 direction;
    vec3 top;
    
    // 0 - view_mat
    // 1 - proj_mat
    ivec4 index;
};

layout (std140) uniform Camera {
    CameraType camera;
};

struct MaterialType {
    uint tex_index[5];
    vec4 tex_offset[5];
    vec4 tex_mult[5];
};

layout (std430) buffer Material
{
    MaterialType material[];
};

struct PointLightType {
    vec3 pos;
    vec3 constant_linearic_quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (std430) buffer PointLight
{
    PointLightType point_light[];
};

uniform sampler2DArray inTexAlbedo;
uniform sampler2DArray inTexNormal;
uniform sampler2DArray inTexSpecular;

in vec4 vPos;
in vec2 vUV;
in vec4 vColor;
flat in int vMaterialId;

out vec4 fColor;

vec3 CalcPointLight(PointLightType light, MaterialType material, vec3 fragPos, vec3 viewDir){
    vec3 diffuse = texture(inTexAlbedo, vec3(vUV, material.tex_index[0])).rgb;
    vec3 normal = texture(inTexNormal, vec3(vUV, material.tex_index[1])).rgb;
    vec3 specular = texture(inTexSpecular, vec3(vUV, material.tex_index[2])).rgb;

    normal = normalize(normal * 2.0 - 1.0);

    vec3 lightDir = normalize(light.pos.rgb - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 16;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance    = length(light.pos.rgb - fragPos);
    float attenuation = 1.0 /
        (light.constant_linearic_quadratic[0]
         + light.constant_linearic_quadratic[1] * distance
         + light.constant_linearic_quadratic[2] * (distance * distance));    
    // combine results
    vec3 ambient_light  = light.ambient.rgb * diffuse;
    vec3 diffuse_light  = light.diffuse.rgb * diff * diffuse;
    vec3 specular_light = light.specular.rgb * spec * specular;
    ambient_light  *= attenuation;
    diffuse_light  *= attenuation;
    specular_light *= attenuation;
    return (ambient_light + diffuse_light + specular_light);
} 

void main(){
    MaterialType material = material[vMaterialId];

    fColor = vec4(CalcPointLight(point_light[0], material, vPos.rgb, camera.direction.rgb), 1);
    // fColor = texture(inTexAlbedo, vec3(vUV, material.tex_index[0]));
    // fColor = texture(inTexAlbedo, vec3(vUV, 0));
    // fColor = texture(inTexAlbedo, vec3(vUV, material[vMaterialId].tex_index[0]));
} 