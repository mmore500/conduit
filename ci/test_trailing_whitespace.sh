#!/bin/bash

if [ -z "$( ./ci/find_trailing_whitespace.sh )" ];
then
  exit 0 # success
else
  exit 1 # failure
fi
