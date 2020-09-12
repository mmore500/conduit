#pragma once

#include "MirroredRingBuffer.hpp"

// adapted from https://medium.com/@abhinavagarwal1996/a-fast-circular-ring-buffer-4d102ef4d4a3

namespace uit {

template<typename T, size_t N>
class PrefilledMirroredRingBuffer : public uit::MirroredRingBuffer<T, N> {

  using parent_t = uit::MirroredRingBuffer<T, N>;

public:

  PrefilledMirroredRingBuffer(){ parent_t::Fill(); }

};


} // namespace uit
