SHELL := /bin/bash

TEST_NAMES := \
	concurrent \
	conduit \
	containers \
	debug \
	distributed \
	mesh \
	mpi \
	topology \
	utility \


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

cov: coverage_conversion $(addprefix cov-, $(TEST_NAMES))

coverage_conversion:
	./convert_for_tests.sh

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(TEST_NAMES))

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
	rm -f ./Coverage*
	rm -rf ./temp
