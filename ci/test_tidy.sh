#!/bin/bash

set -e

./ci/test_headerguards.sh && echo "✔ headerguards ok"
./ci/test_merge_conflict_markers.sh && echo "✔ conflict markers ok"
./ci/test_missing_newlines.sh && echo "✔ EOF newlines ok"
./ci/test_tabs.sh && echo "✔ indentation ok"
./ci/test_trailing_whitespace.sh && echo "✔ trailing whitespace ok"
