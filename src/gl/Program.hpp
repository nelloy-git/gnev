#pragma once

#include "gl/Buffer.hpp"
#include "gl/Handler.hpp"
#include "gl/Shader.hpp"
#include "gl/Texture.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

template <typename T>
class ProgramBinding;

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
    std::unique_ptr<ProgramBinding<Buffer>> shader_storage_blocks;
    std::unique_ptr<ProgramBinding<Buffer>> shader_uniform_blocks;
    std::unique_ptr<ProgramBinding<Texture>> shader_texture_samplers;

    GLuint getMaxShaderStorageBufferBindings();
    GLuint getMaxUniformBufferBindings();
    GLuint getMaxTextureImageUnits();

    static GLuint* createHandle();
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl