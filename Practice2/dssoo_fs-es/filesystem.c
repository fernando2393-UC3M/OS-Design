/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	filesystem.c
 * @brief 	Implementation of the core file system funcionalities and auxiliary functions.
 * @date	01/03/2017
 */

#include "include/filesystem.h" // Headers for the core functionality
#include "include/auxiliary.h"  // Headers for auxiliary functions
#include "include/metadata.h"   // Type and structure declaration of the file system
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <libgen.h>

/*
 * @brief   Implementation of a math.ceil funcion of a division
 * @return  int with the result of applying ceil(a/b)
 */
int ceilOfDivision (long a, long b) {
	return (a / b) + ((a % b) != 0);
}

/*
 * @brief   Counts the number of entries that a specific inode has
 * @return  int with the number of entries that a specific inode has
 */
int countNumberEntries (int inode_id){
	int i, count = 0;

    for (i = 0; i < sblock.numInodes; i++) {
        if (inodes[i].father == inode_id) {
            count ++;
        }
    }
	return count;
}

/*
 * @brief   Gets the father route
 * @return  char * with the next directory from root.
 */
char * getFather(char * path) {
	
	char * directories = strdup(path);
	char * father = dirname(directories);

	return father;
}

/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */
int mkFS(long deviceSize)
{

	if (deviceSize < (50 * pow(2,10))){ // Minimum size is 50 KiB
		fprintf(stderr, "Error in mkFS: device size too small\n");
		return -1;
	}

	if (deviceSize > (10 * pow(2,20))){ // Maximum size is 10 MiB
		fprintf(stderr, "Error in mkFS: device size too big\n");
		return -1;
	}

	int fd; // Device descriptor

	/* Open disk image for read and write */
    fd = open(DEVICE_IMAGE, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Error in mkFS: while opening %s\n", DEVICE_IMAGE);
        return -1;
    }
    unsigned long diskSize = (unsigned long) lseek(fd, 0, SEEK_END);
    close(fd);

    if (diskSize < deviceSize) {
        fprintf(stderr, "Error in mkFS: disk is too small\n");
        return -1;
    }

	/* No necesitamos bloque de arranque, por lo que no lo añadimos */
	int superblocks = 1;

	/* Blocks calculation */

	int totalBlocks = ceilOfDivision(deviceSize, BLOCK_SIZE); // Maximum number of blocks --> 5120 --> int (already rounded if necessary)

	/* Preguntar el por que de esto */
	int inodeBlocks = ceilOfDivision(sizeof(inode_t)*MAX_FILES, BLOCK_SIZE);
	int inodeMapBlocks = ceilOfDivision(inodeBlocks, BLOCK_SIZE);

	int dataMapBlocks = ceilOfDivision((totalBlocks-inodeBlocks-inodeMapBlocks), BLOCK_SIZE); // Blocks represented per byte

	// total blocks minus those reserved (boot, superblock) and maps and inodes
	int dataBlocks = totalBlocks - superblocks - inodeMapBlocks - dataMapBlocks - inodeBlocks;

	if (dataBlocks < 0) {
		fprintf(stderr, "Error in mkFS: not enough space available. Try with a bigger size image!\n");
		return -1;
	}

	/* Superblock initialization */

	sblock.magicNumber = MAGIC_NUMBER; // Magic number --> 0x000D5500
	sblock.numINodeMapBlocks = inodeMapBlocks; // Number of blocks of inode map
	sblock.numDataMapBlocks = dataMapBlocks; // Number of blocks of data map
	sblock.numInodes = MAX_FILES; // 1 inode per file
	sblock.firstInodeBlock = superblocks + inodeMapBlocks + dataMapBlocks;
	sblock.numDataBlocks = dataBlocks;
	sblock.firstDataBlock = superblocks + inodeMapBlocks + dataMapBlocks + inodeBlocks;
	sblock.deviceSize = deviceSize;
	memset(sblock.padding, '0', sizeof(sblock.padding)); // Fill with '0' remaining space after substracting busy space (size in metadata.h)

	int i;

	 /* Allocate space in memory for inodes map and initialize its elements to 0 */
	i_map = (char *) malloc(sblock.numINodeMapBlocks*BLOCK_SIZE); /* Bits allocation */
    for (i = 0; i < sblock.numInodes; i++) {
        bitmap_setbit(i_map, i, 0); // Set bit i inside i_map
    }

	/* Allocate space in memory for data blocks map and initialize its elements to 0 */
    b_map = (char *) malloc(sblock.numDataMapBlocks*BLOCK_SIZE);
    for (i = 0; i < sblock.numDataBlocks; i++) {
        bitmap_setbit(b_map, i, 0); // Set bit i inside b_map
    }

    /* Initialize array of iNodes to 0 */
    for (i = 0; i < sblock.numInodes; i++) {
        memset(&(inodes[i]), 0, sizeof(inode_t));
    }

    /* Call syncFS() to write data in memory to the disk image */
    if (syncFS() < 0) {
        fprintf(stderr, "Error in mkFS: failed to write data to the disk image\n");
        return -1;
    }

	return 0;
}

