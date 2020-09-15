SHELL := /bin/bash

TEST_NAMES += CircularIndex
TEST_NAMES +=	Curry
TEST_NAMES +=	IdentityWrapper
TEST_NAMES +=	imemstream
TEST_NAMES +=	IsFirstExecutionChecker
TEST_NAMES +=	math_utils
TEST_NAMES +=	MirroredRingBuffer
TEST_NAMES +=	NamedArrayElement
TEST_NAMES +=	PrefilledMirroredRingBuffer
TEST_NAMES +=	RingBuffer
TEST_NAMES +=	safe_compare
TEST_NAMES +=	ScopeGuard
TEST_NAMES +=	spector
TEST_NAMES +=	Uninitialized
TEST_NAMES +=	upper_uniquify
TEST_NAMES +=	ValueInitialized
TEST_NAMES +=	VectorMap
TEST_NAMES +=	WarnOnce


TO_ROOT := $(shell git rev-parse --show-cdup)

FLAGS = -std=c++17 -g -pthread -march=native -Wall -Wno-unused-function -Wno-unused-private-field -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/

MPICXX ?= mpic++
MPIEXEC ?= mpiexec

default: test

test-%: %.cpp
	echo "MPICXX" $(MPICXX)
	echo "MPIEXEC" $(MPIEXEC)
	$(MPICXX) $(FLAGS) $< -lbenchmark -lpthread -o $@.out
	# execute test
	$(MPIEXEC) -n 1 ./$@.out

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

# Test optimized version without debug features
opt: FLAGS := -std=c++17 -pthread -march=native -DNDEBUG -O3 -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/
opt: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -pedantic -DEMP_TRACK_MEM -DUIT_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 # -Wmisleading-indentation
fulldebug: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

cranky: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -pedantic -DEMP_TRACK_MEM -DUIT_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++
cranky: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

cov: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/coverage_include/ -I$(TO_ROOT)/third-party/ -DEMP_TRACK_MEM -DUIT_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 -fprofile-instr-generate -fcoverage-mapping -fno-inline -fno-elide-constructors -O0
cov: $(addprefix cov-, $(TEST_NAMES))

cov-%: %.cpp
	export OMPI_CXX=clang++ && $(MPICXX) $(FLAGS) $< -lbenchmark -lpthread -o $@.out
	#echo "running $@.out"
	# execute test
	./$@.out
	llvm-profdata merge default.profraw -o default.profdata
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
