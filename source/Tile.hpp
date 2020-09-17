#pragma once

#include <assert.h>
#include <sstream>

#include "uit/conduit/Inlet.hpp"
#include "uit/conduit/Outlet.hpp"
#include "uitsl/utility/print_utils.hpp"

#include "config_utils.hpp"
#include "State.hpp"

class Tile {

  uit::Outlet<Spec> input;
  uit::Inlet<Spec> output;

  State state;

  char Transition(const State& neighbor_state) {

    // sequence a: x -> X -> x
    // sequence b: o -> O -> o
    // sequence c: _ -> _
    switch (neighbor_state) {
      case 'o':
        return 'O';
      case 'O':
        return 'O';
      case 'x':
        return 'X';
      case 'X':
        return 'x';
      case '_':
        return '_';
      default:
        uitsl::error_message_mutex.lock();
        std::cerr
          << uitsl::format_member("neighbor_state", neighbor_state)
          << std::endl;
        std::cerr << uitsl::format_member("prev Tile", *prev) << std::endl;
        std::cerr << uitsl::format_member("this Tile", *this) << std::endl;
        std::cerr << uitsl::format_member("next Tile", *next) << std::endl;
        throw "bad Transition on Tile";
    }

  }

  void DoSetState(const State& state_) {state = state_;}

  void FlushState() {
    output.TryPut(state);
  }

public:
  Tile(uit::Outlet<Spec> input_, uit::Inlet<Spec> output_)
  : input(input_), output(output_)
  { }

  Tile *next;
  Tile *prev;
  size_t id;

  void Update() {
    const char neighbor_state = input.JumpGet();
    const char next_state = Transition(neighbor_state);
    SetState(next_state);
  }

  const State& GetState() const { return state; }

  void SetState(const char& state) {
    DoSetState(state);
    FlushState();
  }

  size_t GetSuccessfulPutCount() const {
    return output.GetSuccessfulPutCount();
  }

  size_t GetBlockedPutCount() const { return output.GetBlockedPutCount(); }

  size_t GetDroppedPutCount() const { return output.GetDroppedPutCount(); }

  size_t GetReadCount() const { return input.GetReadCount(); }

  size_t GetReadRevisionCount() const { return input.GetRevisionCount(); }

  size_t GetNetFlux() const { return input.GetNetFlux(); }

  std::string ToString() const {
    std::stringstream ss;
    ss << uitsl::format_member("id", id) << std::endl;
    ss << uitsl::format_member("State state", state) << std::endl;
    ss << uitsl::format_member("Outlet<Spec> input", input) << std::endl;
    ss << uitsl::format_member("Inlet<Spec> output", output);
    return ss.str();
  }

};
