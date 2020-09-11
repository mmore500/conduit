TEST_NAMES := \
	AtomicAccumulatingDuct \


MPICXX ?= mpic++
MPIEXEC ?= mpiexec

FLAGS = -std=c++17 -g -pthread -march=native -Wall -Wno-unused-function -Wno-unused-private-field -I../../../../include/ -I../../../../source/ -I../../../../third-party/

default: test

test-%: %.cpp
	echo "MPICXX" $(MPICXX)
	echo "MPIEXEC" $(MPIEXEC)
	$(MPICXX) $(FLAGS) $< -lbenchmark -lpthread  -o $@.out
	# execute test
	for n in $$(seq 2 8); do \
		$(MPIEXEC) -n 1 ./$@.out --num_threads $$n || exit 1; \
	done

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

# Test optimized version without debug features
opt: FLAGS := -std=c++17 -pthread -march=native -DNDEBUG -O3 -Wno-unused-function -I../../../../include/ -I../../../../source/ -I../../../../third-party/
opt: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

# Test in debug mode with pointer tracking
fulldebug: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I../../../../include/ -I../../../../source/ -I../../../../third-party/ -pedantic -DEMP_TRACK_MEM -DUIT_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 # -Wmisleading-indentation
fulldebug: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

cranky: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I../../../../include/ -I../../../../source/ -I../../../../third-party/ -pedantic -DEMP_TRACK_MEM -DUIT_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -Wconversion -Weffc++
cranky: $(addprefix test-, $(TEST_NAMES))
	rm -rf test*.out

cov: FLAGS := -std=c++17 -pthread -march=native -g -Wall -Wno-unused-function -I../../../../coverage_include/ -I../../../../source -I../../../../third-party/ -DEMP_TRACK_MEM -DUIT_AUDIT_OCCUPANCY -Wnon-virtual-dtor -Wcast-align -Woverloaded-virtual -ftemplate-backtrace-limit=0 -fprofile-instr-generate -fcoverage-mapping -fno-inline -fno-elide-constructors -O0
cov: $(addprefix cov-, $(TEST_NAMES))

cov-%: %.cpp
	export OMPI_CXX=clang++ && $(MPICXX) $(FLAGS) $< -lbenchmark -lpthread -o $@.out
	#echo "running $@.out"
	# execute test
	./$@.out
	llvm-profdata merge default.profraw -o default.profdata
	llvm-cov show ./$@.out -instr-profile=default.profdata > coverage_$@.txt
	python ../../../../third-party/force-cover/fix_coverage.py coverage_$@.txt

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
