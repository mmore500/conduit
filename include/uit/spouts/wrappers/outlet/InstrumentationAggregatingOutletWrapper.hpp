#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_OUTLET_INSTRUMENTATIONAGGREGATINGOUTLETWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_OUTLET_INSTRUMENTATIONAGGREGATINGOUTLETWRAPPER_HPP_INCLUDE

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include "../../../../uit_emp/data/DataFile.hpp"
#include "../../../../uit_emp/tools/string_utils.hpp"

#include "../../../../uitsl/algorithm/accumulate_if.hpp"
#include "../../../../uitsl/containers/safe/unordered_set.hpp"
#include "../../../../uitsl/countdown/coarse_runtime.hpp"
#include "../../../../uitsl/mpi/comm_utils.hpp"
#include "../../../../uitsl/mpi/proc_id_t.hpp"
#include "../../../../uitsl/parallel/thread_utils.hpp"

#include "../impl/round_trip_touch_counter.hpp"

namespace uit {
namespace internal {

template<typename Outlet>
class InstrumentationAggregatingOutletWrapper {

  using ImplSpec = typename Outlet::ImplSpec;
  using this_t = InstrumentationAggregatingOutletWrapper<Outlet>;

  using outlet_t = Outlet;
  outlet_t outlet;

  using value_type = typename ImplSpec::value_type;

  inline static uitsl::safe::unordered_set<const this_t*> registry;

  template<typename Filter>
  struct RegistryAggregator {

    static size_t GetNumReadsPerformed() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumReadsPerformed();
        },
        Filter{}
      );
    }

