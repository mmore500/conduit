SHELL := /bin/bash

TARGET_NAMES += duct
TARGET_NAMES += mpi

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
bench: $(addprefix bench-, $(TARGET_NAMES))
	./script/fixup_and_validate.sh
	python3 script/consolidate.py $$(find duct -name '*.json')
	for f in *run_type=iteration*.csv; do \
		python3 script/graph.py $$f; \
	done
	./script/upload.sh

build: $(addprefix build-, $(TARGET_NAMES))

opt: $(addprefix opt-, $(TARGET_NAMES))

fulldebug: $(addprefix fulldebug-, $(TARGET_NAMES))

cranky: $(addprefix cranky-, $(TARGET_NAMES))

clean: $(addprefix clean-, $(TARGET_NAMES))
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
