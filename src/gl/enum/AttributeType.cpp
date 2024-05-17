#include "gl/enum/AttributeType.hpp"

namespace gnev::gl {

std::string_view format_as(AttributeType value) {
    switch (value) {
        using enum AttributeType;
    case BYTE:
        return "BYTE";
    case UNSIGNED_BYTE:
        return "UNSIGNED_BYTE";
    case SHORT:
        return "SHORT";
    case UNSIGNED_SHORT:
        return "UNSIGNED_SHORT";
    case INT:
        return "INT";
    case UNSIGNED_INT:
        return "UNSIGNED_INT";
    case FIXED:
        return "FIXED";
    case FLOAT:
        return "FLOAT";
    case HALF_FLOAT:
        return "HALF_FLOAT";
    case DOUBLE:
        return "DOUBLE";
    case INT_2_10_10_10_REV:
        return "INT_2_10_10_10_REV";
    case UNSIGNED_INT_2_10_10_10_REV:
        return "UNSIGNED_INT_2_10_10_10_REV";
    case UNSIGNED_INT_10F_11F_11F_REV:
        return "UNSIGNED_INT_10F_11F_11F_REV";
    default:
        return "UNKNOWN";
    }
}

} // namespace gnev::gl