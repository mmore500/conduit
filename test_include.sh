#!/bin/bash

shopt -s globstar
for f in include/uit/**/*.hpp include/uitsl/**/*.hpp include/netuit/**/*.hpp; do
  printf "#include \"${f}\"\nint main(){}" | mpicxx -std=c++17 -x c++ - -lmetis &
  printf "#include \"${f}\"\nint main(){}" | mpicxx -std=c++17 -DUIT_VENDORIZE_EMP -x c++ - -lmetis &
  echo "${f}"

  # adapted from https://unix.stackexchange.com/a/436713
  if [[ $(jobs -r -p | wc -l) -ge 4 ]]; then
      # now there are $N jobs already running, so wait here for any job
      # to be finished so there is a place to start next one.
      wait -n
  fi
done
