SHELL := /bin/bash

BENCH_NAMES += conduit
BENCH_NAMES += mpi

VERSION := $$(git describe --dirty --always)

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
	./script/fixup_and_validate.sh
	python3 script/consolidate.py $$(find conduit -name '*.json')
	for f in *run_type=iteration*.csv; do \
		python3 script/graph.py $$f; \
	done
	./script/upload.sh

build: $(addprefix build-, $(BENCH_NAMES))

opt: $(addprefix opt-, $(BENCH_NAMES))

fulldebug: $(addprefix fulldebug-, $(BENCH_NAMES))

cranky: $(addprefix cranky-, $(BENCH_NAMES))

clean: $(addprefix clean-, $(BENCH_NAMES))
	rm -f *.out
	rm -f *.png
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f *.csv
	rm -f *.json
	rm -f ./Coverage*
	rm -rf ./temp
