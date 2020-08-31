#pragma once

#include <cmath>
#include <deque>

#include "../../../third-party/Empirical/source/base/vector.h"

using Point = emp::vector<size_t>;
using Dims = emp::vector<size_t>;

namespace uit {

size_t linear_encode(const Point& coords, const Dims& len) {
    /* dims {a, b, c, d}
    * {6, 4, 2, 1} -> 1 * bcd + 2 * cd + 4 * d + 6
    *
    * {w, x, y, z} -> w * bcd + x * cd + y * d + z = r0
    * r0 % d = z
    * r0 / d = w * bc + x * c + y = r1
    * r1 % c = y
    * r1 / c = w * b + x = r2
    * r2 % b = x
    * r2 / b = w
    *
    *
    * r/d % c - z/d = y
    *
    */

    size_t encoded{};
    for (int i = coords.size() - 1; i >= 0; --i) {
        encoded += std::pow(len[i], i) * coords[i];
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