/*-------------------------------------------------------------------
 *  UW CSE 351 Lab 5 Starter code:
 *        single doubly-linked free block list with LIFO policy
 *        with support for coalescing adjacent free blocks
 *
 *  Eric Boris
 *  1976637
 *
 * Terminology:
 * o We will implement an explicit free list allocator.
 * o We use "next" and "previous" to refer to blocks as ordered in
 *   the free list.
 * o We use "following" and "preceding" to refer to adjacent blocks
 *   in memory.
 *-------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

/* Macros for unscaled pointer arithmetic to keep other code cleaner.
   Casting to a char* has the effect that pointer arithmetic happens at
   the byte granularity (i.e. POINTER_ADD(0x1, 1) would be 0x2).  (By
   default, incrementing a pointer in C has the effect of incrementing
   it by the size of the type to which it points (e.g. BlockInfo).)
   We cast the result to void* to force you to cast back to the
   appropriate type and ensure you don't accidentally use the resulting
   pointer as a char* implicitly.
*/
#define UNSCALED_POINTER_ADD(p, x) ((void*)((char*)(p) + (x)))
#define UNSCALED_POINTER_SUB(p, x) ((void*)((char*)(p) - (x)))


/******** FREE LIST IMPLEMENTATION ***********************************/


/* A BlockInfo contains information about a block, including the size
   and usage tags, as well as pointers to the next and previous blocks
   in the free list.  This is exactly the "explicit free list" structure
   illustrated in the lecture slides.

   Note that the next and prev pointers and the boundary tag are only
   needed when the block is free.  To achieve better utilization, mm_malloc
   should use the space for next and prev as part of the space it returns.

   +--------------+
   | sizeAndTags  |  <-  BlockInfo pointers in free list point here
   |  (header)    |
   +--------------+
   |     next     |  <-  Pointers returned by mm_malloc point here
   +--------------+
   |     prev     |
   +--------------+
   |  space and   |
   |   padding    |
   |     ...      |
   |     ...      |
   +--------------+
   | boundary tag |
   |  (footer)    |
   +--------------+
*/
struct BlockInfo {
  // Size of the block (in the high bits) and tags for whether the
  // block and its predecessor in memory are in use.  See the SIZE()
  // and TAG macros, below, for more details.
  size_t sizeAndTags;
  // Pointer to the next block in the free list.
  struct BlockInfo* next;
  // Pointer to the previous block in the free list.
  struct BlockInfo* prev;
};
typedef struct BlockInfo BlockInfo;


/* Pointer to the first BlockInfo in the free list, the list's head.

   A pointer to the head of the free list in this implementation is
   always stored in the first word in the heap.  mem_heap_lo() returns
   a pointer to the first word in the heap, so we cast the result of
   mem_heap_lo() to a BlockInfo** (a pointer to a pointer to
   BlockInfo) and dereference this to get a pointer to the first
   BlockInfo in the free list. */
#define FREE_LIST_HEAD *((BlockInfo **)mem_heap_lo())

/* Size of a word on this architecture. */
#define WORD_SIZE sizeof(void*)

/* Minimum block size (to account for size header, next ptr, prev ptr,
   and boundary tag) */
#define MIN_BLOCK_SIZE (sizeof(BlockInfo) + WORD_SIZE)

/* Alignment of blocks returned by mm_malloc. */
#define ALIGNMENT 8

/* SIZE(blockInfo->sizeAndTags) extracts the size of a 'sizeAndTags' field.
   Also, calling SIZE(size) selects just the higher bits of 'size' to ensure
   that 'size' is properly aligned.  We align 'size' so we can use the low
   bits of the sizeAndTags field to tag a block as free/used, etc, like this:

      sizeAndTags:
      +-------------------------------------------+
      | 63 | 62 | 61 | 60 |  . . . .  | 2 | 1 | 0 |
      +-------------------------------------------+
        ^                                       ^
      high bit                               low bit

   Since ALIGNMENT == 8, we reserve the low 3 bits of sizeAndTags for tag
   bits, and we use bits 3-63 to store the size.

   Bit 0 (2^0 == 1): TAG_USED
   Bit 1 (2^1 == 2): TAG_PRECEDING_USED
*/
#define SIZE(x) ((x) & ~(ALIGNMENT - 1))

