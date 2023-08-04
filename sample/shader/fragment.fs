#version 430 core

struct MaterialTexture {
    int diffuse_array_index;
    int diffuse_element_index;
    int normal_array_index;
    int normal_element_index;
    int specular_array_index;
    int specular_element_index;
};

struct Material
{
    vec4 color;
    vec4 normal;
    vec4 specular;
    MaterialTexture texture;
};

layout (std430) buffer MaterialBuffer
{
    Material materials[];
};

struct Camera
{
    mat4 view_mat;
    mat4 projection_mat;
    mat4 result_mat;
    vec4 position;
    vec4 direction;
    vec4 top;
    float fov;
    float width;
    float height;
    float near_z;
    float far_z;
    float reserved_alighment[3];
};

layout (std430) buffer CameraBuffer {
    Camera camera;
};

struct PointLight { 
    vec4 position;
    
    float constant;
    float linear;
    float quadratic;
    float reserve;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

layout (std430) buffer PointLightBuffer
{
    PointLight lights[];
};

uniform sampler2DArray diffuse_0;
// uniform sampler2DArray diffuse_1;
uniform sampler2DArray normal_0;
// uniform sampler2DArray normal_1;
uniform sampler2DArray specular_0;
// uniform sampler2DArray specular_1;

in vec4 vPos;
in vec2 vUV;
flat in int vMaterialId;

out vec4 fColor;

vec3 CalcPointLight(PointLight light, Material material, vec3 fragPos, vec3 viewDir)
{
    vec3 diffuse = texture(diffuse_0, vec3(vUV, material.texture.diffuse_element_index)).rgb;
    vec3 normal = texture(normal_0, vec3(vUV, material.texture.normal_element_index)).rgb;
    vec3 specular = texture(specular_0, vec3(vUV, material.texture.specular_element_index)).rgb;

    normal = normalize(normal * 2.0 - 1.0);

    vec3 lightDir = normalize(light.position.rgb - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 16;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance    = length(light.position.rgb - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
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
    Material material = materials[vMaterialId];
    fColor = vec4(CalcPointLight(lights[0], material, vPos.rgb, camera.direction.rgb), 1);
} 