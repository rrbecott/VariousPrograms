#include "recordio.h"
#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/* You must use this struct when constructing records. It is
 * expected that all binary data in the index file will follow
 * this format. Do not modify the struct.
 */
struct record_descriptor
{
	int position; //byte offset relative to the beginning of the data file
	int length;   //length of record in bytes
};

/* rio_open
 * Open data file and index file. If create is requested, open both 
 * the data file and the index file with O_CREAT. If O_RDONLY or
 * O_RDWR is requested, make sure that the index file is present and 
 * return an error otherwise. On success the data file's descriptor
 * should be returned and the file descriptor for the index file
 * must be maintained within the abstraction.
 */
int rio_open(const char *pathname, int flags, mode_t mode){

        char *rinx = ".rinx.";
        char *inxf = (char *) malloc(strlen(pathname) + 7);
        strcpy(inxf, rinx);
        strcat(inxf, pathname);

	int inx = open(inxf, flags, mode);
	if ((flags & O_RDONLY) == O_RDONLY || (flags & O_RDWR) == O_RDWR){
		if (inx < 0){
			return -1;
		}
	}
	if ((flags & O_TRUNC) == O_TRUNC){
		if (inx < 0){
			int inx = open(inxf, O_CREAT, mode);
		}
	}
	int file = open(pathname, flags, mode);
	if (inx < 0 || file < 0){
		return -1;
	}
	int encoded = (file << 16) | inx;
	return encoded; //replace with correct return value
}

/* rio_read
 * Allocate a buffer large enough to hold the requested record, read 
 * the record into the buffer and return the pointer to the allocated 
 * area. The I/O result should be returned through the return_value 
 * argument. On success this will be the number of bytes read into
 * the allocated buffer. If any system call returns an error, this
 * should be communicated to the caller through return_value.
 */
void *rio_read(int fd, int *return_value){
	int file = (fd >> 16) & 0xFFFF;
	int inx = (fd) & 0xFFFF;
	struct record_descriptor rd;
	ssize_t inxreadnum = read(inx, &rd, sizeof(struct record_descriptor));
	if (inxreadnum < 0){
		return (void *) -1;
	}
	char *buf = malloc(rd.length + 1);
	ssize_t filereadnum = read(file, buf, rd.length);
	if (filereadnum < 0){
		return (void *) -1;
	}
	buf[rd.length] = '\0';
	*return_value = filereadnum;
	return buf;
}

/* rio_write
 * Write a new record. If appending to the file, create a record_descriptor 
 * and fill in the values. Write the record_descriptor to the index file and 
 * the supplied data to the data file for the requested length. If updating 
 * an existing record, read the record_descriptor, check to see if the new 
 * record fits in the allocated area and rewrite. Return an error otherwise.
 */
int rio_write(int fd, const void*buf, int count){
	int file = (fd >> 16) & 0xFFFF;
        int inx = (fd) & 0xFFFF;
	struct record_descriptor rd;
        ssize_t inxreadnum = read(inx, &rd, sizeof(struct record_descriptor));
	if (inxreadnum <= 0){
		rd.length = count;
		off_t lseeknumfile = lseek(file, 0, SEEK_CUR);
		rd.position = lseeknumfile;
		ssize_t writenumfile = write(file, buf, count);
		ssize_t writenuminx = write(inx, &rd, sizeof(struct record_descriptor));
		if (lseeknumfile < 0 || writenumfile < 0 || writenuminx < 0){
			return -1;
		}
		return writenumfile;
	} else {
		if (count <= rd.length){
			off_t lseeknuminx = lseek(inx, -1*sizeof(struct record_descriptor), SEEK_CUR);
			ssize_t writenumfile = write(file, buf, count);
			rd.length = count;
			ssize_t writenuminx = write(inx, &rd, sizeof(struct record_descriptor));
			if (writenumfile < 0 || writenuminx < 0){
                        	return -1;
                	}
			//off_t lseeknumfile = lseek(file, count, SEEK_CUR);
                	//lseeknuminx = lseek(inx, sizeof(struct record_descriptor), SEEK_CUR);
                	//if (lseeknumfile < 0 || lseeknuminx < 0){
                        //	return -1;
                	//}
			return writenumfile;
		} else {
			return -1;
		}
	}
}

/* rio_lseek
 * Seek both files (data and index files) to the beginning of the requested 
 * record so that the next I/O is performed at the requested position. The
 * offset argument is in terms of records not bytes (relative to whence).
 * whence assumes the same values as lseek whence argument.
 *
 */
int rio_lseek(int fd, int offset, int whence){
	int file = (fd >> 16) & 0xFFFF;
        int inx = (fd) & 0xFFFF;
	off_t lseeknuminx = lseek(inx, (offset * sizeof(struct record_descriptor)), whence);
	if (lseeknuminx < 0){
		return -1;
	}
	struct record_descriptor rd;
        ssize_t inxreadnum = read(inx, &rd, sizeof(struct record_descriptor));
	off_t lseeknumfile = lseek(file, rd.position, SEEK_SET);
	if (lseeknumfile < 0 || inxreadnum < 0){
                return -1;
        }
	lseeknuminx = lseek(inx, (offset * sizeof(struct record_descriptor)), whence);
	if (lseeknuminx < 0){
                return -1;
        }
	int returnnum = lseeknuminx / sizeof(struct record_descriptor);
	return returnnum;
}

/* rio_close
 * Close both files. Even though a single integer is passed as an argument, 
 * your abstraction must close the other file as well. It is suggested 
 * in rio_open that you return the descriptor obtained by opening the data file 
 * to the user and keep the index file descriptor number in the 
 * abstraction and associate them. You may also shift and pack them together 
 * into a single integer.
 */
int rio_close(int fd){
	int file = (fd >> 16) & 0xFFFF;
        int inx = (fd) & 0xFFFF;
	int inxclose = close(inx);
	int fileclose = close(file);
	if (inxclose == -1 || fileclose == -1){
		return -1;
	}
	return 1;
}
