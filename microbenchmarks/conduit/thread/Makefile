BENCH_NAMES := HeadTailDuct AtomicPendingDuct

CXX := mpic++

FLAGS = -std=c++17 -pthread -DNDEBUG -O3 -Wno-unused-function -I../../../source/ -I../../../include/ -I../../../third-party/

default: bench

bench-%: %.cpp
	$(CXX) $(FLAGS) $< -lbenchmark -lpthread -o $@.out
	# execute bench
	mpiexec --bind-to none -n 1 ./$@.out --benchmark_counters_tabular=true --benchmark_format=json --benchmark_out="impl=$*+procs=1+ext=.json" | ../../script/headtail.sh

# Test in debug mode without pointer tracker
bench: $(addprefix bench-, $(BENCH_NAMES))
	rm -rf bench*.out

build-%: %.cpp
	$(CXX) $(FLAGS) $< -lbenchmark -lpthread -o $@.out

# Test in debug mode without pointer tracker
build: $(addprefix build-, $(BENCH_NAMES))

# Test optimized version without debug features
debug: FLAGS := -std=c++17 -g -pthread -Wall -Wno-unused-function -Wno-unused-private-field -I../../../include/ -I../../../source/ -I../../../third-party/
debug: $(addprefix bench-, $(BENCH_NAMES))
	rm -rf bench*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++17 -pthread -g -Wall -Wno-unused-function  -I../../../include/ -I../../../source/ -I../../../third-party/ -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 # -Wmisleading-indentation
fulldebug: $(addprefix bench-, $(BENCH_NAMES))
	rm -rf bench*.out

cranky: FLAGS := -std=c++17 -pthread -g -Wall -Wno-unused-function -I../../../include/ -I../../../source/ -I../../../third-party/ -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++
cranky: $(addprefix bench-, $(BENCH_NAMES))
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
