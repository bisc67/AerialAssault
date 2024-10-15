#include "x_types.hpp"
#include "e_semaphore.hpp"

#include "vm_init.hpp"
#include "vm_structs.hpp"
#include "vm_vars.hpp"
#include "vm_pager.hpp"
#include "vm_util.hpp"
#include "vm_fault.hpp"

vm_vars vm;

extern "C"
{
    void vm_Bootstrap(void);
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void vm_Init(void)
{
    s32     i;
    u8*     pData;

    //
    // At this point, the tlb entries should have already been initialized to
    // a steady state.
    //
    ASSERT(!vm.m_Initialized);
    // Just for now, we're using physical memory range 1M->2M for swapspace.
    x_DebugMsg("vm_Init(): Initialization start\n");
    vm.m_LockSemaphore = sema_Create(1,1);
    pData = (u8*)0x100000;

    for (i=0;i<VM_MAX_PHYS_PAGES;i++)
    {
        vm.m_PageTable[i].Flags = 0;
        vm.m_PageTable[i].pData = pData;
        pData += VM_PAGE_SIZE;
    }

    vm.m_RegionList = NULL;
    vm.m_Initialized = TRUE;
    vm_Bootstrap();             // Reset all default TLB layouts.
    vm_InitPager();
    vm_InitFault();
    vm_Unlock();
    x_DebugMsg("vm_Init: Initialization complete\n");
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void vm_Kill(void)
{
    // It's important to shut down things in the reverse order of startup
    // since this may cause issues with requests pending if not done correctly.
    // Most of the time, this doesn't matter because we're nearly always never
    // going to shut the system down.
    ASSERT(vm.m_Initialized);
    vm_KillFault();
    vm_KillPager();
    sema_Delete(vm.m_LockSemaphore);
    vm.m_Initialized = FALSE;
}
