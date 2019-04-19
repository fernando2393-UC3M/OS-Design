/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	metadata.h
 * @brief 	Definition of the structures and data types of the file system.
 * @date	01/03/2017
 */

#define MAGIC_NUMBER 0x000D5500 /* Superblock magic number (slides) */

/* Preguntar número de inodos */

#define MAX_FILES 40 /* Maxium number of files */
#define MAX_ENTRIES 10 /* Maxium number of entries per inode */
#define MAX_FILE_NAME 32 /* Longest inode name */
#define MAX_PATH_LEN_FOLDER 99 /* Longest folder path name */
#define MAX_PATH_LEN_FILE 132 /* Longest file path name */
#define TYPE_FILE 0 /* File type inode */
#define TYPE_FOLDER 1 /* Folder type inode */

#define bitmap_getbit(bitmap_, i_) (bitmap_[i_ >> 3] & (1 << (i_ & 0x07)))
static inline void bitmap_setbit(char *bitmap_, int i_, int val_) {
  if (val_)
    bitmap_[(i_ >> 3)] |= (1 << (i_ & 0x07));
  else
    bitmap_[(i_ >> 3)] &= ~(1 << (i_ & 0x07));
}

typedef struct {

  unsigned int magicNumber;                             /* Superblock Magic Number */
  unsigned int numINodeMapBlocks;                       /* Inode Map Number of blocks */
  unsigned int numDataMapBlocks;                        /* Data Map Number of blocks */
  unsigned int numInodes;                               /* Number of inodes */
  unsigned int firstInodeBlock;                              /* First device inode */
  unsigned int numDataBlocks;                            /* Data Blocks Number */
  unsigned int firstDataBlock;                          /* First device data block */
  unsigned int deviceSize;                              /* Total disk space (in bytes) */
  char padding[BLOCK_SIZE - 8 * sizeof(unsigned int)];  /* Padding field (to complete a block) */

} superblock_t;

typedef struct {
  char name [MAX_FILE_NAME + 1]; // Due to the final character \0
  unsigned int dataBlockPos; // Position of the datablock
  unsigned int tipo;
  unsigned int size;
  unsigned int entradas [MAX_ENTRIES];
} inode_t;

/* Metadata of the FS */

superblock_t sblock;  /* FS superblock*/

inode_t inodes [MAX_FILES];

char *i_map;  /* Map of used iNodes */
char *b_map;  /* Map of used dataBlocks */

struct {
  int position; /* Position of the file seek pointer */
  int opened; /* 0 if file is closed, 1 if opened */
} inodes_x[MAX_FILES];
