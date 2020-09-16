SHELL := /bin/bash

TARGET_NAMES += algorithm
TARGET_NAMES += chrono
TARGET_NAMES += concurrent
TARGET_NAMES += conduit
TARGET_NAMES += containers
TARGET_NAMES += countdown
TARGET_NAMES += datastructs
TARGET_NAMES += debug
TARGET_NAMES += distributed
TARGET_NAMES += math
TARGET_NAMES += mesh
TARGET_NAMES += meta
TARGET_NAMES += mpi
TARGET_NAMES += nonce
TARGET_NAMES += topology
TARGET_NAMES += utility

default: test

test-%: %
	cd $< && make test

opt-%: %
	cd $< && make opt

fulldebug-%: %
	cd $< && make fulldebug

cranky-%: %
	cd $< && make cranky

clean-%: %
	cd $< && make clean

cov-%:
	cd $(@:cov-%=%) && make cov

cov: coverage_conversion $(addprefix cov-, $(TARGET_NAMES))

coverage_conversion:
	./convert_for_tests.sh

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(TARGET_NAMES))

opt: $(addprefix opt-, $(TARGET_NAMES))

fulldebug: $(addprefix fulldebug-, $(TARGET_NAMES))

cranky: $(addprefix cranky-, $(TARGET_NAMES))

clean: $(addprefix clean-, $(TARGET_NAMES))
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f ./Coverage*
	rm -rf ./temp
