//==============================================================================
//
//  x_memory.cpp
//
//==============================================================================

//==============================================================================
//  INCLUDES
//==============================================================================

#ifndef X_MEMORY_HPP
#include "..\x_memory.hpp"
#endif

#ifndef X_FILES_PRIVATE_HPP
#include "x_files_private.hpp"
#endif

#ifndef X_STDIO_HPP
#include "..\x_stdio.hpp"
#endif

#ifndef X_STRING_HPP
#include "..\x_string.hpp"
#endif

#include <stdio.h>

//==============================================================================
//  DEFINES
//==============================================================================

#ifdef x_malloc
#undef x_malloc
#endif

#ifdef new
#undef new
#endif

#ifdef X_MEM_DEBUG
#define PAD_BYTES    8      // Pad mem_header to 64 bytes (multiple of 16).
#else
#define PAD_BYTES    0      // Pad mem_header to 16 bytes (multiple of 16).
#endif  

#define PAD_VALUE       (('~' << 24) | ('~' << 16) | ('~' << 8) | ('~' << 0))

#define MAX_MARKS 1024

//==============================================================================
//  TYPES
//==============================================================================

struct mem_header
{
    s32         Sequence;
    s32         RequestedSize;
    mem_header* pNext;
    mem_header* pPrev;

    #ifdef X_MEM_DEBUG
        s32     Mark;
        s32     LineNumber;
        char    FileName[ 32 ];
    #endif  

    #if PAD_BYTES > 0
        u32     Pad[ PAD_BYTES / 4 ];
    #endif
};

//==============================================================================
//  STORAGE
//==============================================================================

static s32   Sequence = 1;
static xbool UsingNew = FALSE;

#ifdef X_MEM_DEBUG
    static s32  CurrentCount = 0;
    static s32  CurrentBytes = 0;
    static s32  TotalBytes   = 0;
    static s32  MaxBytes     = 0;

    static s32  Mark         = 0;
    static s32  MarkSequence[ MAX_MARKS ]      = { 0 };
    static char MarkComment [ MAX_MARKS * 28 ];
#endif

static mem_header   Anchor;
#ifdef TARGET_PS2
static xbool   SuspendMallocDumps=FALSE;
void ps2_GetFreeMem(s32 *pFree,s32 *pLargest,s32 *pFragmentCount);
#endif

//==============================================================================
//  FUNCTIONS
//==============================================================================
//  PLACEHOLDER IMPLEMENTATIONS!
#include <malloc.h>
//==============================================================================

void x_MemInit( void )
{
    Anchor.Sequence      =  0;
    Anchor.RequestedSize = -1;
    Anchor.pNext         = &Anchor;
    Anchor.pPrev         = &Anchor;

    #ifdef X_MEM_DEBUG
        Anchor.Mark          =  0;
        Anchor.LineNumber    = -1;
        x_memset( Anchor.FileName, '*', 32 );
        #if PAD_BYTES > 0
            x_memset( Anchor.Pad, '*',  PAD_BYTES );
        #endif
        x_memset( MarkComment, 0, sizeof(MarkComment) );
        x_strcpy( MarkComment, "Default 1st Mark" );
    #endif

#ifdef TARGET_PS2
    s32 AmountFree;
    s32 LargestFree;
    s32 NFragments;
    ps2_GetFreeMem( &AmountFree, &LargestFree, &NFragments );
#endif
}

//==============================================================================

void x_MemKill( void )
{
    #ifdef X_MEM_DEBUG
    x_MemSanity();
    // The current thread has its "globals" allocated, so the count can't be 0.
    if( CurrentCount > 1 )
        x_MemDump();
    #endif
}

//==============================================================================

