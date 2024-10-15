#include "x_files.hpp"
#include "vm_util.hpp"
#include "vm_pager.hpp"
#include "vm_vars.hpp"
#include "eekernel.h"
#include "e_semaphore.hpp"


//byte SwapSpace[VM_MAX_PHYS_PAGES * VM_PAGE_SIZE] PS2_ALIGNMENT(VM_PAGE_SIZE);

//-----------------------------------------------------------------------------
// Lock the VM lists. This will be exclusive access to the internal tables
//-----------------------------------------------------------------------------
void vm_Lock(void)
{
    sema_Wait(vm.m_LockSemaphore);
}

//-----------------------------------------------------------------------------
// Release the VM list lock.
//-----------------------------------------------------------------------------

void vm_Unlock(void)
{
    sema_Signal(vm.m_LockSemaphore);
}

void vm_MapTLB   ( s32 Index, void *vAddr,void *pAddrEven,void *pAddrOdd,s32 PageSize)
{
    vm_tlb_entry& TLB=vm.m_TLB[Index];

    x_memset(&TLB,0,sizeof(TLB));

    TLB.EntryLo0.Bits.G   = 1;
    TLB.EntryLo0.Bits.D   = 1;  // Always read only
    TLB.EntryLo0.Bits.C   = 3;  // Cached with write back, and write allocate

    TLB.EntryLo1.Bits.D   = 1;  // Always read only
    TLB.EntryLo1.Bits.C   = 3;  // Cached with write back, and write allocate
    TLB.EntryLo1.Bits.G   = 1;

    if ((s32)pAddrEven != -1)
    {
        TLB.EntryLo0.Bits.PFN = (s32)pAddrEven / VM_PAGE_SIZE;
        TLB.EntryLo0.Bits.V   = 1;
    }

    if ((s32)pAddrOdd != -1)
    {
        TLB.EntryLo1.Bits.PFN = (s32)pAddrOdd / VM_PAGE_SIZE;
        TLB.EntryLo1.Bits.V   = 1;
    }

    if ((s32)vAddr != -1)
    {
        TLB.EntryHi.Bits.VPN  = (s32)vAddr / (VM_PAGE_SIZE * 2);
    }

    TLB.PageMask.Bits.MASK = PageSize;

    vm_WriteTLB(Index,TLB);
    x_memset(&TLB,0xff,sizeof(TLB));
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void vm_WriteTLB(s32 Index, const vm_tlb_entry &TLB)
{
    asm("
        mtc0 %0,$0;         /* Index    */
        mtc0 %1,$2;         /* EntryLo0 */
        mtc0 %2,$3;         /* EntryLo1 */
        mtc0 %3,$10;        /* EntryHi  */
        mtc0 %4,$5;         /* PageMask */
        tlbwi;
        sync.p;
        ":/* No output Parameters */
         :"r"(Index),
          "r"(TLB.EntryLo0.Raw),
          "r"(TLB.EntryLo1.Raw),
          "r"(TLB.EntryHi.Raw),
          "r"(TLB.PageMask.Raw)
          );
}

const vm_tlb_entry& vm_ReadTLB(s32 Index)
{
    asm("
        mtc0 %4,$0;         /* Index    */
        tlbr;
        sync.p;
        mfc0 %0,$2;         /* EntryLo0 */
        mfc0 %1,$3;         /* EntryLo1 */
        mfc0 %2,$10;        /* EntryHi  */
        mfc0 %3,$5;         /* PageMask */
        "
            :"=r"(vm.m_TLB[Index].EntryLo0.Raw),
             "=r"(vm.m_TLB[Index].EntryLo1.Raw),
             "=r"(vm.m_TLB[Index].EntryHi.Raw),
             "=r"(vm.m_TLB[Index].PageMask.Raw)
            :"r"(Index) 
        );
        return vm.m_TLB[Index];
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void vm_Debug_DumpTLBs(void)
{
    s32             i;
    vm_tlb_entry    TLB;

    for (i=0;i<48;i++)
    {
        u32 vlo,vhi;
        u32 p0lo,p0hi;
        u32 p1lo,p1hi;
        s32 pagesize;
        char flags0[16],flags1[16];
        char *pFlags;

        x_memset(flags0,0,sizeof(flags0));
        x_memset(flags1,0,sizeof(flags1));
        
        TLB = vm_ReadTLB(i);

        x_DebugMsg("[%d]: %08x,%08x,%08x,%08x\n",i,TLB.EntryLo0.Raw,TLB.EntryLo1.Raw,TLB.EntryHi.Raw,TLB.PageMask.Raw);

        switch (TLB.PageMask.Bits.MASK)
        {
        case 0x000:
            pagesize=4;
            break;
        case 0x003:
            pagesize=16;
            break;
        case 0x00f:
            pagesize=64;
            break;
        case 0x03f:
            pagesize=256;
            break;
        case 0x0ff:
            pagesize=1024;
            break;
        case 0x3ff:
            pagesize=4096;
            break;
        case 0xfff:
            pagesize=16384;
            break;
        default:
            pagesize=-1;
        }
// Build mapping information
        vlo = TLB.EntryHi.Bits.VPN << 13;
        p0lo = TLB.EntryLo0.Bits.PFN << 12;
        p1lo = TLB.EntryLo1.Bits.PFN << 12;
        p0hi = p0lo + pagesize * 1024 - 1;
        p1hi = p1lo + pagesize * 1024 - 1;
        vhi = vlo + pagesize * 1024 - 1;
// Build flags
        static char CacheFlags[8]={'?','?','U','C','?','?','?','A'};

        pFlags = flags0;
        *pFlags++=CacheFlags[TLB.EntryLo0.Bits.C];
        if (TLB.EntryLo0.Bits.D)    *pFlags++='D'; else *pFlags++='-';
        if (TLB.EntryLo0.Bits.V)    *pFlags++='V'; else *pFlags++='-';
        if (TLB.EntryLo0.Bits.G)    *pFlags++='G'; else *pFlags++='-';
        if (TLB.EntryLo0.Bits.S)    *pFlags++='S'; else *pFlags++='-';

        pFlags = flags1;
        *pFlags++=CacheFlags[TLB.EntryLo1.Bits.C];
        if (TLB.EntryLo1.Bits.D)    *pFlags++='D'; else *pFlags++='-';
        if (TLB.EntryLo1.Bits.V)    *pFlags++='V'; else *pFlags++='-';
        if (TLB.EntryLo1.Bits.G)    *pFlags++='G'; else *pFlags++='-';

        x_DebugMsg("TLB %02d EVEN - VM: %08x->%08x PHYS: %08x->%08x, flags: %s\n",i,vlo,vhi,p0lo,p0hi,flags0);
        vlo+=pagesize * 1024;
        vhi+=pagesize * 1024;
        x_DebugMsg("TLB %02d ODD  - VM: %08x->%08x PHYS: %08x->%08x, flags: %s\n",i,vlo,vhi,p1lo,p1hi,flags1);

    }
}