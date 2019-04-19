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
#include <math.h>

/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */
int mkFS(long deviceSize)
{

	if (deviceSize < (50 * pow(2,10))){ // Minimum size is 50 KiB
		perror("Error: Device size too small!\n");
		return -1;
	}

	if (deviceSize > (10 * pow(2,20))){ // Maximum size is 10 MiB
		perror("Error: Device size too big!\n");
		return -1;
	}

	int fd; // Device descriptor

	/* Open disk image for read and write */
    fd = open(DEVICE_IMAGE, O_RDWR);
    if (fd < 0) {
        perror("Error while opening 'disk.dat'");
        return -1;
    }
    unsigned long diskSize = (unsigned long) lseek(fd, 0, SEEK_END);
    close(fd);

    if (diskSize < deviceSize) {
        fprintf(stderr, "Error in mkFS: Disk too small\n");
        return -1;
    }

	/* No necesitamos bloque de arranque, por lo que no lo añadimos */
	int superblocks = 1;

	/* Blocks calculation */

	int totalBlocks = ceil(deviceSize / BLOCK_SIZE); // Maximum number of blocks --> 5120 --> int (already rounded if necessary)

	/* Preguntar el por que de esto */
	int inodeBlocks = ceil(sizeof(inode_t)*MAX_FILES / BLOCK_SIZE);
	int inodeMapBlocks = ceil(inodeBlocks / BLOCK_SIZE);

	int dataMapBlocks = ceil((totalBlocks-inodeBlocks-inodeMapBlocks) / BLOCK_SIZE); // Blocks represented per byte

	// total blocks minus those reserved (boot, superblock) and maps and inodes
	int dataBlocks = totalBlocks - superblocks - inodeMapBlocks - dataMapBlocks - inodeBlocks;

	if (dataBlocks < 0) {
		perror("Error: No enough space available. Try with a bigger size image!\n");
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
	i_map = (char *) malloc(ceil(MAX_FILES / 8)); /* Bits allocation */
    for (i = 0; i < sblock.numInodes; i++) {
        bitmap_setbit(i_map, i, 0); // Set bit i inside i_map
    }

	/* Allocate space in memory for data blocks map and initialize its elements to 0 */
    b_map = (char *) malloc(ceil(dataBlocks / 8));
    for (i = 0; i < sblock.numDataBlocks; i++) {
        bitmap_setbit(b_map, i, 0); // Set bit i inside b_map
    }

    /* Initialize array of iNodes to 0 */
    for (i = 0; i < sblock.numInodes; i++) {
        memset(&(inodes[i]), 0, sizeof(inode_t));
    }

    /* Call fssync() to write all the metadata created in the disk */
    if (fssync() < 0) {
        perror("mkFS failed: Error when writing metadata\n");
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
	return -1;
}

/*
 * @brief 	Unmounts the file system from the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int unmountFS(void)
{
	return -1;
}

/*
 * @brief	Creates a new file, provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the file already exists, -2 in case of error.
 */
int createFile(char *path)
{
	return -2;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *path)
{
	return -2;
}

/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *path)
{
	return -2;
}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{
	return -1;
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{
	return -1;
}

/*
 * @brief	Writes a number of bytes from a buffer and into a file.
 * @return	Number of bytes properly written, -1 in case of error.
 */
int writeFile(int fileDescriptor, void *buffer, int numBytes)
{
	return -1;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{
	return -1;
}

/*
 * @brief	Creates a new directory provided it it doesn't exist in the file system.
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
 * @brief   Writes the metadata in memory to the disk image
 * @return  0 if success, -1 if error
 */
int fssync(void) {
    int i;
    /* Write super into disk */
    bwrite(DEVICE_IMAGE, 0, (char *) &sblock);

    /* Write inode map to disk */
    for (i = 0; i < sblock.numINodeMapBlocks; i++) {
        bwrite(DEVICE_IMAGE, 1 + i, ((char *) i_map + i * BLOCK_SIZE));
    }

    /* Write block map to disk */
    for (i = 0; i < sblock.numDataMapBlocks; i++) {
        bwrite(DEVICE_IMAGE, 1 + i + sblock.numINodeMapBlocks, ((char *) b_map + i * BLOCK_SIZE));
    }

    /* Write inodes to disk */
    for (i = 0; i < (sblock.numInodes * sizeof(inode_t) / BLOCK_SIZE); i++) {
        bwrite(DEVICE_IMAGE, i + sblock.firstInodeBlock, ((char *) inodes + i * BLOCK_SIZE));
    }

    return 0;
}
