#pragma once

#include <cstddef>

#include "../../../third-party/Empirical/source/base/optional.h"

namespace uitsl {

template<typename T>
class CachePacket {

  size_t cache_id{};

  emp::optional<T> data;

public:

  CachePacket() = default;

  CachePacket(const size_t cache_id_) : cache_id(cache_id_) { ; }

  CachePacket(
    const size_t cache_id_,
    const T& data_
  ) : cache_id(cache_id_)
  , data(data_)
  { ; }

  template<typename P>
  CachePacket(
    const size_t cache_id_,
    P&& data_
  ) : cache_id(cache_id_)
  , data( std::forward<P>(data_) )
  { ; }

  bool HasData() const { return data.has_value(); }

  T& GetData() { return *data; }

  const T& GetData() const { return *data; }

  size_t GetID() const { return cache_id; }

};

} // namespace uitsl
