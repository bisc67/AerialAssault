#ifndef VM_UTIL_HPP
#define VM_UTIL_HPP

#include "x_types.hpp"
#include "x_files.hpp"
#include "vm_structs.hpp"

#define VM_MAX_TLB_ENTRIES  48

#define VM_PAGE_SIZE_4K     0x000
#define VM_PAGE_SIZE_16K    0x003
#define VM_PAGE_SIZE_64K    0x00f
#define VM_PAGE_SIZE_256K   0x03f
#define VM_PAGE_SIZE_1M     0x0ff
#define VM_PAGE_SIZE_4M     0x3ff
#define VM_PAGE_SIZE_16M    0xfff

struct vm_tlb_entrylo0
{
    union
    {
        u32     Raw;
        struct
        {
            unsigned int G   :1;
            unsigned int V   :1;
            unsigned int D   :1;
            unsigned int C   :3;
            unsigned int PFN :20;
            unsigned int ZERO:5;            // Unused, set to 0.
            unsigned int S   :1;
        } Bits;
    };
};

struct vm_tlb_entrylo1
{
    union
    {
        u32         Raw;
        struct
        {
            unsigned int G   :1;
            unsigned int V   :1;
            unsigned int D   :1;
            unsigned int C   :3;
            unsigned int PFN :20;
            unsigned int ZERO:6;
        } Bits;
    };
};

struct vm_tlb_entryhi
{
    union
    {
        u32     Raw;
        struct
        {
            unsigned int ASID:8;
            unsigned int ZERO:5;
            unsigned int VPN :19;
        } Bits;
    };
};

struct vm_tlb_pagemask
{
    union
    {
        u32 Raw;
        struct
        {
            unsigned int ZERO:13;
            unsigned int MASK:12;
            unsigned int ZERO2:7;
        } Bits;
    };
};

struct vm_tlb_entry
{
    vm_tlb_entryhi  EntryHi;
    vm_tlb_entrylo0 EntryLo0;
    vm_tlb_entrylo1 EntryLo1;
    vm_tlb_pagemask PageMask;
};

//-----------------------------------------------------------------------------
struct vm_page
{
    vm_page*        pPrev;
    vm_page*        pNext;
    s32             Flags;
    void*           pData;
};

//-----------------------------------------------------------------------------
struct vm_region
{
    vm_region*      pPrev;
    vm_region*      pNext;
    void*           pBase;
    s32             PageCount;
    vm_page_flags   Flags;
};

void                vm_Init     ( void );
void                vm_Kill     ( void );
void                vm_WriteTLB ( s32 Index, const vm_tlb_entry &TLB );
const vm_tlb_entry& vm_ReadTLB  ( s32 Index );
void                vm_Debug_DumpTLBs(void);
void                vm_MapTLB   ( s32 Index, void *pvAddr,void *pAddrEven,void *pAddrOdd,s32 PageSize);
void                vm_MapFile  ( X_FILE *fp,void *pvAddr, s32 Size);
void                vm_MapCompressed(void *pCompressedData,void *pvAddr,s32 Size);
void*               vm_Alloc    ( s32 Size, s32 flags );
void                vm_Free     ( void* pvAddr );

void                vm_Lock     ( void );
void                vm_Unlock   ( void );

#endif //VM_UTIL_HPP