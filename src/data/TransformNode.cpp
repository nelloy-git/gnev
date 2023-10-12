#include "data/TransformNode.hpp"

#include <stdexcept>

#include "data/TransformNode.hpp"

using namespace gnev::data;

TransformNode::TransformNode(TransformStorage& storage, Transform::Index index)
    : index(index)
    , storage(storage)
    , data(storage[index]) {}

TransformNode::~TransformNode() {}

void TransformNode::setParent(const TransformNode& parent) {
    if (&storage != &parent.storage) {
        throw std::out_of_range("");
    }
    data.setParentIndex(parent.index);
}

void TransformNode::clearParent(){
    data.setParentIndex(Transform::RESERVED_INDEX);
}

std::optional<TransformNode> TransformNode::getParent() const {
    if (data.getParentIndex() == Transform::RESERVED_INDEX) {
        return std::nullopt;
    }
    return TransformNode(storage, data.getParentIndex());
}