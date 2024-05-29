#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>

#include "Ctx.hpp"
#include "gl/VertexArray.hpp"
#include "gl/container/IBufferContainer.hpp"
#include "gl/data/Index.hpp"
#include "gl/data/Vertex.hpp"
#include "gl/data/VertexAttributeInfo.hpp"
#include "util/Reflection.hpp"

namespace gnev {

class Mesh {
public:
    template <gl::isIndexType T>
    void setIndexes(GLenum draw_type_,
                    const std::shared_ptr<IBufferContainer<T>>& buffer,
                    GLenum index_type_) {
        draw_type = draw_type_;
        index_type = index_type_;
        indexes = buffer;
        vao.setElementBuffer(buffer->getBuffer());
    }

    template <template <typename> typename C, gl::isIndexType T>
        requires(std::is_convertible_v<C<T>*, IBufferContainer<T>*>)
    void setIndexes(GLenum draw_type_, const std::shared_ptr<C<T>>& buffer) {
        static constexpr GLenum type = gl::getIndexTypeEnum<T>();
        setIndexes(draw_type_,
                   std::static_pointer_cast<IBufferContainer<T>>(buffer),
                   type);
    }

    template <refl::Key ATTR, gl::IsVertex T>
    void addAttribute(GLuint shader_loc,
                      const std::shared_ptr<IBufferContainer<T>>& buffer,
                      std::size_t attr_offset,
                      std::size_t attr_stride,
                      const gl::VertexAttributeInfo& attr_info) {
        auto found = std::find(vertex_bindings.begin(), vertex_bindings.end(), buffer);
        GLuint binding = std::distance(vertex_bindings.begin(), found);

        vao.enableAttribute(shader_loc);
        if (found == vertex_bindings.end()) {
            vertex_bindings.push_back(buffer);
            vao.setVertexBuffer(binding, buffer->getBuffer(), 0, attr_stride);
        }
        vao.setAttributeFormat(shader_loc,
                               attr_info.elements,
                               attr_info.type,
                               attr_info.normalized,
                               attr_offset);
        vao.setAttributeBinding(shader_loc, binding);
    }

    template <refl::Key ATTR, template <typename> typename C, gl::IsVertex T>
        requires(std::is_convertible_v<C<T>*, IBufferContainer<T>*>)
    void addAttribute(GLuint shader_loc, const std::shared_ptr<C<T>>& buffer) {
        static constexpr std::size_t OFFSET = T::template getAttrOffset<ATTR>();
        static constexpr std::size_t STRIDE = sizeof(T);
        static constexpr gl::VertexAttributeInfo INFO = T::template getAttrInfo<ATTR>();

        addAttribute<ATTR>(shader_loc,
                           std::static_pointer_cast<IBufferContainer<T>>(buffer),
                           OFFSET,
                           STRIDE,
                           INFO);
    }

    void draw() const {
        GNEV_LOG_L1("Mesh::draw({}, {}, {}, {})", draw_type, indexes->size(), index_type, nullptr);
        vao.bind();
        Ctx::Get().glDrawElements(draw_type, indexes->size(), index_type, nullptr);
    }

private:
    gl::VertexArray vao;
    GLenum draw_type;
    GLenum index_type;
    std::shared_ptr<IBufferContainerBase> indexes;
    std::vector<std::shared_ptr<IBufferContainerBase>> vertex_bindings;
};

}; // namespace gnev