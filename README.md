# KVFS

KVFS is a key-value file system.

Some highlights of it are:

 - Easily integrated with existing projects. Consists of two files and a permissive license.
 - Designed to run easily in user space and in kernel space.
 - Allocates zero memory, which saves system resources and allows it to be used without a standard C library.
 - Can serialize and deserialize the file system on any medium with the right callbacks (i.e. ram, hard drive, and files.)

### Building

All the standard targets are supported with GNU Make:

```bash
# Build the library.
make

# Run the tests.
make test

# Install the project
sudo make install PREFIX=/opt/kvfs

```
