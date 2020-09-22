SHELL := /bin/bash

TARGET_NAMES += accumulating+type=fundamental
TARGET_NAMES += accumulating+type=span
TARGET_NAMES += accumulating+type=spanfundamental
TARGET_NAMES += accumulating+type=trivial
TARGET_NAMES += impl
TARGET_NAMES += put=dropping+get=skipping+type=span
TARGET_NAMES += put=dropping+get=skipping+type=trivial
TARGET_NAMES += put=dropping+get=stepping+type=cereal
TARGET_NAMES += put=dropping+get=stepping+type=span
TARGET_NAMES += put=dropping+get=stepping+type=trivial
TARGET_NAMES += put=growing+get=skipping+type=trivial
TARGET_NAMES += put=growing+get=stepping+type=cereal
TARGET_NAMES += put=growing+get=stepping+type=trivial

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
