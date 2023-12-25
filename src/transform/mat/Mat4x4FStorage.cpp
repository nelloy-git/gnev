#include "transform/mat/Mat4x4FStorage.hpp"

namespace gnev {

using DataElem = Mat4x4Storage::DataElem;

Ref<Mat4x4Storage> Mat4x4Storage::MakeCoherent(GLuint capacity) {
    return MakeSharable<Mat4x4Storage>(DataView::MakeCoherent(capacity));
}

Mat4x4Storage::Mat4x4Storage(const Ref<DataView> data_view)
    : data_view(data_view) {}

Ref<DataElem> Mat4x4Storage::lockMat(const MatGLdata& initial) {
    return MakeSharable<DataElem>(data_view, initial);
}

} // namespace gnev