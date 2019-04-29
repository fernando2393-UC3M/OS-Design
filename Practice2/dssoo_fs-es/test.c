/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	test.c
 * @brief 	Implementation of the client test routines.
 * @date	01/03/2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/filesystem.h"

// Color definitions for asserts
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"

#define N_BLOCKS 60					  // Number of blocks in the device
#define DEV_SIZE N_BLOCKS *BLOCK_SIZE // Device size, in bytes

int main()
{

	int ret, ret2, fd1;
	int inodesDir[10];
	char namesDir[10][33];
	char * buffer = malloc(sizeof(char) * 13);
	char * buffer2 = malloc(sizeof(char) * 4);

	fprintf(stdout, "\n%sTest 1: %sTry to make a very small file system\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkFS(51199);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 2: %sTry to make a very big file system\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkFS(1024*1024*11);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 3: %sTry to make a file system bigger than disk.dat\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkFS(1024*1024*8);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	///////
	fprintf(stdout, "%sTest 4: %sTry to make a correct file system\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkFS(DEV_SIZE);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	///////

	fprintf(stdout, "%sTest 5: %sTry to mount file system\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mountFS();
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	///////

	fprintf(stdout, "%sTest 17: %sTry to create a correct simple file\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 9: %sTry to create a non existing simple file\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/folder/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 10: %sTry to create a file with a route including another existing file\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/test.txt/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 11: %sTry to create a file with very long name\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/veryveryveryveryverylongname.txtt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 12: %sTry to create a file with null name\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile(NULL);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 13: %sTry to create a file an existing name\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	///////

	fprintf(stdout, "%sTest 30: %sTry to open a correct simple file\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = openFile("/test.txt");
	fd1 = ret;
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);


	fprintf(stdout, "%sTest 26: %sTry to open an inexistent file\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = openFile("/test232.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 27: %sTry to open a file with null name\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = openFile(NULL);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 28: %sTry to open a file already openned\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = openFile("/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	///////

	fprintf(stdout, "%sTest 32: %sTry to close a descriptor out of range\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = closeFile(-2);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 33: %sTry to close a descriptor of non existing inode\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = closeFile(20);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 36: %sTry to close a correct opened descriptor\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = closeFile(fd1);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);


	fprintf(stdout, "%sTest 34: %sTry to close a descriptor of already closed file\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = closeFile(fd1);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	///////

	fprintf(stdout, "%sTest 22: %sTry to remove a file with null name\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = removeFile(NULL);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 20: %sTry to remove an openned file\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	fd1 = openFile("/test.txt");
	ret = removeFile("/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 24: %sTry to remove a file correctly\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = closeFile(fd1);
	ret = removeFile("/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 21: %sTry to remove a non-existing file\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = removeFile("/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	///////

	fprintf(stdout, "%sTest 25: %sTry to create file that was removed\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 31: %sTry to open file that was removed\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = openFile("/test.txt");
	fd1 = ret;
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);




	fprintf(stdout, "%sTest 70: %sTry to make a folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkDir("/myFolder");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 63: %sTry to make a folder with null name\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkDir(NULL);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 69: %sTry to make a folder with very long name\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkDir("veryveryveryveryveryveryverylongname");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 64: %sTry to make a folder with existing name\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkDir("/myFolder");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 75: %sTry to remove a folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = rmDir("/myFolder");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST rmDir ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST rmDir ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 72: %sTry to remove a folder with null name\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = rmDir(NULL);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST rmDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST rmDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 71: %sTry to make a folder anidated\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkDir("/myFolder");
	ret = mkDir("/myFolder/folderAnin");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 77: %sTry to see the contents of a folder with null name\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lsDir(NULL, inodesDir, namesDir);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lsDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lsDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 18: %sTry to create a file anidated\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/myFolder/folderAnin/fileAnidado.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 73: %sTry to remove a folder with file route\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = rmDir("/myFolder/folderAnin/fileAnidado.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST rmDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST rmDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 78: %sTry to see the contents of a folder with file route\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lsDir("/myFolder/folderAnin/fileAnidado.txt", inodesDir, namesDir);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lsDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lsDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 74: %sTry to remove a non-existing folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = rmDir("/myFolderas");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST rmDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST rmDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 79: %sTry to see the contents of a non-existing folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lsDir("/myFolderas", inodesDir, namesDir);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lsDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lsDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}


	fprintf(stdout, "%sTest 29: %sTry to open a file with route folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = openFile("/myFolderas");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST openFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 23: %sTry to remove a file with route folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = removeFile("/myFolderas");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST removeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 35: %sTry to close a file with route folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = closeFile(3);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST closeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 37: %sTry to read a file with route folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = readFile(3, buffer, 4);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 45: %sTry to write a file with route folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = writeFile(3, "hola", 4);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 53: %sTry to lseek a file with route folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lseekFile(3, 0, FS_SEEK_CUR);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 80: %sTry to see the contents of a folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	createFile("/myFolder/file.txt");
	ret = lsDir("/myFolder", inodesDir, namesDir);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lsDir ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lsDir ", ANSI_COLOR_GREEN, "SUCCESS: ", ANSI_COLOR_RESET, "read ",namesDir[0] ," ",namesDir[1] ,"\n\n");

	fprintf(stdout, "%sTest 81: %sTry to see the contents of a folder anidated\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	mkDir("/myFolder/folderAnin/eyou");
	createFile("/myFolder/folderAnin/eyou/file1.txt");
	createFile("/myFolder/folderAnin/eyou/file2.txt");
	createFile("/myFolder/folderAnin/eyou/file3.txt");
	createFile("/myFolder/folderAnin/eyou/file4.txt");
	createFile("/myFolder/folderAnin/eyou/file5.txt");
	createFile("/myFolder/folderAnin/eyou/file6.txt");
	createFile("/myFolder/folderAnin/eyou/file7.txt");
	createFile("/myFolder/folderAnin/eyou/file8.txt");
	createFile("/myFolder/folderAnin/eyou/file9.txt");
	createFile("/myFolder/folderAnin/eyou/file10.txt");
	ret = lsDir("/myFolder/folderAnin/eyou", inodesDir, namesDir);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lsDir ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lsDir ", ANSI_COLOR_GREEN, "SUCCESS: ", ANSI_COLOR_RESET, "read ",namesDir[0] ," ",namesDir[1], " ",
	namesDir[2] ," ",namesDir[3], " ",namesDir[4] ," ",namesDir[5], " ",namesDir[6] ," ",namesDir[7], " ",namesDir[8] ," ",namesDir[9],"\n\n");


	fprintf(stdout, "%sTest 19: %sTry to create a file with same name but in another folder\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/myFolder/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);


	fprintf(stdout, "%sTest 15: %sTry to create file when there is no more space\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/myFolder/folderAnin/eyou/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 14: %sTry to create file too deep\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/myFolder/folderAnin/eyou/deep/test.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 66: %sTry to create folder when there is no more space\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	createFile("/myFolder/folderAnin/file1.txt");
	createFile("/myFolder/folderAnin/file2.txt");
	createFile("/myFolder/folderAnin/file3.txt");
	createFile("/myFolder/folderAnin/file4.txt");
	createFile("/myFolder/folderAnin/file5.txt");
	createFile("/myFolder/folderAnin/file6.txt");
	createFile("/myFolder/folderAnin/file7.txt");
	createFile("/myFolder/folderAnin/file8.txt");
	ret = mkDir("/myFolder/folderAnin/noSpace");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 65: %sTry to create folder too deep\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	removeFile("/myFolder/folderAnin/eyou/file10.txt");
	ret = mkDir("/myFolder/folderAnin/eyou/deep");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	createFile("/myFolder/folderAnin/eyou/file10.txt");

	fprintf(stdout, "%sTest 67: %sTry to create a folder including a file in the path\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkDir("/myFolder/test.txt/hola");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 68: %sTry to create a folder when there are 40 inodes\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mkDir("/myFolder/myFolder2");
	ret = createFile("/myFolder/myFolder2/text1.txt");
	ret = createFile("/myFolder/myFolder2/text2.txt");
	ret = createFile("/myFolder/myFolder2/text3.txt");
	ret = createFile("/myFolder/myFolder2/text4.txt");
	ret = createFile("/myFolder/myFolder2/text5.txt");
	ret = createFile("/myFolder/myFolder2/text6.txt");
	ret = createFile("/myFolder/myFolder2/text7.txt");
	ret = createFile("/myFolder/myFolder2/text8.txt");
	ret = createFile("/myFolder/myFolder2/text9.txt");
	ret = createFile("/myFolder/myFolder2/text10.txt");
	ret = createFile("/hola1.txt");
	ret = createFile("/hola2.txt");
	ret = createFile("/hola3.txt");
	ret = createFile("/hola4.txt");
	ret = mkDir("/aFolder");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkDir ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 16: %sTry to create a file when there are 40 inodes\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = createFile("/holaNo.txt");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 76: %sTry to remove a folder anidated\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = rmDir("/myFolder/folderAnin");
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST rmDir ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST rmDir ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 6: %sTry unmount when there are open files\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = unmountFS();
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 7: %sCheck that unmountFS works\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	closeFile(fd1);
	ret = unmountFS();
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 8: %sCheck that mount maintains the same state as before\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = mountFS();
	fd1 = openFile("/test.txt");
	ret2 = mkDir("/myFolder/newFolder");
	if (ret < 0 || fd1 < 0 || ret2 < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS and openFile and mkDir ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS and openFile and mkDir ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 39: %sTry read a file with negative descriptor\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = readFile(-1, buffer, 4);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 48: %sTry write a file with negative descriptor\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = writeFile(-1, "hola", 4);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 56: %sTry lseek a file with negative descriptor\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lseekFile(-1, 0, FS_SEEK_CUR);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 38: %sTry read a file that is closed\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	closeFile(fd1);
	ret = readFile(fd1, buffer, 4);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 47: %sTry write a file that is closed\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = writeFile(fd1, "hola", 4);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 55: %sTry lseek a file that is closed\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lseekFile(fd1, 0, FS_SEEK_CUR);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 40: %sTry read a null inode\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	createFile("/bad.txt");
	fd1 = openFile("/bad.txt");
	closeFile(fd1);
	removeFile("/bad.txt");
	ret = readFile(fd1, buffer, 4);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 46: %sTry write a null inode\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = writeFile(fd1, "hola", 4);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 54: %sTry lseek a null inode\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lseekFile(fd1, 0, FS_SEEK_CUR);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 49: %sTry write a file\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	fd1 = openFile("/test.txt");
	ret = writeFile(fd1, "Hello I'm Poe", 13);
	if (ret != 13)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_RED, "FAILED\n\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	ret = writeFile(fd1, "Hello I'm Poe", 13);

	fprintf(stdout, "%sTest 59: %sTry lseek a file to the beginning \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lseekFile(fd1, 0, FS_SEEK_BEGIN);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 41: %sTry read a file \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = readFile(fd1, buffer, 13);
	if (ret != 13 || strcmp(buffer, "Hello I'm Poe")!=0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);
	memset(buffer,0,strlen(buffer));

	fprintf(stdout, "%sTest 61: %sTry lseek go behind and in front \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lseekFile(fd1, -4, FS_SEEK_CUR);
	ret = lseekFile(fd1, 2, FS_SEEK_CUR);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 42: %sTry read several times from a file \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = readFile(fd1, buffer, 1);
	ret2 = readFile(fd1, buffer2, 1);
	if (ret != 1 || ret2 != 1 || strcmp(buffer, "o")!=0 || strcmp(buffer2, "e")!=0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);
	memset(buffer,0,strlen(buffer));
	memset(buffer2,0,strlen(buffer2));

	fprintf(stdout, "%sTest 57: %sTry lseek with an unknown whence\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lseekFile(fd1, 0, 10);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 60: %sTry lseek a file to the end \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lseekFile(fd1, 0, FS_SEEK_END);
	if (ret < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 43: %sTry read file from the end \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = readFile(fd1, buffer, 10);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 58: %sTry lseek a file out of range \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lseekFile(fd1, 2, FS_SEEK_CUR);
	ret2 = lseekFile(fd1, -2050, FS_SEEK_CUR);
	if (ret < 0 && ret2 < 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_GREEN, "FAILED\n\n", ANSI_COLOR_RESET);
	} else {
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST lseekFile ", ANSI_COLOR_RED, "SUCCESS\n\n", ANSI_COLOR_RESET);
		return -1;
	}

	fprintf(stdout, "%sTest 50: %sTry write more bytes than what is available in buffer \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret2 = lseekFile(fd1, 0, FS_SEEK_BEGIN);
	ret = writeFile(fd1, "buffer", 20);
	ret2 = lseekFile(fd1, 0, FS_SEEK_BEGIN);
	ret2 = readFile(fd1, buffer, 13);
	if (ret != 6 || ret2 != 13 || strcmp(buffer, "bufferI'm Poe")!=0 )
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);
	memset(buffer,0,strlen(buffer));

	fprintf(stdout, "%sTest 51: %sTry write less bytes than what is available in buffer \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret2 = lseekFile(fd1, 0, FS_SEEK_BEGIN);
	ret = writeFile(fd1, "Again I am writing something", 6);
	ret2 = lseekFile(fd1, 0, FS_SEEK_BEGIN);
	ret2 = readFile(fd1, buffer, 13);
	if (ret != 6 || ret2 != 13 || strcmp(buffer, "Again I'm Poe")!=0 )
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);
	memset(buffer,0,strlen(buffer));

	fprintf(stdout, "%sTest 52: %sTry write more bytes than what is available in file \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret2 = lseekFile(fd1, 0, FS_SEEK_END);
	ret2 = lseekFile(fd1, -5, FS_SEEK_CUR);
	ret = writeFile(fd1, "Hello I'm Poe", 13);
	if (ret != 5)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST writeFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);

	fprintf(stdout, "%sTest 44: %sTry read more bytes than what is available in file \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret2 = lseekFile(fd1, -5, FS_SEEK_CUR);
	ret = readFile(fd1, buffer, 13);
	if (ret != 5 || strcmp(buffer, "Hello")!=0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST readFile ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);
	memset(buffer,0,strlen(buffer));

	fprintf(stdout, "%sTest 62: %sTry read + lseek + write + lseek + read \n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
	ret = lseekFile(fd1, 0, FS_SEEK_BEGIN);
	ret = readFile(fd1, buffer, 4);
	ret2 = lseekFile(fd1, -3, FS_SEEK_CUR);
	ret2 = writeFile(fd1, "&&", 2);
	ret2 = lseekFile(fd1, 0, FS_SEEK_BEGIN);
	ret2 = readFile(fd1, buffer2, 4);
	if (ret != 4 || strcmp(buffer, "Agai")!=0 || ret2 != 4 || strcmp(buffer2, "A&&i") != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST read + lseek + write + lseek + read ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST read + lseek + write + lseek + read ", ANSI_COLOR_GREEN, "SUCCESS\n\n", ANSI_COLOR_RESET);
	memset(buffer,0,strlen(buffer));
	memset(buffer,0,strlen(buffer2));


	free (buffer);
	free (buffer2);

	return 0;
}
