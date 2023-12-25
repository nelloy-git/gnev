#version 430 core

struct CameraType
{
    vec3 position;
    vec3 direction;
    vec3 top;
    
    uint view_mat_index;
    uint proj_mat_index;
};

layout (std430) buffer Camera {
    CameraType camera;
};

layout (std430) buffer Mat {
    mat4 matrix[];
};

in vec3 inPos;
in vec2 inUV;
in ivec4 inIds;

out vec4 vPos;
out vec2 vUV;
flat out int vMaterialId;

void main(){
    vPos = matrix[camera.proj_mat_index] * matrix[camera.view_mat_index] * vec4(inPos, 1.0);
    // vPos = vec4(inPos, 1.0);
    vUV = inUV;
    vMaterialId = inIds[0];
    gl_Position = vPos;
}