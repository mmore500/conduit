SHELL := /bin/bash

TARGET_NAMES += inflate
TARGET_NAMES += NamedArrayElement
TARGET_NAMES += untar

TO_ROOT := $(shell git rev-parse --show-cdup)

FLAGS = -std=c++17 -g -pthread -march=native -Wall -Wno-unused-function -Wno-unused-private-field -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/

UIT_MPICXX ?= mpicxx
UIT_MPIEXEC ?= mpiexec

default: test

test-%: %.cpp
	echo "UIT_MPICXX" $(UIT_MPICXX)
	echo "UIT_MPIEXEC" $(UIT_MPIEXEC)
	$(UIT_MPICXX) $(FLAGS) $< -lbenchmark -lpthread -lstdc++fs -lz -o $@.out
	# execute test
	$(UIT_MPIEXEC) -n 1 ./$@.out

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(TARGET_NAMES))
	rm -rf test*.out

# Test optimized version without debug features
opt: FLAGS := -std=c++17 -pthread -march=native -DNDEBUG -O3 -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/
opt: $(addprefix test-, $(TARGET_NAMES))
	rm -rf test*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -pedantic -DEMP_TRACK_MEM -DUITSL_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 # -Wmisleading-indentation
fulldebug: $(addprefix test-, $(TARGET_NAMES))
	rm -rf test*.out

cranky: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -pedantic -DEMP_TRACK_MEM -DUITSL_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++
cranky: $(addprefix test-, $(TARGET_NAMES))
	rm -rf test*.out

cov: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/coverage_include/ -I$(TO_ROOT)/third-party/ -DEMP_TRACK_MEM -DUITSL_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 -fprofile-instr-generate -fcoverage-mapping -fno-inline -fno-elide-constructors -O0
cov: $(addprefix cov-, $(TARGET_NAMES))

cov-%: %.cpp
	export OMPI_CXX=clang++ && $(UIT_MPICXX) $(FLAGS) $< -lbenchmark -lpthread -lstdc++fs -lz -o $@.out
	# execute test
	echo "UIT_MPICXX" $(UIT_MPICXX)
	echo "UIT_MPIEXEC" $(UIT_MPIEXEC)
	# see https://clang.llvm.org/docs/UsersManual.html#profiling-with-instrumentation
	for n in 1; do \
		export n; \
		$(UIT_MPIEXEC) --bind-to none $$($(UIT_MPIEXEC) --version | grep -q "OpenRTE" && echo "--oversubscribe") -n $$n \
			/bin/bash -c '\
				LLVM_PROFILE_FILE="%p-%m-%h-$${n}-$${OMPI_COMM_WORLD_RANK}.profraw" \
				./$@.out' \
			|| exit 1; \
	done
	llvm-profdata merge *.profraw -o default.profdata
	llvm-cov show ./$@.out -instr-profile=default.profdata > coverage_$@.txt
	python $(TO_ROOT)/third-party/force-cover/fix_coverage.py coverage_$@.txt

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f *.txt
	rm -f *.profdata
	rm -f *.profraw
	rm -f ./Coverage*
	rm -rf ./temp
	rm -rf empty never rick.txt twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son-the-jaws-that-bite-the-claws-that-catch-beware-the-jubjub-bird-and-shun-the-frumious-bandersnatch beautiful-is-better-than-ugly-explicit-is-better-than-implicit-simple-is-better-than-complex-complex-is-better-than-complicated-flat-is-better-than-nested-sparse-is-better-than-dense-readability-counts-special-cases-arent-special-enough-to-break-the-rules
