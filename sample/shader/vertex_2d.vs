#version 430 core

in vec3 inPos;
in vec3 inColor;

out vec4 vPos;
out vec3 vColor;

void main(){
    vPos = vec4(inPos, 0);
    vColor = inColor;
}