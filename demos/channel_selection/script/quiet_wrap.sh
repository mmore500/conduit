#!/bin/sh
ARGS=$@
if [[ $PMI_RANK == 0 ]]
then
  $ARGS
else
  $ARGS 1>/dev/null 2>/dev/null
fi
