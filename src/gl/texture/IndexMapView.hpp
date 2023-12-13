#pragma once

#include "gl/texture/Accessor3d.hpp"
#include "util/IndexStorage.hpp"
#include "util/Ref.hpp"

namespace gnev::gl::texture {

class EXPORT IndexMapView : public IndexStorage {
public:
    using Changer = Accessor3d::Changer;

    IndexMapView(const Ref<Accessor3d>& accessor);
    virtual ~IndexMapView() = default;

    Ref<Accessor3d> accessor;

    void set(GLuint index, const Image& src);
    void get(GLuint index, Image& dst) const;
    void change(GLuint index, const ImageInfo& info, Changer changer);
};

} // namespace gnev::gl::texture