void x_free( void* pMemory )
{
    if( pMemory == NULL )
        return;

    mem_header* pHeader = ((mem_header*)pMemory) - 1;

    // Look for trouble!
    #ifdef X_MEM_DEBUG
        #if PAD_BYTES > 0
            for( s32 i = 0; i < (PAD_BYTES/4); i++ )
                ASSERT( pHeader->Pad[i] == PAD_VALUE );     // Corruption!
        #endif
        ASSERT( pHeader->pNext->pPrev == pHeader );         // Corruption!
        ASSERT( pHeader->pPrev->pNext == pHeader );         // Corruption!
    #endif

    // Update stats.
    #ifdef X_MEM_DEBUG
        CurrentCount -= 1;
        CurrentBytes -= pHeader->RequestedSize;
    #endif

    // Remove from double link list.
    pHeader->pNext->pPrev = pHeader->pPrev;
    pHeader->pPrev->pNext = pHeader->pNext;

    // Done!
    free( pHeader );
}

//==============================================================================
#ifdef X_MEM_DEBUG
//==============================================================================
s32 XMEMORY_WATCH_SIZE = -1;

void* x_debug_malloc( s32         NBytes,
                      const char* pFileName, 
                      s32         LineNumber )
{
    ASSERT( NBytes >= 0 );
    ASSERT( pFileName );

    if( XMEMORY_WATCH_SIZE != -1 )
    {
        if( XMEMORY_WATCH_SIZE == NBytes )
        {
            BREAK;
        }
    }

    s32         Len     = x_strlen( pFileName ) + 1;
    mem_header* pHeader = (mem_header*)malloc( NBytes + sizeof(mem_header) );
   
    if( pHeader == NULL )
    {
#ifdef TARGET_PS2
        if (SuspendMallocDumps)
            return NULL;
#endif
        x_MemDump( "AllocationFailure.txt" );
        x_MemSanity();
    }

    // Make sure we haven't run out of heap space.
    ASSERTS( pHeader, 
             (const char*)xfs( "Heap exhuasted.  Malloc %d bytes from %s(%d).", 
                               NBytes, pFileName, LineNumber ) );

    // Insert into double linked list BEFORE the anchor.
    pHeader->pNext        = &Anchor;
    pHeader->pPrev        = Anchor.pPrev;
    Anchor.pPrev          = pHeader;
    pHeader->pPrev->pNext = pHeader;

    // Set up header fields.
    pHeader->RequestedSize = NBytes;
    pHeader->Mark          = Mark;
    pHeader->Sequence      = UsingNew ? Sequence : -Sequence;
    pHeader->LineNumber    = LineNumber;    
    if( Len > 32 )  pFileName += (Len-32);
    x_strncpy( pHeader->FileName, pFileName, 32 );
    #if PAD_BYTES > 0
        for( s32 i = 0; i < (PAD_BYTES/4); i++ )
            pHeader->Pad[i] = PAD_VALUE;
    #endif
    Sequence = MAX( Sequence+1, 1 );

    // Update stats.
    CurrentCount += 1;
    CurrentBytes += NBytes;
    TotalBytes   += NBytes;
    MaxBytes      = MAX( MaxBytes, CurrentBytes );

    // Done!
    return( pHeader + 1 );
}

//==============================================================================
#else // X_MEM_DEBUG
//==============================================================================
#ifdef TARGET_PS2
#include "stdio.h"
#endif
void* x_malloc( s32 NBytes )
{
    ASSERT( NBytes >= 0 );

    mem_header* pHeader = (mem_header*)malloc( NBytes + sizeof(mem_header) );

#ifdef TARGET_PS2
    if( pHeader == NULL )
    {
        if (SuspendMallocDumps)
            return NULL;
        else
        {
            s32 Free,Largest,Fragments;
            ps2_GetFreeMem(&Free,&Largest,&Fragments);
            printf("Allocation failed: size=%d, free=%d, largest=%d,fragments=%d\n",NBytes,Free,Largest,Fragments);

        }
    }
#endif

    // Make sure we haven't run out of heap space.
    ASSERTS( pHeader, 
             (const char*)xfs( "Heap exhuasted.  Malloc %d bytes.", NBytes ) );

    // Insert into double linked list BEFORE the anchor.
    pHeader->pNext        = &Anchor;
    pHeader->pPrev        = Anchor.pPrev;
    Anchor.pPrev          = pHeader;
    pHeader->pPrev->pNext = pHeader;

    // Set up header fields.
    pHeader->RequestedSize = NBytes;
    pHeader->Sequence      = UsingNew ? Sequence : -Sequence;
    Sequence = MAX( Sequence+1, 1 );

    // Done!
    return( pHeader + 1 );
}