/*
 * @brief 	Mounts a file system in the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int mountFS(void)
{
    /* Read superblock (disk block 0) and store it into sblock */
    if (bread(DEVICE_IMAGE, 0, (char *) &sblock) < 0) {
        fprintf(stderr, "Error in mountFS: superblock cannot be read\n");
        return -1;
    }

	int i;
    /* Read from disk inode map */
    for (i = 0; i < sblock.numINodeMapBlocks; i++) {
        if (bread(DEVICE_IMAGE, 1 + i, ((char *) i_map + i * BLOCK_SIZE)) < 0) {
            fprintf(stderr, "Error in mountFS: can't read inodes map\n");
            return -1;
        }
    }

    /* Read disk block map */
    for (i = 0; i < sblock.numDataMapBlocks; i++) {
        if (bread(DEVICE_IMAGE, 1 + i + sblock.numINodeMapBlocks, ((char *) b_map + i * BLOCK_SIZE)) < 0) {
            fprintf(stderr, "Error in mountFS: can't read data block map\n");
            return -1;
        }
    }

    /* Read inodes from disk */
    for (i = 0; i < ceilOfDivision(sblock.numInodes * sizeof(inode_t), BLOCK_SIZE); i++) {
        if (bread(DEVICE_IMAGE, i + sblock.firstInodeBlock, ((char *) inodes + i * BLOCK_SIZE)) < 0) {
            fprintf(stderr, "Error in mountFS: can't read iNodes\n");
            return -1;
        }
    }

    return 0;
}

/*
 * @brief 	Unmounts the file system from the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int unmountFS(void)
{
	int i;

    /* Check if any file still opened */
    for (i = 0; i < sblock.numInodes; i++) {
        if (inodes_x[i].opened == 1) {
            fprintf(stderr, "Error in unmountFS: file %s is opened\n", inodes[i].name);
            return -1;
        }
    }

    /* Call syncFS() to write data in memory to the disk image */
    if (syncFS() < 0) {
        fprintf(stderr, "Error in unmountFS: failed to write data to the disk image\n");
        return -1;
    }

    return 0;
}

/*
 * @brief	Creates a new file, provided that it doesn't exist in the file system.
 * @return	0 if success, -1 if the file already exists, -2 in case of error.
 */
int createFile(char *path)
{
	if (path == NULL)
	{
		fprintf(stderr, "Error createFile\n");
		return -2;
	}

	int b_id;

	// First look for the directory

	int inode_id = namei(path);

	if (inode_id >= 0)
	{
		fprintf(stderr, "Error in createFile: file already exists\n");
		return -1;
	}

	inode_id = ialloc(); // Returns id of available inode

	if (inode_id < 0)
	{
		fprintf(stderr, "Error in createFile: no inodes available\n");
		return -2;
	}

	b_id = alloc();

	if (b_id < 0)
	{
		fprintf(stderr, "Error in createFile: no data blocks available\n");
		return -2;
	}

	/* Check if path includes a directory */

	if (strcmp(getFather(path), "/") != 0) {

			int father_inode_id = namei(getFather(path));

			if (inode_id < 0)
			{
				fprintf(stderr, "Error in createFile: directory does not exist\n");
				return -1;
			}

			if (countNumberEntries(father_inode_id) == MAX_ENTRIES) {
				fprintf(stderr, "Error in createFile: directory does not exist\n");
				return -1;
			}
	}

	inodes[inode_id].type = TYPE_FILE; // Inode points to a file
	strcpy(inodes[inode_id].name, path);
	inodes[inode_id].dataBlockPos = b_id;
	inodes[inode_id].size = MAX_FILE_SIZE;
	inodes_x[inode_id].position = 0;
	inodes_x[inode_id].opened = 0;

	return 0;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *path)
{

	if (path == NULL) {
        fprintf(stderr, "Error removeFile\n");
        return -1;
	}

	int inode_id = namei(path);

	if (inode_id < 0){
		fprintf(stderr, "Error in removeFile: file does not exist\n");
        return -1;
	}

	if (inodes[inode_id].type != TYPE_FILE) {
		fprintf(stderr, "Error removeFile: not a file\n");
		return -2;
	}

	if (inodes_x[inode_id].opened != 0) {
		fprintf(stderr, "Error removeFile: file is opened!\n");
		return -2;
	}

	bfree(inodes[inode_id].dataBlockPos); // Free data block
	memset(&(inodes[inode_id]), 0, sizeof(inode_t));
	ifree(inode_id);

	return 0;
}


