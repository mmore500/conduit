# Pull base image.
FROM ubuntu:18.04

COPY . /opt/conduit

SHELL ["/bin/bash", "-c"]

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

# Install.
RUN \
  apt-get update -qq \
    && \
  echo "initialized packaging system"

RUN \
  apt-get install -qq \
    curl=7.58.0-2ubuntu3.10 \
    git=1:2.17.1-1ubuntu0.7 \
    gzip=1.6-5ubuntu1 \
    unzip=6.0-21ubuntu1 \
    tar=1.29b-2ubuntu0.1 \
    wget=1.19.4-1ubuntu2.2 \
    && \
  echo "installed fundamentals"

# adapted in part from https://askubuntu.com/a/916451
RUN \
  rm /etc/apt/apt.conf.d/docker-gzip-indexes \
    && \
  apt-get purge apt-show-versions \
    && \
  rm /var/lib/apt/lists/*lz4 \
    && \
  apt-get -o Acquire::GzipIndexes=false update \
    && \
  apt-get install -qq \
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
  apt-get upgrade -qq \
    && \
  echo "configured packaging system"

RUN \
  apt-get install -qq \
    g++-8=8.4.0-1ubuntu1~18.04 \
    libclang-7-dev=1:7.1.0~svn353565-1~exp1~20190408084827.60 \
    llvm-7=1:7.1.0~svn353565-1~exp1~20190408084827.60 \
    clang-7=1:7.1.0~svn353565-1~exp1~20190408084827.60 \
    libstdc++-7-dev=7.5.0-3ubuntu1~18.04 \
    cmake=3.10.2-1ubuntu2.18.04.1 \
    build-essential=12.4ubuntu1 \
    python-virtualenv=15.1.0+ds-1.1 \
    python3-virtualenv=15.1.0+ds-1.1 \
    python-pip \
    python3-pip \
    libpthread-stubs0-dev=0.3-4 \
    libc6-dbg=2.27-3ubuntu1.2 \
    gdb=8.2-0ubuntu1~18.04 \
    && \
  echo "installed core dependencies"

RUN \
  apt-get install -qq \
    libopenmpi-dev=2.1.1-8 \
    libopenmpi2=2.1.1-8 \
    openmpi-bin=2.1.1-8 \
    openmpi-common=2.1.1-8 \
    openmpi-doc=2.1.1-8 \
    libmpich-dev=3.3~a2-4 \
    libmpich12=3.3~a2-4 \
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
  echo "installed hpc dependencies"

RUN \
  apt-get install -qq \
    nodejs=8.10.0~dfsg-2ubuntu0.4 \
    npm=3.5.2-0ubuntu4 \
    gconf-service=3.2.6-4ubuntu1 \
    libasound2=1.1.3-5ubuntu0.5 \
    libatk1.0-0=2.28.1-1 \
    libc6=2.27-3ubuntu1.2 \
    libcairo2=1.15.10-2ubuntu0.1 \
    libcups2=2.2.7-1ubuntu2.8 \
    libdbus-1-3=1.12.2-1ubuntu1.2 \
    libexpat1=2.2.5-3ubuntu0.2 \
    libfontconfig1=2.12.6-0ubuntu2 \
    libgcc1=1:10.1.0-2ubuntu1~18.04 \
    libgconf-2-4=3.2.6-4ubuntu1 \
    libgdk-pixbuf2.0-0=2.36.11-2 \
    libglib2.0-0=2.56.4-0ubuntu0.18.04.6 \
    libgtk-3-0=3.22.30-1ubuntu4 \
    libnspr4=2:4.18-1ubuntu1 \
    libpango-1.0-0=1.40.14-1ubuntu0.1 \
    libpangocairo-1.0-0=1.40.14-1ubuntu0.1 \
    libstdc++6=10.1.0-2ubuntu1~18.04 \
    libx11-6=2:1.6.4-3ubuntu0.3 \
    libx11-xcb1=2:1.6.4-3ubuntu0.3 \
    libxcb1=1.13-2~ubuntu18.04 \
    libxcomposite1=1:0.4.4-2 \
    libxcursor1=1:1.1.15-1 \
    libxdamage1=1:1.1.4-3 \
    libxext6=2:1.3.3-1 \
    libxfixes3=1:5.0.3-1 \
    libxi6=2:1.7.9-1 \
    libxrandr2=2:1.5.1-1 \
    libxrender1=1:0.9.10-1 \
    libxss1=1:1.2.2-1 \
    libxtst6=2:1.2.3-1 \
    ca-certificates=20190110~18.04.1 \
    fonts-liberation=1:1.07.4-7~18.04.1 \
    libappindicator1=12.10.1+18.04.20180322.1-0ubuntu1 \
    libnss3=2:3.35-2ubuntu2.12 \
    lsb-release=9.20170808ubuntu1 \
    xdg-utils=1.1.2-1ubuntu2.3 \
    && \
  echo "installed web dependencies"

# magic from https://github.com/puppeteer/puppeteer/issues/3451#issuecomment-523961368
RUN echo 'kernel.unprivileged_userns_clone=1' > /etc/sysctl.d/userns.conf

RUN \
  apt-get install -qq \
    man \
    vim=2:8.0.1453-1ubuntu1.3 \
    nano=2.9.3-2 \
    emacs=47.0 \
    htop=2.1.0-3 \
    && \
  echo "installed creature comforts"

RUN \
  pip3 install -r /opt/conduit/third-party/requirements.txt \
    && \
  echo "installed Python packages"

RUN \
  apt-get install -qq \
    doxygen=1.8.13-10 \
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
  cd /opt/conduit/ \
    && \
  cd third-party \
    && \
  ./install_dependencies.sh \
    && \
  echo "installed third party dependencies"

# Use mimalloc override within the container.
ENV LD_PRELOAD=/usr/local/lib/mimalloc-1.6/libmimalloc.so

RUN \
  cd /opt/conduit \
    && \
  git remote set-url origin https://github.com/mmore500/conduit.git \
    && \
  echo "switched to https origin remote url"

# Define default working directory.
WORKDIR /opt/conduit

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
