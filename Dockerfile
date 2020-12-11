# Pull base image.
FROM ubuntu:18.04

COPY . /opt/conduit/

SHELL ["/bin/bash", "-c"]

# Define default working directory.
WORKDIR /opt/conduit

# Prevent interactive time zone config.
# adapted from https://askubuntu.com/a/1013396
ENV DEBIAN_FRONTEND=noninteractive

RUN \
  echo 'Acquire::http::Timeout "60";' >> "/etc/apt/apt.conf.d/99timeout" \
    && \
  echo 'Acquire::ftp::Timeout "60";' >> "/etc/apt/apt.conf.d/99timeout" \
    && \
  echo 'Acquire::Retries "100";' >> "/etc/apt/apt.conf.d/99timeout" \
    && \
  echo "buffed apt-get resiliency"

RUN \
  apt-get update -qq \
    && \
  apt-get install -qq --no-install-recommends \
    curl=7.58.0-2ubuntu3.10 \
    git=1:2.17.1-1ubuntu0.7 \
    gzip=1.6-5ubuntu1 \
    unzip=6.0-21ubuntu1 \
    tar=1.29b-2ubuntu0.1 \
    wget=1.19.4-1ubuntu2.2 \
    gpg-agent=2.2.4-1ubuntu1.3 \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed fundamentals"

