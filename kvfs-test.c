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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

const uint64_t mock_offset = 0x10;

struct mock_disk {
	uint64_t seek_index;
	uint64_t write_index;
	struct kvfs_disk disk;
};

static int mock_seek(void *handle, uint64_t offset) {

	struct mock_disk *disk;

	disk = (struct mock_disk *) handle;

	if (disk->seek_index == 0) {
		assert(offset == mock_offset);
	}

	disk->seek_index++;

	return 0;
}

static int mock_write(void *handle, const void *buf, uint64_t buf_size) {

	struct mock_disk *disk;

	disk = (struct mock_disk *) handle;

	if (disk->write_index == 0) {
		/* signature */
		assert(buf_size == 4);
		assert(memcmp(buf, "kvfs", 4) == 0);
	} else if (disk->write_index == 1) {
		/* version */
		assert(buf_size == 4);
		assert(memcmp(buf, "\x00\x00\x00\x00", 4) == 0);
	} else if (disk->write_index == 2) {
		/* size */
		assert(buf_size == 8);
		assert(memcmp(buf, "\xff\xff\xff\xff\x00\x00\x00\x00", 8) == 0);
	}

	disk->write_index++;

	return 0;
}

static void mock_disk_init(struct mock_disk *mock_disk) {
	kvfs_disk_init(&mock_disk->disk);
	mock_disk->disk.handle = mock_disk;
	mock_disk->disk.seek = mock_seek;
	mock_disk->disk.write = mock_write;
	mock_disk->seek_index = 0;
	mock_disk->write_index = 0;
}

static void mock_disk_done(struct mock_disk *mock_disk) {
	assert(mock_disk->seek_index == 1);
	assert(mock_disk->write_index == 3);
}

int main(void) {

	int err;
	struct kvfs kvfs;
	struct mock_disk mock_disk;

	mock_disk_init(&mock_disk);

	kvfs_init(&kvfs);

	kvfs_set_disk(&kvfs, &mock_disk.disk);

	kvfs_set_offset(&kvfs, mock_offset);

	err = kvfs_format(&kvfs, 0xffffffff);
	assert(err == 0);

	kvfs_done(&kvfs);

	mock_disk_done(&mock_disk);

	return EXIT_SUCCESS;
}
