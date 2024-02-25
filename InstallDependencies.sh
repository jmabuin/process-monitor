#!/bin/bash

# Generic
CURRENT_FOLDER=$(pwd)
echo $CURRENT_FOLDER

sudo apt install libtool autopoint gettext autoconf automake pkg-config libncurses-dev

echo "Creating third-party folders..."
mkdir -p third-party
mkdir -p third-party/libs

# For libproc2

echo "Installing libproc2 ..."
cd third-party

if ! test -d procps; then
  git clone -b v4.0.4 https://gitlab.com/procps-ng/procps.git
  cd procps
  ./autogen.sh
  ./configure
  make
fi

cd $CURRENT_FOLDER

if ! test -f third-party/libs/libproc2.a; then
  cp third-party/procps/library/.libs/libproc2.a third-party/libs/
fi

# For PAPI
echo "Installing PAPI ..."
cd third-party

if ! test -d papi; then
  curl https://icl.utk.edu/projects/papi/downloads/papi-7.1.0.tar.gz --output papi-7.1.0.tar.gz
  tar xzvf papi-7.1.0.tar.gz
  cd papi-7.1.0/src
  ./configure --with-components="rapl"
  make
fi

cd $CURRENT_FOLDER

if ! test -f third-party/libs/libpapi.a; then
  cp third-party/papi-7.1.0/src/libpapi.a third-party/libs/
fi