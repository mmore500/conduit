# Conduit

[![build status](https://img.shields.io/travis/mmore500/conduit.svg)](https://travis-ci.org/mmore500/conduit)
[![documentation status](https://readthedocs.org/projects/conduit/badge/?version=latest)](https://conduit.readthedocs.io/en/latest/?badge=latest)
[![code coverage status](https://codecov.io/gh/mmore500/conduit/branch/master/graph/badge.svg)](https://codecov.io/gh/mmore500/conduit)
[![DockerHub status](https://img.shields.io/docker/build/mmore500/conduit.svg)](https://hub.docker.com/r/mmore500/conduit)

A multithread profiling project.

Check out the live in-browser web app at [https://mmore500.github.io/conduit](https://mmore500.github.io/conduit).

* Free software: MIT license
* Documentation: [https://conduit.readthedocs.io](https://conduit.readthedocs.io).

## TODO

* namespace (uit? ondu? cdt? con? duit? cndt? cnd?)
* move utility to Empirical
* write tests
* add automated test coverage
* collect data from benchmarks
* add benchmarks to CI build
* add opt, fulldebug, and cranky tests to build
* come up with some sort of system to archive benchmark data
  * datafile named for hash of current commit?
* break tests up into multiple builds
* switch README.rst to Markdown
* write description and link blog posts
* move TODOs to GH tracker
* register with Zenodo
* add how to cite info to README
* come up with interface to reflect
  * variable-length,
* switch to branch-based workflow
* consider re-organizing header files
* re-organize header files in includes: one header file in each subdirectory that grabs the rest?
  * like `include/concurrent/concurrent.h` would have includes for all the other header files in that directory in it

## Features

* TODO

## Reading List

* [spsc-bip-buffer](https://github.com/utaal/spsc-bip-buffer/)
* [Lockless Ring Buffer Design](https://www.kernel.org/doc/Documentation/trace/ring-buffer-design.txt)
* [The design and implementation of a lock-free ring-buffer with contiguous reservations](https://ferrous-systems.com/blog/lock-free-ring-buffer/)
* [boost spsc_queue docs](https://www.boost.org/doc/libs/1_73_0/doc/html/boost/lockfree/spsc_queue.html)
* [boost spsc_queue implementation](https://github.com/boostorg/lockfree/blob/771e8c1016a5d9b61acbc871e442a3fb139e1494/include/boost/lockfree/spsc_queue.hpp)
* [The Bip Buffer - The Circular Buffer with a Twist](https://www.codeproject.com/Articles/3479/The-Bip-Buffer-The-Circular-Buffer-with-a-Twist)

## Credits

This package was created with [Cookiecutter](https://github.com/audreyr/cookiecutter) and the [devosoft/cookiecutter-empirical-project](https://github.com/devosoft/cookiecutter-empirical-project) project template.

This project is built using the [Empirical C++ library](https://github.com/devosoft/Empirical/).
