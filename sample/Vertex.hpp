#pragma once

#include "data/Vertex.hpp"
#include "data/VertexInfo.hpp"

static constexpr gnev::data::VertexAttributeInfo AttribPos(3, GL_FLOAT, false);
static constexpr gnev::data::VertexAttributeInfo AttribUV(2, GL_FLOAT, false);
static constexpr gnev::data::VertexAttributeInfo AttribMaterialID(1, GL_INT, false);

using Vertex = gnev::data::Vertex<AttribPos, AttribUV, AttribMaterialID>;
static constexpr gnev::data::VertexInfo VertInfo = Vertex::info;
