#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_BACKEND_IMPL_INLETMEMORYACCUMULATINGPOOL_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_BACKEND_IMPL_INLETMEMORYACCUMULATINGPOOL_HPP_INCLUDE

#include <algorithm>
#include <cassert>
#include <optional>
#include <vector>

#include "../../../../../fixtures/Sink.hpp"
#include "../../../../../setup/InterProcAddress.hpp"
#include "../../../../../spouts/Inlet.hpp"

#include "../RuntimeSizeBackEnd.hpp"

namespace uit {

template<typename PoolSpec>
class InletMemoryAccumulatingPool {

  using address_t = uit::InterProcAddress;
  std::vector<address_t> addresses;

  template<typename T>
  using inlet_wrapper_t = typename PoolSpec::template inlet_wrapper_t<T>;
  std::optional<inlet_wrapper_t<uit::Inlet<PoolSpec>>> inlet;

  using T = typename PoolSpec::T;
  T buffer{};

  // incremented every time TryFlush is called
  // then reset to zero once every member of the pool has called
  size_t pending_flush_counter{};

  #ifndef NDEBUG
    std::unordered_set<size_t> flush_index_checker;
  #endif

  using value_type = typename PoolSpec::T::value_type;

  bool PutPool() {
    assert( IsInitialized() );

    pending_flush_counter = 0;
    #ifndef NDEBUG
      flush_index_checker.clear();
    #endif


    // should we also flush the inlet?
    const bool res = inlet->TryPut( std::move(buffer) );
    buffer.resize( GetSize() );

    if (res) std::fill( std::begin(buffer), std::end(buffer), value_type{} );

    return res;
  }

  void CheckCallingProc() const {
    [[maybe_unused]] const auto& rep = addresses.front();
    assert( rep.GetInletProc() == uitsl::get_rank( rep.GetComm() ) );
  }

public:

  bool IsInitialized() const { return inlet.has_value(); }

  size_t GetSize() const { return addresses.size(); }

  /// Retister a duct for an entry in the pool.
  void Register(const address_t& address) {
    assert( !IsInitialized() );
    assert( std::find(
      std::begin( addresses ), std::end( addresses ), address
    ) == std::end( addresses ) );
    addresses.push_back(address);
  }

  /// Get index of this duct's entry in the pool.
  size_t Lookup(const address_t& address) const {
    assert( IsInitialized() );
    assert( std::find(
      std::begin(addresses), std::end(addresses), address
    ) != std::end(addresses) );
    assert( std::is_sorted( std::begin(addresses), std::end(addresses) ) );
    CheckCallingProc();
    return std::distance(
      std::begin( addresses ),
      std::lower_bound( // get address by binary search
        std::begin( addresses ), std::end( addresses ), address
      )
    );
  }

  /// Get the querying duct's current value from the underlying duct.
  bool TryPut(const value_type& val, const size_t index) {
    assert( IsInitialized() );
    CheckCallingProc();

    buffer[index] += val;

    return true;

  }

  // TODO add move overload?
  bool TryFlush(const size_t index) {
    assert( IsInitialized() );
    CheckCallingProc();

    assert( flush_index_checker.insert(index).second );

    if ( ++pending_flush_counter == GetSize() ) return PutPool();
    else return true;

  }

  /// Call after all members have requested a position in the pool.
  void Initialize() {

    assert( !IsInitialized() );

    assert( std::adjacent_find(
      std::begin(addresses), std::end(addresses),
      [](const auto& a, const auto& b){
        return a.WhichProcsThreads() != b.WhichProcsThreads()
          || a.GetComm() != b.GetComm()
        ;
      }
    ) == std::end(addresses) );

    std::sort( std::begin( addresses ), std::end( addresses ) );

    buffer.resize( addresses.size() );
    auto backend = std::make_shared<
      typename PoolSpec::ProcBackEnd
    >( GetSize() );

    auto sink = uit::Sink<PoolSpec>{
      std::in_place_type_t<
        typename PoolSpec::ProcInletDuct
      >{},
      addresses.front(),
      backend
    };

    backend->Initialize();

    inlet = sink.GetInlet();

    assert( IsInitialized() );

  }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_BACKEND_IMPL_INLETMEMORYACCUMULATINGPOOL_HPP_INCLUDE