//==============================================================================
#endif // X_MEM_DEBUG
//==============================================================================

void* x_realloc( void* pMemory, s32 NewNBytes )
{
    ASSERT( NewNBytes >= 0 );

    if( pMemory == NULL )
    {
        #ifdef X_MEM_DEBUG
            return( x_debug_malloc( NewNBytes, "<realloc>", -1 ) );
        #else   
            return( x_malloc( NewNBytes ) );
        #endif
    }

    mem_header* pHeader = ((mem_header*)pMemory) - 1;

    // Remove from double link list.
    pHeader->pNext->pPrev = pHeader->pPrev;
    pHeader->pPrev->pNext = pHeader->pNext;

    // Update stats.
    #ifdef X_MEM_DEBUG
        CurrentBytes -= pHeader->RequestedSize;
        CurrentBytes += NewNBytes;
        TotalBytes   += MAX( 0, NewNBytes - pHeader->RequestedSize );
        MaxBytes      = MAX( MaxBytes, CurrentBytes );
    #endif

    // Reallocate.
    pHeader = (mem_header*)realloc( pHeader, NewNBytes + sizeof(mem_header) );

    if( pHeader == NULL )
    {
        byte* pBuff = (byte*)malloc( NewNBytes + sizeof(mem_header) );
        if( pBuff )
        {
            x_memcpy( pBuff + sizeof(mem_header), pMemory, NewNBytes );
            pHeader = (mem_header*)pBuff;
            free(pMemory);
        }
    }

#ifdef TARGET_PS2
    if( pHeader == NULL )
    {
        if (SuspendMallocDumps)
            return NULL;
        else
        {
            s32 Free,Largest,Fragments;
            ps2_GetFreeMem(&Free,&Largest,&Fragments);
            printf("Allocation failed: size=%d, free=%d, largest=%d,fragments=%d\n",NewNBytes,Free,Largest,Fragments);

        }
    }
#endif

    // Make sure we haven't run out of heap space.
    ASSERTS( pHeader, 
             (const char*)xfs( "Heap exhuasted.  Realloc %d bytes.", NewNBytes ) );

    // Insert into double linked list BEFORE the anchor.
    pHeader->pNext        = &Anchor;
    pHeader->pPrev        = Anchor.pPrev;
    Anchor.pPrev          = pHeader;
    pHeader->pPrev->pNext = pHeader;

    // Update header fields.
    #ifdef X_MEM_DEBUG
    pHeader->Mark          = Mark;
    #endif
    pHeader->RequestedSize = NewNBytes;
    pHeader->Sequence      = UsingNew ? Sequence : -Sequence;
    Sequence = MAX( Sequence+1, 1 );

    // Done!
    return( pHeader + 1 );
}

//==============================================================================
//  C++ OPERATORS 'new' AND 'delete'
//==============================================================================

//==============================================================================
#ifndef USE_SYSTEM_NEW_DELETE
//==============================================================================

void* operator new( xalloctype Size )
{
    void* pResult;
    UsingNew = TRUE;
    #ifdef X_MEM_DEBUG
    pResult  = x_debug_malloc( (s32)Size, "<unknown>", -1 );
    #else
    pResult  = x_malloc( (s32)Size );
    #endif
    UsingNew = FALSE;
    return( pResult );
}

//==============================================================================

void* operator new [] ( xalloctype Size )
{
    void* pResult;
    UsingNew = TRUE;
    #ifdef X_MEM_DEBUG
    pResult  = x_debug_malloc( (s32)Size, "<unknown>", -1 );
    #else
    pResult  = x_malloc( (s32)Size );
    #endif
    UsingNew = FALSE;
    return( pResult );
}

//==============================================================================

void operator delete( void* pMemory )
{
    if( pMemory == NULL )
        return;
    UsingNew = TRUE;
    x_free( pMemory );
    UsingNew = FALSE;
}

