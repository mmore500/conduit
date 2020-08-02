#pragma once

#include <assert.h>
#include <sstream>

#include "conduit/Inlet.h"
#include "conduit/Outlet.h"
#include "utility/print_utils.h"
#include "State.h"

class Tile {

  uit::Outlet<State> input;
  uit::Inlet<State> output;

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
        uit::error_message_mutex.lock();
        std::cerr
          << uit::format_member("neighbor_state", neighbor_state)
          << std::endl;
        std::cerr << uit::format_member("prev Tile", *prev) << std::endl;
        std::cerr << uit::format_member("this Tile", *this) << std::endl;
        std::cerr << uit::format_member("next Tile", *next) << std::endl;
        throw "bad Transition on Tile";
    }

  }

  void DoSetState(const State& state_) {state = state_;}

  void FlushState() {
    output.MaybePut(state);
  }

public:
  Tile(uit::Outlet<State> input_, uit::Inlet<State> output_)
  : input(input_), output(output_)
  { }

  Tile *next;
  Tile *prev;
  size_t id;

  void Update() {
    const char neighbor_state = input.GetCurrent();
    const char next_state = Transition(neighbor_state);
    SetState(next_state);
  }

  const State& GetState() const { return state; }

  void SetState(const char& state) {
    DoSetState(state);
    FlushState();
  }

  size_t GetSuccessfulWriteCount() const {
    return output.GetSuccessfulWriteCount();
  }

  size_t GetBlockedWriteCount() const { return output.GetBlockedWriteCount(); }

  size_t GetDroppedWriteCount() const { return output.GetDroppedWriteCount(); }

  size_t GetReadCount() const { return input.GetReadCount(); }

  size_t GetReadRevisionCount() const { return input.GetRevisionCount(); }

  size_t GetNetFlux() const { return input.GetNetFlux(); }

  template <typename WhichDuct, typename... Args>
  void EmplaceOutputDuct(Args&&... args) {
    output.EmplaceDuct<WhichDuct>(std::forward<Args>(args)...);
  }

  template <typename WhichDuct, typename... Args>
  void EmplaceInputDuct(Args&&... args) {
    input.EmplaceDuct<WhichDuct>(std::forward<Args>(args)...);
  }

  template <typename WhichDuct, typename... Args>
  void SplitOutputDuct(Args&&... args) {
    output.SplitDuct<WhichDuct>(std::forward<Args>(args)...);
  }

  template <typename WhichDuct, typename... Args>
  void SplitInputDuct(Args&&... args) {
    input.SplitDuct<WhichDuct>(std::forward<Args>(args)...);
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << uit::format_member("id", id) << std::endl;
    ss << uit::format_member("State state", state) << std::endl;
    ss << uit::format_member("Outlet<State> input", input) << std::endl;
    ss << uit::format_member("Inlet<State> output", output);
    return ss.str();
  }

};
