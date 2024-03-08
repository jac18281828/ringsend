# ringsend

## Description

This is a simple command line utility that creates a fixed number of sockets, each with its own thread.  Each thread reads
arbitrary data and then forwards it to its peer.   At the end of the sequence, the message is forwarded to the beginning
and the cycle continues

## Installation (GNU Make)

To build ringsend:

```bash
make all
```

To run binary:

```bash
./ringsend 3 3000
```

## Installation (CMake)

To build ringsend

```bash
mkdir build
cmake -H. -Bbuild -DPROJECT_NAME=ringsend -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_VERBOSE_MAKEFILE=on -Wno-dev "-GUnix Makefiles"
cmake --build build --verbose --config RELEASE
make -j
(cd build && ctest)
```

To run from build location:

```bash
build/ringsend 3 3000
```

To install from build location:

```bash
sudo make install
```
