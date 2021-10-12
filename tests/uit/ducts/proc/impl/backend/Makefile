SHELL := /bin/bash

TARGET_NAMES += backend
TARGET_NAMES += impl

default: test

test-%: %
	$(MAKE) test -C $<

opt-%: %
	$(MAKE) opt -C $<

fulldebug-%: %
	$(MAKE) fulldebug -C $<

cranky-%: %
	$(MAKE) cranky -C $<

cov-%: %
	$(MAKE) cov -C $<

clean-%: %
	$(MAKE) clean -C $<

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
