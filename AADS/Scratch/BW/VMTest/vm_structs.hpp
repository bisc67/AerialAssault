#ifndef VM_STRUCTS_HPP
#define VM_STRUCTS_HPP

#include "x_types.hpp"
#define VM_MAX_PHYS_PAGES   1024
#define VM_PAGE_SIZE        256             // 1M swapspace

enum vm_page_flags
{
    B_VM_FLAGS_ALLOCATED,
    B_VM_FLAGS_LOCKED,
    B_VM_FLAGS_READONLY,
};

#define VM_FLAGS_ALLOCATED  (1<<B_VM_FLAGS_ALLOCATED)
#define VM_FLAGS_LOCKED     (1<<B_VM_FLAGS_LOCKED)
#define VM_FLAGS_READONLY   (1<<B_VM_FLAGS_READONLY)

#endif // VM_STRUCTS_HPP