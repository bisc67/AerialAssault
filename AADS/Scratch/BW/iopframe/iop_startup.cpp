#include "x_files.hpp"
#include "x_threads.hpp"

extern "C"
{
//	#include <loadcore.h>

	void __do_global_ctors(void);
	void __do_global_dtors(void);
//	ModuleInfo Module = {"inev_iop_driver", 0xf101};

}

extern int main(int argc, char*argv[]);
static void	Entrypoint(void);

xthread* s_pMainThread;
//-----------------------------------------------------------------------------
static int s_argc;
static char**s_argv;
int start (int argc,char *argv[])
{
    s32 status;


	x_Init();

    x_DebugMsg("Inevitable Generic Iop driver\n");
    x_DebugMsg("Compiled %s,%s\n",__DATE__,__TIME__);

    s_argc = argc;
	s_argv = argv;
    s_pMainThread = new xthread(Entrypoint,"Application Entrypoint",32768,0);

    x_DebugMsg ("ineviop: Thread created. Loader finished.\n");
    return 0;
}

static void Entrypoint(void)
{
	__do_global_ctors();
	main(s_argc,s_argv);
	__do_global_dtors();
	x_Kill();
}


