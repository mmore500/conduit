# Pull base image.
FROM ubuntu:16.04

COPY . /opt/conduit

SHELL ["/bin/bash", "-c"]

# Install.
RUN \
  apt-get update -y \
    && \
  apt-get install -y software-properties-common \
    && \
  add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && \
  apt-get update -y \
    && \
  apt-get -y upgrade \
    && \
  echo "configured packaging system"

RUN \
  apt-get install -y \
    g++-8 \
    cmake \
    build-essential \
    python-virtualenv \
    python-pip \
    tar \
    gzip \
    libpthread-stubs0-dev \
    libc6-dbg \
    gdb \
    libopenmpi-dev \
    libopenmpi-bin \
    hdf5-tools \
    libhdf5-10 \
    libhdf5-10-dbg \
    libhdf5-cpp-11 \
    libhdf5-cpp-11-dbg \
    libhdf5-dev \
    libhdf5-doc \
    libhdf5-mpi-dev \
    && \
  echo "installed core dependencies"

RUN \
  apt-get install -y \
    curl \
    git \
    htop \
    man \
    unzip \
    vim \
    nano \
    wget \
    && \
  echo "installed creature comforts"

RUN \
  update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 90 \
  && \
  echo "finalized set up dependency versions"

RUN \ 
  git clone https://github.com/google/benchmark.git
    && \
  git clone https://github.com/google/googletest.git benchmark/googletest
    && \
  cd benchmark
    && \
  mkdir build && cd build
    && \
  cmake ../  
    && \
  make
    && \
  make test
    && \
  make install
    && \
  echo "installed Google benchmark"

RUN \
  cd /opt/conduit \
    && \
  git submodule deinit -f . \
    && \
  git submodule init \
    && \
  git submodule update -f \
    && \
  echo "initialized submodules"

RUN \
  cd /opt/conduit \
    && \
  git remote set-url origin https://github.com/mmore500/conduit.git \
    && \
  echo "switched to https origin remote url"

RUN \
  cd /opt/conduit/ \
    && \
  make \
    && \
  cd /opt/conduit/tests/utility \
    && \
  make \
    && \
  echo "representative compile and tests passed!"

# Define default working directory.
WORKDIR /opt/conduit

CMD ["bash"]
