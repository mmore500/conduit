TEST_NAMES := conduit

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
bench: $(addprefix bench-, $(TEST_NAMES))
	for f in */*/*.json; do \
		osf -u mmore500.login@gmail.com -p 7jkgp \
		  upload $$f $(VERSION)/$$CONTEXT/$$f; \
	done
	python3 consolidate.py */*/*.json
	osf -u mmore500 -p 7jkgp \
		upload "consolidated.csv" $(VERSION)/$$CONTEXT/consolidated.csv

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
	rm -f ./Coverage*
	rm -rf ./temp
