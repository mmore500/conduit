#!/bin/bash

export LD_PRELOAD=/mnt/home/mmore500/libmimalloc.so

export PP_USE_OMP=0
export PP_TACITURN=1
export PP_NUM_SECONDS=5

NPROC=$(nproc)
echo "NPROC: ${NPROC}"

# MPI_THRESH = 0 -> MPI_PROCS = 1
MPI_THRESH=0

STRONG_SCALING=0

unset PP_NUM_UPDATES
for SYNCHRONOUS in 0 1; do
  export PP_SYNCHRONOUS=$SYNCHRONOUS
  for TREATMENT in \
    Compute_Lean Compute_Moderate Compute_Intensive \
    Memory_Lean Memory_Moderate Memory_Intensive \
    ; do

    case $TREATMENT in
      Compute*)
        GRID_SLOPE=1
        RESISTANCE_SLOPE=0
        GRID_INTERCEPT=0
        ;;
      Memory*)
        RESISTANCE_SLOPE=1
        GRID_SLOPE=0
        RESISTANCE_INTERCEPT=0
        ;;
      *)
        echo "bad1 TREATMENT: ${TREATMENT}"
        exit 1
        ;;
    esac

    case $TREATMENT in
      Compute_Lean)
        RESISTANCE_INTERCEPT=1
        ;;
      Compute_Moderate)
        RESISTANCE_INTERCEPT=512
        ;;
      Compute_Intensive)
        RESISTANCE_INTERCEPT=262144
        ;;
      Memory_Lean)
        GRID_INTERCEPT=1
        ;;
      Memory_Moderate)
        GRID_INTERCEPT=512
        ;;
      Memory_Intensive)
        GRID_INTERCEPT=262144
        ;;
      *)
        echo "bad2 TREATMENT: ${TREATMENT}"
        exit 1
        ;;
    esac

  OUT_FILE="Treatment=${TREATMENT}+Synchronous=${SYNCHRONOUS}+ext=.csv"
  echo "Threads,Work,Replicate,Unit Productivity" > $OUT_FILE
  for REP in {0..4}; do
    for LOAD_PER in 1 4 16 64 256 1024 4096; do
      for NUM_THREADS in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096; do

        # too many threads per proc causes crash
        # and isn't interesting to profile
        if ((NUM_THREADS > NPROC * 64)); then
          echo "Skipping ${NUM_THREADS}-thread eval on ${NPROC}-core alloc"
          continue
        fi

        echo "MPI_THRESH: ${MPI_THRESH}"
        MPI_PROCS=$(( \
          $MPI_THRESH \
            ? $NUM_THREADS < $MPI_THRESH ? 1 : $NUM_THREADS / $MPI_THRESH \
            : 1 \
        ))
        echo "MPI_PROCS: ${MPI_PROCS}"

        echo "NUM_THREADS: ${NUM_THREADS}"
        export PP_NUM_THREADS=$(( $NUM_THREADS / $MPI_PROCS ))
        echo "PP_NUM_THREADS: ${PP_NUM_THREADS}"

        AMT_WORK=$(( \
          STRONG_SCALING \
            ? $LOAD_PER \
            : $NUM_THREADS * $LOAD_PER \
        ))
        echo "AMT_WORK: ${AMT_WORK}"

        RESISTANCE=$(( $AMT_WORK * $RESISTANCE_SLOPE + $RESISTANCE_INTERCEPT ))
        echo "RESISTANCE: ${RESISTANCE}"
        export PP_RESISTANCE=$RESISTANCE

        GRID_SIZE=$(( $AMT_WORK * $GRID_SLOPE + $GRID_INTERCEPT ))
        echo "GRID_SIZE: ${GRID_SIZE}"
        export PP_GRID_SIZE=$(( $GRID_SIZE / $MPI_PROCS ))

        if ((NUM_THREADS > GRID_SIZE)); then
          echo "Skipping ${NUM_THREADS}-thread eval on ${NPROC}-tile grid"
          continue
        fi

        echo "LOAD_PER: ${LOAD_PER}"
        echo "REP: ${REP}"
        echo "SYNCHRONOUS: ${SYNCHRONOUS}"

        mpiexec -n $MPI_PROCS ./pipe-profile > tmp
        UNIT_PRODUCTIVITY=$(cat tmp)
        echo "UNIT_PRODUCTIVITY: ${UNIT_PRODUCTIVITY}"
        TOTAL_PRODUCTIVITY=$(( \
          STRONG_SCALING \
            ? UNIT_PRODUCTIVITY * MPI_PROCS \
            : UNIT_PRODUCTIVITY * NUM_THREADS \
        ))
        echo "TOTAL_PRODUCTIVITY: ${TOTAL_PRODUCTIVITY}"
        echo "${NUM_THREADS},${AMT_WORK},${REP},${UNIT_PRODUCTIVITY}" \
          >> $OUT_FILE
        echo
        echo "========================="
        echo
      done;
    done;
  done;
  done;
done
