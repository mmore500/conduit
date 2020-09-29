#!/bin/bash

find . \( -name third-party -o -name .git -o -path './docs/assets/*' \) -prune -o -type f -exec egrep -l " +$" {} \;
