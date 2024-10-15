#include <eekernel.h>
#include "ioptypes.h"
#include <sif.h>
#include <sifrpc.h>
#include <sifdev.h>
#include <stdio.h>

#ifdef TARGET_PS2_DEV
#define NETCNF_ICON_FILE "icon=host:c:/projects/t2/xcore/entropy/ps2/modules/devnetcnf/sys_net.ico" "\0" "iconsys=host:c:/projects/t2/xcore/entropy/ps2/modules/devnetcnf/icon.sys"
#define INET_CONFIG_FILE "-no_auto"
#define SPDUART_CONFIG_PARAMS "dial=host:c:/projects/t2/xcore/entropy/ps2/modules/devnetcnf/dial_spd.cnf"
#else
#define NETCNF_ICON_FILE "icon=cdrom:\\TRIBES\\MODULES\\CDNETCNF\\SYS_NET.ICO;1" "\0" "iconsys=cdrom:\\MODULES\\CDNETCNF\\ICON.SYS;1"
#define INET_CONFIG_FILE "-no_auto"
#define SPDUART_CONFIG_PARAMS "dial=cdrom:\\TRIBES\\MODULES\\CDNETCNF\\DIAL_SPD.CNF;1"
#endif

#define INET_CONFIG_PARAMS "mem=96KB"

//-----------------------------------------------------------------------------
s32 iop_LoadModule (char *module_name,char *pArg,s32 arglength)
{
    char NameBuffer[128];

    sprintf(NameBuffer,"host:c:/projects/t2/xcore/entropy/ps2/modules/%s",module_name);
    return sceSifLoadModule(NameBuffer,arglength,pArg) >= 0;
}

int main(s32 argc,char *argv[])
{
    (void)argc;
    (void)argv;

    sceSifInitRpc(0);

    iop_LoadModule("sio2man.irx",NULL,0);
    iop_LoadModule("padman.irx",NULL,0);
    iop_LoadModule("mcman.irx",NULL,0);
    iop_LoadModule("mcserv.irx",NULL,0);

    iop_LoadModule("inet.irx",INET_CONFIG_PARAMS,sizeof(INET_CONFIG_PARAMS));
    iop_LoadModule("netcnf.irx",NETCNF_ICON_FILE,sizeof(NETCNF_ICON_FILE));
    iop_LoadModule("inetctl.irx",INET_CONFIG_FILE,sizeof(INET_CONFIG_FILE));
    iop_LoadModule("dev9.irx",NULL,0);
    iop_LoadModule("ppp.irx",NULL,0);
    iop_LoadModule("smap.irx",NULL,0);
    iop_LoadModule("spduart.irx",SPDUART_CONFIG_PARAMS,sizeof(SPDUART_CONFIG_PARAMS));
    iop_LoadModule("modemiop.irx",NULL,0);
    while(1)
    {
        SleepThread();
    }
}

