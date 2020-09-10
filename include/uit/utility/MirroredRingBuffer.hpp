#pragma once

#include <unistd.h>
#include <sys/mman.h>

#include "../../../third-party/Empirical/source/polyfill/span.h"

#include "../debug/err_audit.hpp"

#include "divide_utils.hpp"
#include "numeric_cast.hpp"

// adapted from https://medium.com/@abhinavagarwal1996/a-fast-circular-ring-buffer-4d102ef4d4a3
// see also https://lo.calho.st/posts/black-magic-buffer/

// TODO fix undefined behavior

namespace uit {

template<typename T, size_t N>
class MirroredRingBuffer {

  const size_t byte_size { uit::numeric_cast<size_t>(
    uit::div_ceil(N * sizeof(T), getpagesize()) * getpagesize()
  ) };
  const size_t allocation_size { 2 * byte_size };
  const int fd { fileno( tmpfile() ) };
  std::byte *buffer;

  std::byte *head;
  std::byte *tail;

public:

  MirroredRingBuffer() {

    emp_assert( byte_size % getpagesize() == 0 );

    uit::err_audit(ftruncate(
      fd, // int fd
      byte_size // off_t length
    ));

    // get virtual address space of 2 * byte_size for our buffer
    buffer = reinterpret_cast<std::byte*>( mmap(
      nullptr, // void *addr
      allocation_size, // size_t length
      PROT_NONE, // int prot
      MAP_PRIVATE | MAP_ANONYMOUS, // int flags
      -1, // int fd
      0 // off_t offset
    ) );

    emp_assert( buffer != MAP_FAILED ); emp_assert( buffer != nullptr );

    // map front half of our buffer to underlying buffer
    { const auto res = mmap(
      buffer, // void *addr
      byte_size, // size_t length
      PROT_READ | PROT_WRITE, // int prot
      MAP_SHARED | MAP_FIXED, // int flags
      fd, // int fd
      0 // off_t offset
    ); emp_assert( res != MAP_FAILED ); }

    // map back half of our buffer to underlying buffer
    { const auto res = mmap(
      buffer + byte_size, // void *addr
      byte_size, // size_t length
      PROT_READ | PROT_WRITE, // int prot
      MAP_SHARED | MAP_FIXED, // int flags
      fd, // int fd
      0 // off_t offset
    ); emp_assert( res != MAP_FAILED ); }

    // point head and tail to beginning of buffer
    head = buffer;
    tail = buffer;

  }

  ~MirroredRingBuffer() {
    uit::err_audit(munmap(
      buffer, // void *addr
      allocation_size // size_t length
    ));
  }

  std::byte* step_pointer(std::byte* curr) const {
    curr += sizeof(T);
    // wraparound if off the end
    curr -= (std::distance(buffer, curr) / byte_size) * byte_size;
    return curr;
  }

  size_t GetSize() const {
    return head >= tail
      ? std::distance(tail, head) / sizeof(T)
      : (byte_size - std::distance(head, tail)) / sizeof(T); // wraparound case
  }

  bool Put(const T& t) {
    if (GetSize() == N) return false;
    else {
      std::memcpy(head, &t, sizeof(T));
      head = step_pointer(head);
      return true;
    }
  }

  bool Pop() {
    if (GetSize()) {
      tail = step_pointer(tail);
      return true;
    } else return false;
  }

  void Fill(const T& t=T{}) { while( Put(t) ); }

  std::byte* GetHeadPtr() { return head; }

  std::byte* GetTailPtr() { return tail; }

  const std::byte* GetHeadPtr() const { return head; }

  const std::byte* GetTailPtr() const { return tail; }

  T GetHead() const {
    emp_assert( GetSize() );
    T res;
    std::memcpy( &res, head, sizeof(T) );
    return res;
  }

  T GetTail() const {
    emp_assert( GetSize() );
    T res;
    std::memcpy( &res, tail, sizeof(T) );
    return res;
  }

  T Get(const size_t i) const {
    emp_assert( i < GetSize() );
    T res;
    std::memcpy( &res, tail + i * sizeof(T), sizeof(T) );
    return res;
  }

};

} // namespace uit
