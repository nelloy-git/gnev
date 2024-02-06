#include "gl/enum/ShaderType.hpp"
#include "gl/enum/TextureFormat.hpp"
#include "gl/enum/TextureInternalFormat.hpp"
#include "gl/enum/TextureMagFilter.hpp"
#include "gl/enum/TextureMinFilter.hpp"
#include "gl/enum/TextureTarget.hpp"
#include "gl/enum/TextureType.hpp"
#include "gl/enum/TextureWrapS.hpp"
#include "gl/enum/TextureWrapT.hpp"

std::ostream& gnev::operator<<(std::ostream& os, const gnev::ShaderType& value) {
    switch (value) {
        using enum gnev::ShaderType;
    case COMPUTE_SHADER:
        return os << "COMPUTE_SHADER";
    case VERTEX_SHADER:
        return os << "VERTEX_SHADER";
    case TESS_CONTROL_SHADER:
        return os << "TESS_CONTROL_SHADER";
    case TESS_EVALUATION_SHADER:
        return os << "TESS_EVALUATION_SHADER";
    case GEOMETRY_SHADER:
        return os << "GEOMETRY_SHADER";
    case FRAGMENT_SHADER:
        return os << "FRAGMENT_SHADER";

    default:
        return os << "UNKNOWN";
    }
}

std::ostream& gnev::operator<<(std::ostream& os, const gnev::TextureMinFilter& value) {
    switch (value) {
        using enum gnev::TextureMinFilter;
    case NEAREST:
        return os << "NEAREST";
    case LINEAR:
        return os << "LINEAR";
    case NEAREST_MIPMAP_NEAREST:
        return os << "NEAREST_MIPMAP_NEAREST";
    case LINEAR_MIPMAP_NEAREST:
        return os << "LINEAR_MIPMAP_NEAREST";
    case NEAREST_MIPMAP_LINEAR:
        return os << "NEAREST_MIPMAP_LINEAR";
    case LINEAR_MIPMAP_LINEAR:
        return os << "LINEAR_MIPMAP_LINEAR";

    default:
        return os << "UNKNOWN";
    }
}

std::ostream& gnev::operator<<(std::ostream& os, const gnev::TextureMagFilter& value) {
    switch (value) {
        using enum gnev::TextureMagFilter;
    case NEAREST:
        return os << "NEAREST";
    case LINEAR:
        return os << "LINEAR";

    default:
        return os << "UNKNOWN";
    }
}

std::ostream& gnev::operator<<(std::ostream& os, const gnev::TextureWrapS& value) {
    switch (value) {
        using enum gnev::TextureWrapS;
    case REPEAT:
        return os << "REPEAT";
    case MIRRORED_REPEAT:
        return os << "MIRRORED_REPEAT";
    case CLAMP_TO_EDGE:
        return os << "CLAMP_TO_EDGE";
    case MIRROR_CLAMP_TO_EDGE:
        return os << "MIRROR_CLAMP_TO_EDGE";

    default:
        return os << "UNKNOWN";
    }
}

std::ostream& gnev::operator<<(std::ostream& os, const gnev::TextureWrapT& value) {
    switch (value) {
        using enum gnev::TextureWrapT;
    case REPEAT:
        return os << "REPEAT";
    case MIRRORED_REPEAT:
        return os << "MIRRORED_REPEAT";
    case CLAMP_TO_EDGE:
        return os << "CLAMP_TO_EDGE";
    case MIRROR_CLAMP_TO_EDGE:
        return os << "MIRROR_CLAMP_TO_EDGE";

    default:
        return os << "UNKNOWN";
    }
}

