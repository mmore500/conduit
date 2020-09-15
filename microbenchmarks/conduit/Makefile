SHELL := /bin/bash

BENCH_NAMES += thread
BENCH_NAMES += intra
BENCH_NAMES += proc

default: bench

bench-%: %
	cd $< && make bench

build-%: %
	cd $< && make build

opt-%: %
	cd $< && make opt

fulldebug-%: %
	cd $< && make fulldebug

cranky-%: %
	cd $< && make cranky

clean-%: %
	cd $< && make clean

# Test in debug mode without pointer tracker
bench: $(addprefix bench-, $(BENCH_NAMES))

build: $(addprefix build-, $(BENCH_NAMES))

opt: $(addprefix opt-, $(BENCH_NAMES))

fulldebug: $(addprefix fulldebug-, $(BENCH_NAMES))

cranky: $(addprefix cranky-, $(BENCH_NAMES))

clean: $(addprefix clean-, $(BENCH_NAMES))
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
