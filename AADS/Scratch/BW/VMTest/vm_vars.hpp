#ifndef VM_VARS_HPP
#define VM_VARS_HPP

#include "x_types.hpp"
#include "e_mqueue.hpp"

#include "vm_util.hpp"
#include "vm_pager.hpp"

struct vm_vars
{
    xbool           m_Initialized;
    vm_tlb_entry    m_TLB[VM_MAX_TLB_ENTRIES];
    vm_page         m_PageTable[VM_MAX_PHYS_PAGES];
    s32             m_LockSemaphore;
    vm_region*      m_RegionList;
    s32             m_PagerThreadId;
    message_queue   m_qAvailableRequests;
    message_queue   m_qPendingRequests;
    vm_page_request m_PageRequests[VM_PAGE_REQUEST_COUNT];
    xbool           m_ExitPager;
    void*           m_OldFaultHandler[3];

};

extern vm_vars vm;

#endif // VM_VARS_HPP