/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *path){

	if (path == NULL) {
        fprintf(stderr, "Error openFile\n");
        return -1;
	}

	int inode_id = namei(path);

	if (inode_id < 0)
	{
		fprintf(stderr, "Error in openFile: file does not exist\n");
		return -1;
	}

	if (inodes[inode_id].type != TYPE_FILE)
	{
		fprintf(stderr, "Error openFile: not a file\n");
		return -2;
	}

	if (inodes_x[inode_id].opened != 0)
	{
		fprintf(stderr, "Error openFile: file is already opened!\n");
		return -2;
	}

	inodes_x[inode_id].position = 0;
	inodes_x[inode_id].opened = 1;

	return inode_id;

}

/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */

/*
int openFile(char *path)
{

	int i, j, isFound;
	char *found, *prevFound;


	while( (found = strsep(&path,"/")) != NULL ) {
		isFound = 0;

		if (prevFound == NULL) {
			prevFound = found;
			continue;
		}

		for (i = 0; i < sblock.numInodes; i++) {
			if (!isFound && !strcmp(inodes[i].name, prevFound)) {
				if (inodes[i].type == TYPE_FOLDER){
					for (j = 0; j < MAX_ENTRIES; j++) {
						if (!isFound && !strcmp(inodes[inodes[i].entradas[j]].name, found)) {
							prevFound = found;
							isFound = 1;
						}
					}
				}
			}
		}

		if (!isFound) {
			fprintf(stderr, "not found!!\n");
			return -1;
		}
	}

	int inode_id;

	*/

	/* Search for the inode of the file */

	/*
	inode_id = namei(prevFound);
	if (inode_id < 0) {
		fprintf(stderr, "Error in openFile: file %s not found\n", path);
		return -1;
	}

	if (inodes[inode_id].type != TYPE_FILE) {
		fprintf(stderr, "Error openFile: not a file\n");
		return -2;
	}

	if (inodes_x[inode_id].opened != 0) {
		fprintf(stderr, "Error openFile: file is already opened!\n");
		return -2;
	}

	*/


	// inodes_x[inode_id].position = 0; /* Set seek descriptor to begin */
	// inodes_x[inode_id].opened = 1;  /* Set file state to open */

	// return inode_id;