//==============================================================================

void operator delete [] ( void* pMemory )
{
    if( pMemory == NULL )
        return;
    UsingNew = TRUE;
    x_free( pMemory );
    UsingNew = FALSE;
}

//==============================================================================
#ifdef X_MEM_DEBUG
//==============================================================================

void* operator new( xalloctype Size, char* pFileName, s32 LineNumber )
{
    void* pResult;
    UsingNew = TRUE;
    pResult  = x_debug_malloc( (s32)Size, pFileName, LineNumber );
    UsingNew = FALSE;
    return( pResult );
}

//==============================================================================

void* operator new [] ( xalloctype Size, char* pFileName, s32 LineNumber )
{
    void* pResult;
    UsingNew = TRUE;
    pResult  = x_debug_malloc( (s32)Size, pFileName, LineNumber );
    UsingNew = FALSE;
    return( pResult );
}

//==============================================================================

void operator delete( void* pMemory, char*, s32 )
{
    if( pMemory == NULL )
        return;
    UsingNew = TRUE;
    x_free( pMemory );
    UsingNew = FALSE;
}

//==============================================================================

void operator delete [] ( void* pMemory, char*, s32 )
{
    if( pMemory == NULL )
        return;
    UsingNew = TRUE;
    x_free( pMemory );
    UsingNew = FALSE;
}

//==============================================================================
#endif // X_MEM_DEBUG
//==============================================================================
#endif // !USE_SYSTEM_NEW_DELETE
//==============================================================================

void x_MemAddMark( const char* pComment )
{
    #ifdef X_MEM_DEBUG
    Mark += 1;

    if( Mark == MAX_MARKS )
    {
        mem_header* pHeader = Anchor.pNext;
        while( pHeader != &Anchor )
        {
            pHeader->Mark = 0;
            pHeader = pHeader->pNext;
        }
        Mark = 0;
    }

    x_strncpy( &MarkComment[ Mark * 28 ], pComment, 27 );
    MarkSequence[Mark] = Sequence;
    Sequence = MAX( Sequence+1, 1 );
    #else
    (void)pComment;
    #endif
}

//==============================================================================

void x_MemClearMarks( void )
{
    #ifdef X_MEM_DEBUG
    Mark = 0;
    x_strcpy( MarkComment, "Default 1st Mark" );
    MarkSequence[0] = 0;
    #endif
}

//==============================================================================

void x_MemDump( void )
{
    x_MemDump( "MemDump.txt" );
}

//==============================================================================

