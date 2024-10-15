#include "vm_fault.hpp"
#include "vm_vars.hpp"
#include "x_files.hpp"
#include "eekernel.h"
#include "stdio.h"

typedef u_long128 u128;

typedef 	void (*handler)(u_int, u_int, u_int, u_int, u_int, u_long128 *);

extern "C" void _kExitTLBHandler(void);
void vm_FaultHandler(u32 stat,u32 cause,u32 epc,u32 bva,u32 pba,u128 *gpr);

void vm_FaultHandler(u32 stat,u32 cause,u32 epc,u32 bva,u32 pba,u128 *gpr)
{
    static s32 FaultTick = 0;


    s32 ThreadId;
    scePrintf("[%d] Fault occurred, cause=%08x, epc=%08x, bva=%08x\n",FaultTick++,cause,epc,bva);
    scePrintf("                stat=%08x, pba=%08x, gpr=%08x\n",stat,pba,gpr);
    ThreadId = GetThreadId();
    iSuspendThread(ThreadId);

    scePrintf("Thread id = %d\n",ThreadId);
    if (FaultTick > 100)
        BREAK;
    asm("
    sw  %0,0x30($29)
    ":/* No output Parameters */
     :"r"(_kExitTLBHandler)
      );

    ExitHandler();
}

extern "C" void* SetVTLBRefillHandler(s32,void *);
extern "C" void* SetVCommonHandler(s32,void*);

void vm_InitFault(void)
{

#if 0
    vm.m_OldFaultHandler[0] = SetDebugHandler(TLB_MODIFICATION,vm_FaultHandler);       // TLB Change
    vm.m_OldFaultHandler[1] = SetDebugHandler(TLB_REFILL_LOAD, vm_FaultHandler);       // TLB Load or Instruction Fetch
    vm.m_OldFaultHandler[2] = SetDebugHandler(TLB_REFILL_STORE,vm_FaultHandler);       // TLB Write
#else
    vm.m_OldFaultHandler[0] = SetVTLBRefillHandler(TLB_MODIFICATION,vm_FaultHandler);       // TLB Change
    vm.m_OldFaultHandler[1] = SetVTLBRefillHandler(TLB_REFILL_LOAD, vm_FaultHandler);       // TLB Load or Instruction Fetch
    vm.m_OldFaultHandler[2] = SetVTLBRefillHandler(TLB_REFILL_STORE,vm_FaultHandler);       // TLB Write

    vm.m_OldFaultHandler[0] = SetVCommonHandler(TLB_MODIFICATION,vm_FaultHandler);       // TLB Change
    vm.m_OldFaultHandler[1] = SetVCommonHandler(TLB_REFILL_LOAD, vm_FaultHandler);       // TLB Load or Instruction Fetch
    vm.m_OldFaultHandler[2] = SetVCommonHandler(TLB_REFILL_STORE,vm_FaultHandler);       // TLB Write
#endif
}

void vm_KillFault(void)
{
    SetDebugHandler(1,(handler)vm.m_OldFaultHandler[0]);
    SetDebugHandler(2,(handler)vm.m_OldFaultHandler[1]);
    SetDebugHandler(3,(handler)vm.m_OldFaultHandler[2]);
}
