#pragma once

#include <tuple>
#include <type_traits>

namespace uit {

template<typename T>
class RDMAPacket {

  static_assert(std::is_trivially_copyable<T>::value);

  T data{};
  size_t epoch{};

public:

  RDMAPacket() = default;

  RDMAPacket(const T& data_, const size_t& epoch_)
  : data(data_), epoch(epoch_)
  { ; }

  const T& GetData() const { return data; }

  T& GetData() { return data; }

  const size_t& GetEpoch() const { return epoch; }

};

} // namespace uit