void x_MemDump( const char* pFileName )
{
    X_FILE* pFile = x_fopen( pFileName, "wt" );
    
    ASSERT( pFile );

    x_fprintf( pFile, "Mark Sequence Malloc     Size       Addr Line File\n" );
    x_fprintf( pFile, "==== ======== ====== ======== ========== ==== ================================\n" );

    s32 CurrentTotal=0;
    mem_header* pHeader = Anchor.pNext;
    while( pHeader != &Anchor )
    {
        #ifdef X_MEM_DEBUG
        if( pHeader->Mark > pHeader->pPrev->Mark )
        {
            x_fprintf( pFile, "%4d %8d ------ -------- ---------- ---- ----%s\n",
                               pHeader->Mark, 
                               MarkSequence[ pHeader->Mark ], 
                               &MarkComment[ pHeader->Mark * 28 ] );
        }
        #endif

        #ifdef X_MEM_DEBUG
        x_fprintf( pFile, "%4d %8d %s %8d 0x%08X %4d %s\n", 
                          pHeader->Mark,
                          ABS( pHeader->Sequence ), 
                          (pHeader->Sequence > 0) ? "new   " : "malloc",
                          pHeader->RequestedSize, 
                          (u32)(pHeader+1),
                          pHeader->LineNumber, 
                          pHeader->FileName );
        #else
        x_fprintf( pFile, ".... %8d %s %8d 0x%08X .... ....\n", 
                          ABS( pHeader->Sequence ), 
                          (pHeader->Sequence > 0) ? "new   " : "malloc",
                          pHeader->RequestedSize,
                          (u32)(pHeader+1) );
        #endif

        CurrentTotal += pHeader->RequestedSize;

        pHeader = pHeader->pNext;
    }


    x_fprintf( pFile, "==== ======== ====== ======== ========== ==== ================================\n" );
    x_fprintf( pFile, "Mark Sequence Malloc     Size       Addr Line File\n" );
    #ifndef X_MEM_DEBUG
    x_fprintf( pFile, ".... ........ ...... %8d ........ .... ....\n", CurrentTotal);
    #endif

    #ifdef X_MEM_DEBUG
    if( Mark > 0 )
    {
        s32         i;
        s32         Allocs;
        s32         Bytes;
        mem_header* pHeader;

        x_fprintf( pFile, "\n\n\n" );
        x_fprintf( pFile, "Mark   Allocs                 TotalBytes      Comment\n" );
        x_fprintf( pFile, "==== ========                 ==========      ================================\n" );

        for( i = 0; i <= Mark; i++ )
        {
            pHeader = Anchor.pNext;
            Allocs  = 0;
            Bytes   = 0;

            while( pHeader != &Anchor )
            {
                if( pHeader->Mark == i )
                {
                    Allocs += 1;
                    Bytes  += pHeader->RequestedSize;
                }
                pHeader = pHeader->pNext;
            }

            x_fprintf( pFile, "%4d %8d ...... ........ %10d .... %s\n",
                              i, Allocs, Bytes, &MarkComment[i*28] );
        }

        x_fprintf( pFile, "==== ========                 ==========      ================================\n" );
        x_fprintf( pFile, "Mark   Allocs                 TotalBytes      Comment\n" );
    }
    #endif

    #ifdef X_MEM_DEBUG
    {
        mem_header* pA = Anchor.pNext;
        mem_header* pB = Anchor.pNext;

        x_fprintf( pFile, "\n\n\n" );
        x_fprintf( pFile, "       Allocs                 TotalBytes      File\n" );
        x_fprintf( pFile, "     ========                 ==========      ================================\n" );

        pA = Anchor.pNext;

        while( pA != &Anchor )
        {
            // Check to see if we've seen allocations from this file before.
            pB = Anchor.pNext;
            while( pB != &Anchor )
            {
                if( x_stricmp( pA->FileName, pB->FileName ) == 0 )
                    break;
                pB = pB->pNext;
            }

            // If we haven't seen anything from this file before, add to the 
            // total.
            if( pB == pA )
            {
                s32 Count = 0;
                s32 Sum   = 0;

                while( pB != &Anchor )
                {
                    if( x_stricmp(pA->FileName,pB->FileName) == 0 )
                    {
                        Count++;
                        Sum += pB->RequestedSize;
                    }
                    pB = pB->pNext;
                }

                // Display values.
                x_fprintf( pFile, "     %8d ...... ........ %10d .... %s\n", Count, Sum, pA->FileName );
            }

            pA = pA->pNext;
        }

        x_fprintf( pFile, "     ========                 ==========      ================================\n" );
        x_fprintf( pFile, "       Allocs                 TotalBytes      File\n" );
    }
    #endif

    #ifdef X_MEM_DEBUG  
    {
        x_fprintf( pFile, "\n\n\n" );
        x_fprintf( pFile, "                                   Value      Summary\n" );
        x_fprintf( pFile, "                              ==========      ================================\n" );
        x_fprintf( pFile, "                              %10d .... Sequence\n",        Sequence     );
        x_fprintf( pFile, "                              %10d .... Current Count\n\n", CurrentCount );
        x_fprintf( pFile, "                              %10d .... Total Bytes\n\n",   TotalBytes   );
        x_fprintf( pFile, "                              %10d .... Maximum Bytes\n",   MaxBytes     );
        x_fprintf( pFile, "                              %10d .... Current Bytes\n",   CurrentBytes );
        x_fprintf( pFile, "\n" );
    }
    #endif  

#if defined( TARGET_PS2 )
    {
        s32 FreeBytes;
        s32 LargestFree;
        s32 Fragments;

        ps2_GetFreeMem(&FreeBytes,&LargestFree,&Fragments);

        x_fprintf( pFile, "                              %10d .... Free Bytes\n",      FreeBytes    );
        x_fprintf( pFile, "                              %10d .... Largest block\n",   LargestFree  );
        x_fprintf( pFile, "                              %10d .... Block Fragments\n", Fragments    );
        x_fprintf( pFile, "\n" );
    }
#endif

    x_fclose( pFile );
}

