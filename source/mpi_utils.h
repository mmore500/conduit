#pragma once

#include "mpi.h"
#include "print_utils.h"

// TODO rename pid_t
using proc_id_t = int;

int get_nprocs() {
  int res;
  MPI_Comm_size(
    MPI_COMM_WORLD,
    &res
  );
  return res;
}

//TODO replace with get_proc_id
proc_id_t get_rank() {
  int res;
  MPI_Comm_rank(
    MPI_COMM_WORLD,
    &res
  );
  return res;
}

proc_id_t get_proc_id() {
  return get_rank();
}

bool is_root() { return get_rank() == 0; }

bool is_multiprocess() { return get_nprocs() > 1; }

std::string to_string(const MPI_Status & status) {
  std::stringstream ss;
  ss << format_member(
    "MPI_Get_count",
    [&](){
      int res;
      MPI_Get_count(&status, MPI_BYTE, &res);
      return res;
    }()
  ) << std::endl;
  ss << format_member(
    "MPI_Test_cancelled",
    [&](){
      int res;
      MPI_Test_cancelled(&status, &res);
      return res;
    }()
  ) << std::endl;
  ss << format_member(
    "int MPI_SOURCE",
    (int) status.MPI_SOURCE
  ) << std::endl;
  ss << format_member(
    "int MPI_TAG",
    (int) status.MPI_TAG
  ) << std::endl;
  ss << format_member(
    "int MPI_ERROR",
    (int) status.MPI_ERROR
  ) << std::endl;
  ss << format_member(
    "int u_count",
    (int) status._ucount
  );
  return ss.str();
}

void verify (int err) {

  std::string name;

  switch(err) {

    case MPI_SUCCESS:
      name = "MPI_SUCCESS";
      break;
    case MPI_ERR_BUFFER:
      name = "MPI_ERR_BUFFER";
      break;
    case MPI_ERR_COUNT:
      name = "MPI_ERR_COUNT";
      break;
    case MPI_ERR_TYPE:
      name = "MPI_ERR_TYPE";
      break;
    case MPI_ERR_TAG:
      name = "MPI_ERR_TAG";
      break;
    case MPI_ERR_COMM:
      name = "MPI_ERR_COMM";
      break;
    case MPI_ERR_RANK:
      name = "MPI_ERR_RANK";
      break;
    case MPI_ERR_ROOT:
      name = "MPI_ERR_ROOT";
      break;
    case MPI_ERR_GROUP:
      name = "MPI_ERR_GROUP";
      break;
    case MPI_ERR_OP:
      name = "MPI_ERR_OP";
      break;
    case MPI_ERR_TOPOLOGY:
      name = "MPI_ERR_TOPOLOGY";
      break;
    case MPI_ERR_DIMS:
      name = "MPI_ERR_DIMS";
      break;
    case MPI_ERR_ARG:
      name = "MPI_ERR_ARG";
      break;
    case MPI_ERR_UNKNOWN:
      name = "MPI_ERR_UNKNOWN";
      break;
    case MPI_ERR_TRUNCATE:
      name = "MPI_ERR_TRUNCATE";
      break;
    case MPI_ERR_OTHER:
      name = "MPI_ERR_OTHER";
      break;
    case MPI_ERR_INTERN:
      name = "MPI_ERR_INTERN";
      break;
    case MPI_ERR_IN_STATUS:
      name = "MPI_ERR_IN_STATUS";
      break;
    case MPI_ERR_PENDING:
      name = "MPI_ERR_PENDING";
      break;
    case MPI_ERR_REQUEST:
      name = "MPI_ERR_REQUEST";
      break;
    case MPI_ERR_ACCESS:
      name = "MPI_ERR_ACCESS";
      break;
    case MPI_ERR_AMODE:
      name = "MPI_ERR_AMODE";
      break;
    case MPI_ERR_BAD_FILE:
      name = "MPI_ERR_BAD_FILE";
      break;
    case MPI_ERR_CONVERSION:
      name = "MPI_ERR_CONVERSION";
      break;
    case MPI_ERR_DUP_DATAREP:
      name = "MPI_ERR_DUP_DATAREP";
      break;
    case MPI_ERR_FILE_EXISTS:
      name = "MPI_ERR_FILE_EXISTS";
      break;
    case MPI_ERR_FILE_IN_USE:
      name = "MPI_ERR_FILE_IN_USE";
      break;
    case MPI_ERR_FILE:
      name = "MPI_ERR_FILE";
      break;
    case MPI_ERR_INFO:
      name = "MPI_ERR_INFO";
      break;
    case MPI_ERR_INFO_KEY:
      name = "MPI_ERR_INFO_KEY";
      break;
    case MPI_ERR_INFO_VALUE:
      name = "MPI_ERR_INFO_VALUE";
      break;
    case MPI_ERR_INFO_NOKEY:
      name = "MPI_ERR_INFO_NOKEY";
      break;
    case MPI_ERR_IO:
      name = "MPI_ERR_IO";
      break;
    case MPI_ERR_NAME:
      name = "MPI_ERR_NAME";
      break;
    case MPI_ERR_NO_MEM:
      name = "MPI_ERR_NO_MEM";
      break;
    case MPI_ERR_NOT_SAME:
      name = "MPI_ERR_NOT_SAME";
      break;
    case MPI_ERR_NO_SPACE:
      name = "MPI_ERR_NO_SPACE";
      break;
    case MPI_ERR_NO_SUCH_FILE:
      name = "MPI_ERR_NO_SUCH_FILE";
      break;
    case MPI_ERR_PORT:
      name = "MPI_ERR_PORT";
      break;
    case MPI_ERR_QUOTA:
      name = "MPI_ERR_QUOTA";
      break;
    case MPI_ERR_READ_ONLY:
      name = "MPI_ERR_READ_ONLY";
      break;
    case MPI_ERR_SERVICE:
      name = "MPI_ERR_SERVICE";
      break;
    case MPI_ERR_SPAWN:
      name = "MPI_ERR_SPAWN";
      break;
    case MPI_ERR_UNSUPPORTED_DATAREP:
      name = "MPI_ERR_UNSUPPORTED_DATAREP";
      break;
    case MPI_ERR_UNSUPPORTED_OPERATION:
      name = "MPI_ERR_UNSUPPORTED_OPERATION";
      break;
    case MPI_ERR_WIN:
      name = "MPI_ERR_WIN";
      break;
    case MPI_ERR_BASE:
      name = "MPI_ERR_BASE";
      break;
    case MPI_ERR_LOCKTYPE:
      name = "MPI_ERR_LOCKTYPE";
      break;
    case MPI_ERR_KEYVAL:
      name = "MPI_ERR_KEYVAL";
      break;
    case MPI_ERR_RMA_CONFLICT:
      name = "MPI_ERR_RMA_CONFLICT";
      break;
    case MPI_ERR_RMA_SYNC:
      name = "MPI_ERR_RMA_SYNC";
      break;
    case MPI_ERR_SIZE:
      name = "MPI_ERR_SIZE";
      break;
    case MPI_ERR_DISP:
      name = "MPI_ERR_DISP";
      break;
    case MPI_ERR_ASSERT:
      name = "MPI_ERR_ASSERT";
      break;
    case MPI_ERR_LASTCODE:
      name = "MPI_ERR_LASTCODE";
      break;
    default:
      name = "unknown";

  }

  emp_assert(err == 0, err, name);

}
