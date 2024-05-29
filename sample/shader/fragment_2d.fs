#version 430 core

in vec4 vPos;
in vec3 vColor;

out vec4 fColor;

void main(){
    fColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // fColor = texture(inTexAlbedo, vec3(vUV, material.tex_index[0]));
    // fColor = texture(inTexAlbedo, vec3(vUV, 0));
    // fColor = texture(inTexAlbedo, vec3(vUV, material[vMaterialId].tex_index[0]));
} 