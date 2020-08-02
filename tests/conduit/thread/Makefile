TEST_NAMES := HeadTailDuct AtomicPendingDuct

EMP_DIR := ../../../third-party/Empirical/source

#CXX = clang++
CXX := mpic++

FLAGS = -std=c++17 -pthread -Wall -Wno-unused-function -Wno-unused-private-field -I../../../include/ -I../../../source/ -I../../../third-party/ -I$(EMP_DIR)

default: test

test-%: %.cc
	$(CXX) $(FLAGS) $< -lbenchmark -lpthread  -o $@.out
	# execute test
	./$@.out

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

# Test optimized version without debug features
opt: FLAGS := -std=c++17 -pthread -DNDEBUG -O3 -Wno-unused-function -I../../../include/ -I../../../source/ -I../../../third-party/ -I$(EMP_DIR)
opt: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++17 -pthread -g -Wall -Wno-unused-function -I../../../include/ -I../../../source/ -I../../../third-party/ -I$(EMP_DIR) -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 # -Wmisleading-indentation
fulldebug: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

cranky: FLAGS := -std=c++17 -pthread -g -Wall -Wno-unused-function -I../../../include/ -I../../../source/ -I../../../third-party/ -I$(EMP_DIR) -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++
cranky: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

cov: FLAGS := -std=c++17 -pthread -g -Wall -Wno-unused-function -I../../../coverage_include/ -I../../../source -I../../../third-party/ -I$(EMP_DIR) -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 -fprofile-instr-generate -fcoverage-mapping -fno-inline -fno-elide-constructors -O0
cov: $(addprefix cov-, $(TEST_NAMES))

cov-%: %.cc ../../../third-party/Catch/single_include/catch2/catch.hpp
	export OMPI_CXX=clang++ && $(CXX) $(FLAGS) $< -o -lbenchmark -lpthread $@.out
	#echo "running $@.out"
	# execute test
	./$@.out
	llvm-profdata merge default.profraw -o default.profdata
	llvm-cov show ./$@.out -instr-profile=default.profdata > coverage_$@.txt
	python ../../../third-party/force-cover/fix_coverage.py coverage_$@.txt

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f ./Coverage*
	rm -rf ./temp
