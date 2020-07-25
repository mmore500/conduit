#pragma once

size_t circular_index(const size_t pos, const size_t len, const int diff) {
  return (pos + len + diff%len) % len;
}
