Terms of KVFS
=============

There are several names used to describe portions of the KVFS structure that are used.

### Allocation Table

The allocation table is used to keep track of what portions of the disk are currently being used (including the allocation table itself).
When the file system is first formatted, it resides right after the header.
As the table grows and resizes itself, it moves around different areas of the disk.

### Key-Value Table

The key-value table is used to lookup the location of all files, directories, symbolic links and pipes within the file system.

### File Descriptor Table

The table containing all currently open file descriptors.
Typically kernels will store this in memory, but KVFS stores it on disk.
Performance hits may be overcome by caching this location into memory.
