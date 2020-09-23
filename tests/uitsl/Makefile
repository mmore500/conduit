SHELL := /bin/bash

TARGET_NAMES += algorithm
TARGET_NAMES += chrono
TARGET_NAMES += concurrent
TARGET_NAMES += containers
TARGET_NAMES += countdown
TARGET_NAMES += datastructs
TARGET_NAMES += debug
TARGET_NAMES += distributed
TARGET_NAMES += math
TARGET_NAMES += meta
TARGET_NAMES += mpi
TARGET_NAMES += nonce
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

cov-%: %
	cd $< && make cov

clean-%: %
	cd $< && make clean

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(TARGET_NAMES))

opt: $(addprefix opt-, $(TARGET_NAMES))

fulldebug: $(addprefix fulldebug-, $(TARGET_NAMES))

cranky: $(addprefix cranky-, $(TARGET_NAMES))

cov: $(addprefix cov-, $(TARGET_NAMES))

clean: $(addprefix clean-, $(TARGET_NAMES))
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f ./Coverage*
	rm -rf ./temp
