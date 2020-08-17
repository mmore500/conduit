# Conduit

[![version](https://img.shields.io/endpoint?url=https%3A%2F%2Fmmore500.com%2Fconduit%2Fversion-badge.json)](https://github.com/mmore500/conduit/releases)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/eeb84ac3b8a3419f9714819f9191d7bf)](https://app.codacy.com/manual/mmore500/conduit?utm_source=github.com&utm_medium=referral&utm_content=mmore500/conduit&utm_campaign=Badge_Grade_Dashboard)
[![build status](https://travis-ci.com/mmore500/conduit.svg)](https://travis-ci.com/mmore500/conduit)
[![documentation status](https://readthedocs.org/projects/uit/badge/?version=latest)](https://uit.readthedocs.io/en/latest/?badge=latest)
[![documentation coverage](https://img.shields.io/endpoint?url=https%3A%2F%2Fmmore500.com%2Fconduit%2Fdocumentation-coverage-badge.json)](https://uit.readthedocs.io/en/latest/)
[![code coverage status](https://codecov.io/gh/mmore500/conduit/branch/master/graph/badge.svg)](https://codecov.io/gh/mmore500/conduit)
[![DockerHub status](https://img.shields.io/docker/build/mmore500/conduit.svg)](https://hub.docker.com/r/mmore500/conduit)
[![Lines of Code](https://tokei.rs/b1/github/mmore500/conduit?category=code)](https://github.com/XAMPPRocky/tokei)
[![Comments](https://tokei.rs/b1/github/mmore500/conduit?category=comments)](https://github.com/XAMPPRocky/tokei)

A multithread profiling project.

Check out the live in-browser web app at [https://mmore500.github.io/conduit](https://mmore500.github.io/conduit).

* Free software: MIT license
* Documentation: [http://conduit.fyi](http://conduit.fyi).

## TODO

* move utility to Empirical
* write tests
* write description and link blog posts
* move TODOs to GH tracker
* make a 0.1.0 release and add Zenodo badgev
* add how to cite info to README
* re-organize header files in includes: one header file in each subdirectory that grabs the rest?
  * like `include/concurrent/concurrent.h` would have includes for all the other header files in that directory in it
* make the read position and the write position internal to duct
* add valgrind to continuous integration
* refactor error_mutex_lock to make a uit_assert
* break up/refactor mpi_utils.h
* move utilities/assign_utils to assignment/
* add a way for ducts to specify what they are versus aren't capable of
  * can only do getcurrent
  * can do getcurrent or get next
    * can do varlen types (would wrap T with a function to get length?)
* setup ReadTheDocs
* investigate leaking MPI_Comms
* write Docstrings
* write adaptor that caches and then sends every n
* write backend that caches and then sends every n
* add real time fairness tests
* refactor I / no-I ducts for less code duplication
* add examples directory
* make apps or demos directory, move most of source there
* debug RDMADuct
* rename ring mesh to bucket brigade
* make macrobenchmarks run for fixed time not iters, add proc macrobenchmarks, upload macrobenchmark results to osf
* fix https on documentation site 

* make_random_mesh
  * degree is argument
* make_toroidal_mesh (make ring mesh is special case of this)
  * dimension is argument
* make_grid_mesh (like toroidal mesh, but no wraparound) lattice?
  * dimension is argument
* make_complete_mesh

## Design Plan

```
      sharedptr       sharedptr
Inlet  --->     duct    <--- Outlet
      std::variant
        *IntraDuct
        *ThreadDuct
        *ProcessDuct

Inlet  --->
    sharedptr

Node object
- vector of NodeOutput
- vector of NodeInput

class MeshState; (better name for this)

only when the inlet is part of a mesh
(NodeOutput) Inlet Wrapper (better name for this)
- (private secret) shared_ptr to MeshState
- (private) Inlet member object
- write methods that forward to Inlet member object
- is there a better way to do than composition? (inheritance, encapsulation?)
(NodeInput) Outlet Wrapper (better name for this)
- shared_ptr to MeshState
- private secret Outlet member object


++ Mesh ++

new shared_ptr MeshState
- RDMAWindowManager

- vector of Inlets
- vector of Outlets

- get me wrappers of I'm responsible for
- vector of Nodes
```

## Features

* TODO

## Reading List

* [spsc-bip-buffer](https://github.com/utaal/spsc-bip-buffer/)
* [Lockless Ring Buffer Design](https://www.kernel.org/doc/Documentation/trace/ring-buffer-design.txt)
* [The design and implementation of a lock-free ring-buffer with contiguous reservations](https://ferrous-systems.com/blog/lock-free-ring-buffer/)
* [boost spsc_queue docs](https://www.boost.org/doc/libs/1_73_0/doc/html/boost/lockfree/spsc_queue.html)
* [boost spsc_queue implementation](https://github.com/boostorg/lockfree/blob/771e8c1016a5d9b61acbc871e442a3fb139e1494/include/boost/lockfree/spsc_queue.hpp)
* [The Bip Buffer - The Circular Buffer with a Twist](https://www.codeproject.com/Articles/3479/The-Bip-Buffer-The-Circular-Buffer-with-a-Twist)
* [MPI: blocking vs non-blocking](https://stackoverflow.com/a/47041382)

## Credits

This package was created with [Cookiecutter](https://github.com/audreyr/cookiecutter) and the [devosoft/cookiecutter-empirical-project](https://github.com/devosoft/cookiecutter-empirical-project) project template.

This project is built using the [Empirical C++ library](https://github.com/devosoft/Empirical/).
