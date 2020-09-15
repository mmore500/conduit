# Project-specific settings
PROJECT := signalgp
UIT_DIR := ../../include/

# Flags to use regardless of compiler
CFLAGS_all := -Wall -Wno-unused-function -std=c++17 -I$(UIT_DIR) -Iinclude/ -fopenmp

# Native compiler information
MPICXX := mpic++
CFLAGS_nat := -O3 -DNDEBUG -march=native $(CFLAGS_all)
CFLAGS_nat_debug := -g $(CFLAGS_all)
MPIEXEC ?= mpiexec

# Emscripten compiler information
CXX_web := emcc
OFLAGS_web_all := -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" -s TOTAL_MEMORY=67108864 --js-library $(UIT_DIR)/web/library_emp.js -s EXPORTED_FUNCTIONS="['_main', '_empCppCallback']" -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=1 #--embed-file configs
OFLAGS_web := -Oz -DNDEBUG
OFLAGS_web_debug := -g4 -Oz -Wno-dollar-in-identifier-extension

CFLAGS_web := $(CFLAGS_all) $(OFLAGS_web) $(OFLAGS_web_all)
CFLAGS_web_debug := $(CFLAGS_all) $(OFLAGS_web_debug) $(OFLAGS_web_all)


default: $(PROJECT)
native: $(PROJECT)
web: $(PROJECT).js

debug: CFLAGS_nat := $(CFLAGS_nat_debug)
debug: $(PROJECT)

debug-web: CFLAGS_web := $(CFLAGS_web_debug)
debug-web: $(PROJECT).js

web-debug: debug-web

test: debug
	$(MPIEXEC) -n 2 ./$(PROJECT)

$(PROJECT):	source/native.cpp
	$(MPICXX) $(CFLAGS_nat) source/native.cpp -lstdc++fs -lbenchmark -lpthread -o $(PROJECT)
	@echo To build the web version use: make web

$(PROJECT).js: web/$(PROJECT).cpp
	$(CXX_web) $(CFLAGS_web) web/$(PROJECT).cpp -o web/$(PROJECT).js

clean:
	rm -f $(PROJECT) web/$(PROJECT).js web/*.js.map web/*.js.map *~ source/*.o web/*.wasm web/*.wast *.csv
