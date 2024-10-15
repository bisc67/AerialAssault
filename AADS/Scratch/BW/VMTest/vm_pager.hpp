#ifndef VM_PAGER_HPP
#define VM_PAGER_HPP

#include "x_files.hpp"

// This page request count block really only needs to contain the same
// number of requests as there are threads since each thread can only
// in effect issue one page request at a time.
#define VM_PAGE_REQUEST_COUNT   8

struct vm_page_request
{
    void    *pvAddr;            // Virtual address for this page
    void    *ppAddr;            // Physical address for this page
    s32     ThreadId;           // Thread to be reactivated when the request is complete
    s32     FileId;             // Identifier for page file (or compressed memory region)
    s32     FileIndex;          // Index within the file or compressed region
    s32     Flags;              // Flags for this request
    s32     Length;             // Page in request length
};

void    vm_InitPager(void);
void    vm_KillPager(void);

#endif // VM_PAGER_HPP