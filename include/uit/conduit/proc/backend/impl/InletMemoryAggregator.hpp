#pragma once

#include <algorithm>
#include <mutex>
#include <set>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/base/optional.h"

#include "../../../InterProcAddress.hpp"
#include "../../../Inlet.hpp"
#include "../../../Sink.hpp"

namespace uit {

template<typename AggregatorSpec>
class InletMemoryAggregator {

  using address_t = uit::InterProcAddress;
  std::set<address_t> addresses;

  emp::optional<uit::Inlet<AggregatorSpec>> inlet;

  // multimap of tag -> data
  using T = typename AggregatorSpec::T;
  T buffer{};

  constexpr static inline size_t B{ AggregatorSpec::B };

  // incremented every time TryFlush is called
  // then reset to zero once every member of the pool has called
  size_t pending_flush_counter{};
  // // did the most recent put request succeed?
  // // (can we put new entries in the buffer?)
  // bool last_flush_status{ true };

  #ifndef NDEBUG
    std::unordered_set<size_t> flush_index_checker;
  #endif

  using value_type = typename AggregatorSpec::T::mapped_type;

  bool FlushAggregate() {
    emp_assert( IsInitialized() );

    pending_flush_counter = 0;
    #ifndef NDEBUG
      flush_index_checker.clear();
    #endif

    if ( buffer.empty() ) return inlet->TryFlush();
    else if ( inlet->TryPut( std::move(buffer) ) ) {
      buffer.clear();
      return inlet->TryFlush();
    } else return false;

  }

  void CheckCallingProc() const {
    [[maybe_unused]] const auto& rep = *addresses.begin();
    emp_assert( rep.GetInletProc() == uit::get_rank( rep.GetComm() ) );
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

  /// Get the querying duct's current value from the underlying duct.
  bool TryPut(const value_type& val, const int tag) {
    emp_assert( IsInitialized() );
    CheckCallingProc();

    if (buffer.count(tag) < B) {
      buffer.emplace( std::make_pair( tag, val) );
      return true;
    } else return false;

  }

  // TODO add move overload?

  bool TryFlush(const int tag) {
    emp_assert( IsInitialized() );
    CheckCallingProc();

    emp_assert( flush_index_checker.insert(tag).second );

    if ( ++pending_flush_counter == addresses.size() ) return FlushAggregate();
    else return true;

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

    auto backend = std::make_shared<
      typename AggregatorSpec::ProcBackEnd
    >();

    auto sink = uit::Sink<AggregatorSpec>{
      std::in_place_type_t<
        typename AggregatorSpec::ProcInletDuct
      >{},
      *addresses.begin(),
      backend
    };

    backend->Initialize();

    inlet = sink.GetInlet();

  }

};

} // namespace uit
