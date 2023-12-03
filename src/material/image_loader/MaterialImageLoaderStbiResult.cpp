#include "material/image_loader/MaterialImageLoaderStbiResult.hpp"

#include "magic_enum.hpp"

namespace gnev {

MaterialImageLoaderStbiResult::MaterialImageLoaderStbiResult(std::shared_future<bool>&&
                                                                 done,
                                                             WeakRef<base::MaterialTex>
                                                                 tex_ref)
    : MaterialImageLoaderResult(std::forward<decltype(done)>(done), tex_ref) {}

MaterialImageLoaderStbiResult::~MaterialImageLoaderStbiResult(){};

std::ostream& operator<<(std::ostream& out, const MaterialImageLoaderStbiResult& result) {
    static constexpr auto NAME = "StbiResult";
    static constexpr auto MSGS = "msgs";

    out << NAME << "{";

    out << MSGS << ": [";
    if (result.messages.size() > 0) {
        out << magic_enum::enum_name(result.messages[0]);
        for (int i = 1; i < result.messages.size(); ++i) {
            out << ", " << magic_enum::enum_name(result.messages[i]);
        }
    }
    out << "]";
    out << "}";

    return out;
}

} // namespace gnev