#include "x_files.hpp"
#include "x_threads.hpp"

extern "C"
{
	#include <kernel.h>
	#include <sifrpc.h>
	#include <sif.h>
	#include <libsd.h>

}

xmutex* g_DmaLockMutex;
xsema*  g_DmaSemaphore;

s32     _SpuDmaInt(void *common)
{
    (void)common;

    g_DmaSemaphore->Release(X_TH_INTERRUPT);
    return 1;
}

void spu_Init(void)
{
    g_DmaLockMutex = new xmutex;
	g_DmaSemaphore = new xsema(1,1);

    // Initialize the sound hardware.
    sceSdInit(0);
    sceSdSetCoreAttr(SD_C_SPDIF_MODE,(SD_SPDIF_MEDIA_DVD | SD_SPDIF_OUT_PCM | SD_SPDIF_COPY_NORMAL));
    sceSdSetTransIntrHandler (0, (sceSdSpu2IntrHandler) _SpuDmaInt, NULL);
}

void ee_Transfer(void* pBase, s32 dest, s32 size)
{
  sceSifDmaData fdma;
  int id;
  s32 oldstat;

  fdma.data = (s32)pBase;
  fdma.addr = dest;
  fdma.size = (size & 0xfffffff0) + ((size % 0x10) ? 0x10 : 0);
  fdma.mode = 0;
  FlushDcache();
  CpuSuspendIntr(&oldstat);
  id = sceSifSetDma(&fdma, 1);
  CpuResumeIntr(oldstat);
  g_DmaSemaphore->Acquire();
  while(0 <= sceSifDmaStat(id));
}
s32 wait;
//-----------------------------------------------------------------------------
// Synchronous data transfer to Spu
void    spu_Transfer(u8 *pBase,s32 Dest,s32 length,s32 direction)
{
    s32 status;
    s32 xferlen;
  int id;

    FlushDcache();
    length = (length + 15) & ~15;
    ASSERT( ((s32)pBase & 15) == 0);
    ASSERT( (Dest & 15) == 0);

    direction = SD_TRANS_MODE_WRITE | SD_TRANS_BY_DMA;

    while (length)
    {
        xferlen = length;
        if (xferlen > 16 * 1024)
            xferlen = 16 * 1024;
    
        g_DmaLockMutex->Enter();
        status = sceSdVoiceTrans(0,
                    direction,
                    pBase,
                    Dest,
                    xferlen);
        g_DmaSemaphore->Acquire();
        g_DmaLockMutex->Exit();
        length -= xferlen;
        pBase += xferlen;
        Dest += xferlen;
    }
	wait=0;
#if 1
	while(0 <= sceSifDmaStat(id))
  {
	  wait++;
  };
#endif
}
//-----------------------------------------------------------------------------
int main(int,char**)
{
	xtick t;
	s32 count;
	u8* buffer;

	f32 a,b,c,d,e;

	a = 1.5f;
	b = 2.0f;
	c = a * b;
	d = c / b;
	e = c / a;

	a = -2.0f;
	a = 2.3f;
	a = x_frand(1.0f,20.0f);

	a = d + e + x_GetTimeSec();

	b = -2.0f;
	for (s32 i=0;i<100;i++)
	{
		x_DebugMsg("Cycle %i, value %s\n",i,(char*)b);
		b+=0.04f;
	}

	spu_Init();

	t = x_GetTime();

	buffer = (u8*)x_malloc(8192);
	ASSERT(buffer);

	x_DebugMsg("Transfer Start\n");
	while (x_GetTime() - t < x_GetTicksPerMs()*1000)
	{
	//	fl = fl + ((f32)x_GetTime() / 10000);
		spu_Transfer(buffer,0x4000,4096,0);
		count++;
		//ee_Transfer(buffer,48*1048576,4096);
	}

	t = x_GetTime() - t;

	x_DebugMsg("%d KB transferred in 1 second.",count*4);

	while(1)
	{
		x_DelayThread(1000);
		x_DebugMsg("Tick %s\n",(char*)f32(x_GetTimeSec()));
	}
	BREAK;
    return 0;
}

