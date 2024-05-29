#pragma once

#include "gl/enum/AttributeType.hpp"

namespace gnev::gl {

struct VertexAttributeInfo {
    constexpr VertexAttributeInfo(GLuint elements, AttributeType type, bool normalized)
        : elements(elements)
        , type(type)
        , normalized(normalized)
        , is_packed(is_packed_type(type))
        , is_signed(is_signed_type(type)) {
        check(elements > 0 and elements <= 4, "elements > 0 and elements <= 4");

        if (is_packed) {
            switch (type) {
                using enum AttributeType;
            case INT_2_10_10_10_REV:
            case UNSIGNED_INT_10_10_10_2:
            case UNSIGNED_INT_2_10_10_10_REV:
                check(elements == 4, "elements == 4");
                break;
            case UNSIGNED_INT_10F_11F_11F_REV:
                check(elements == 3, "elements == 3");
                break;
            default:
                check(false, "Unsupported type");
            }
        }
    };

    GLuint elements;
    AttributeType type;
    bool normalized;
    bool is_packed;
    bool is_signed;

private:
    static constexpr void check(bool cond, const std::string& msg) {
        if (not cond) {
            throw msg;
        }
    }

    static constexpr bool is_packed_type(AttributeType type) {
        switch (type) {
        case AttributeType::HALF_FLOAT:
        case AttributeType::FLOAT:
        case AttributeType::DOUBLE:
        case AttributeType::INT:
        case AttributeType::BYTE:
        case AttributeType::SHORT:
        case AttributeType::UNSIGNED_INT:
        case AttributeType::UNSIGNED_BYTE:
        case AttributeType::UNSIGNED_SHORT:
            return false;

        case AttributeType::UNSIGNED_INT_10_10_10_2:
        case AttributeType::INT_2_10_10_10_REV:
            return true;

        default:
            check(false, "Unsupported type");
            return false;
        }
    };

    static constexpr bool is_signed_type(AttributeType type) {
        switch (type) {
        case AttributeType::HALF_FLOAT:
        case AttributeType::FLOAT:
        case AttributeType::DOUBLE:
        case AttributeType::INT:
        case AttributeType::BYTE:
        case AttributeType::SHORT:
        case AttributeType::INT_2_10_10_10_REV:
            return true;

        case AttributeType::UNSIGNED_INT:
        case AttributeType::UNSIGNED_BYTE:
        case AttributeType::UNSIGNED_SHORT:
        case AttributeType::UNSIGNED_INT_10_10_10_2:
            return false;

        default:
            check(false, "Unsupported type");
            return false;
        }
    }
};

} // namespace gnev::gl