    static size_t GetNumReadsThatWereFresh() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumReadsThatWereFresh();
        },
        Filter{}
      );
    }

    static size_t GetNumReadsThatWereStale() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumReadsThatWereStale();
        },
        Filter{}
      );
    }

    static size_t GetNumRevisionsPulled() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumRevisionsPulled();
        },
        Filter{}
      );
    }

    static size_t GetNumTryPullsAttempted() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumTryPullsAttempted();
        },
        Filter{}
      );
    }

    static size_t GetNumBlockingPulls() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumBlockingPulls();
        },
        Filter{}
      );
    }

    static size_t GetNumBlockingPullsThatBlocked() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumBlockingPullsThatBlocked();
        },
        Filter{}
      );
    }

    static size_t GetNumRevisionsFromTryPulls() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumRevisionsFromTryPulls();
        },
        Filter{}
      );
    }

    static size_t GetNumRevisionsFromBlockingPulls() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumRevisionsFromBlockingPulls();
        },
        Filter{}
      );
    }

    static size_t GetNumPullsAttempted() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumPullsAttempted();
        },
        Filter{}
      );
    }

    static size_t GetNumPullsThatWereLadenEventually() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumPullsThatWereLadenEventually();
        },
        Filter{}
      );
    }

    static size_t GetNumBlockingPullsThatWereLadenImmediately() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumBlockingPullsThatWereLadenImmediately();
        },
        Filter{}
      );
    }

    static size_t GetNumBlockingPullsThatWereLadenEventually() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumBlockingPullsThatWereLadenEventually();
        },
        Filter{}
      );
    }

    static size_t GetNumPullsThatWereLadenImmediately() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumPullsThatWereLadenImmediately();
        },
        Filter{}
      );
    }

    static size_t GetNumTryPullsThatWereLaden() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumTryPullsThatWereLaden();
        },
        Filter{}
      );
    }

    static size_t GetNumTryPullsThatWereUnladen() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNumTryPullsThatWereUnladen();
        },
        Filter{}
      );
    }

    static size_t GetNumRoundTripTouches() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetCurRoundTripTouchCount();
        },
        Filter{}
      );
    }

    static double GetFractionTryPullsThatWereLaden() {
      struct Adder {
        size_t num_try_pulls_that_were_laden{};
        size_t num_try_pulls_attempted{};
        double GetRatio() const {
          return num_try_pulls_that_were_laden / static_cast<double>(
            num_try_pulls_attempted
          );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_try_pulls_that_were_laden
            += inlet->GetNumTryPullsThatWereLaden();
          accum.num_try_pulls_attempted += inlet->GetNumTryPullsAttempted();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }


    static double GetFractionTryPullsThatWereUnladen() {
      struct Adder {
        size_t num_try_pulls_that_were_unladen{};
        size_t num_try_pulls_attempted{};
        double GetRatio() const {
          return num_try_pulls_that_were_unladen / static_cast<double>(
            num_try_pulls_attempted
          );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_try_pulls_that_were_unladen
            += inlet->GetNumTryPullsThatWereUnladen();
          accum.num_try_pulls_attempted += inlet->GetNumTryPullsAttempted();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionBlockingPullsThatBlocked() {
      struct Adder {
        size_t num_blocking_pulls_that_blocked{};
        size_t num_blocking_pulls{};
        double GetRatio() const {
          return num_blocking_pulls_that_blocked / static_cast<double>(
            num_blocking_pulls
          );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_blocking_pulls_that_blocked
            += inlet->GetNumBlockingPullsThatBlocked();
          accum.num_blocking_pulls += inlet->GetNumBlockingPulls();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionBlockingPullsThatWereLadenImmediately() {
      struct Adder {
        size_t num_blocking_pulls_that_were_laden_immediately{};
        size_t num_blocking_pulls{};
        double GetRatio() const {
          return num_blocking_pulls_that_were_laden_immediately
            / static_cast<double>( num_blocking_pulls );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_blocking_pulls_that_were_laden_immediately
            += inlet->GetNumBlockingPullsThatWereLadenImmediately();
          accum.num_blocking_pulls += inlet->GetNumBlockingPulls();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionBlockingPullsThatWereLadenEventually() {
      struct Adder {
        size_t num_blocking_pulls_that_were_laden_eventually{};
        size_t num_blocking_pulls{};
        double GetRatio() const {
          return num_blocking_pulls_that_were_laden_eventually
            / static_cast<double>( num_blocking_pulls );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_blocking_pulls_that_were_laden_eventually
            += inlet->GetNumBlockingPullsThatWereLadenEventually();
          accum.num_blocking_pulls += inlet->GetNumBlockingPulls();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionPullsThatWereLadenImmediately() {
      struct Adder {
        size_t num_blocking_pulls_that_were_laden_immediately{};
        size_t num_pulls_attempted{};
        double GetRatio() const {
          return num_blocking_pulls_that_were_laden_immediately
            / static_cast<double>( num_pulls_attempted );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_blocking_pulls_that_were_laden_immediately
            += inlet->GetNumPullsThatWereLadenImmediately();
          accum.num_pulls_attempted += inlet->GetNumPullsAttempted();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionPullsThatWereLadenEventually() {
      struct Adder {
        size_t num_blocking_pulls_that_were_laden_eventually{};
        size_t num_pulls_attempted{};
        double GetRatio() const {
          return num_blocking_pulls_that_were_laden_eventually
            / static_cast<double>( num_pulls_attempted );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_blocking_pulls_that_were_laden_eventually
            += inlet->GetNumPullsThatWereLadenEventually();
          accum.num_pulls_attempted += inlet->GetNumPullsAttempted();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static size_t GetNetFluxThroughDuct() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), size_t{},
        [](size_t accum, const this_t* outlet) {
          return accum + outlet->GetNetFluxThroughDuct();
        },
        Filter{}
      );
    }

    static double GetFractionReadsThatWereFresh() {
      struct Adder {
        size_t num_reads_that_were_fresh{};
        size_t num_reads_performed{};
        double GetRatio() const {
          return num_reads_that_were_fresh
            / static_cast<double>( num_reads_performed );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_reads_that_were_fresh += inlet->GetNumReadsThatWereFresh();
          accum.num_reads_performed += inlet->GetNumReadsPerformed();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionReadsThatWereStale() {
      struct Adder {
        size_t num_reads_that_were_stale{};
        size_t num_reads_performed{};
        double GetRatio() const {
          return num_reads_that_were_stale
            / static_cast<double>( num_reads_performed );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_reads_that_were_stale += inlet->GetNumReadsThatWereStale();
          accum.num_reads_performed += inlet->GetNumReadsPerformed();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionRevisionsThatWereRead() {
      struct Adder {
        size_t num_reads_that_were_fresh{};
        size_t num_revisions_pulled{};
        double GetRatio() const {
          return num_reads_that_were_fresh
            / static_cast<double>( num_revisions_pulled );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_reads_that_were_fresh += inlet->GetNumReadsThatWereFresh();
          accum.num_revisions_pulled += inlet->GetNumRevisionsPulled();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionRevisionsThatWereNotRead() {
      return 1.0 - GetFractionRevisionsThatWereRead();
    }

    static double GetFractionDuctFluxThatWasSteppedThrough() {
      struct Adder {
        size_t num_revisions_pulled{};
        size_t net_flux_through_duct{};
        double GetRatio() const {
          return num_revisions_pulled
            / static_cast<double>( net_flux_through_duct );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_revisions_pulled += inlet->GetNumRevisionsPulled();
          accum.net_flux_through_duct += inlet->GetNetFluxThroughDuct();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetFractionDuctFluxThatWasJumpedOver() {
      return 1.0 - GetFractionDuctFluxThatWasSteppedThrough();
    }

    static double GetFractionDuctFluxThatWasRead() {
      struct Adder {
        size_t num_reads_that_were_fresh{};
        size_t net_flux_through_duct{};
        double GetRatio() const {
          return num_reads_that_were_fresh
            / static_cast<double>( net_flux_through_duct );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_reads_that_were_fresh += inlet->GetNumReadsThatWereFresh();
          accum.net_flux_through_duct += inlet->GetNetFluxThroughDuct();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static double GetRoundTripTouchesPerAttemptedPull() {
      struct Adder {
        size_t num_round_trip_touches{};
        size_t num_attempted_pulls{};
        double GetRatio() const {
          return num_round_trip_touches
            / static_cast<double>( num_attempted_pulls );
        }
      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), Adder{},
        [](Adder accum, const this_t* inlet) {
          accum.num_round_trip_touches += inlet->GetCurRoundTripTouchCount();
          accum.num_attempted_pulls += inlet->GetNumPullsAttempted();
          return accum;
        },
        Filter{}
      ).GetRatio();
    }

    static size_t GetNumOutlets() {
      std::shared_lock lock{ registry.GetMutex() };
      return std::count_if(
        std::begin(registry), std::end(registry),
        Filter{}
      );
    }

    static double GetMeanFractionTryPullsThatWereLaden() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumTryPullsThatWereLaden()
            / static_cast<double>( outlet->GetNumTryPullsAttempted() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }


    static double GetMeanFractionTryPullsThatWereUnladen() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumTryPullsThatWereUnladen()
            / static_cast<double>( outlet->GetNumTryPullsAttempted() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanFractionBlockingPullsThatBlocked() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumBlockingPullsThatBlocked()
            / static_cast<double>( outlet->GetNumBlockingPulls() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanFractionBlockingPullsThatWereLadenImmediately() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumBlockingPullsThatWereLadenImmediately()
            / static_cast<double>( outlet->GetNumBlockingPulls() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanFractionBlockingPullsThatWereLadenEventually() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumBlockingPullsThatWereLadenEventually()
            / static_cast<double>( outlet->GetNumBlockingPulls() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanFractionPullsThatWereLadenImmediately() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumPullsThatWereLadenImmediately()
            / static_cast<double>( outlet->GetNumPullsAttempted() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanFractionPullsThatWereLadenEventually() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumPullsThatWereLadenEventually()
            / static_cast<double>( outlet->GetNumPullsAttempted() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanFractionReadsThatWereFresh() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumReadsThatWereFresh()
            / static_cast<double>( outlet->GetNumReadsPerformed() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanFractionReadsThatWereStale() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumReadsThatWereStale()
            / static_cast<double>( outlet->GetNumReadsPerformed() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanFractionRevisionsThatWereRead() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumReadsThatWereFresh()
            / static_cast<double>( outlet->GetNumRevisionsPulled() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanFractionRevisionsThatWereNotRead() {
      return 1.0 - GetMeanFractionRevisionsThatWereRead();
    }

    static double GetMeanFractionDuctFluxThatWasSteppedThrough() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumRevisionsPulled()
            / static_cast<double>( outlet->GetNetFluxThroughDuct() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanFractionDuctFluxThatWasJumpedOver() {
      return 1.0 - GetMeanFractionDuctFluxThatWasSteppedThrough();
    }

    static double GetMeanFractionDuctFluxThatWasRead() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetNumReadsThatWereFresh()
            / static_cast<double>( outlet->GetNetFluxThroughDuct() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    static double GetMeanRoundTripTouchesPerAttemptedPull() {
      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), double{},
        [](size_t accum, const this_t* outlet) {
          return accum + (
            outlet->GetCurRoundTripTouchCount()
            / static_cast<double>( outlet->GetNumPullsAttempted() )
          );
        },
        Filter{}
      ) / GetNumOutlets();
    }

    // capture these from each outlet as close together as possible
    static std::string JointGet_NumPullsAttempted_NumRoundTripTouches() {
      struct JointAdder {
        size_t num_pulls_attempted{};
        size_t num_round_trip_touches{};

        std::string pack() const {
          return uit_emp::to_string(
            num_pulls_attempted, ',', num_round_trip_touches
          );
        }

      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), JointAdder{},
        [](auto accum, const this_t* outlet) {
          accum.num_pulls_attempted += outlet->GetNumPullsAttempted();
          accum.num_round_trip_touches += outlet->GetCurRoundTripTouchCount();
          return accum;
        },
        Filter{}
      ).pack();
    }

    // capture these from each outlet as close together as possible
    static std::string JointGet_NetFluxThroughDuct_NumTryPullsAttempted_NumTryPullsThatWereLaden() {
      struct JointAdder {
        size_t net_flux_through_duct{};
        size_t num_try_pulls_attempted{};
        size_t num_try_pulls_that_were_laden{};

        std::string pack() const {
          return uit_emp::to_string(
            net_flux_through_duct,
            ',', num_try_pulls_attempted,
            ',', num_try_pulls_that_were_laden
          );
        }

      };

      std::shared_lock lock{ registry.GetMutex() };
      return uitsl::accumulate_if(
        std::begin(registry), std::end(registry), JointAdder{},
        [](auto accum, const this_t* outlet) {
          accum.net_flux_through_duct += outlet->GetNetFluxThroughDuct();
          accum.num_try_pulls_attempted += outlet->GetNumTryPullsAttempted();
          accum.num_try_pulls_that_were_laden
            += outlet->GetNumTryPullsThatWereLaden();
          return accum;
        },
        Filter{}
      ).pack();
    }

    static uit_emp::DataFile MakeSummaryDataFile(const std::string& filename) {
      uit_emp::DataFile res( filename );
      res.AddFun(
        [](){
          return std::chrono::time_point_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now()
          ).time_since_epoch().count();
        },
        "Row Initial Timepoint (ns)"
      );
      res.AddVal(uitsl::get_proc_id(), "proc");
      res.AddVal(Filter::name(), "Impl Filter");
      res.AddFun(GetNumOutlets, "Num Outlets");
      res.AddFun(GetNumReadsPerformed, "Num Reads Performed");
      res.AddFun(GetNumReadsThatWereFresh, "Num Reads That Were Fresh");
      res.AddFun(GetNumReadsThatWereStale, "Num Reads That Were Stale");
      res.AddFun(GetNumRevisionsPulled, "Num Revisions Pulled");
      res.AddFun(
        JointGet_NetFluxThroughDuct_NumTryPullsAttempted_NumTryPullsThatWereLaden,
        "Net Flux Through Duct,Num Try Pulls Attempted,Num Try Pulls That Were Laden"
      );
      res.AddFun(GetNumBlockingPulls, "Num Blocking Pulls");
      res.AddFun(
        GetNumBlockingPullsThatBlocked, "Num Blocking Pulls That Blocked"
      );
      res.AddFun(
        GetNumRevisionsFromTryPulls, "Num Revisions From Try Pulls"
      );
      res.AddFun(
        GetNumRevisionsFromBlockingPulls, "Num Revisions From Blocking Pulls"
      );
      res.AddFun(
        JointGet_NumPullsAttempted_NumRoundTripTouches,
        "Num Pulls Attempted,Num Round Trip Touches"
      );
      res.AddFun(
        GetNumPullsThatWereLadenEventually,
        "Num Pulls That Were Laden Eventually"
      );
      res.AddFun(
        GetNumBlockingPullsThatWereLadenImmediately,
        "Num Blocking Pulls That Were Laden Immediately"
      );
      res.AddFun(
        GetNumBlockingPullsThatWereLadenEventually,
        "Num Blocking Pulls That Were Laden Eventually"
      );
      res.AddFun(
        GetNumPullsThatWereLadenImmediately,
        "Num Pulls That Were Laden Immediately"
      );
      res.AddFun(
        GetNumTryPullsThatWereUnladen, "Num Try Pulls That Were Unladen"
      );
      res.AddFun(
        GetFractionTryPullsThatWereLaden, "Fraction Try Pulls That Were Laden"
      );
      res.AddFun(
        GetFractionTryPullsThatWereUnladen,
        "Fraction Try Pulls That Were Unladen"
      );
      res.AddFun(
        GetFractionBlockingPullsThatBlocked,
        "Fraction Blocking Pulls That Blocked"
      );
      res.AddFun(
        GetFractionBlockingPullsThatWereLadenImmediately,
        "Fraction Blocking Pulls That Were Laden Immediately"
      );
      res.AddFun(
        GetFractionBlockingPullsThatWereLadenEventually,
        "Fraction Blocking Pulls That Were Laden Eventually"
      );
      res.AddFun(
        GetFractionPullsThatWereLadenImmediately,
        "Fraction Pulls That Were Laden Immediately"
      );
      res.AddFun(
        GetFractionPullsThatWereLadenEventually,
        "Fraction Pulls That Were Laden Eventually"
      );
      res.AddFun(
        GetFractionReadsThatWereFresh, "Fraction Reads That Were Fresh"
      );
      res.AddFun(
        GetFractionReadsThatWereStale,
        "Fraction Reads That Were Stale"
      );
      res.AddFun(
        GetFractionRevisionsThatWereRead,
        "Fraction Revisions That Were Read"
      );
      res.AddFun(
        GetFractionRevisionsThatWereNotRead,
        "Fraction Revisions That Were Not Read"
      );
      res.AddFun(
        GetFractionDuctFluxThatWasSteppedThrough,
        "Fraction Duct Flux That Was Stepped Through"
      );
      res.AddFun(
        GetFractionDuctFluxThatWasJumpedOver,
        "Fraction Duct Flux That Was Jumped Over"
      );
      res.AddFun(
        GetFractionDuctFluxThatWasRead,
        "Fraction Duct Flux That Was Read"
      );
      res.AddFun(
        GetRoundTripTouchesPerAttemptedPull,
        "Round Trip Touches Per Attempted Pull"
      );
      res.AddFun(
        GetMeanFractionTryPullsThatWereLaden,
        "Mean Fraction Try Pulls That Were Laden"
      );
      res.AddFun(
        GetMeanFractionTryPullsThatWereUnladen,
        "Mean Fraction Try Pulls That Were Unladen"
      );
      res.AddFun(
        GetMeanFractionBlockingPullsThatBlocked,
        "Mean Fraction Blocking Pulls That Blocked"
      );
      res.AddFun(
        GetMeanFractionBlockingPullsThatWereLadenImmediately,
        "Mean Fraction Blocking Pulls That Were Laden Immediately"
      );
      res.AddFun(
        GetMeanFractionBlockingPullsThatWereLadenEventually,
        "Mean Fraction Blocking Pulls That Were Laden Eventually"
      );
      res.AddFun(
        GetMeanFractionPullsThatWereLadenImmediately,
        "Mean Fraction Pulls That Were Laden Immediately"
      );
      res.AddFun(
        GetMeanFractionPullsThatWereLadenEventually,
        "Mean Fraction Pulls That Were Laden Eventually"
      );
      res.AddFun(
        GetMeanFractionReadsThatWereFresh, "Mean Fraction Reads That Were Fresh"
      );
      res.AddFun(
        GetMeanFractionReadsThatWereStale,
        "Mean Fraction Reads That Were Stale"
      );
      res.AddFun(
        GetMeanFractionRevisionsThatWereRead,
        "Mean Fraction Revisions That Were Read"
      );
      res.AddFun(
        GetMeanFractionRevisionsThatWereNotRead,
        "Mean Fraction Revisions That Were Not Read"
      );
      res.AddFun(
        GetMeanFractionDuctFluxThatWasSteppedThrough,
        "Mean Fraction Duct Flux That Was Stepped Through"
      );
      res.AddFun(
        GetMeanFractionDuctFluxThatWasJumpedOver,
        "Mean Fraction Duct Flux That Was Jumped Over"
      );
      res.AddFun(
        GetMeanFractionDuctFluxThatWasRead,
        "Mean Fraction Duct Flux That Was Read"
      );
      res.AddFun(
        GetMeanRoundTripTouchesPerAttemptedPull,
        "Mean Round Trip Touches Per Attempted Pull"
      );
      res.AddFun(
        [](){ return uitsl::coarse_runtime<>.GetElapsed().count(); },
        "Runtime Seconds Elapsed"
      );
      res.AddFun(
        [](){
          return std::chrono::time_point_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now()
          ).time_since_epoch().count();
        },
        "Row Final Timepoint (ns)"
      );
      return res;
    }

    static auto MakeContainerDataFile(const std::string& filename) {

      auto res = uit_emp::MakeContainerDataFile<decltype(&registry)>(
        [](){ return &registry; },
        filename
      );
      res.SetFilterContainerFun( Filter{} );
      res.SetLockContainerFun( [](const auto& container_ptr){
        using mutex_t = std::decay_t<decltype(container_ptr->GetMutex())>;
        using lock_t = std::shared_lock<mutex_t>;
        return std::make_shared<lock_t>(
          container_ptr->GetMutex()
        );
      } );
      res.AddFun(
        [](){
          return std::chrono::time_point_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now()
          ).time_since_epoch().count();
        },
        "Row Initial Timepoint (ns)"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){ return outlet_ptr->WhichImplHeld(); },
        "Which Impl"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){ return outlet_ptr->GetNumReadsPerformed(); },
        "Num Reads Performed"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumReadsThatWereFresh();
        },
        "Num Reads That Were Fresh"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumReadsThatWereStale();
        },
        "Num Reads That Were Stale"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumRevisionsPulled();
        },
        "Num Revisions Pulled"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumTryPullsAttempted();
        },
        "Num Try Pulls Attempted"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){ return outlet_ptr->GetNumBlockingPulls(); },
        "Num Blocking Pulls"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumBlockingPullsThatBlocked();
        },
        "Num Blocking Pulls That Blocked"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumRevisionsFromTryPulls();
        },
        "Num Revisions From Try Pulls"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumRevisionsFromBlockingPulls();
        },
        "Num Revisions From Blocking Pulls"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumPullsAttempted();
        },
        "Num Pulls Attempted"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumPullsThatWereLadenEventually();
        },
        "Num Pulls That Were Laden Eventually"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumBlockingPullsThatWereLadenImmediately();
        },
        "Num Blocking Pulls That Were Laden Immediately"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumBlockingPullsThatWereLadenEventually();
        },
        "Num Blocking Pulls That Were Laden Eventually"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumPullsThatWereLadenImmediately();
        },
        "Num Pulls That Were Laden Immediately"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){ return outlet_ptr->GetNumTryPullsThatWereLaden(); },
        "Num Try Pulls That Were Laden"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNumTryPullsThatWereUnladen();
        },
        "Num Try Pulls That Were Unladen"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionTryPullsThatWereLaden();
        },
        "Fraction Try Pulls That Were Laden"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionTryPullsThatWereUnladen();
        },
        "Fraction Try Pulls That Were Unladen"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionBlockingPullsThatBlocked();
        },
        "Fraction Blocking Pulls That Blocked "
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionBlockingPullsThatWereLadenImmediately();
        },
        "Fraction Blocking Pulls That Were Laden Immediately"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionPullsThatWereLadenImmediately();
        },
        "Fraction Pulls That Were Laden Immediately"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionPullsThatWereLadenEventually();
        },
        "Fraction Pulls That Were Laden Eventually"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetNetFluxThroughDuct();
        },
        "Net Flux Through Duct "
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionReadsThatWereFresh();
        },
        "Fraction Reads That Were Fresh "
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionReadsThatWereStale();
        },
        "Fraction Reads That Were Stale "
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionRevisionsThatWereRead();
        },
        "Fraction Revisions That Were Read "
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionRevisionsThatWereNotRead();
        },
        "Fraction Revisions That Were Not Read"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionDuctFluxThatWasSteppedThrough();
        },
        "Fraction Duct Flux That Was Stepped Through"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionDuctFluxThatWasJumpedOver();
        },
        "Fraction Duct Flux That Was Jumped Over"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetFractionDuctFluxThatWasRead();
        },
        "Fraction Duct Flux That Was Read"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetCurRoundTripTouchCount() / static_cast<double>(
            outlet_ptr->GetNumPullsAttempted()
          );
        },
        "Round Trip Touches Per Attempted Pull"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          return outlet_ptr->GetCurRoundTripTouchCount();
        },
        "Num Round Trip Touches"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){ return outlet_ptr->WhichImplHeld(); },
        "Held Impl"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          const auto res = outlet_ptr->LookupInletProc();
          return res.has_value() ? uit_emp::to_string(*res) : "null";
        }, "Inlet Proc"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          const auto res = outlet_ptr->LookupOutletProc();
          return res.has_value() ? uit_emp::to_string(*res) : "null";
        }, "Outlet Proc"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          const auto res = outlet_ptr->LookupInletThread();
          return res.has_value() ? uit_emp::to_string(*res) : "null";
        }, "Inlet Thread"
      );
      res.AddContainerFun(
        [](const auto outlet_ptr){
          const auto res = outlet_ptr->LookupOutletThread();
          return res.has_value() ? uit_emp::to_string(*res) : "null";
        }, "Outlet Thread"
      );
      res.AddFun(
        [](){ return uitsl::runtime<>.GetElapsed().count(); },
        "Runtime Seconds Elapsed"
      );
      res.AddFun(
        [](){
          return std::chrono::time_point_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now()
          ).time_since_epoch().count();
        },
        "Row Final Timepoint (ns)"
      );
      return res;
    }

  };

  struct AllFilter {
    bool operator()( const this_t* ) const { return true; }
    static std::string name() { return "all"; }
  };

  struct IntraFilter {
    bool operator()( const this_t* outlet_ptr ) const {
      return outlet_ptr->HoldsIntraImpl().value();
    }
    static std::string_view name() { return "intra"; }
  };

  struct ThreadFilter {
    bool operator()( const this_t* outlet_ptr ) const {
      return outlet_ptr->HoldsThreadImpl().value();
    }
    static std::string_view name() { return "thread"; }
  };

  struct ProcFilter {
    bool operator()( const this_t* outlet_ptr ) const {
      return outlet_ptr->HoldsProcImpl().value();
    }
    static std::string_view name() { return "proc"; }
  };

  using touch_count_address_cache_t = std::optional<
    uit::impl::round_trip_touch_addr_t
  >;
  mutable touch_count_address_cache_t touch_count_address_cache{ std::nullopt };

  void DoRefreshTouchCountAddressCache() const {
    touch_count_address_cache = uit::impl::round_trip_touch_addr_t{
      *LookupMeshID(), *LookupOutletNodeID(), *LookupInletNodeID()
    };
  }

  void RefreshTouchCountAddressCacheIfNecesssary() const {
    if ( !touch_count_address_cache.has_value() )
      DoRefreshTouchCountAddressCache();
  }

  decltype(auto) GetTouchCountAddr() const {
    RefreshTouchCountAddressCacheIfNecesssary();
    return *touch_count_address_cache;
  }

  size_t GetCurRoundTripTouchCount() const {
    assert(
      uit::impl::round_trip_touch_counter.count( GetTouchCountAddr() ) &&
      "round_trip_touch_counter should be initialized during Mesh construction."
    );
    return uit::impl::round_trip_touch_counter.at( GetTouchCountAddr() );
  }

  void ProgressRoundTripTouchCount() const {
    assert(
      uit::impl::round_trip_touch_counter.count( GetTouchCountAddr() ) &&
      "round_trip_touch_counter should be initialized during Mesh construction."
    );
    uit::impl::round_trip_touch_counter.at(
      GetTouchCountAddr()
    ) = std::max( outlet.Get().round_trip_count , GetCurRoundTripTouchCount() );
  }

