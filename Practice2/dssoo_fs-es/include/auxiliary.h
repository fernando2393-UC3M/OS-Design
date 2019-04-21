/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	auxiliary.h
 * @brief 	Headers for the auxiliary functions required by filesystem.c.
 * @date	01/03/2017
 */

 /*
  * @brief   Implementation of a math.ceil funcion of a division
  * @return  int with the result of applying ceil(a/b)
  */
int ceilOfDivision (long a, long b);

/*
 * @brief   Counts the number of entries that a specific inode has
 * @return  int with the number of entries that a specific inode has
 */
int countNumberEntries (int inode_id);

/*
 * @brief   Gets the root directory of a path
 * @return  char * with the root directory.
 */
char * getRootDirectory(char * path);

/*
 * @brief   Gets the next directory from root
 * @return  char * with the next directory from root.
 */
char * getRealDirectory(char * path, char * root);

/*
 * @brief   Gets file from current directory
 * @return  inode_id of file, -1 if it does not exist, -2 if error.
 */
int getFile(char * filename);

/*
 * @brief   Reaches and gets file through a path with directories
 * @return  inode_id of file, -1 if the directory does not exist, -2 if name not of a directory,
 * 			-3 if file does not exist and directory full, -4 if it does not exist but could be created
 */
int getFileFromDir(char * rootpath, char * filename, char * path);

 /*
  * @brief   Search for a free inode and set its value in inodes map to 1
  * @return  ID of the inode available if any, -1 if not
  */
 int ialloc(void);

 /*
  * @brief   Search for a free data block, set its bytes to 0 and set its
  *          value in data blocks map to 1
  * @return  ID of the data block available if any, -1 if not
  */
 int alloc(void);

 /*
  * @brief   Sets the value of the inode provided to 0 in inodes map
  * @return  0 if success, -1 if inode not found
  */
 int ifree(int inode_id);

 /*
  * @brief   Sets the value of the data block provided to 0 in data block map
  * @return  0 if success, -1 if data block not found
  */
 int bfree(int block_id);

 /*
  * @brief   Found the inode ID containing the file passed
  * @return  ID of the inode, -1 if not found
  */
 int namei(char *fname);

 /*
  * @brief   Return the index of the data block that contains the byte indicated
  *          by the offset
  * @return  Index of the block, -1 if not found
  */
 int bmap(int inode_id, int offset);

 /*
  * @brief   Writes data in memory to the disk image
  * @return  0 if success, -1 if error
  */
 int syncFS(void);
