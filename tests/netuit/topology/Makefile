TARGET_NAMES += DyadicTopologyFactory
TARGET_NAMES += RingTopologyFactory
TARGET_NAMES += TopoNode
TARGET_NAMES += LoopTopologyFactory
TARGET_NAMES += TopoEdge
TARGET_NAMES += TopoNodeInput
TARGET_NAMES += ProConTopologyFactory
TARGET_NAMES += Topology
TARGET_NAMES += TopoNodeOutput
TARGET_NAMES += ToroidalTopologyFactory

TO_ROOT := $(shell git rev-parse --show-cdup)

staging:
	mkdir -p staging/assets

assets: staging
	mkdir assets
	python3 scripts/make_dyadic.py
	python3 scripts/make_loop.py
	python3 scripts/make_procon.py
	python3 scripts/make_ring.py
	python3 scripts/make_toroidal.py

test:: assets
opt:: assets
fulldebug:: assets

clean::
	rm -rf assets
	rm -rf staging

include $(TO_ROOT)/tests/MaketemplateUniproc
