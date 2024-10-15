#include "sifrpc.h"
#include "sifdev.h"
#include "stdlib.h"
#include "stdio.h"
#include "libgraph.h"

char* SearchPath[]=
{
	"host:"XCORE_PATH"/3rdparty/ps2/sn/iop/modules",
#ifdef X_DEBUG
	"host:ps2-iop-debug",
#else
	"host:ps2-iop-release",
#endif
	"host:"XCORE_PATH"/entropy/ps2/modules",
	NULL
};

char *KernelError(int status)
{
    switch (status)
    {
    case -200:
        return "KE_LINKERR";
        break;
    case -201:
        return "KE_ILLEGAL_OBJECT";
        break;
    case -202:
        return "KE_UNKNOWN_MODULE";
        break;
    case -203:
        return "KE_NOFILE";
        break;
    case -204:
        return "KE_FILEERR";
        break;
    case -205:
        return "KE_MEMINUSE";
        break;
	case -400:
		return "KE_NO_MEMORY";
		break;
    default:
        break;
    }
    return "<unknown>";
}

void LoadModule(const char* pFilename)
{
	char**	pSearch;
	char	path[256];
	int		retcode;

	pSearch = SearchPath;

	while(*pSearch)
	{
		sprintf(path,"%s/%s",*pSearch,pFilename);
		retcode = sceSifLoadModule(path,0,NULL);
		if (retcode != -203)
			break;
		pSearch++;
	}
	if (retcode < 0)
	{
		scePrintf("LoadModule(%s): sceSifLoadModule returned error %d(%s)\n",pFilename,retcode,KernelError(retcode));
		asm("break 0x001");
	}
}


int main(void)
{

	sceSifInitRpc(0);
	LoadModule("sndbgext.irx");
	LoadModule("libsd.irx");
	LoadModule("iopframe.irx");
	while(1)
	{
		sceGsSyncV(0);
	}
}