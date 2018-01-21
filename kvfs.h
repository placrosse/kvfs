/* *Copyright (c) 2018 Taylor Holberton
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/** @file */

#ifndef KVFS_H
#define KVFS_H

#ifdef KVFS_WITH_LIBC
#include <stdio.h>
#endif /* KVFS_WITH_LIBC */

#include <inttypes.h>

/** The current version of the KVFS library.
 * */

#define KVFS_VERSION 0x000000

/** The file signature for KVFS.
 * */

#define KVFS_SIGNATURE 0x7366766b

/** A bad address was passed to
 * a KVFS function.
 * */

#define KVFS_EFAULT 0x01

/** Certain required functionality
 * was not implemented.
 * */

#define KVFS_ENOSYS 0x02

#ifdef __cplusplus
extern "C" {
#endif

/** The disk medium that contains
 * the file system.
 * */

struct kvfs_disk {
	/** Implementation data */
	void *handle;
	/** Assign the position for the next IO operation.
	 * */
	int (*seek)(void *handle, uint64_t offset);
	/** The read callback. This function should handle
	 * reading sizes other than a sector size.
	 * */
	int (*read)(void *handle, void *buf, uint64_t buf_size);
	/** The write callback. This function should handle
	 * writing sizes other than a sector size.
	 * */
	int (*write)(void *handle, const void *buf, uint64_t buf_size);
};

/** Initializes a KVFS disk structure.
 * @param disk An uninitialized disk structure.
 * */

void kvfs_disk_init(struct kvfs_disk *disk);

/** Move the disk to a specific position.
 * @param disk An initialized disk structure.
 * @param offset The new offset of the disk position.
 * @returns Zero on success, a negative error code on
 * failure.
 * */

int kvfs_disk_seek(struct kvfs_disk *disk, uint64_t offset);

/** Writes data to the disk.
 * @param disk An initialized disk structure.
 * @param buf The data to write to disk.
 * @param buf_size The number of bytes to write to disk.
 * @returns Zero on success, a negative error code on failure.
 * */

int kvfs_disk_write(struct kvfs_disk *disk, const void *buf, uint64_t buf_size);

#ifdef KVFS_WITH_LIBC

/** A file which behaves like
 * a disk. This is useful for creating
 * a file system on a regular file.
 * */

struct kvfs_fdisk {
	/** The base disk structure,
	 * which contains all the required
	 * callbacks for the file system. */
	struct kvfs_disk disk;
	/** The file associated with the disk. */
	FILE *handle;
};

/** Initializes a file disk.
 * @param fdisk An initialized file disk.
 * */

void kvfs_fdisk_init(struct kvfs_fdisk *fdisk);

/** Closes a file disk.
 * @param fdisk An initialized file disk.
 * */

void kvfs_fdisk_done(struct kvfs_fdisk *fdisk);

/** Open a disk file.
 * @param fdisk An initialized file disk.
 * @param path The path of the file containing the disk data.
 * @param mode The mode to open the file with.
 * This should be typically be rb+, unless the file doesn't exist.
 * @returns Zero on success, a negative number on failure.
 * */

int kvfs_fdisk_open(struct kvfs_fdisk *fdisk, const char *path, const char *mode);

/** Cast a disk file to a regular disk structure.
 * This is useful for passing a disk structure to
 * the @ref kvfs_set_disk function.
 * @param fdisk An initialized file disk structure.
 * @returns The disk structure for the file disk.
 * This function never returns a null pointer.
 * */

struct kvfs_disk *kvfs_fdisk_cast(struct kvfs_fdisk *fdisk);

#endif /* KVFS_WITH_LIBC */

/** A directory entry.
 * */

struct kvfs_dirent {
	/** Type of entry. */
	unsigned char type;
	/** The name of the entry. */
	char name[256];
};

/** A key-value file system.
 * */

struct kvfs {
	/** A container for the disk handle. */
	struct kvfs_disk *disk;
	/** The location of the file system
	 * on the disk. */
	uint64_t offset;
};

/** Initializes a key-value file system.
 * @param kvfs An uninitialized file system.
 * */

void kvfs_init(struct kvfs *kvfs);

/** Releases a key-value file system. Although
 * the file system does not allocate memory, this
 * function is needed to close all the open file
 * descriptors.
 * @param kvfs An initialized file system.
 * */

void kvfs_done(struct kvfs *kvfs);

/** Format the disk. A disk should be
 * assigned to the file system with @ref kvfs_set_disk
 * before calling this function. The
 * function @ref kvfs_set_offset should
 * also be called if the file system does
 * not begin at the start of the disk.
 * @param kvfs An initialized file system structure.
 * @param size The number of bytes to reserve for the file system.
 * @returns Zero on success, a negative
 * error code on failure.
 * */

int kvfs_format(struct kvfs *kvfs, uint64_t size);

/** Verify that the disk assigned to the
 * file system structure is formatted.
 * @param kvfs An initialized file system structure.
 * @returns Zero if the disk is formatted,
 * a negative error code if it is not.
 * */

int kvfs_verify(struct kvfs *kvfs);

/** Set the disk assigned to the file system.
 * If there is already a disk assigned to this
 * file system structure, then @ref kvfs_done
 * and then @ref kvfs_init should be called first.
 * If the file system is not located at the beginning
 * of the disk, then @ref kvfs_set_offset should be
 * called after this function.
 * @param kvfs An initialized file system.
 * @param disk The new disk to find the file
 * system on.
 * */

void kvfs_set_disk(struct kvfs *kvfs, struct kvfs_disk *disk);

/** Set the offset of the file system
 * on the disk. This allows the library
 * to find the file system properly.
 * @param kvfs An initialized file system.
 * @param offset The offset to find the file
 * system at.
 * */

void kvfs_set_offset(struct kvfs *kvfs, uint64_t offset);

/** Opens a regular file.
 * @param kvfs An initialized file system.
 * @param path The path of the file.
 * @param flags Flags to assign the open file.
 * @returns The file descriptor on success, a negative error code on failure.
 * */

int kvfs_open(struct kvfs *kvfs, const char *path, int flags);

/** Open a directory.
 * @param kvfs An initialized file system.
 * @param path The path of the directory to open.
 * @returns A positive file descriptor for the directory on success.
 * On failure, a negative error code.
 * */

int kvfs_opendir(struct kvfs *kvfs, const char *path);

/** Closes a file descriptor.
 * @param kvfs An initialized file system.
 * @param fd A valid file descriptor.
 * @returns Zero on success, a negative error code on failure.
 * */

int kvfs_close(struct kvfs *kvfs, int fd);

/** Removes a regular file from the file system.
 * @param kvfs An initialized file system.
 * @param path The path of the file to delete.
 * @returns Zero on success, a negative number on failure.
 * */

int kvfs_rm(struct kvfs *kvfs, const char *path);

/** Removes a directory form the file system.
 * @param kvfs An initialized file system.
 * @param path The path of the directory to remove.
 * @returns Zero on success, a negative number on failure.
 * */

int kvfs_rmdir(struct kvfs *kvfs, const char *path);

/** Create a directory.
 * @param kvfs An initialized file system.
 * @param path The path to the directory.
 * @param mode The mode to create the directory with.
 * @returns Zero on success, a negative number on failure.
 * */

int kvfs_mkdir(struct kvfs *kvfs, const char *path, int mode);

/** Read data from a file.
 * @param kvfs An initialized file system.
 * @param fd The file descriptor to read from.
 * @param buf The buffer to put the data in.
 * @param buf_size The number of bytes to read.
 * @returns Zero on success, a negative number on failure.
 * */

int kvfs_read(struct kvfs *kvfs, int fd, void *buf, uint64_t buf_size);

/** Read a directory entry from a directory.
 * @param kvfs An initialized file system.
 * @param fd The file descriptor of the directory.
 * @param dirent The directory entry structure to put the info into.
 * @returns Zero on success, a negative number on failure.
 * */

int kvfs_readdir(struct kvfs *kvfs, int fd, struct kvfs_dirent *dirent);

/** Write to a file.
 * @param kvfs An initialized file system.
 * @param fd The file descriptor of the file.
 * @param buf The buffer containing the data to write.
 * @param buf_size The number of bytes to write.
 * @returns Zero on success, a negative number on failure.
 * */

int kvfs_write(struct kvfs *kvfs, int fd, const void *buf, uint64_t buf_size);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* KVFS_H */
