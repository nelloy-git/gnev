#version 430 core

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

in vec3 inPos;
in vec2 inUV;
in int inMaterialId;

out vec4 vPos;
out vec2 vUV;
flat out int vMaterialId;

void main(){
    vPos = camera.result_mat * vec4(inPos, 1.0);
    vUV = inUV;
    vMaterialId = inMaterialId;
    gl_Position = vPos;
}