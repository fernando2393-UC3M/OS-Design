/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	metadata.h
 * @brief 	Definition of the structures and data types of the file system.
 * @date	01/03/2017
 */

#define MAGIC_NUMBER 0x000D5500 /* Superblock magic number (slides) */

#define bitmap_getbit(bitmap_, i_) (bitmap_[i_ >> 3] & (1 << (i_ & 0x07)))
static inline void bitmap_setbit(char *bitmap_, int i_, int val_) {
  if (val_)
    bitmap_[(i_ >> 3)] |= (1 << (i_ & 0x07));
  else
    bitmap_[(i_ >> 3)] &= ~(1 << (i_ & 0x07));
}

typedef struct {

  unsigned int magicNumber;                             /* Superblock Magic Number */
  unsigned int inodeMapNumBlocks;                       /* Inode Map Number of blocks */
  unsigned int dataMapNumBlocks;                        /* Data Map Number of blocks */
  unsigned int numInodes;                               /* Number of inodes */
  unsigned int firstInode;                              /* First device inode */
  unsigned int dataBlockNum;                            /* Data Blocks Number */
  unsigned int firstDataBlock;                          /* First device data block */
  unsigned int deviceSize;                              /* Total disk space (in bytes) */
  char padding[BLOCK_SIZE - 8 * sizeof(unsigned int)];  /* Padding field (to complete a block) */

} superblock;

/* Metadata of the FS */

superblock sblock;  /* FS superblock*/
