#pragma once

#include <tuple>

#include <mpi.h>

#include "../../uitsl/mpi/mpi_utils.hpp"
#include "../../uitsl/parallel/thread_utils.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
class InterProcAddress {

  uit::proc_id_t outlet_proc;
  uit::proc_id_t inlet_proc;
  uit::thread_id_t outlet_thread;
  uit::thread_id_t inlet_thread;
  int tag;
  MPI_Comm comm;

public:

  InterProcAddress(
    const uit::proc_id_t outlet_proc_=0,
    const uit::proc_id_t inlet_proc_=0,
    const uit::thread_id_t outlet_thread_=0,
    const uit::thread_id_t inlet_thread_=0,
    const int tag_=0,
    const MPI_Comm comm_=MPI_COMM_WORLD
  ) : outlet_proc(outlet_proc_)
  , inlet_proc(inlet_proc_)
  , outlet_thread(outlet_thread_)
  , inlet_thread(inlet_thread_)
  , tag(tag_)
  , comm(comm_)
  { ; }

  uit::proc_id_t GetOutletProc() const { return outlet_proc; }

  uit::proc_id_t GetInletProc() const { return inlet_proc; }

  uit::thread_id_t GetOutletThread() const { return outlet_thread; }

  uit::thread_id_t GetInletThread() const { return inlet_thread; }

  int GetTag() const { return tag; }

  MPI_Comm GetComm() const { return comm; }

  bool operator<(const InterProcAddress& other) const {
    return std::make_tuple(
      outlet_proc,
      inlet_proc,
      outlet_thread,
      inlet_thread,
      tag,
      comm
    ) < std::make_tuple(
      other.outlet_proc,
      other.inlet_proc,
      other.outlet_thread,
      other.inlet_thread,
      other.tag,
      other.comm
    );
  }

  bool operator==(const InterProcAddress& other) const {
    return std::make_tuple(
      outlet_proc,
      inlet_proc,
      outlet_thread,
      inlet_thread,
      tag,
      comm
    ) == std::make_tuple(
      other.outlet_proc,
      other.inlet_proc,
      other.outlet_thread,
      other.inlet_thread,
      other.tag,
      other.comm
    );
  }

  bool operator!=(const InterProcAddress& other) const {
    return !operator==(other);
  }

  std::string ToString() const {

    std::stringstream ss;
    ss << format_member("uit::proc_id_t outlet_proc", outlet_proc) << std::endl;
    ss << format_member("uit::proc_id_t inlet_proc", inlet_proc) << std::endl;
    ss << format_member(
      "uit::thread_id_t outlet_thread",
      outlet_thread
    ) << std::endl;
    ss << format_member(
      "uit::thread_id_t inlet_thread",
      inlet_thread
    ) << std::endl;
    ss << format_member("int tag", tag) << std::endl;
    ss << format_member("MPI_Comm comm", uit::comm_to_string(comm));

    return ss.str();
  }

};

} // namespace uit
