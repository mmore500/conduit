#!/bin/bash

set -e # exit with error if any of this fails

script_dir="$(dirname "$(readlink -f "$0")")"

which_python="$(\
  command -v python3.8 \
  || command -v python3.9 \
  || echo python3 \
)"
echo "which_python ${which_python}"

"${which_python}" -c "import sys; print(sys.version_info)"

"${which_python}" "$(which nosetests)" "${script_dir}"/**/test_*.py
