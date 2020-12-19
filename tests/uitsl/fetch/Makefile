TARGET_NAMES += detect_gz
TARGET_NAMES += detect_tar
TARGET_NAMES += fetch
TARGET_NAMES += inflate
TARGET_NAMES += untar

TO_ROOT := $(shell git rev-parse --show-cdup)

include $(TO_ROOT)/tests/MaketemplateUniproc

web: $(addprefix web-, $(TARGET_NAMES))

web-%: %.cpp
	source $(TO_ROOT)/third-party/emsdk/emsdk_env.sh && \
	emcc -std=c++17 -pipe -I$(TO_ROOT)/include/ -I$(TO_ROOT)/third-party/ -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" -s EXPORTED_FUNCTIONS="['_main']" -s DISABLE_EXCEPTION_CATCHING=0 -s FETCH=1 -s USE_ZLIB=1 -s EXIT_RUNTIME=1 --embed-file assets -o $@.js $<
	npm install
	node $@.js
	rm -rf $@.js

test:: web

clean::
	rm -f *.js
	rm -f *.wasm
