#pragma once

#include <array>

#include "Ctx.hpp"
#include "gl/VertexArray.hpp"
#include "gl/container/BufferPool.hpp"
#include "mesh/SubMesh.hpp"

namespace gnev {

template <typename T>
constexpr bool IsValidIndexType =
    std::is_same_v<T, GLubyte> || std::is_same_v<T, GLushort> ||
    std::is_same_v<T, GLuint>;

template <typename T>
concept IsIndex = IsValidIndexType<T>;

template <IsIndex Index_, IsTriviallyCopyable Vertex_>
class EXPORT Mesh {
    template <typename T>
    static consteval GLenum GetIndexEnum() {
        if constexpr (std::is_same_v<T, GLubyte>) {
            return GL_UNSIGNED_BYTE;
        } else if constexpr (std::is_same_v<T, GLushort>) {
            return GL_UNSIGNED_SHORT;
        } else if constexpr (std::is_same_v<T, GLuint>) {
            return GL_UNSIGNED_INT;
        } else {
            []<bool B = true>() { static_assert(B, ""); }();
        }
    }

public:
    static constexpr unsigned MAX_ATTRIBUTES = 16;

    using Index = Index_;
    using Vertex = Vertex_;
    static constexpr GLenum IndexGL = GetIndexEnum<Index_>();


    static constexpr GLuint BUFFER_BINDING = 0;

    Mesh(std::unique_ptr<gl::BufferPool<Index>>&& index_pool,
         std::unique_ptr<gl::BufferPool<Vertex>>&& vertex_pool)
        : vao{std::make_unique<gl::VertexArray>()}
        , index_pool{std::move(index_pool)}
        , vertex_pool{std::move(vertex_pool)} {

        vao->setElementBuffer(index_pool->getBuffer());
        vao->setVertexBuffer(BUFFER_BINDING, vertex_pool.getBuffer(), 0, sizeof(Vertex));
    }
    virtual ~Mesh() = default;

    void setAttribFormat(unsigned mesh_attrib_index,
                         GLuint elements,
                         GLenum type,
                         GLboolean normalized,
                         GLuint relative_offset) {
        attrib_format[mesh_attrib_index] = {.applied = false,
                                            .elements = elements,
                                            .type = type,
                                            .normalized = normalized,
                                            .relative_offset = relative_offset};
    }

    void cleatAttribFormat(unsigned mesh_attrib_index) {
        attrib_format[mesh_attrib_index].reset();
    }

    void setAttribLoc(unsigned mesh_attrib_index, GLuint shader_attrib_loc) {
        attrib_loc[mesh_attrib_index] = shader_attrib_loc;
        auto& fmt_opt = attrib_format[mesh_attrib_index];
        if (fmt_opt.has_value()) {
            fmt_opt->applied = false;
        }
    }

    void clearAttribLoc(unsigned mesh_attrib_index) {
        attrib_loc[mesh_attrib_index].reset();
    }

    void draw() const {
        for (unsigned i = 0; i < attrib_loc.size(); ++i) {
            auto loc_opt = attrib_loc[i];
            if (not loc_opt.has_value()) {
                continue;
            }

            auto fmt_opt = attrib_format[i];
            if (not fmt_opt.has_value()) {
                continue;
            }

            if (fmt_opt->applied) {
                continue;
            }

            vao->setAttributeBinding(*loc_opt, BUFFER_BINDING);
            vao->setAttributeFormat(*loc_opt,
                                    fmt_opt->elements,
                                    fmt_opt->type,
                                    fmt_opt->normalized,
                                    fmt_opt->relative_offset);
            vao->enableAttribute(*loc_opt);
            fmt_opt->applied = true;
        }
        vao->bind();
        Ctx::Get().glDrawElements(GL_TRIANGLES, index_pool->size(), IndexGL, 0);
    }

private:
    struct AttribFormat {
        bool applied{false};
        GLuint elements;
        GLenum type;
        GLboolean normalized;
        GLuint relative_offset;
    };

    std::unique_ptr<gl::VertexArray> vao;
    std::unique_ptr<gl::BufferPool<Index>> index_pool;
    std::unique_ptr<gl::BufferPool<Vertex>> vertex_pool;

    std::array<std::optional<AttribFormat>, MAX_ATTRIBUTES> attrib_format;
    std::array<std::optional<GLuint>, MAX_ATTRIBUTES> attrib_loc;
};
} // namespace gnev