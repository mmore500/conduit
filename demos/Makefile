SHELL := /bin/bash

DEMO_NAMES += signalgp

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

cov: coverage_conversion $(addprefix cov-, $(DEMO_NAMES))

coverage_conversion:
	./convert_for_tests.sh

# Test in debug mode without pointer tracker
test: $(addprefix test-, $(DEMO_NAMES))

opt: $(addprefix opt-, $(DEMO_NAMES))

fulldebug: $(addprefix fulldebug-, $(DEMO_NAMES))

cranky: $(addprefix cranky-, $(DEMO_NAMES))

clean: $(addprefix clean-, $(DEMO_NAMES))
	rm -f *.out
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.info
	rm -f *.gcov
	rm -f ./Coverage*
	rm -rf ./temp
