#include "entropy.hpp"
#include "movieplayer/movieplayer.hpp"
#include "audiomgr/audio.hpp"
#include "ps2/ps2_except.hpp"

#define AVG_FRAMES 100

void ps2_GetFreeMem(s32 *,s32 *,s32 *);
void test(void);

void AppMain(s32,char **)
{
    eng_Init();
    audio_Init();
#if 0
    s32     effects,music,voice;
    effects=    audio_LoadContainer("/projects/t2/demo1/data/audio/effects.pkg");
    music=      audio_LoadContainer("/projects/t2/demo1/data/audio/music.pkg");
    voice=      audio_LoadContainer("/projects/t2/demo1/data/audio/voice.pkg");

    audio_UnloadContainer(voice);
    audio_UnloadContainer(music);
    audio_UnloadContainer(effects);
#endif
    audio_SetMasterVolume(1.0f,1.0f);

    test();

    while (1)
    {
        mq_DebugDump();
        movie_PlayAsync("mixed.pss",512,448);
        movie_WaitAsync(FALSE);
    }

}

extern u8 test1(f32 value);
extern u8 test2(f32 value);

f32 fast_sqrt(f32 v)
{
    f32 ret;
  __asm__ __volatile__("
        mfc0 $0,$0
		" :"=f"(ret) : "f" (v));

        return ret;

}

#if 0
  __asm__ __volatile__("
		lqc2	vf4, 0x0(%1)
		lqc2	vf5, 0x0(%2)
		vsub.xyz vf5, vf5, vf4
		vmul.xyz vf4, vf5, vf5
		vmr32.xy vf5, vf4
		vmr32.x  vf6, vf5
		vadd.x   vf7, vf4, vf5
		vadd.x   vf5, vf6, vf7
		vsqrt   Q, vf5x
		vwaitq
		cfc2     $2, $vi22
		mtc1     $2,%0
		" :"=f"(ret) : "r" (src), "r" (dst) :"$2");
#endif

void test(void)
{
    s32 i;
    xtimer t;


    t.Reset();
    t.Start();

    for (i=0;i<1000000;i++)
    {
        if (test1(i)!=test2(i))
        {
            ASSERT(FALSE);
        }
    }
    t.Stop();
    x_DebugMsg("Test1 Took %2.2f ms\n",t.ReadMs());

    t.Reset();
    t.Start();

    for (i=0;i<1000000;i++)
    {
        if (test2(i)!=test2(i+1))
        {
            //ASSERT(FALSE);
        }
    }
    t.Stop();
    x_DebugMsg("Test2 Took %2.2f ms\n",t.ReadMs());
}

