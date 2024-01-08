#pragma once

#include "gl/Buffer.hpp"
#include "gl/Handler.hpp"
#include "gl/Shader.hpp"
#include "gl/Texture.hpp"
#include "util/IndexStorage.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

class EXPORT Program : public Handler {
public:
    Program();
    Program(const Program& other) = delete;
    Program(Program&& other) = default;
    virtual ~Program();

    void attach(const Shader& shader);
    void validate();
    bool isValidateSucceed() const;
    void link();
    bool isLinkSucceed() const;
    void use() const;

    GLsizei getInfoLogLength() const;
    std::string getInfoLog() const;

    GLint getAttributeLoc(const GLchar* name) const;
    GLint getUniformLoc(const GLchar* name) const;
    GLint getResourceIndex(GLenum interface, const GLchar* name) const;

    void bindShaderStorageBlockBuffer(const GLchar* storage_block_name,
                                      const Ref<Buffer>& buffer);
    void bindShaderStorageBlockBuffer(GLuint storage_block_index,
                                      const Ref<Buffer>& buffer);

    void bindShaderUniformBlockBuffer(const GLchar* uniform_block_name,
                                      const Ref<Buffer>& buffer);
    void bindShaderUniformBlockBuffer(GLuint uniform_block_index,
                                      const Ref<Buffer>& buffer);

    void bindShaderTextureSampler(const GLchar* texture_sampler_name,
                                  const Ref<Texture>& texture);
    void bindShaderTextureSampler(GLuint texture_block_index,
                                  const Ref<Texture>& texture);

private:
    template <typename T>
    struct Bindings {
        Bindings(GLuint capacity);
        IndexStorage binds;
        std::unordered_map<GLuint, std::pair<GLuint, Ptr<T>>> map;
    };

    std::unique_ptr<Bindings<Buffer>> shader_storage_blocks;
    std::unique_ptr<Bindings<Buffer>> shader_uniform_blocks;
    std::unique_ptr<Bindings<Texture>> shader_texture_samplers;

    static GLuint getMaxShaderStorageBufferBindings();
    static GLuint getMaxUniformBufferBindings();
    static GLuint getMaxTextureImageUnits();

    static GLuint* createHandle();
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl