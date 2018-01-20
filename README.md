# KVFS

KVFS is a key-value file system.

Some highlights of it are:

 - Easily integrated with existing projects. Consists of two files and a permissive license.
 - Designed to run easily in user space and in kernel space.
 - Allocates zero memory, which saves system resources and allows it to be used without a standard C library.
 - Can serialize and deserialize the file system on any medium with the right callbacks (i.e. ram, hard drive, and files.)

### Building

The build system used in KVFS is CMake (although other build systems could easily be ported.)

Build it like so:

```
mkdir build
cd build
cmake ..
cmake --build .
```

To test the project:

```
ctest
```

If CMake generated Makefiles, you can also use standard Makefile targets.

```
make
make test
sudo make install
```
