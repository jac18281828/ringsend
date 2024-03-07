# ringsend

## Description

This is a simple command line utility that creates a fixed number of sockets, each with its own thread.  Each thread reads
arbitrary data and then forwards it to its peer.   At the end of the sequence, the message is forwarded to the beginning
and the cycle continues

## Installation

To install the utility, you can use the following command:

```bash
mkdir build
RUN cmake -H. -Bbuild -DPROJECT_NAME=ringsend -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_VERBOSE_MAKEFILE=on -Wno-dev "-GUnix Makefiles"
RUN cmake --build build --verbose --config RELEASE
make -j
sudo make install
```