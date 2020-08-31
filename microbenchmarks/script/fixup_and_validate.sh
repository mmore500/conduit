#!/bin/bash

for f in $(find . -name "*.json"); do
  sed -i "s/: Infinity,/: null,/g w /dev/stdout" $f | wc -l
  jsonlint $f
done
