// #pragma once

// #include "gl/buffer/HeapBuffer.hpp"
// #include "material/pbr/Material_PBR.hpp"

// namespace gnev {

// class EXPORT MaterialStorage_PBR {
// public:
//     using DataGL = MaterialDataGL_PBR;

//     using TexView = Material_PBR::TexView;
//     using TexElem = Material_PBR::TexElem;

//     static Ref<MaterialStorage_PBR>
//     MakeDynamic(GLuint img_levels, GLuint img_width, GLuint img_height, GLuint capacity);

//     MaterialStorage_PBR(std::unique_ptr<gl::HeapBuffer<DataGL>>&& material_storage,
//                         std::unique_ptr<TexView>&& albedo_storage,
//                         std::unique_ptr<TexView>&& normal_storage,
//                         std::unique_ptr<TexView>&& metallic_storage,
//                         std::unique_ptr<TexView>&& roughness_storage,
//                         std::unique_ptr<TexView>&& ambient_occlusion_storage);
//     virtual ~MaterialStorage_PBR() = default;

//     gl::HeapBuffer<DataGL>& getDataView() const;
//     Ref<TexView> getTexView(MaterialTexType_PBR type) const;
//     Ref<gl::Texture> getTex(MaterialTexType_PBR type) const;

// private:
//     std::unique_ptr<gl::HeapBuffer<DataGL>> data_view;
//     std::array<Ref<TexView>, MaterialDataGL_PBR::TexSize> tex_view;
// };
// } // namespace gnev