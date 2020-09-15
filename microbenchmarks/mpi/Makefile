SHELL := /bin/bash

TARGET_NAMES += MPI_Isend
TARGET_NAMES += MPI_Irsend
TARGET_NAMES += MPI_Rsend
TARGET_NAMES += MPI_Send
TARGET_NAMES += MPI_Test
TARGET_NAMES += MPI_Testall
TARGET_NAMES += MPI_Testsome
TARGET_NAMES += MPI_Irecv

FLAGS = -std=c++17 -pthread -march=native -DNDEBUG -O3 -Wno-unused-function/ -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/

MPICXX ?= mpic++
MPIEXEC ?= mpiexec

default: bench

bench-%: %.cpp
	$(MPICXX) $(FLAGS) $< -lbenchmark -lpthread -o $@.out
	# execute bench
	let i=1; \
	test -z "$$SLURM_NTASKS" && SLURM_NTASKS=$$(nproc); \
	while :; do \
		let n=$$((2**$$i)); \
		test $$n -gt $$SLURM_NTASKS && break; \
		$(MPIEXEC) --bind-to none -n $$n ./$@.out --benchmark_counters_tabular=true --benchmark_format=json --benchmark_out="impl=$*+procs=$$n+ext=.json" | ../script/headtail.sh; \
		let i++; \
	done

# Test in debug mode without pointer tracker
bench: $(addprefix bench-, $(TARGET_NAMES))
	rm -rf bench*.out

build-%: %.cpp
	$(MPICXX) $(FLAGS) $< -lbenchmark -lpthread -o $@.out

# Test in debug mode without pointer tracker
build: $(addprefix build-, $(TARGET_NAMES))

# Test optimized version without debug features
debug: FLAGS := -std=c++17 -g -pthread -march=native -Wall -Wno-unused-function -Wno-unused-private-field -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -g
debug: $(addprefix bench-, $(TARGET_NAMES))
	rm -rf bench*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function  -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -pedantic -DEMP_TRACK_MEM -DUIT_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 -g # -Wmisleading-indentation
fulldebug: $(addprefix bench-, $(TARGET_NAMES))
	rm -rf bench*.out

cranky: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -pedantic -DEMP_TRACK_MEM -DUIT_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++
cranky: $(addprefix bench-, $(TARGET_NAMES))
	rm -rf bench*.out

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f *.csv
	rm -f *.json
	rm -f ./Coverage*
	rm -rf ./temp
