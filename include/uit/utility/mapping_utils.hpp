#pragma once

#include <cmath>
#include <deque>

#include "../../../third-party/Empirical/source/base/vector.h"

using Point = emp::vector<size_t>;
using Dims = emp::vector<size_t>;

namespace uit {

size_t linear_encode(const Point& p, const Dims& dims) {
    size_t mx = 1;
    size_t encoded = 0;
    for (size_t i = 0; i < dims.size(); ++i) {
        encoded += p[i] * mx;
        mx *= dims[i];
    }
    return encoded;
}

Point linear_decode(size_t r, const Dims& len) {
    std::deque<size_t> decoded;
    for (auto it = std::rbegin(len); it != std::rend(len); ++it) {
        decoded.push_front(r % (*it));
        r /= (*it);
    }
    return Point(
        decoded.begin(),
        decoded.end()
    );
}

// TODO: add a Hilbert curve variant


};