/* TAG_USED is the bit mask used in sizeAndTags to mark a block as used. */
#define TAG_USED 1

/* TAG_PRECEDING_USED is the bit mask used in sizeAndTags to indicate
   that the block preceding it in memory is used. (used in turn for
   coalescing).  If the previous block is not used, we can learn the size
   of the previous block from its boundary tag */
#define TAG_PRECEDING_USED 2

// /************************/
// /*                      */
// /*     Examine Heap     */
// /*                      */
// /************************/
//
// /* Print the heap by iterating through it as an implicit free list. */
// static void examine_heap() {
//   BlockInfo *block;
//
//   /* print to stderr so output isn't buffered and not output if we crash */
//   fprintf(stderr, "FREE_LIST_HEAD: %p\n", (void *)FREE_LIST_HEAD);
//
//   for (block = (BlockInfo *)UNSCALED_POINTER_ADD(mem_heap_lo(), WORD_SIZE); /* first block on heap */
//       SIZE(block->sizeAndTags) != 0 && block < (BlockInfo *)mem_heap_hi();
//       block = (BlockInfo *)UNSCALED_POINTER_ADD(block, SIZE(block->sizeAndTags))) {
//
//     /* print out common block attributes */
//     fprintf(stderr, "%p: %ld %ld %ld\t",
//     (void *)block,
//     SIZE(block->sizeAndTags),
//     block->sizeAndTags & TAG_PRECEDING_USED,
//     block->sizeAndTags & TAG_USED);
//
//     /* and allocated/free specific data */
//     if (block->sizeAndTags & TAG_USED) {
//       fprintf(stderr, "ALLOCATED\n");
//     } else {
//       fprintf(stderr, "FREE\tnext: %p, prev: %p\n",
//       (void *)block->next,
//       (void *)block->prev);
//     }
//   }
//   fprintf(stderr, "END OF HEAP\n\n");
// }


/****************************/
/*                          */
/*     Search Free List     */
/*                          */
/****************************/

/* Find a free block of the requested size in the free list.  Returns
   NULL if no free block is large enough. */
static BlockInfo* searchFreeList(size_t reqSize) {
  BlockInfo* freeBlock;

  freeBlock = FREE_LIST_HEAD;
  while (freeBlock != NULL) {
    if (SIZE(freeBlock->sizeAndTags) >= reqSize) {
      return freeBlock;
    } else {
      freeBlock = freeBlock->next;
    }
  }
  return NULL;
}

/*****************************/
/*                           */
/*     Insert Free Block     */
/*                           */
/*****************************/

/* Insert freeBlock at the head of the list.  (LIFO) */
static void insertFreeBlock(BlockInfo* freeBlock) {
  BlockInfo* oldHead = FREE_LIST_HEAD;
  freeBlock->next = oldHead;
  if (oldHead != NULL) {
    oldHead->prev = freeBlock;
  }
  freeBlock->prev = NULL;
  FREE_LIST_HEAD = freeBlock;
}

/*****************************/
/*                           */
/*     Remove Free Block     */
/*                           */
/*****************************/

/* Remove a free block from the free list. */
static void removeFreeBlock(BlockInfo* freeBlock) {
  BlockInfo *nextFree, *prevFree;

  nextFree = freeBlock->next;
  prevFree = freeBlock->prev;

  // If the next block is not null, patch its prev pointer.
  if (nextFree != NULL) {
    nextFree->prev = prevFree;
  }

  // If we're removing the head of the free list, set the head to be
  // the next block, otherwise patch the previous block's next pointer.
  if (freeBlock == FREE_LIST_HEAD) {
    FREE_LIST_HEAD = nextFree;
  } else {
    prevFree->next = nextFree;
  }
}

