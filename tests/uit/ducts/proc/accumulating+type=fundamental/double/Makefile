SHELL := /bin/bash

#TARGET_NAMES += aggregated+inlet=Accumulate+outlet=WithdrawingWindow_f\:\:AggregatedIaOwwDuct
#TARGET_NAMES += aggregated+inlet=Raccumulate+outlet=WithdrawingWindow_f\:\:AggregatedIrOwwDuct
#TARGET_NAMES += inlet=Accumulate+outlet=WithdrawingWindow_f\:\:IaOwwDuct
#TARGET_NAMES += inlet=Raccumulate+outlet=WithdrawingWindow_f\:\:IrOwwDuct

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
	# TODO why does rdma hang with large proc counts on travis?
	for n in $$(seq 2 4); do \
		$(MPIEXEC) --bind-to none -n $$n ./$@.out || exit 1; \
	done

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(TARGET_NAMES))
	rm -rf test*.out

# Test optimized version without debug features
opt: FLAGS := -std=c++17 -pthread -march=native -DNDEBUG -O3 -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/
opt: $(addprefix test-, $(TARGET_NAMES))
	rm -rf test*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 # -Wmisleading-indentation
fulldebug: $(addprefix test-, $(TARGET_NAMES))
	rm -rf test*.out

cranky: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++
cranky: $(addprefix test-, $(TARGET_NAMES))
	rm -rf test*.out

cov: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I$(TO_ROOT)/coverage_include/ -I$(TO_ROOT)/third-party/ -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 -fprofile-instr-generate -fcoverage-mapping -fno-inline -fno-elide-constructors -O0
cov: $(addprefix cov-, $(TARGET_NAMES))

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
