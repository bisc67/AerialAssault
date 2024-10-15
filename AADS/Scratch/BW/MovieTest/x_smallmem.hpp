#include "x_types.hpp"
#include "x_memory.hpp"

// This is a general purpose small memory area allocator. 
// Since there are some times a large number of small memory allocations (<256 bytes) associated
// with x_bitmaps and various other allocated areas, the overhead from the standard system 
// allocator plays a significant part. This set of "small allocators" is designed to be simple and
// very little if any overhead. There are a couple of caveats:

// 1. Memory can be allocated and free'd at will. When memory is free'd up, the pool it is associated
// with will only be purged when ALL items in that pool have been freed. This means if you malloc/free/malloc/free etc etc
// you WILL eventually run out of memory if even one item is left in the block.

void    x_small_Init    (void);
void    x_small_Kill    (void);
void *  x_small_malloc  (s32 size);
void    x_small_free    (void *ptr);
