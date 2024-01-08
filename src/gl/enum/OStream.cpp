#include "gl/enum/TextureMagFilter.hpp"
#include "gl/enum/TextureMinFilter.hpp"
#include "gl/enum/TextureWrapS.hpp"
#include "gl/enum/TextureWrapT.hpp"

std::ostream& operator<<(std::ostream& os, const gnev::TextureMinFilter& value) {
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

std::ostream& operator<<(std::ostream& os, const gnev::TextureMagFilter& value) {
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

std::ostream& operator<<(std::ostream& os, const gnev::TextureWrapS& value) {
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

std::ostream& operator<<(std::ostream& os, const gnev::TextureWrapT& value) {
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