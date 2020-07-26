BENCH_NAMES := ThreadDuct

EMP_DIR := ../third-party/Empirical/source

#CXX = clang++
CXX := mpic++

FLAGS = -std=c++17 -pthread -DNDEBUG -O3 -Wno-unused-function -I../source/ -I../third-party/ -I$(EMP_DIR)

default: bench

bench-%: %.cc
	$(CXX) $(FLAGS) $< -o $@.out
	# execute bench
	mpiexec -n 1 ./$@.out

# Test in debug mode without pointer tracker
bench: $(addprefix bench-, $(BENCH_NAMES))
	rm -rf bench*.out

# Test optimized version without debug features
debug: FLAGS := -std=c++17 -pthread -Wall -Wno-unused-function -Wno-unused-private-field -I../source/ -I../third-party/ -I$(EMP_DIR)
debug: $(addprefix bench-, $(BENCH_NAMES))
	rm -rf bench*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++17 -pthread -g -Wall -Wno-unused-function -I../source/ -I../third-party/ -I$(EMP_DIR) -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 # -Wmisleading-indentation
fulldebug: $(addprefix bench-, $(BENCH_NAMES))
	rm -rf bench*.out

cranky: FLAGS := -std=c++17 -pthread -g -Wall -Wno-unused-function -I../source/ -I../third-party/ -I$(EMP_DIR) -pedantic -DEMP_TRACK_MEM -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++
cranky: $(addprefix bench-, $(BENCH_NAMES))
	rm -rf bench*.out

clean:
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f ./Coverage*
	rm -rf ./temp
