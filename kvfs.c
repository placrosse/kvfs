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

#include "kvfs.h"

#ifndef NULL
#define NULL ((void *) 0x00)
#endif

void kvfs_disk_init(struct kvfs_disk *disk) {
	disk->handle = NULL;
	disk->read = NULL;
	disk->write = NULL;
	disk->seek = NULL;
}

int kvfs_disk_seek(struct kvfs_disk *disk, uint64_t offset) {

	if (disk->seek == NULL)
		return KVFS_ENOSYS;
	else
		return disk->seek(disk->handle, offset);
}

int kvfs_disk_write(struct kvfs_disk *disk, const void *buf, uint64_t buf_size) {

	if (disk->write == NULL)
		return KVFS_ENOSYS;
	else
		return disk->write(disk->handle, buf, buf_size);
}

void kvfs_init(struct kvfs *kvfs) {
	kvfs->disk = NULL;
}

void kvfs_done(struct kvfs *kvfs) {
	kvfs->disk = NULL;
}

int kvfs_format(struct kvfs *kvfs) {

	int err;

	if (kvfs->disk == NULL)
		return KVFS_EFAULT;

	err = kvfs_disk_seek(kvfs->disk, kvfs->offset);
	if (err != 0)
		return err;

	return 0;
}

void kvfs_set_disk(struct kvfs *kvfs, struct kvfs_disk *disk) {
	if (disk != NULL)
		kvfs->disk = disk;
}

void kvfs_set_offset(struct kvfs *kvfs, uint64_t offset) {
	kvfs->offset = offset;
}