//==============================================================================

void x_MemSanity( void )
{
    mem_header* pHeader = Anchor.pNext;
    while( pHeader != &Anchor )
    {
        ASSERT( pHeader->pNext->pPrev == pHeader );         // Corruption!
        ASSERT( pHeader->pPrev->pNext == pHeader );         // Corruption!
        ASSERT( pHeader->RequestedSize >= 0 );              // Corruption!
        #if PAD_BYTES > 0
            for( s32 i = 0; i < (PAD_BYTES/4); i++ )
                ASSERT( pHeader->Pad[i] == PAD_VALUE );     // Corruption!
        #endif
        pHeader = pHeader->pNext;
    }
}

//==============================================================================
#ifdef TARGET_PS2

#if defined(TARGET_PS2_DEV) && defined(X_DEBUG)
s32 AdditionalMemoryForDebug = 1*1048576;
#else
s32 AdditionalMemoryForDebug = 0*1048576;
#endif  // X_DEBUG

#define MAX_ALLOCS  512

extern u8 _memory_size[];
extern u8 _end[];
extern u8 _stack_size[];

void ps2_GetFreeMem(s32 *pFree,s32 *pLargest,s32 *pFragments)
{
#if defined(X_DEBUG) && !defined(bwatson)
    *pFree = 1;
    *pLargest = 1;
    *pFragments = 1;
    return;
#endif

    void    *malloc_array[MAX_ALLOCS];
    s32     MaxAllocatable;
    s32     LargestAllocation;
    s32     HighestAddress;
    s32     LowestAddress;
    s32     AllocSize;
    s32     index,i;

    AllocSize = 1048576 * 128;
    AllocSize = (u32)_memory_size - (u32)_end - (u32)_stack_size;
#ifdef TARGET_PS2_DEV
    AllocSize += AdditionalMemoryForDebug;
#endif

    index=0;
    MaxAllocatable = 0;
    HighestAddress = 0;
    LowestAddress = 256*1024*1024;
    SuspendMallocDumps = TRUE;
    while(1)
    {
        s32 addr;

#ifdef X_MEM_DEBUG
        malloc_array[index]=x_debug_malloc(AllocSize,__FILE__,__LINE__);
#else
        malloc_array[index]=x_malloc(AllocSize);
#endif
        addr = (s32)malloc_array[index];
        if (!malloc_array[index])
        {
            AllocSize -= 512;
            if (AllocSize <= 128)
                break;
        }
        else
        {
            MaxAllocatable += AllocSize;
            if (addr > HighestAddress)
            {
                HighestAddress = addr;
            }
            if (addr < LowestAddress)
            {
                LowestAddress = addr;

            }

            printf("fragment %8d %08X\n",AllocSize,addr);

            index++;
            if (index >= MAX_ALLOCS)
            {
                printf("Way too many fragments to count\n");
                index --;
                break;
            }
        }

    }


    for (i=0;i<index;i++)
    {
        x_free(malloc_array[i]);
    }

    LargestAllocation = MaxAllocatable;
    while (LargestAllocation > 0)
    {
#ifdef X_MEM_DEBUG
        malloc_array[0]=x_debug_malloc(LargestAllocation,__FILE__,__LINE__);
#else
        malloc_array[0]=x_malloc(LargestAllocation);
#endif
        if (malloc_array[0])
            break;
        LargestAllocation -= 512;
    }

    if (LargestAllocation)
        x_free(malloc_array[0]);

    *pFree = MaxAllocatable;
    *pLargest = LargestAllocation;
    *pFragments = index;
    SuspendMallocDumps = FALSE;
}
#endif      // TARGET_PS2