std::ostream& gnev::operator<<(std::ostream& os, const gnev::TextureTarget& value) {
    switch (value) {
        using enum gnev::TextureTarget;
    case TEXTURE_1D:
        return os << "TEXTURE_1D";
    case TEXTURE_2D:
        return os << "TEXTURE_2D";
    case TEXTURE_3D:
        return os << "TEXTURE_3D";
    case TEXTURE_1D_ARRAY:
        return os << "TEXTURE_1D_ARRAY";
    case TEXTURE_2D_ARRAY:
        return os << "TEXTURE_2D_ARRAY";
    case TEXTURE_RECTANGLE:
        return os << "TEXTURE_RECTANGLE";
    case TEXTURE_CUBE_MAP:
        return os << "TEXTURE_CUBE_MAP";
    case TEXTURE_CUBE_MAP_ARRAY:
        return os << "TEXTURE_CUBE_MAP_ARRAY";
    case TEXTURE_BUFFER:
        return os << "TEXTURE_BUFFER";
    case TEXTURE_2D_MULTISAMPLE:
        return os << "TEXTURE_2D_MULTISAMPLE";
    case TEXTURE_2D_MULTISAMPLE_ARRAY:
        return os << "TEXTURE_2D_MULTISAMPLE_ARRAY";

    default:
        return os << "UNKNOWN";
    }
}

std::ostream& gnev::operator<<(std::ostream& os,
                               const gnev::TextureInternalFormat& value) {
    switch (value) {
        using enum gnev::TextureInternalFormat;
    case R8:
        return os << "R8";
    case R8_SNORM:
        return os << "R8_SNORM";
    case R16:
        return os << "R16";
    case R16_SNORM:
        return os << "R16_SNORM";
    case RG8:
        return os << "RG8";
    case RG8_SNORM:
        return os << "RG8_SNORM";
    case RG16:
        return os << "RG16";
    case RG16_SNORM:
        return os << "RG16_SNORM";
    case R3_G3_B2:
        return os << "R3_G3_B2";
    case RGB4:
        return os << "RGB4";
    case RGB5:
        return os << "RGB5";
    case RGB8:
        return os << "RGB8";
    case RGB8_SNORM:
        return os << "RGB8_SNORM";
    case RGB10:
        return os << "RGB10";
    case RGB12:
        return os << "RGB12";
    case RGB16_SNORM:
        return os << "RGB16_SNORM";
    case RGBA2:
        return os << "RGBA2";
    case RGBA4:
        return os << "RGBA4";
    case RGB5_A1:
        return os << "RGB5_A1";
    case RGBA8:
        return os << "RGBA8";
    case RGBA8_SNORM:
        return os << "RGBA8_SNORM";
    case RGB10_A2:
        return os << "RGB10_A2";
    case RGB10_A2UI:
        return os << "RGB10_A2UI";
    case RGBA12:
        return os << "RGBA12";
    case RGBA16:
        return os << "RGBA16";
    case SRGB8:
        return os << "SRGB8";
    case SRGB8_ALPHA8:
        return os << "SRGB8_ALPHA8";
    case R16F:
        return os << "R16F";
    case RG16F:
        return os << "RG16F";
    case RGB16F:
        return os << "RGB16F";
    case RGBA16F:
        return os << "RGBA16F";
    case R32F:
        return os << "R32F";
    case RG32F:
        return os << "RG32F";
    case RGB32F:
        return os << "RGB32F";
    case RGBA32F:
        return os << "RGBA32F";
    case R11F_G11F_B10F:
        return os << "R11F_G11F_B10F";
    case RGB9_E5:
        return os << "RGB9_E5";
    case R8I:
        return os << "R8I";
    case R8UI:
        return os << "R8UI";
    case R16I:
        return os << "R16I";
    case R16UI:
        return os << "R16UI";
    case R32I:
        return os << "R32I";
    case R32UI:
        return os << "R32UI";
    case RG8I:
        return os << "RG8I";
    case RG8UI:
        return os << "RG8UI";
    case RG16I:
        return os << "RG16I";
    case RG16UI:
        return os << "RG16UI";
    case RG32I:
        return os << "RG32I";
    case RG32UI:
        return os << "RG32UI";
    case RGB8I:
        return os << "RGB8I";
    case RGB8UI:
        return os << "RGB8UI";
    case RGB16I:
        return os << "RGB16I";
    case RGB16UI:
        return os << "RGB16UI";
    case RGB32I:
        return os << "RGB32I";
    case RGB32UI:
        return os << "RGB32UI";
    case RGBA8I:
        return os << "RGBA8I";
    case RGBA8UI:
        return os << "RGBA8UI";
    case RGBA16I:
        return os << "RGBA16I";
    case RGBA16UI:
        return os << "RGBA16UI";
    case RGBA32I:
        return os << "RGBA32I";
    case RGBA32UI:
        return os << "RGBA32UI";
    case DEPTH_COMPONENT32F:
        return os << "DEPTH_COMPONENT32F";
    case DEPTH_COMPONENT24:
        return os << "DEPTH_COMPONENT24";
    case DEPTH_COMPONENT16:
        return os << "DEPTH_COMPONENT16";
    case DEPTH32F_STENCIL8:
        return os << "DEPTH32F_STENCIL8";
    case DEPTH24_STENCIL8:
        return os << "DEPTH24_STENCIL8";
    case STENCIL_INDEX8:
        return os << "STENCIL_INDEX8";

    default:
        return os << "UNKNOWN";
    }
}