/*******************************/
/*                             */
/*     Coalesce Free Block     */
/*                             */
/*******************************/

/* Coalesce 'oldBlock' with any preceeding or following free blocks. */
static void coalesceFreeBlock(BlockInfo* oldBlock) {
  BlockInfo *blockCursor;
  BlockInfo *newBlock;
  BlockInfo *freeBlock;
  // size of old block
  size_t oldSize = SIZE(oldBlock->sizeAndTags);
  // running sum to be size of final coalesced block
  size_t neWORD_SIZE = oldSize;

  // Coalesce with any preceding free block
  blockCursor = oldBlock;
  while ((blockCursor->sizeAndTags & TAG_PRECEDING_USED) == 0) {
    // While the block preceding this one in memory (not the
    // prev. block in the free list) is free:

    // Get the size of the previous block from its boundary tag.
    size_t size = SIZE(*((size_t*)UNSCALED_POINTER_SUB(blockCursor, WORD_SIZE)));
    // Use this size to find the block info for that block.
    freeBlock = (BlockInfo*)UNSCALED_POINTER_SUB(blockCursor, size);
    // Remove that block from free list.
    removeFreeBlock(freeBlock);

    // Count that block's size and update the current block pointer.
    neWORD_SIZE += size;
    blockCursor = freeBlock;
  }
  newBlock = blockCursor;

  // Coalesce with any following free block.
  // Start with the block following this one in memory
  blockCursor = (BlockInfo*)UNSCALED_POINTER_ADD(oldBlock, oldSize);
  while ((blockCursor->sizeAndTags & TAG_USED) == 0) {
    // While the block is free:

    size_t size = SIZE(blockCursor->sizeAndTags);
    // Remove it from the free list.
    removeFreeBlock(blockCursor);
    // Count its size and step to the following block.
    neWORD_SIZE += size;
    blockCursor = (BlockInfo*)UNSCALED_POINTER_ADD(blockCursor, size);
  }

  // If the block actually grew, remove the old entry from the free
  // list and add the new entry.
  if (neWORD_SIZE != oldSize) {
    // Remove the original block from the free list
    removeFreeBlock(oldBlock);

    // Save the new size in the block info and in the boundary tag
    // and tag it to show the preceding block is used (otherwise, it
    // would have become part of this one!).
    newBlock->sizeAndTags = neWORD_SIZE | TAG_PRECEDING_USED;
    // The boundary tag of the preceding block is the word immediately
    // preceding block in memory where we left off advancing blockCursor.
    *(size_t*)UNSCALED_POINTER_SUB(blockCursor, WORD_SIZE) = neWORD_SIZE | TAG_PRECEDING_USED;

    // Put the new block in the free list.
    insertFreeBlock(newBlock);
  }
  return;
}

/******************************/
/*                            */
/*     Request More Space     */
/*                            */
/******************************/

/* Get more heap space of size at least reqSize. */
static void requestMoreSpace(size_t reqSize) {
  size_t pagesize = mem_pagesize();
  size_t numPages = (reqSize + pagesize - 1) / pagesize;
  BlockInfo *newBlock;
  size_t totalSize = numPages * pagesize;
  size_t prevLastWordMask;

  void* mem_sbrk_result = mem_sbrk(totalSize);
  if ((size_t)mem_sbrk_result == -1) {
    printf("ERROR: mem_sbrk failed in requestMoreSpace\n");
    exit(0);
  }
  newBlock = (BlockInfo*)UNSCALED_POINTER_SUB(mem_sbrk_result, WORD_SIZE);

  /* initialize header, inherit TAG_PRECEDING_USED status from the
     previously useless last word however, reset the fake TAG_USED
     bit */
  prevLastWordMask = newBlock->sizeAndTags & TAG_PRECEDING_USED;
  newBlock->sizeAndTags = totalSize | prevLastWordMask;
  // Initialize boundary tag.
  ((BlockInfo*)UNSCALED_POINTER_ADD(newBlock, totalSize - WORD_SIZE))->sizeAndTags =
    totalSize | prevLastWordMask;

  /* initialize "new" useless last word
     the previous block is free at this moment
     but this word is useless, so its use bit is set
     This trick lets us do the "normal" check even at the end of
     the heap and avoid a special check to see if the following
     block is the end of the heap... */
  *((size_t*)UNSCALED_POINTER_ADD(newBlock, totalSize)) = TAG_USED;

  // Add the new block to the free list and immediately coalesce newly
  // allocated memory space
  insertFreeBlock(newBlock);
  coalesceFreeBlock(newBlock);
}

