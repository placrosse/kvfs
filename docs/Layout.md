On-Disk Layout
==============

The start of the file system may be loaded anywhere, but it would typically be found at the beginning of a disk or partition.

All values are encoded as little-endian and are unsigned.

### Header

The following table describes the header of the file system, located at the start of the disk, partition, or disk file.

| Field Name              | Size | Offset | Description                                                              |
|-------------------------|------|--------|--------------------------------------------------------------------------|
| Signature               | 4    | 0      | Used for checking for the presence of KVFS. This field should be 'kvfs'. |
| Version                 | 4    | 4      | The version of the this header layout.                                   |
| Allocation Table Offset | 8    | 8      | The location on disk where the space allocation table resides.           |
| Key-Value Table Offset  | 8    | 16     | The location on disk where the key-value table resides.                  |
| File Descriptor Table   | 8    | 24     | The location of the open file descriptor table.                          |
| CRC32 Header Checksum   | 4    | 32     | The CRC32 checksum of this header (is zero during calculation.)          |

### Allocation Table

The allocation table begins with a header followed by the entries.

When the allocation table is first created, the entry immediately following the header is the entry for the allocation table itself.

The following table describes the header of the allocation table.

| Field Name              | Size | Offset | Description                                                              |
|-------------------------|------|--------|--------------------------------------------------------------------------|
| Entry Count             | 8    | 0      | The number of entries in the allocation table.                           |

The following table describes an entry in the allocation table.

| Field Name              | Size | Offset | Description                                                              |
|-------------------------|------|--------|--------------------------------------------------------------------------|
| Offset                  | 8    | 0      | The location, on disk, where the entry data resides.                     |
| Size                    | 8    | 8      | The number of bytes allocated by the entry data.                         |

### Key-Value Table

The key-value table begins with a header and is followed by the table entries.

The following table describes the header of the key-value table.

| Field Name              | Size | Offset | Description                                                              |
|-------------------------|------|--------|--------------------------------------------------------------------------|
| Entry Count             | 8    | 0      | The number of entries in the allocation table.                           |
| Hash Algorithm          | 8    | 8      | The code that indicates which algorithm to use.                          |

The following table describes an entry in the key-value table.
Note that the choice of the hash algorithm will determine how big the hash code is at the beginning of an entry.

| Field Name              | Size | Offset | Description                                                              |
|-------------------------|------|--------|--------------------------------------------------------------------------|
| Hash Code               | s    | 0      | The hash (determined by the hash algorithm) of the entry.                |
| Type                    | 8    | s      | Indicates whether the entry is a file, directory, symbolic link or pipe. |
| Offset                  | 8    | s + 8  | The offset of the entry within the disk.                                 |

