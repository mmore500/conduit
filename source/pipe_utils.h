#pragma once

#include <iostream>
#include <utility>
#include <memory>
#include <atomic>

#include "Duct.h"
#include "Inlet.h"
#include "Outlet.h"

template<typename T, size_t N=1024>
std::pair<Inlet<T,N>, Outlet<T,N>> make_pipe() {

  std::tuple<std::shared_ptr<Duct<T, N>>> args{std::make_shared<Duct<T, N>>()};
  return std::pair<Inlet<T,N>, Outlet<T,N>>(
    std::piecewise_construct_t{},
    args,
    args
  );

}