public:

  using all = RegistryAggregator<AllFilter>;
  using intra = RegistryAggregator<IntraFilter>;
  using thread = RegistryAggregator<ThreadFilter>;
  using proc = RegistryAggregator<ProcFilter>;

  /**
   * Copy constructor.
   */
  InstrumentationAggregatingOutletWrapper(
    InstrumentationAggregatingOutletWrapper& other
  ) : outlet( other.outlet ) {
    assert( !registry.contains(this) );
    registry.insert(this);
  }

  /**
   * Copy constructor.
   */
  InstrumentationAggregatingOutletWrapper(
    const InstrumentationAggregatingOutletWrapper& other
  ) : outlet( other.outlet ) {
    assert( !registry.contains(this) );
    registry.insert(this);
  };

  /**
   * Move constructor.
   */
  InstrumentationAggregatingOutletWrapper(
    InstrumentationAggregatingOutletWrapper&& other
  ) : outlet( std::move(other.outlet) ) {
    assert( !registry.contains(this) );
    registry.insert(this);
  };

  /**
   * Forwarding constructor.
   */
  template <typename... Args>
  explicit InstrumentationAggregatingOutletWrapper(Args&&... args)
  : outlet(std::forward<Args>(args)...) {
    assert( !registry.contains(this) );
    registry.insert(this);
  }

  ~InstrumentationAggregatingOutletWrapper() {
    [[maybe_unused]] const size_t res = registry.erase( this );
    assert( res == 1 );
  }

  size_t TryStep(const size_t num_steps) {
    const size_t res = outlet.TryStep( num_steps );
    ProgressRoundTripTouchCount();
    return res;
  }

  decltype(auto) Jump() {
    const size_t res = outlet.Jump();
    ProgressRoundTripTouchCount();
    return res;
  }

  const value_type& Get() const { return outlet.Get().data; }

  value_type& Get() { return outlet.Get().data; }

  decltype(auto) JumpGet() {
    Jump();
    return Get();
  }

  void Step(const size_t num_steps=1) {
    outlet.Step(num_steps);
    ProgressRoundTripTouchCount();
  }

  decltype(auto) GetNext() {
    Step();
    return Get();
  }

  using optional_ref_t = std::optional<
    std::reference_wrapper<const value_type>
  >;

  optional_ref_t GetNextOrNullopt() {
    return TryStep()
      ? optional_ref_t{ std::reference_wrapper{ Get() } }
      : std::nullopt;
  }

  decltype(auto) GetNumReadsPerformed() const {
    return outlet.GetNumReadsPerformed();
  }

  decltype(auto) GetNumReadsThatWereFresh() const {
    return outlet.GetNumReadsThatWereFresh();
  }

  decltype(auto) GetNumReadsThatWereStale() const {
    return outlet.GetNumReadsThatWereStale();
  }

  decltype(auto) GetNumRevisionsPulled() const {
    return outlet.GetNumRevisionsPulled();
  }

  decltype(auto) GetNumTryPullsAttempted() const {
    return outlet.GetNumTryPullsAttempted();
  }

  decltype(auto) GetNumBlockingPulls() const {
    return outlet.GetNumBlockingPulls();
  }

  decltype(auto) GetNumBlockingPullsThatBlocked() const {
    return outlet.GetNumBlockingPullsThatBlocked();
  }

  decltype(auto) GetNumRevisionsFromTryPulls() const {
    return outlet.GetNumRevisionsFromTryPulls();
  }

  decltype(auto) GetNumRevisionsFromBlockingPulls() const {
    return outlet.GetNumRevisionsFromBlockingPulls();
  }

  decltype(auto) GetNumPullsAttempted() const {
    return outlet.GetNumPullsAttempted();
  }

  decltype(auto) GetNumPullsThatWereLadenEventually() const {
    return outlet.GetNumPullsThatWereLadenEventually();
  }

  decltype(auto) GetNumBlockingPullsThatWereLadenImmediately() const {
    return outlet.GetNumBlockingPullsThatWereLadenImmediately();
  }

  decltype(auto) GetNumBlockingPullsThatWereLadenEventually() const {
    return outlet.GetNumBlockingPullsThatWereLadenEventually();
  }

  decltype(auto) GetNumPullsThatWereLadenImmediately() const {
    return outlet.GetNumPullsThatWereLadenImmediately();
  }

  decltype(auto) GetNumTryPullsThatWereLaden() const {
    return outlet.GetNumTryPullsThatWereLaden();
  }

  decltype(auto) GetNumTryPullsThatWereUnladen() const {
    return outlet.GetNumTryPullsThatWereUnladen();
  }

  decltype(auto) GetFractionTryPullsThatWereLaden() const {
    return outlet.GetFractionTryPullsThatWereLaden();
  }

  decltype(auto) GetFractionTryPullsThatWereUnladen() const {
    return outlet.GetFractionTryPullsThatWereUnladen();
  }

  decltype(auto) GetFractionBlockingPullsThatBlocked() const {
    return outlet.GetFractionBlockingPullsThatBlocked();
  }

  decltype(auto) GetFractionBlockingPullsThatWereLadenImmediately() const {
    return outlet.GetFractionBlockingPullsThatWereLadenImmediately();
  }

  decltype(auto) GetFractionPullsThatWereLadenImmediately() const {
    return outlet.GetFractionPullsThatWereLadenImmediately();
  }

  decltype(auto) GetFractionPullsThatWereLadenEventually() const {
    return outlet.GetFractionPullsThatWereLadenEventually();
  }

  decltype(auto) GetNetFluxThroughDuct() const {
    return outlet.GetNetFluxThroughDuct();
  }

  decltype(auto) GetFractionReadsThatWereFresh() const {
    return outlet.GetFractionReadsThatWereFresh();
  }

  decltype(auto) GetFractionReadsThatWereStale() const {
    return outlet.GetFractionReadsThatWereStale();
  }

  decltype(auto) GetFractionRevisionsThatWereRead() const {
    return outlet.GetFractionRevisionsThatWereRead();
  }

  decltype(auto) GetFractionRevisionsThatWereNotRead() const {
    return outlet.GetFractionRevisionsThatWereNotRead();
  }

  decltype(auto) GetFractionDuctFluxThatWasSteppedThrough() const {
    return outlet.GetFractionDuctFluxThatWasSteppedThrough();
  }

  decltype(auto) GetFractionDuctFluxThatWasJumpedOver() const {
    return outlet.GetFractionDuctFluxThatWasJumpedOver();
  }

  decltype(auto) GetFractionDuctFluxThatWasRead() const {
    return outlet.GetFractionDuctFluxThatWasRead();
  }

  template <typename WhichDuct, typename... Args>
  void EmplaceDuct(Args&&... args) {
    touch_count_address_cache.reset();
    outlet.template EmplaceDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  template <typename WhichDuct, typename... Args>
  void SplitDuct(Args&&... args) {
    touch_count_address_cache.reset();
    outlet.template SplitDuct<WhichDuct>( std::forward<Args>(args)... );
  }

  decltype(auto) GetDuctUID() const { return outlet.GetUID(); }

  decltype(auto) HoldsIntraImpl() const { return outlet.HoldsIntraImpl(); }

  decltype(auto) HoldsThreadImpl() const { return outlet.HoldsThreadImpl(); }

  decltype(auto) HoldsProcImpl() const { return outlet.HoldsProcImpl(); }

  decltype(auto) WhichImplHeld() const { return outlet.WhichImplHeld(); }

  decltype(auto) CanStep() const { return outlet.CanStep(); }

  void RegisterInletProc(const uitsl::proc_id_t proc) const {
    outlet.RegisterInletProc(proc);
  }

  void RegisterInletThread(const uitsl::thread_id_t thread) const {
    outlet.RegisterInletThread(thread);
  }

  void RegisterOutletProc(const uitsl::proc_id_t proc) const {
    outlet.RegisterOutletProc(proc);
  }

  void RegisterOutletThread(const uitsl::thread_id_t thread) const {
    outlet.RegisterOutletThread(thread);
  }

  void RegisterEdgeID(const size_t edge_id) const {
    outlet.RegisterEdgeID(edge_id);
  }

  void RegisterInletNodeID(const size_t node_id) const {
    touch_count_address_cache.reset();
    outlet.RegisterInletNodeID(node_id);
  }

  void RegisterOutletNodeID(const size_t node_id) const {
    touch_count_address_cache.reset();
    outlet.RegisterOutletNodeID(node_id);
  }

  void RegisterMeshID(const size_t mesh_id) const {
    touch_count_address_cache.reset();
    outlet.RegisterMeshID(mesh_id);
  }

  decltype(auto) LookupOutletProc() const { return outlet.LookupOutletProc(); }

  decltype(auto) LookupOutletThread() const {
    return outlet.LookupOutletThread();
  }

  decltype(auto) LookupInletProc() const { return outlet.LookupInletProc(); }

  decltype(auto) LookupInletThread() const {
    return outlet.LookupInletThread();
  }

  decltype(auto) LookupEdgeID() const { return outlet.LookupEdgeID(); }

  decltype(auto) LookupInletNodeID() const {
    return outlet.LookupInletNodeID();
  }

  decltype(auto) LookupOutletNodeID() const {
    return outlet.LookupOutletNodeID();
  }

  decltype(auto) LookupMeshID() const { return outlet.LookupMeshID(); }

};

} // namespace internal
} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_OUTLET_INSTRUMENTATIONAGGREGATINGOUTLETWRAPPER_HPP_INCLUDE
