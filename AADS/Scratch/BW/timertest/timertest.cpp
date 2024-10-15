#include "eekernel.h"
#include "eeregs.h"
#include "stdio.h"

// Types for PS2 targets under SN Systems' compiler.
#ifdef __GNUC__
typedef unsigned short      u16;
typedef   signed int        s32;
typedef unsigned long long  u64;
#endif


static inline const void inline_test(const char*,...) {}
#define XTICKS_PER_MS   576

static volatile s32 s_TimerWrapCount;
static s32 s_HandlerId;

//==============================================================================
static s32 TimerCallback(s32)
{
    // Make sure timer will fire off an interrupt when it wraps again
    DPUT_T0_MODE( DGET_T0_MODE());
    s_TimerWrapCount++;
    ExitHandler();
    return 0;
}

//==============================================================================
void Init(void)
{
	inline_test("Hello World");
    s_HandlerId = AddIntcHandler(INTC_TIM0,TimerCallback,0);
    s_TimerWrapCount=0;
    DPUT_T0_COUNT(0);
    DPUT_T0_MODE(   (2<<0)|         // Clock is BUSCLK/256
                    (1<<7)|         // Continue counting
                    (1<<9)          // Overflow will generate an interrupt
                    );
    EnableIntc(INTC_TIM0);
}


//==============================================================================
void ResetTime(void)
{
    DPUT_T0_COUNT(0);
    s_TimerWrapCount = 0;
}

//==============================================================================
u64 ReadTime(void)
{
    u64 v1,v2;
    u16 count;
    u64 t;

    v1 = s_TimerWrapCount;
    count = *T0_COUNT;
    v2 = s_TimerWrapCount;

    // Confirms an interrupt didn't occur between the wrap count and timer count reads
    if (v1!=v2)
    {
       t =(v2 << 16) | *T0_COUNT;
    }
    else
    {
        t = (v1<<16)|count; 
    }
    
    return t / XTICKS_PER_MS;

}

//-----------------------------------------------------------------------------
void    DelayCallback(s32 id, u16 count,void *arg)
{
    (void)id;
    (void)count;
    iWakeupThread((s32)arg);
    ExitHandler();
}

//-----------------------------------------------------------------------------
void    Delay(s32 milliseconds)
{
    SetAlarm(16*milliseconds,DelayCallback,(void *)GetThreadId());
    SleepThread();
}
//==============================================================================
//==============================================================================
char TimeString[128];

int main( int ,char **)
{
    u64 c;
    s32 t;
    s32 h,m,s,ms;

    Init();

    printf("Timer test started\n");
    while(1)
    {
        t = ReadTime();
        Delay(10);
        t = ReadTime() - t;

        c = ReadTime();
        ms = (c % 1000);
        s  = (c / 1000) % 60;
        m  = (c / 60000) % 60;
        h =  (c / 3600000);
        sprintf(TimeString,"%02d:%02d:%02d.%03d",h,m,s,ms);

        if (t>11)
        {
            printf("[%s] Timer exceeded by %dms\n",TimeString,t);
        }
    }
    return 0;
}

