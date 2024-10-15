#include "x_smallmem.hpp"

#define SHORT_BLOCK_SIZE (32*1024)

struct s_mem_block
{
    s_mem_block*    pNext;
    s32             UseCount;
    s32             Current;
    s32             Free;
    byte            Data[SHORT_BLOCK_SIZE];
};

static s_mem_block *s_BaseBlock = NULL;

void x_small_Init(void)
{
    s_BaseBlock = NULL;
}

void x_small_Kill(void)
{
    ASSERTS(s_BaseBlock==NULL,"Not all small allocations have been freed\n");
}

void *x_small_malloc(s32 size)
{
    s_mem_block *pBlock;
    void        *pAddr;

    pBlock = s_BaseBlock;
    size = (size+15)&~15;

    while (pBlock)
    {
        // Do we have a block with enough space free in it?
        if ( size >= pBlock->Free )
            break;
        pBlock = pBlock->pNext;
    }

    if (!pBlock)
    {
        pBlock = (s_mem_block *)x_malloc(sizeof(s_mem_block));
        if (!pBlock)
            return NULL;
        pBlock->Current     = 0;
        pBlock->Free        = sizeof(pBlock->Data);
        pBlock->pNext       = s_BaseBlock;
        pBlock->UseCount    = 0;
        s_BaseBlock = pBlock;
    }

    pAddr = pBlock->Data+pBlock->Current;
    pBlock->UseCount++;
    pBlock->Current+=size;
    pBlock->Free -= size;
    ASSERT(pBlock->Free >= 0);

    return pAddr;
}

void x_small_free(void *ptr)
{
    s_mem_block *pBlock,*pPrevBlock;

    pBlock = s_BaseBlock;
    pPrevBlock = NULL;

    // Find the current block that contains the memory being freed. Hold the
    // previous block around for unlinking as well.
    while (pBlock)
    {
        if ( (ptr >= pBlock->Data) && (ptr < pBlock->Data+pBlock->Current) )
        {
            break;
        }
        pPrevBlock = pBlock;
        pBlock = pBlock->pNext;
    }
    //
    // We always should find a block, otherwise this means we tried to free
    // memory that's already freed OR we're passing in a bad ptr.
    //
    ASSERT(pBlock);
    pBlock->UseCount--;
    //
    // If the "UseCount" gets to zero then that means all the memory within
    // this chunk has been freed. So we can then release it to the main
    // memory manager.
    //
    if (pBlock->UseCount==0)
    {
        if (pPrevBlock)
        {
            pPrevBlock->pNext = pBlock->pNext;
        }
        else
        {
            s_BaseBlock = pBlock->pNext;
        }
        x_free(pBlock);
    }
}