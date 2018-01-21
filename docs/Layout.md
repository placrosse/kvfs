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
