#pragma once

#include <unordered_map>

#include "../third-party/Empirical/source/base/vector.h"

namespace uitsl {

template <typename Input, typename Output>
class EnumeratedFunctor {
    std::unordered_map<Input, Output> map;
public:
    EnumeratedFunctor(const std::unordered_map<Input, Output>& map_) : map(map_) { ; }
    EnumeratedFunctor(const emp::vector<Output>& vect) {
        for (size_t i = 0; i < vect.size(); ++i) map[i] = vect[i];
    }

    Output operator()(const Input& a) const { return map.at(a); }
    size_t GetSize() const { return map.size(); }
};

}