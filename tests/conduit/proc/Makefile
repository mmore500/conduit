SHELL := /bin/bash

TEST_NAMES := \
	accumulating+type=fundamental \
	backend \
	inlet \
	outlet \
	put=dropping+get=skipping+type=span \
	put=dropping+get=skipping+type=trivial \
	put=dropping+get=stepping+type=cereal \
	put=dropping+get=stepping+type=span \
	put=dropping+get=stepping+type=trivial \
	put=growing+get=skipping+type=trivial \
	put=growing+get=stepping+type=cereal \
	put=growing+get=stepping+type=trivial \


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
test: $(addprefix test-, $(TEST_NAMES))

opt: $(addprefix opt-, $(TEST_NAMES))

fulldebug: $(addprefix fulldebug-, $(TEST_NAMES))

cranky: $(addprefix cranky-, $(TEST_NAMES))

cov: $(addprefix cov-, $(TEST_NAMES))

clean: $(addprefix clean-, $(TEST_NAMES))
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f ./Coverage*
	rm -rf ./temp