# adapted in part from https://askubuntu.com/a/916451
RUN \
  apt-get update -qq \
    && \
  rm /etc/apt/apt.conf.d/docker-gzip-indexes \
    && \
  apt-get purge apt-show-versions \
    && \
  rm /var/lib/apt/lists/*lz4 \
    && \
  apt-get -o Acquire::GzipIndexes=false update \
    && \
  apt-get install -qq --no-install-recommends \
    software-properties-common=0.96.24.32.14 \
    apt-show-versions=0.22.7ubuntu1 \
    && \
  add-apt-repository -y ppa:ubuntu-toolchain-r/test \
    && \
  wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && \
  apt-add-repository "deb https://apt.llvm.org/xenial/ llvm-toolchain-xenial-7 main" \
    && \
  apt-get update -qq \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "configured packaging system"

RUN \
  apt-get update -qq \
    && \
  apt-get install -qq --no-install-recommends \
    g++-8 \
    libclang-7-dev \
    llvm-7 \
    llvm-7-dev \
    clang-7 \
    libstdc++-7-dev \
    build-essential \
    ninja-build \
    python-virtualenv \
    python3-virtualenv \
    python-dev \
    python3-dev \
    python-pip \
    libbenchmark-dev \
    python3-pip \
    python-setuptools \
    python3-setuptools \
    python-wheel \
    python3-wheel \
    libpthread-stubs0-dev \
    libc6-dbg \
    gdb \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed core dependencies"

RUN \
  apt-get update -qq \
    && \
  apt-get install -qq --no-install-recommends \
    libopenmpi-dev=2.1.1-8 \
    libopenmpi2=2.1.1-8 \
    openmpi-bin=2.1.1-8 \
    openmpi-common=2.1.1-8 \
    openmpi-doc=2.1.1-8 \
    mpich=3.3~a2-4 \
    libmpich-dev=3.3~a2-4 \
    libmpich12=3.3~a2-4 \
    openssh-server=1:7.6p1-4ubuntu0.3 \
    hdf5-helpers=1.10.0-patch1+docs-4 \
    hdf5-tools=1.10.0-patch1+docs-4 \
    libhdf5-100=1.10.0-patch1+docs-4 \
    libhdf5-cpp-100=1.10.0-patch1+docs-4 \
    libhdf5-dev=1.10.0-patch1+docs-4 \
    libhdf5-doc=1.10.0-patch1+docs-4 \
    libhdf5-mpi-dev=1.10.0-patch1+docs-4 \
    libhdf5-mpich-100=1.10.0-patch1+docs-4 \
    libhdf5-mpich-dev=1.10.0-patch1+docs-4 \
    libhdf5-openmpi-100=1.10.0-patch1+docs-4 \
    libhdf5-openmpi-dev=1.10.0-patch1+docs-4 \
    libhdf5-serial-dev=1.10.0-patch1+docs-4 \
    python-h5py=2.7.1-2 \
    python3-h5py=2.7.1-2 \
    slurm-client=17.11.2-1build1 \
    multitail=6.4.2-3 \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed hpc dependencies"

RUN \
  apt-get update -qq \
    && \
  apt-get install -qq --no-install-recommends \
    nodejs \
    npm \
    gconf-service \
    libasound2 \
    libatk1.0-0 \
    libc6 \
    libcairo2 \
    libcups2 \
    libdbus-1-3 \
    libexpat1 \
    libfontconfig1 \
    libgcc1 \
    libgconf-2-4 \
    libgdk-pixbuf2.0-0 \
    libglib2.0-0 \
    libgtk-3-0 \
    libnspr4 \
    libpango-1.0-0 \
    libpangocairo-1.0-0 \
    libstdc++6 \
    libx11-6 \
    libx11-xcb1 \
    libxcb1 \
    libxcomposite1 \
    libxcursor1 \
    libxdamage1 \
    libxext6 \
    libxfixes3 \
    libxi6 \
    libxrandr2 \
    libxrender1 \
    libxss1 \
    libxtst6 \
    ca-certificates \
    fonts-liberation \
    libappindicator1 \
    libnss3 \
    lsb-release \
    xdg-utils \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed web dependencies"

# magic from https://github.com/puppeteer/puppeteer/issues/3451#issuecomment-523961368
RUN echo 'kernel.unprivileged_userns_clone=1' > /etc/sysctl.d/userns.conf

RUN \
  apt-get update -qq \
    && \
  apt-get install -qq --no-install-recommends \
    man \
    vim \
    nano=2.9.3-2 \
    emacs=47.0 \
    htop=2.1.0-3 \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed creature comforts"

RUN \
  pip3 install -r /opt/conduit/third-party/requirements.txt \
    && \
  echo "installed Python packages"

RUN \
  apt-get update -qq \
    && \
  apt-get install -qq --no-install-recommends \
    doxygen=1.8.13-10 \
    && \
  apt-get clean \
    && \
  rm -rf /var/lib/apt/lists/* \
    && \
  echo "installed documentation dependencies"

RUN \
  pip3 install -r /opt/conduit/docs/requirements.txt \
    && \
  echo "installed documentation build requirements"

RUN \
  update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 90 \
    && \
  update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-7 90 \
    && \
  update-alternatives --install /usr/bin/llvm-config llvm-config /usr/bin/llvm-config-7 90 \
    && \
  update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-7 90 \
    && \
  update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-7 90 \
    && \
  npm install -g n@6.7.0 \
    && \
  n 12.18.2 \
    && \
  export python="/usr/bin/python" \
    && \
  npm install source-map@0.7.3 \
    && \
  echo "finalized dependency versions"

RUN \
  npm install -g jsonlint@1.6.3 \
    && \
  echo "installed npm dependencies"

RUN \
  cd third-party \
    && \
  ./install_dependencies.sh \
    && \
  echo "installed third party dependencies"

# Use mimalloc override within the container.
ENV LD_PRELOAD=/usr/local/lib/mimalloc-1.6/libmimalloc.so

RUN \
  git remote set-url origin https://github.com/mmore500/conduit.git \
    && \
  echo "switched to https origin remote url"

# Perform any further action as an unprivileged user.
# adapted from https://stackoverflow.com/a/27703359
# and https://superuser.com/a/235398
RUN \
  useradd --create-home --shell /bin/bash user \
    && \
  groupadd group \
    && \
  gpasswd -a user group \
    && \
  chgrp --recursive user /opt \
    && \
  chmod --recursive g+rwx /opt \
    && \
  echo "user added and granted permissions to /opt"

USER user

CMD ["bash"]
