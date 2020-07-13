#pragma once

#include <assert.h>
#include <sstream>

#include "Inlet.h"
#include "Outlet.h"
#include "print_utils.h"

class Tile {

  Outlet<char> input;
  Inlet<char> output;

  char state;

  char Transition(const char& neighbor_state) {

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
        error_message_mutex.lock();
        std::cerr
          << format_member("neighbor_state", neighbor_state)
          << std::endl;
        std::cerr << format_member("prev Tile", *prev) << std::endl;
        std::cerr << format_member("this Tile", *this) << std::endl;
        std::cerr << format_member("next Tile", *next) << std::endl;
        assert(false);
    }

  }

  void DoSetState(const char& state_) {state = state_;}

  void FlushState() {
    output.MaybePut(state);
  }

public:
  Tile(Outlet<char> input_, Inlet<char> output_)
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

  const char& GetState() const { return state; }

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
    ss << format_member("id", id) << std::endl;
    ss << format_member("char state", state) << std::endl;
    ss << format_member("Outlet<char> input", input) << std::endl;
    ss << format_member("Inlet<char> output", output);
    return ss.str();
  }

};