/*******************/
/*                 */
/*     MM INIT     */
/*                 */
/*******************/

/* Initialize the allocator. */
int mm_init() {
  // Head of the free list.
  BlockInfo *firstFreeBlock;

  // Initial heap size: WORD_SIZE byte heap-header (stores pointer to head
  // of free list), MIN_BLOCK_SIZE bytes of space, WORD_SIZE byte heap-footer.
  size_t initSize = WORD_SIZE+MIN_BLOCK_SIZE+WORD_SIZE;
  size_t totalSize;

  void* mem_sbrk_result = mem_sbrk(initSize);
  //  printf("mem_sbrk returned %p\n", mem_sbrk_result);
  if ((ssize_t)mem_sbrk_result == -1) {
    printf("ERROR: mem_sbrk failed in mm_init, returning %p\n",
           mem_sbrk_result);
    exit(1);
  }

  firstFreeBlock = (BlockInfo*)UNSCALED_POINTER_ADD(mem_heap_lo(), WORD_SIZE);

  // Total usable size is full size minus heap-header and heap-footer words
  // NOTE: These are different than the "header" and "footer" of a block!
  // The heap-header is a pointer to the first free block in the free list.
  // The heap-footer is used to keep the data structures consistent (see
  // requestMoreSpace() for more info, but you should be able to ignore it).
  totalSize = initSize - WORD_SIZE - WORD_SIZE;

  // The heap starts with one free block, which we initialize now.
  firstFreeBlock->sizeAndTags = totalSize | TAG_PRECEDING_USED;
  firstFreeBlock->next = NULL;
  firstFreeBlock->prev = NULL;
  // boundary tag
  *((size_t*)UNSCALED_POINTER_ADD(firstFreeBlock, totalSize - WORD_SIZE)) = totalSize | TAG_PRECEDING_USED;

  // Tag "useless" word at end of heap as used.
  // This is the is the heap-footer.
  *((size_t*)UNSCALED_POINTER_SUB(mem_heap_hi(), WORD_SIZE - 1)) = TAG_USED;

  // set the head of the free list to this new free block.
  FREE_LIST_HEAD = firstFreeBlock;
  return 0;
}

/********************/
/*                  */
/*     MM Alloc     */
/*                  */
/********************/

/* Allocate a block of size size and return a pointer to it. If size is zero,
 * returns null.
 */