std::ostream& gnev::operator<<(std::ostream& os, const gnev::TextureFormat& value) {
    switch (value) {
        using enum gnev::TextureFormat;
    case RED:
        return os << "RED";
    case RG:
        return os << "RG";
    case RGB:
        return os << "RGB";
    case BGR:
        return os << "BGR";
    case RGBA:
        return os << "RGBA";
    case DEPTH_COMPONENT:
        return os << "DEPTH_COMPONENT";
    case STENCIL_INDEX:
        return os << "STENCIL_INDEX";

    default:
        return os << "UNKNOWN";
    }
}

std::ostream& gnev::operator<<(std::ostream& os, const gnev::TextureType& value) {
    switch (value) {
        using enum gnev::TextureType;
    case UNSIGNED_BYTE:
        return os << "UNSIGNED_BYTE";
    case BYTE:
        return os << "BYTE";
    case UNSIGNED_SHORT:
        return os << "UNSIGNED_SHORT";
    case SHORT:
        return os << "SHORT";
    case UNSIGNED_INT:
        return os << "UNSIGNED_INT";
    case INT:
        return os << "INT";
    case FLOAT:
        return os << "FLOAT";
    case UNSIGNED_BYTE_3_3_2:
        return os << "UNSIGNED_BYTE_3_3_2";
    case UNSIGNED_BYTE_2_3_3_REV:
        return os << "UNSIGNED_BYTE_2_3_3_REV";
    case UNSIGNED_SHORT_5_6_5:
        return os << "UNSIGNED_SHORT_5_6_5";
    case UNSIGNED_SHORT_5_6_5_REV:
        return os << "UNSIGNED_SHORT_5_6_5_REV";
    case UNSIGNED_SHORT_4_4_4_4:
        return os << "UNSIGNED_SHORT_4_4_4_4";
    case UNSIGNED_SHORT_4_4_4_4_REV:
        return os << "UNSIGNED_SHORT_4_4_4_4_REV";
    case UNSIGNED_SHORT_5_5_5_1:
        return os << "UNSIGNED_SHORT_5_5_5_1";
    case UNSIGNED_SHORT_1_5_5_5_REV:
        return os << "UNSIGNED_SHORT_1_5_5_5_REV";
    case UNSIGNED_INT_8_8_8_8:
        return os << "UNSIGNED_INT_8_8_8_8";
    case UNSIGNED_INT_8_8_8_8_REV:
        return os << "UNSIGNED_INT_8_8_8_8_REV";
    case UNSIGNED_INT_10_10_10_2:
        return os << "UNSIGNED_INT_10_10_10_2";
    case UNSIGNED_INT_2_10_10_10_REV:
        return os << "UNSIGNED_INT_2_10_10_10_REV";

    default:
        return os << "UNKNOWN";
    }
}