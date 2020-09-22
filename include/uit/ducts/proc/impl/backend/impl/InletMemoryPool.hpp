#pragma once

#include <algorithm>
#include <mutex>
#include <set>

#include "../../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../../third-party/Empirical/source/base/optional.h"

#include "../../../../../fixtures/Sink.hpp"
#include "../../../../../setup/InterProcAddress.hpp"
#include "../../../../../spouts/Inlet.hpp"

namespace uit {

template<typename PoolSpec>
class InletMemoryPool {

  using address_t = uit::InterProcAddress;
  std::set<address_t> addresses;

  emp::optional<uit::Inlet<PoolSpec>> inlet;

  using T = typename PoolSpec::T;
  T buffer{};

  // incremented every time TryPut is called
  // then reset to zero once every member of the pool has called
  size_t pending_put_counter{};
  // did the most recent put request succeed?
  // (can we put new entries in the buffer?)
  bool last_put_status{ true };

  #ifndef NDEBUG
    std::unordered_set<size_t> put_index_checker;
  #endif

  using value_type = typename PoolSpec::T::value_type;

  bool PutPool() {
    emp_assert( IsInitialized() );

    pending_put_counter = 0;
    #ifndef NDEBUG
      put_index_checker.clear();
    #endif

    const bool res = inlet->TryPut( std::move(buffer) );
    buffer.resize( addresses.size() );

    return res;
  }

  void CheckCallingProc() const {
    const auto& rep = *addresses.begin();
    emp_assert( rep.GetInletProc() == uitsl::get_rank( rep.GetComm() ) );
  }

public:

  bool IsInitialized() const { return inlet.has_value(); }

  size_t GetSize() const { return addresses.size(); }

  /// Retister a duct for an entry in the pool.
  void Register(const address_t& address) {
    emp_assert( !IsInitialized() );
    emp_assert( !addresses.count(address) );
    addresses.insert(address);
  }

  /// Get index of this duct's entry in the pool.
  size_t Lookup(const address_t& address) const {
    emp_assert( IsInitialized() );
    emp_assert( addresses.count(address) );
    CheckCallingProc();
    return std::distance( std::begin(addresses), addresses.find(address) );
  }

  /// Get the querying duct's current value from the underlying duct.
  bool TryPut(const value_type& val, const size_t index) {
    emp_assert( IsInitialized() );
    CheckCallingProc();

    if ( last_put_status ) {
      buffer[index] = val;
      emp_assert( put_index_checker.insert(index).second );
    }

    const bool res = last_put_status;

    if ( ++pending_put_counter == addresses.size() ) {
      last_put_status = PutPool();
    }

    return res;

  }

  // TODO add move overload?

  bool TryFlush() {
    emp_assert( IsInitialized() );
    CheckCallingProc();

    return inlet->TryFlush();
  }

  /// Call after all members have requested a position in the pool.
  void Initialize() {

    emp_assert( !IsInitialized() );

    emp_assert( std::all_of(
      std::begin(addresses),
      std::end(addresses),
      [this](const auto& addr){ return (
          addr.GetOutletProc() == addresses.begin()->GetOutletProc()
          && addr.GetInletThread() == addresses.begin()->GetInletThread()
          && addr.GetComm() == addresses.begin()->GetComm()
        );
      }
    ) );

    buffer.resize( addresses.size() );
    auto backend = std::make_shared<
      typename PoolSpec::ProcBackEnd
    >( addresses.size() );

    auto sink = uit::Sink<PoolSpec>{
      std::in_place_type_t<
        typename PoolSpec::ProcInletDuct
      >{},
      *addresses.begin(),
      backend
    };

    backend->Initialize();

    inlet = sink.GetInlet();

  }

};

} // namespace uit