void* mm_malloc(size_t size) {
	size_t reqSize;
	BlockInfo * ptrFreeBlock = NULL;
	size_t blockSize;
	size_t precedingBlockUseTag;

	// Zero-size requests get NULL.
	if (size == 0) {
		return NULL;
	}

	// Add one word for the initial size header.
	// Note that we don't need to boundary tag when the block is used!
	size += WORD_SIZE;
	if (size <= MIN_BLOCK_SIZE) {
		// Make sure we allocate enough space for a blockInfo in case we
		// free this block (when we free this block, we'll need to use the
		// next pointer, the prev pointer, and the boundary tag).
		reqSize = MIN_BLOCK_SIZE;
	} else {
		// Round up for correct alignment
		reqSize = ALIGNMENT * ((size + ALIGNMENT - 1) / ALIGNMENT);
	}

	// Get a BlockInfo pointer to a sufficiently large block in the free list
	ptrFreeBlock = searchFreeList(reqSize);
	// if no existing space is large enough
	if (ptrFreeBlock == NULL) {
		// then request one
		requestMoreSpace(reqSize);	

		// and ask for a free block again
		ptrFreeBlock = searchFreeList(reqSize);

		// If there still isn't a free block
		if (ptrFreeBlock == NULL) {
			// then stop trying and return null
			return NULL;
		}
	}

	// Remove the received free block from the free block list
	removeFreeBlock(ptrFreeBlock);

	// get the free block's size
	blockSize = SIZE(ptrFreeBlock->sizeAndTags);

	// and whether the free block's neighbor is allocated
	precedingBlockUseTag = ptrFreeBlock->sizeAndTags & TAG_PRECEDING_USED;

	// Get an additional pointer to the head of the free block
	BlockInfo * ptrAllocBlock = ptrFreeBlock;

	// if the provided free block is larger than necessary 
	// then divide the provided block into an allocated and a free block and
	// return the free block to the free block list
	if ((blockSize - reqSize) >= MIN_BLOCK_SIZE) {
		// set the allocated block pointer's header status
		ptrAllocBlock->sizeAndTags = reqSize | precedingBlockUseTag | TAG_USED;

		// Get a pointer to the head of the new free block
		ptrFreeBlock = (BlockInfo*)UNSCALED_POINTER_ADD(ptrFreeBlock, reqSize);	

		// get the new free block's size
		size_t freeBlockSize = blockSize - reqSize;  

		// set the new free block's header status
		ptrFreeBlock->sizeAndTags = freeBlockSize | TAG_PRECEDING_USED; 

		// and set the new free block's footer status
		*((size_t*)UNSCALED_POINTER_ADD(ptrFreeBlock, freeBlockSize - WORD_SIZE)) = freeBlockSize | TAG_PRECEDING_USED;

		// Give the new free block back to the free list
		insertFreeBlock(ptrFreeBlock);
	// If the provided free block wasn't too large
	} else { 
		// set the allocated block pointer's header status
		ptrAllocBlock->sizeAndTags = blockSize | precedingBlockUseTag | TAG_USED;

		// set the allocated block's neighbor's header status
		((BlockInfo*)UNSCALED_POINTER_ADD(ptrAllocBlock, blockSize))->sizeAndTags |= TAG_PRECEDING_USED;
	}

	// return a pointer to the payload  
	return UNSCALED_POINTER_ADD(ptrAllocBlock, WORD_SIZE);
}

/*******************/
/*                 */
/*     MM Free     */
/*                 */
/*******************/

/* Free the block referenced by ptr. */
void mm_free(void *ptr) {
	size_t blockSize;
	BlockInfo * blockInfo;
	//BlockInfo * followingBlock;

	// get a block info pointer to the payload address
	blockInfo = (BlockInfo*)UNSCALED_POINTER_SUB(ptr, WORD_SIZE);

	// get the size of the block info pointer
	blockSize = SIZE(blockInfo->sizeAndTags);

	// Set the block info footer to reflect that the block is now free
	*((size_t*)UNSCALED_POINTER_ADD(blockInfo, blockSize - WORD_SIZE)) = blockSize & ~TAG_USED;

	// set the block info pointer's neighbor's header status that the block is now free
	((BlockInfo*)UNSCALED_POINTER_ADD(blockInfo, blockSize))->sizeAndTags &= ~TAG_PRECEDING_USED;

	// return the block info pointer block to the free list
	insertFreeBlock(blockInfo);

	// coalesce the block info pointer block with it's neighbors
	coalesceFreeBlock(blockInfo);
}

/********************/
/*                  */
/*     MM Check     */
/*                  */
/********************/

// // TODO: Implement a heap consistency checker as needed.
// int mm_check() {
//   return 0;
// }
