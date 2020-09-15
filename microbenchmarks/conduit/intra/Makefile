SHELL := /bin/bash

TEST_NAMES := \
	accumulating+type=any \
	put=dropping+get=stepping+type=any \
	put=growing+get=skipping+type=any \
	put=growing+get=stepping+type=any \


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
bench: $(addprefix bench-, $(TEST_NAMES))

build: $(addprefix build-, $(TEST_NAMES))

opt: $(addprefix opt-, $(TEST_NAMES))

fulldebug: $(addprefix fulldebug-, $(TEST_NAMES))

cranky: $(addprefix cranky-, $(TEST_NAMES))

clean: $(addprefix clean-, $(TEST_NAMES))
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
