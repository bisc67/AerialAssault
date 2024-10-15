#include <eekernel.h>
#include <libgraph.h>
#include <devvu1.h>
#include <stdio.h>
#include <sifrpc.h>
#include <sifcmd.h>
#include <sifdev.h>
#include <libcdvd.h>
#include <malloc.h>
#include "x_types.hpp"

#include "debug.hpp"
#define ASSERT(x) if (!(x)) asm("break 0x1");



static sceGsDBuff       s_db;
xbool s_PalMode = 0;
sceGsLoadImage gsImage PS2_ALIGNMENT(64);

s32 inc = 1024/64;

int main(int argc,char* argv[])
{
	s32				handle,length;
	u8*				mem;
	u8*				ptr;
	s32				i;
	s32				XRes;


	DebugMsg("Am I here %d?");
	//
	// Initialize CD system
	//
	sceSifInitRpc(0);
	sceCdInit(SCECdINIT);
	sceCdMmode(SCECdCD);

	//
    // GS
    //
    scePrintf("EarlyStart: Initializing graphics environment\n");
    sceGsResetPath();
    sceDevVu1PutDBit(1);
    *(u_long *)&s_db.clear0.rgbaq = SCE_GS_SET_RGBAQ(0x00, 0x00, 0x00, 0x00, 0x3f800000);
    *(u_long *)&s_db.clear1.rgbaq = SCE_GS_SET_RGBAQ(0x00, 0x00, 0x00, 0x00, 0x3f800000);
    if (s_PalMode)
    {
        sceGsResetGraph( 0, SCE_GS_INTERLACE, SCE_GS_PAL, SCE_GS_FIELD);
    }
    else
    {
        sceGsResetGraph( 0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FIELD);
    }
    //
    // VBLANK
    //
    scePrintf("EarlyStart: Waiting for vblank\n");
    FlushCache(0);
    while(sceGsSyncV(0) == 0){};  

#ifdef TARGET_PS2_DEV
	handle = sceOpen("host:splash.tga",SCE_RDONLY);
#else
	handle = sceOpen("cdrom0:\\SPLASH.TGA;1",SCE_RDONLY);
#endif
	ASSERT(handle>=0);

	length = sceLseek(handle,0,SCE_SEEK_END);
	sceLseek(handle,0,SCE_SEEK_SET);

	mem = (u8*)malloc(length);
	ASSERT(mem);

	sceRead(handle,mem,length);
	sceClose(handle);


	XRes=512;


	// Now, download the image to display memory
	ptr = mem+18;
	for (i=0;i<448;i++)
	{
#if 1
		for (s32 j=0;j<XRes;j++)
		{
			u8 t;

			t = ptr[j*4+1];
			ptr[j*4+1] = ptr[j*4+3];
			ptr[j*4+3] = t;
		}
#endif
		sceGsSetDefLoadImage(&gsImage,  0,					// Base address of dest buffer
										512/64,				// Width of transfer
										SCE_GS_PSMCT32,		// Data format
										0,					// Dest X
										i,					// Dest Y
										XRes,				// Dest width
										1					// Dest height
										);
		FlushCache(0);
		sceGsExecLoadImage(&gsImage, (u_long128*)ptr);
		sceGsSyncPath(0, 0);
		ptr+=XRes*sizeof(u32);
	}

	free(mem);
	while(sceGsSyncV(0) == 0){};  
	sceGsDispEnv disp;

	// ////////////////////////////
	// 
	//  Initilize display
	// 
	sceGsSetDefDispEnv(
    	&disp,
	SCE_GS_PSMCT32,
	512,
	448,
	0,
	0
	);
	sceGsPutDispEnv(&disp);

	printf("EarlyStart: Executing game image\n");
    sceSifExitCmd();
	LoadExecPS2("cdrom0:\\TRIBES.ELF;1", argc,argv);

}