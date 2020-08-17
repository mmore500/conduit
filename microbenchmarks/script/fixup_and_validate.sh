#!/bin/bash

for f in $(find . -name "*.json"); do
  sed -iv "s/: Infinity,/: null,/g" $f
  jsonlint $f
done
