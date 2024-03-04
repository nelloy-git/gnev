// #include "mesh/3d/Quad_3D.hpp"

// namespace gnev {

// Quad_3D::Quad_3D(const WeakRef<IndexView>& weak_index_view,
//                  const WeakRef<DataView>& weak_data_view)
//     : index(MakeSharable<IndexElem>(weak_index_view, &cleanupIndex))
//     , data(MakeSharable<DataElem>(weak_data_view, &cleanupQuad)) {
//     GLuint quad_data_index = data->getIndex();
//     GLuint vert_data_index = 4 * quad_data_index;
//     QuadGLindex_3D quad_indices{vert_data_index,
//                                 vert_data_index + 1,
//                                 vert_data_index + 2,
//                                 vert_data_index + 3,
//                                 vert_data_index + 2,
//                                 vert_data_index + 1};
//     index->set(quad_indices);
// }

// void Quad_3D::setQuad(const QuadGLdata_3D& quadrangle) { data->set(quadrangle); }

// void Quad_3D::setVert(GLuint i, const VertGLdata_3D& vertex) {
//     if (i > 3) {
//         throw std::out_of_range("");
//     }
//     data->set(vertex, i * sizeof(vertex));
// }

// void Quad_3D::cleanupIndex(Ref<IndexView>& view, GLuint index) {
//     view->set(index,
//               {InvalidIndex,
//                InvalidIndex,
//                InvalidIndex,
//                InvalidIndex,
//                InvalidIndex,
//                InvalidIndex});
// }

// void Quad_3D::cleanupQuad(Ref<DataView>& view, GLuint index) {
//     static const VertGLdata_3D EMPTY_VERT = {
//         {0, 0, 0},
//         {0, 0},
//         {InvalidIndex, InvalidIndex, InvalidIndex, InvalidIndex}};
//     static const QuadGLdata_3D EMPTY_QUAD = {EMPTY_VERT,
//                                              EMPTY_VERT,
//                                              EMPTY_VERT,
//                                              EMPTY_VERT};
//     view->set(index, EMPTY_QUAD);
// }

// } // namespace gnev