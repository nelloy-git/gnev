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
    mat4 cam_view_mat = matrix[camera.index[0]];
    mat4 cam_proj_mat = matrix[camera.index[1]];

    vPos = cam_proj_mat * cam_view_mat * vec4(inPos, 1.0);
    vUV = inUV;
    vMaterialId = inIds[0];
    gl_Position = vPos;
}