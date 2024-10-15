#include "e_mqueue.hpp"
#include "e_threads.hpp"

#include "eekernel.h"
#include "sifdev.h"

#include "vm_pager.hpp"
#include "vm_vars.hpp"

static void vm_PagerUpdate(void);

//-----------------------------------------------------------------------------
// Create the updater thread and all message queues associated with it
//-----------------------------------------------------------------------------
void vm_InitPager(void)
{
    vm_page_request* pPageRequest;
    s32 i;

    mq_Create(&vm.m_qAvailableRequests,VM_PAGE_REQUEST_COUNT);
    mq_Create(&vm.m_qPendingRequests,VM_PAGE_REQUEST_COUNT);
    for (i=0;i<VM_PAGE_REQUEST_COUNT;i++)
    {
        mq_Send(&vm.m_qAvailableRequests,pPageRequest,MQ_BLOCK);
    }
    vm.m_ExitPager = FALSE;
    vm.m_PagerThreadId = eng_CreateThread(vm_PagerUpdate,"Virtual Memory Pager",8192,2);

}

//-----------------------------------------------------------------------------
void vm_EnqueueRequest(X_FILE *fp,s32 FileIndex,void *pvAddr,void *ppAddr,s32 Length,s32 ThreadId)
{
    vm_page_request* pPageRequest;

    pPageRequest = (vm_page_request*)mq_Recv(&vm.m_qAvailableRequests,MQ_BLOCK);
    ASSERT(pPageRequest);

    pPageRequest->FileId    = (s32)fp;
    pPageRequest->FileIndex = FileIndex;
    pPageRequest->Flags     = 0;
    pPageRequest->Length    = Length;
    pPageRequest->ppAddr    = ppAddr;
    pPageRequest->pvAddr    = pvAddr;
    pPageRequest->ThreadId  = ThreadId;
    mq_Send(&vm.m_qPendingRequests,pPageRequest,MQ_BLOCK);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void vm_KillPager(void)
{
    s32 timeout;
    // We first need to wait until all the paging requests have been completed if
    // any are pending.
    timeout = 1000;
    vm.m_ExitPager = TRUE;
    mq_Send(&vm.m_qPendingRequests,NULL,MQ_BLOCK);
    while (vm.m_ExitPager)
    {
        eng_DelayThread(2);
        timeout--;
        ASSERTS(timeout,"Timed out waiting for pager to go idle.");
    }
    eng_DestroyThread(vm.m_PagerThreadId);
    mq_Destroy(&vm.m_qAvailableRequests);
    mq_Destroy(&vm.m_qPendingRequests);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void vm_PagerUpdate(void)
{
    vm_page_request* pRequest;

    while(1)
    {
        pRequest = (vm_page_request*)mq_Recv(&vm.m_qPendingRequests,MQ_BLOCK);
        if (pRequest)
        {
            sceRead(pRequest->FileId,pRequest->ppAddr,pRequest->Length);
            ResumeThread(pRequest->ThreadId);
            mq_Send(&vm.m_qAvailableRequests,pRequest,MQ_BLOCK);
        }
        else
        {
            ASSERTS(vm.m_ExitPager,"Null ptr received but exit was not requested.");
            vm.m_ExitPager = FALSE;
            break;
        }
    }
    // Thread should kill itself on exit which should make the destroy routine
    // pretty simple.
}