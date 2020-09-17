#pragma once

#include <algorithm>
#include <mutex>
#include <set>

#include "../../../../../../third-party/Empirical/source/base/assert.h"
#include "../../../../../../third-party/Empirical/source/base/optional.h"

#include "../../../InterProcAddress.hpp"
#include "../../../Outlet.hpp"
#include "../../../Source.hpp"

namespace uit {

template<typename PoolSpec>
class OutletMemoryPool {

  using address_t = uit::InterProcAddress;
  std::set<address_t> addresses;

  emp::optional<uit::Outlet<PoolSpec>> outlet;

  // incremented every time TryConsumeGets is called
  // then reset to zero once every member of the pool has called
  size_t consume_call_counter{};

  size_t current_request{}; // num jump steps requested in current round
  size_t current_num_consumed{}; // num jump steps realized in current round

  #ifndef NDEBUG
    std::set<address_t> address_checker;
  #endif

  using value_type = typename PoolSpec::T::value_type;

  void DoTryConsumeGets(const size_t requested) {
    current_request = requested;
    current_num_consumed = outlet->TryStep(requested);

    emp_assert( std::all_of(
      std::begin(address_checker),
      std::end(address_checker),
      [this](const auto& addr){
        return address_checker.begin()->GetOutletThread()
          == addr.GetOutletThread();
      }
    ) );
    #ifndef NDEBUG
      address_checker.clear();
    #endif
  }

  void CheckCallingProc() const {
    const auto& rep = *addresses.begin();
    emp_assert( rep.GetOutletProc() == uitsl::get_rank( rep.GetComm() ) );
  }

public:

  bool IsInitialized() const { return outlet.has_value(); }

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
  value_type& Get(const size_t index) {
    emp_assert( IsInitialized() );
    CheckCallingProc();
    return outlet->Get()[index];
  }

  /// Get the querying duct's current value from the underlying duct.
  const value_type& Get(const size_t index) const {
    emp_assert( IsInitialized() );
    CheckCallingProc();
    return outlet->Get()[index];
  }

  /// Every member of the pool should call this with same requested.
  size_t TryConsumeGets(
    const size_t requested,
    const address_t& address // only incl for debug safety check
  ) {
    emp_assert( IsInitialized() );
    CheckCallingProc();

    if ( consume_call_counter == 0 ) DoTryConsumeGets(requested);

    // check that same request is being made by each caller in current cycle
    emp_assert( current_request == requested );

    // check that address is unique
    emp_assert( address_checker.insert( address ).second );

    ++consume_call_counter;
    consume_call_counter %= addresses.size();

    return current_num_consumed;
  }

  /// Call after all members have requested a position in the pool.
  void Initialize() {

    emp_assert( !IsInitialized() );

    emp_assert( std::all_of(
      std::begin(addresses),
      std::end(addresses),
      [this](const auto& addr){ return (
          addr.GetInletProc() == addresses.begin()->GetInletProc()
          && addr.GetOutletThread() == addresses.begin()->GetOutletThread()
          && addr.GetComm() == addresses.begin()->GetComm()
        );
      }
    ) );
    emp_assert( !addresses.empty() );

    auto backend = std::make_shared<
      typename PoolSpec::ProcBackEnd
    >( addresses.size() );

    auto source = uit::Source<PoolSpec>{
      std::in_place_type_t<
        typename PoolSpec::ProcOutletDuct
      >{},
      *addresses.begin(),
      backend
    };

    backend->Initialize();

    outlet = source.GetOutlet();

  }

  constexpr static bool CanStep() { return PoolSpec::OutletImpl::CanStep(); }

};

} // namespace uit
