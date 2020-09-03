#pragma once

#include <mpi.h>

#include "../distributed/mpi_utils.hpp"

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
  int tag;
  MPI_Comm comm;

public:

  InterProcAddress(
    const uit::proc_id_t outlet_proc_=0,
    const uit::proc_id_t inlet_proc_=0,
    const int tag_=0,
    const MPI_Comm comm_=MPI_COMM_WORLD
  ) : outlet_proc(outlet_proc_)
  , inlet_proc(inlet_proc_)
  , tag(tag_)
  , comm(comm_)
  { ; }

  uit::proc_id_t GetOutletProc() const { return outlet_proc; }

  uit::proc_id_t GetInletProc() const { return inlet_proc; }

  int GetTag() const { return tag; }

  MPI_Comm GetComm() const { return comm; }

  std::string ToString() const {

    std::stringstream ss;
    ss << format_member("uit::proc_id_t outlet_proc", outlet_proc) << std::endl;
    ss << format_member("uit::proc_id_t inlet_proc", inlet_proc) << std::endl;
    ss << format_member("int tag", tag) << std::endl;
    ss << format_member("MPI_Comm comm", uit::comm_to_string(comm));

    return ss.str();
  }

};

} // namespace uit
