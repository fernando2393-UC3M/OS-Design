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

	/* Open device for read and write */

	fd = open(DEVICE_IMAGE, O_RDWR);

	if (fd < 0) {
		fprintf(stderr, "Error: Error opening %s\n", DEVICE_IMAGE);
		return -1;
	}

	/* Blocks calculation */

	int totalBlocks = ceil(deviceSize / BLOCK_SIZE); // Maximum number of blocks --> 5120 --> int (already rounded if necessary)

	int dataMapBlocks = ceil(totalBlocks / BLOCK_SIZE); // Blocks represented per byte

	/* Reminder check */

	// if ((dataMapBlocks % 8) != 0 || dataMapBlocks == 0) { // Increment in one the number of dataMap blocks
	// 	dataMapBlocks = dataMapBlocks / 8;
	// 	dataMapBlocks++;
	// }

	/* Preguntar lo del bloque de arranque */
	int bootBlocks = 1;
	int superblocks = 1;

	/* Preguntar el por que de esto */
	int inodeBlocks = ceil(sizeof(inode)*MAX_FILES / BLOCK_SIZE);
	int inodeMapBlocks = ceil(inodeBlocks / BLOCK_SIZE);

	// total blocks minus those reserved (boot, superblock) and maps and inodes
	int dataBlocks = totalBlocks - bootBlocks - superblocks - inodeMapBlocks - dataMapBlocks - inodeBlocks;

	if (dataBlocks < 0) {
		perror("Error: No enough space available. Try with a bigger size image!\n");
		return -1;
	}

	/* Superblock initialization */

	sblock.magicNumber = MAGIC_NUMBER; // Magic number --> 0x000D5500
	sblock.inodeMapNumBlocks = inodeMapBlocks; // Number of blocks of inode map
	sblock.dataMapNumBlocks = dataMapBlocks; // Number of blocks of data map
	sblock.numInodes = inodeBlocks; // 1 block per inode
	sblock.firstInode = bootBlocks + superblocks + inodeMapBlocks + dataMapBlocks;
	sblock.firstDataBlock = bootBlocks + superblocks + inodeMapBlocks + dataMapBlocks + inodeBlocks;
	sblock.deviceSize = deviceSize;
	memset(sblock.padding, '0', sizeof(sblock.padding)); // Fill with '0' remaining space after substracting busy space (size in metadata.h)

	/* Write superblock to DEVICE_IMAGE */

	bwrite(DEVICE_IMAGE, 1, (char *) &sblock); // Write into DEVICE_IMAGE in position 1 (0 is boot) sblock content

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
