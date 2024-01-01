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
    virtual ~Program() = default;

    void glAttachShader(const Shader& shader);
    void glValidateProgram();
    void glLinkProgram();
    void use() const;
    void glGetProgramiv(GLenum pname, GLint* params) const;
    void glGetProgramInfoLog(GLsizei bufSize, GLsizei* length, GLchar* infoLog) const;
    GLint glGetUniformBlockIndex(const GLchar* uniformBlockName) const;
    void glUniformBlockBinding(GLuint uniformBlockIndex,
                               GLuint uniformBlockBinding) const;
    GLint glGetAttribLocation(const GLchar* name) const;
    GLint glGetUniformLocation(const GLchar* name) const;

    GLint glGetProgramResourceIndex(GLenum programInterface, const GLchar* name) const;
    void glShaderStorageBlockBinding(GLuint storageBlockIndex,
                                     GLuint storageBlockBinding) const;

    void bindShaderStorageBlockBuffer(const std::string& storage_block_name,
                                      const Ref<Buffer>& buffer);
    void bindShaderStorageBlockBuffer(GLuint storage_block_index,
                                      const Ref<Buffer>& buffer);

    void bindShaderUniformBlockBuffer(const std::string& uniform_block_name,
                                      const Ref<Buffer>& buffer);
    void bindShaderUniformBlockBuffer(GLuint uniform_block_index,
                                      const Ref<Buffer>& buffer);

    void bindShaderTextureSampler(const std::string& texture_sampler_name,
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

    Bindings<Buffer> shader_storage_blocks;
    Bindings<Buffer> shader_uniform_blocks;
    Bindings<Texture> shader_texture_samplers;

    static GLuint getMaxShaderStorageBufferBindings();
    static GLuint getMaxUniformBufferBindings();
    static GLuint getMaxTextureImageUnits();

    static GLuint* createHandle();
    static void deleteHandle(GLuint* handle);
};

} // namespace gnev::gl