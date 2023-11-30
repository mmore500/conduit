#!/bin/bash

################################################################################
echo
echo "running execute_notebooks.sh"
echo "---------------------------------------------"
################################################################################

# fail on error
set -e

################################################################################
echo
echo "other initialization"
echo "--------------------"
################################################################################

[[ -f ~/.secrets.sh ]] && source ~/.secrets.sh || echo "~/secrets.sh not found"

# adapted from https://stackoverflow.com/a/24114056
script_dir="$(dirname -- "$BASH_SOURCE")"
echo "script_dir ${script_dir}"

################################################################################
echo
echo "execute notebooks in current directory"
echo "--------------------------------------"
################################################################################

shopt -s nullglob
export KEYNAME_CHOP_CHUNK_SIZE=255
echo "KEYNAME_CHOP_CHUNK_SIZE ${KEYNAME_CHOP_CHUNK_SIZE}"

# Find all .ipynb files and use xargs to run nbconvert in parallel, within the directory of each notebook
find "${script_dir}/" -name "*.ipynb" -print0 | xargs -0 -n 1 -P 2 -I {} bash -c '{
  notebook="{}"
  notebook_dir=$(dirname "$notebook")
  notebook_base=$(basename "$notebook")
  echo "Processing notebook ${notebook_base} in directory ${notebook_dir}"
  cd "${notebook_dir}"
  jupyter nbconvert \
    --to notebook --execute --inplace \
    --ExecutePreprocessor.timeout=600 \
    "$notebook_base"
}'

shopt -u nullglob