// }

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{
	if (fileDescriptor >= sblock.numInodes || fileDescriptor < 0) {
        fprintf(stderr, "Error in closeFile: wrong file descriptor\n");
        return -1;
    }

    inodes_x[fileDescriptor].position = 0; /* Set seek descriptor to begin */
    inodes_x[fileDescriptor].opened = 0;  /* Set file state to closed */

    return 0;
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{
	if (fileDescriptor >= sblock.numInodes || fileDescriptor < 0) {
		fprintf(stderr, "Error in readFile: wrong file descriptor\n");
		return -1;
	}

	if (inodes[fileDescriptor].type != TYPE_FILE) {
      fprintf(stderr, "Error in readFile: not a file\n");
      return -1;
    }

	if (inodes_x[fileDescriptor].opened == 0) {
      fprintf(stderr, "Error in readFile: file not opened\n");
      return -1;
    }

	char b[BLOCK_SIZE] ;
	int b_id;

	if (inodes_x[fileDescriptor].position + numBytes > inodes[fileDescriptor].size) {
		numBytes = inodes[fileDescriptor].size - inodes_x[fileDescriptor].position;
	}
	if (numBytes < 0) {
		fprintf(stderr, "Error in readFile: Segmentation fault\n");
		return -1;
	}

	/* In this case, the seek pointer is located at EOF, so no bytes can be read */
    if (numBytes == 0) {
        return 0;
    }

	b_id = bmap(fileDescriptor, inodes_x[fileDescriptor].position);

	if (b_id < 0) {
		fprintf(stderr, "Error in readFile: error coming from bmap, could not allocate a data block\n");
		return -1;
	}

	if (bread(DEVICE_IMAGE, sblock.firstDataBlock+b_id, b) < 0) {
		fprintf(stderr, "Error in readFile: can't read data block\n");
		return -1;
	}

	memmove(buffer, b+inodes_x[fileDescriptor].position, numBytes);

	inodes_x[fileDescriptor].position += numBytes;

	return numBytes;
}

/*
 * @brief	Writes a number of bytes from a buffer and into a file.
 * @return	Number of bytes properly written, -1 in case of error.
 */
int writeFile(int fileDescriptor, void *buffer, int numBytes)
{
	if (fileDescriptor >= sblock.numInodes || fileDescriptor < 0) {
		fprintf(stderr, "Error in writeFile: wrong file descriptor\n");
		return -1;
	}

	if (inodes[fileDescriptor].type != TYPE_FILE) {
      fprintf(stderr, "Error in writeFile: not a file\n");
      return -1;
    }

	if (inodes_x[fileDescriptor].opened == 0) {
      fprintf(stderr, "Error in writeFile: file not opened\n");
      return -1;
    }

	char b[BLOCK_SIZE] ;
	int b_id;

	if (inodes_x[fileDescriptor].position + numBytes > inodes[fileDescriptor].size) {
		numBytes = inodes[fileDescriptor].size - inodes_x[fileDescriptor].position;
	}
	if (numBytes < 0) {
		fprintf(stderr, "Error in writeFile: Segmentation fault\n");
		return -1;
	}

	/* In this case, the seek pointer is located at EOF, so no bytes
     * can be read */
    if (numBytes == 0) {
        return 0;
    }

	b_id = bmap(fileDescriptor, inodes_x[fileDescriptor].position);

	if (b_id < 0) {
		fprintf(stderr, "Error in writeFile: error coming from bmap, could not allocate a data block\n");
		return -1;
	}

	if (bread(DEVICE_IMAGE, sblock.firstDataBlock+b_id, b) < 0) {
		fprintf(stderr, "Error in writeFile: can't read data block\n");
		return -1;
	}

	memmove(b+inodes_x[fileDescriptor].position, buffer, numBytes);

	if (bwrite(DEVICE_IMAGE, sblock.firstDataBlock+b_id, b) < 0) {
		fprintf(stderr, "Error in writeFile: can't write data block\n");
		return -1;
	}

	inodes_x[fileDescriptor].position += numBytes;

	return numBytes;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{

	if (fileDescriptor >= sblock.numInodes || fileDescriptor < 0) {
		fprintf(stderr, "Error in lseekFile: wrong file descriptor\n");
		return -1;
	}

	if (inodes[fileDescriptor].type != TYPE_FILE) {
      fprintf(stderr, "Error in lseekFile: not a file\n");
      return -1;
    }

	if (inodes_x[fileDescriptor].opened == 0) {
      fprintf(stderr, "Error in lseekFile: file not opened\n");
      return -1;
    }

	if (whence == FS_SEEK_BEGIN){
		inodes_x[fileDescriptor].position = 0;
		return 0;
	}

	if (whence == FS_SEEK_END) {
		inodes_x[fileDescriptor].position = inodes[fileDescriptor].size;
		return 0;
	}

	if (whence == FS_SEEK_CUR){
		if (inodes_x[fileDescriptor].position + offset > inodes[fileDescriptor].size) {
			fprintf(stderr, "Error in lseekFile: Te has pasado por alante\n");
			return -1;
		}
		if (inodes_x[fileDescriptor].position + offset < 0) {
			fprintf(stderr, "Error in lseekFile: Te has pasado por atras\n");
			return -1;
		}
		inodes_x[fileDescriptor].position += offset;
		return 0;
	}

	fprintf(stderr, "Error in lseekFile: did not provide a valid value for whence\n");
	return -1;
}

/*
 * @brief	Creates a new directory provided it doesn't exist in the file system.
 * @return	0 if success, -1 if the directory already exists, -2 in case of error.
 */
int mkDir(char *path)
{
	return -2;
}

/*
 * @brief	Deletes a directory, provided it exists in the file system.
 * @return	0 if success, -1 if the directory does not exist, -2 in case of error..
 */
int rmDir(char *path)
{
	return -2;
}

/*
 * @brief	Lists the content of a directory and stores the inodes and names in arrays.
 * @return	The number of items in the directory, -1 if the directory does not exist, -2 in case of error..
 */
int lsDir(char *path, int inodesDir[10], char namesDir[10][33])
{
	return -2;
}


/*
 * @brief   Search for a free inode and set its value in inodes map to 1
 * @return  ID of the inode available if any, -1 if not
 */
int ialloc(void) {
    int i;
    /* To search for a free inode */
    for (i = 0; i < sblock.numInodes; i++) {
        if (bitmap_getbit(i_map, i) == 0) {
            /* inode busy right now */
            bitmap_setbit(i_map, i, 1);
            /* Default values for the inode */
            memset(&(inodes[i]), 0, sizeof(inode_t));
            /* Return the inode identification */
            return i;
        }
    }

    return -1;
}

/*
 * @brief   Search for a free data block, set its bytes to 0 and set its
 *          value in data blocks map to 1
 * @return  ID of the data block available if any, -1 if not
 */
int alloc(void) {
    int i;
    char buffer[BLOCK_SIZE];

    for (i = 0; i < sblock.numDataBlocks; i++) {
        if (bitmap_getbit(b_map, i) == 0) {
            /* busy block right now */
            bitmap_setbit(b_map, i, 1);

            /* default values for the block */
            memset(buffer, 0, BLOCK_SIZE);
            if (bwrite(DEVICE_IMAGE, i + sblock.firstDataBlock, buffer) < 0) {
				return -1;
			};
            /* it returns the block id */
            return i;
        }
    }

    return -1;
}

/*
 * @brief   Sets the value of the inode provided to 0 in inodes map
 * @return  0 if success, -1 if inode not found
 */
int ifree(int inode_id) {
    /* to check the inode_id vality */
    if (inode_id >= sblock.numInodes || inode_id < 0) {
        return -1;
    }

    /* free inode */
    bitmap_setbit(i_map, inode_id, 0);

    return 0;
}

/*
 * @brief   Sets the value of the data block provided to 0 in data block map
 * @return  0 if success, -1 if data block not found
 */
int bfree(int block_id) {
    /* to check the inode_id vality */
    if (block_id >= sblock.numDataBlocks || block_id < 0) {
        return -1;
    }

    /* free data block */
    bitmap_setbit(b_map, block_id, 0);

    return 0;
}

/*
 * @brief   Found the inode ID containing the file passed
 * @return  ID of the inode, -1 if not found
 */
int namei(char *fname) {

	if (fname == NULL) {
        return -1;
	}

	int i;
    /* seek for the inode with name <fname> */
    for (i = 0; i < sblock.numInodes; i++) {
        if (!strcmp(inodes[i].name, fname)) {
            return i;
        }
    }

    return -1;
}

/*
 * @brief   Return the index of the data block that contains the byte indicated by the offset
 * @return  Index of the block, -1 if not found
 */
int bmap(int inode_id, int offset) {
	/* to check the inode_id vality */
    if (inode_id >= sblock.numInodes || inode_id < 0) {
        return -1;
    }

	if (offset < BLOCK_SIZE) {
		return inodes[inode_id].dataBlockPos;
	}

	return -1;
}

/*
 * @brief   Writes data in memory to the disk image
 * @return  0 if success, -1 if error
 */
int syncFS(void) {
    int i;

    /* Write superblock into disk */
    if (bwrite(DEVICE_IMAGE, 0, (char *) &sblock) < 0) {
		return -1;
	}

    /* Write inode map to disk */
    for (i = 0; i < sblock.numINodeMapBlocks; i++) {
        if (bwrite(DEVICE_IMAGE, 1 + i, ((char *) i_map + i * BLOCK_SIZE)) < 0) {
			return -1;
		}
    }

    /* Write block map to disk */
    for (i = 0; i < sblock.numDataMapBlocks; i++) {
        if (bwrite(DEVICE_IMAGE, 1 + i + sblock.numINodeMapBlocks, ((char *) b_map + i * BLOCK_SIZE)) < 0) {
			return -1;
		}
    }

    /* Write inodes to disk */
    for (i = 0; i < ceilOfDivision(sblock.numInodes * sizeof(inode_t), BLOCK_SIZE); i++) {
        if (bwrite(DEVICE_IMAGE, i + sblock.firstInodeBlock, ((char *) inodes + i * BLOCK_SIZE)) < 0) {
			return -1;
		}
    }

    return 0;
}
