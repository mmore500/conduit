#!/bin/bash

DIRECTORY_EXCLUDES=$(for submodule in $(ls -d third-party/*/); do printf " --exclude-dir=`basename $submodule`"; done)

grep -iIER $DIRECTORY_EXCLUDES --exclude="*dotos*" 'todo|fixme' . | wc -l
