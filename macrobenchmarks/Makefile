SHELL := /bin/bash

BENCH_NAMES += conduit
BENCH_NAMES += sanitycheck

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
	rm -f ./Coverage*
	rm -rf ./temp
