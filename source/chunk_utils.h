#pragma once

#include "grid_utils.h"

#include "ThreadDuct.h"
#include "Tile.h"
#include "State.h"

using grid_t = std::vector<Tile>;
using handle_t = grid_t::iterator;
using chunk_t = std::vector<handle_t>;


chunk_t make_chunk(handle_t begin, handle_t end, const size_t num_chunks) {

  chunk_t res(std::distance(begin, end));
  std::iota(
    std::begin(res),
    std::end(res),
    begin
  );

  return res;

}

chunk_t checkout_chunk(chunk_t & source) {

  auto copy = new grid_t;

  std::transform(
    std::begin(source),
    std::end(source),
    std::back_inserter(*copy),
    [](const auto & handle){ return *handle; }
  );

  chunk_t res(copy->size());
  std::iota(
    std::begin(res),
    std::end(res),
    std::begin(*copy)
  );

  return res;

}

void checkin_chunk(chunk_t & source, const chunk_t & checkedout) {

  emp_assert(source.size() == checkedout.size());

  for (size_t i = 0; i < source.size(); ++i) {
    *source[i] = *checkedout[i];
  }
}


std::vector<chunk_t> make_chunks(grid_t & grid, const size_t num_chunks) {

  const size_t chunk_width = grid.size() / num_chunks;

  assert(grid.size() % num_chunks == 0);

  std::vector<chunk_t> res;

  for (
    auto [chunk_begin, chunk_end] = std::tuple{std::begin(grid), handle_t{}};
    chunk_begin != std::end(grid);
    chunk_begin = chunk_end
  ) {

    chunk_end = (
      std::distance(chunk_end, std::end(grid)) < chunk_width
      ? std::end(grid)
      : std::next(chunk_begin, chunk_width)
    );

    res.push_back(
      make_chunk(chunk_begin, chunk_end, num_chunks)
    );

  }

  assert(res.size() == num_chunks);

  return res;
}

void update_chunk(
  chunk_t & chunk,
  const bool verbose,
  const bool shuffle,
  const size_t resistance
) {


  if (shuffle) std::random_shuffle(
    std::begin(chunk),
    std::end(chunk)
  );

  thread_local std::uniform_int_distribution<size_t> distribution{
    0,
    std::numeric_limits<size_t>::max()
  };
  thread_local std::mt19937 random_number_engine;
  thread_local auto dice_roller = std::bind(distribution, random_number_engine);

  for (auto & handle : chunk) {

    for (size_t i = 0; i < resistance; ++i) {
      if (dice_roller() == 0) std::cout << "do not optimize" << std::endl;
    }

    if (verbose) std::cout << handle->GetState();
    handle->Update();
  }
  if (verbose) std::cout << std::endl;


}
