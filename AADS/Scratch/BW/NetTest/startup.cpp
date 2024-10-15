// Main application entry point
#include "x_files.hpp"
#include "x_threads.hpp"

extern char _bss_org[];
extern char _bss_size[];
extern char _gp[];
extern char _stack[];
extern char _stack_size[];

extern "C"
{
    void _InitSys(void);
}

char*_args[256];

void _root(void)
{
}

extern void ps2eng_Begin(s32,void**);
extern void ps2eng_End(void);

#define STACK_SIZE 1024

s8  s_MainStack[STACK_SIZE];

void AutoExit(void)
{
    while(1);
}

void ENTRYPOINT(void)
{
    xthread *pMainThread;

    // Clear out the bss segment
    x_memset(_bss_org,0,(s32)_bss_size);

    // Set up the main thread

    asm(
        "

            move    $4,%0            # gp
            move    $5,%1            # stack
            move    $6,%2            # stack size
            move    $7,%3            # args
            move    $8,%4            # termination point
            move    $28,%0           # Set our current gp
            move    $29,%1           # Set our stack pointer
            add     $3,$0,60 
            syscall 
        ":                        // No return parameters
         :"r"(_gp),
          "r"(s_MainStack),
          "r"(STACK_SIZE),
          "r"(_args),
          "r"(AutoExit):"$2"
          );

     // Set up the heap


    // Set up the initial thread

    // Initialize the system
    _InitSys();

    // Construct the initial thread
//    ps2eng_Begin(0,NULL);


    ps2eng_End();
}