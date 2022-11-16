FROM ubuntu:20.04

# So programs like tzdata don't stop the image build process
ARG DEBIAN_FRONTEND=noninteractive

# https://stackoverflow.com/a/28406007/802203
# Set the locale
RUN apt-get update &&\
    apt-get install -y locales &&\
    sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    locale-gen

# Some basic tools and
# clang-format for header processing
RUN apt-get update &&\
    apt-get install -y \
        apt-utils \
        wget nano jq \
        git \
        build-essential \
        gdb \
        cmake \
        castxml

# Install python3 and pip3 for tools
RUN apt-get update &&\
    apt-get install -y \
        python3-dev \
        python3-pip
RUN pip3 install yq

# Install 3rd party libraries
# Install fmt as a shared lib
ARG FMT_TAG=7.1.0
WORKDIR /usr/local/src/fmt
RUN cd /usr/local/src &&\
    git clone https://github.com/fmtlib/fmt.git &&\
    cd fmt &&\
    git checkout ${FMT_TAG} &&\
    mkdir build &&\
    cd build &&\
    cmake -DBUILD_SHARED_LIBS=TRUE -DFMT_INSTALL=TRUE -DFMT_DOC=OFF -DFMT_TEST=OFF .. &&\
    make &&\
    make install

# Install plog as a shared lib
ARG PLOG_TAG=master
WORKDIR /usr/local/src/plog
RUN cd /usr/local/src &&\
    git clone https://github.com/SergiusTheBest/plog.git &&\
    cd plog &&\
    git checkout ${PLOG_TAG} &&\
    mkdir build &&\
    cd build &&\
    cmake -DBUILD_SHARED_LIBS=TRUE -DPLOG_BUILD_SAMPLES=OFF -DPLOG_INSTALL=ON .. &&\
    make &&\
    make install

# Install magic enum as a static lib
ARG MAGIC_ENUM_TAG=master
WORKDIR /usr/local/src/magic_enum
RUN cd /usr/local/src &&\
    git clone https://github.com/Neargye/magic_enum.git &&\
    cd magic_enum &&\
    git checkout ${MAGIC_ENUM_TAG} &&\
    mkdir build &&\
    cd build &&\
    cmake -DBUILD_SHARED_LIBS=FALSE -DMAGIC_ENUM_OPT_BUILD_EXAMPLES=OFF -DMAGIC_ENUM_OPT_BUILD_TESTS=OFF -DMAGIC_ENUM_OPT_INSTALL=ON .. &&\
    make &&\
    make install