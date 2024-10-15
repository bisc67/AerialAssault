//=========================================================================
//
// TEST0.CPP
//
//=========================================================================
#include "entropy.hpp"
#include "e_draw.hpp"
#include "e_input.hpp"
#include "testrender.hpp"

#include "cardmgr/cardmgr.hpp"
#ifdef TARGET_PS2
#include "ps2/iop/iop.hpp"
#include "ps2/ps2_except.hpp"
#endif
//=========================================================================

const char *ErrorToString(s32 error)
{
    switch (error)
    {
    case CARD_ERR_OK:
        return "CARD_ERR_OK";
        break;
    case CARD_ERR_GENERAL:
        return "CARD_ERR_GENERAL";
        break;
    case CARD_ERR_NOTFOUND:
        return "CARD_ERR_NOTFOUND";
        break;
    case CARD_ERR_FAILED:
        return "CARD_ERR_FAILED";
        break;
    case CARD_ERR_FULL:
        return "CARD_ERR_FULL";
        break;
    case CARD_ERR_DIRFULL:
        return "CARD_ERR_DIRFULL";
        break;
    case CARD_ERR_UNFORMATTED:
        return "CARD_ERR_UNFORMATTED";
        break;
    case CARD_ERR_CORRUPT:
        return "CARD_ERR_CORRUPT";
        break;
    case CARD_ERR_UNPLUGGED:
        return "CARD_ERR_UNPLUGGED";
        break;
    case CARD_ERR_CHANGED:
        return "CARD_ERR_CHANGED";
        break;
    case CARD_ERR_EXISTS:
        return "CARD_ERR_EXISTS";
        break;
    case CARD_ERR_PROTECTED:
        return "CARD_ERR_PROTECTED";
        break;
    case CARD_ERR_WRITEFAILED:
        return "CARD_ERR_WRITEFAILED";
        break;
    case CARD_ERR_TOOMANYOPEN:
        return "CARD_ERR_TOOMANYOPEN";
        break;
    default:
        return "Bad error code";
        break;
    }
}
//=========================================================================
s32     SourceMask=0;
s32     DestMask=0;
typedef u_long128 u128;

void AppMain( int argc,char **argv )
{
    xtimer timer;
    card_info Info;
    s32 status,i;

    card_file Directory[32];
    s32 DirEntryCount;

    (void)argc;         // Just to get rid of some compiler warnings!
    (void)argv;

    // Init engine in default mode
    eng_Init();
    card_Init();
    except_Init();
#if 0
    {
        xtimer timer;

        u8  *pSource;
        u8  *pDest;
        s32 counter;

        pSource = (u8 *)x_malloc(1048576*4);
        pDest = (u8 *)x_malloc(1048576*4);
        counter = 0;

        x_DebugMsg("sizeof u128=%d\n",sizeof(u128));
        while (1)
        {
            u128  *pDst;
            u128  *pSrc;
            s32     i;

            pDst = (u128 *)((u32)pDest | DestMask);
            pSrc = (u128 *)((u32)pSource | SourceMask);
            timer.Reset();
            timer.Start();
            for (i=0;i<1048576*4;i+=sizeof(u128))
            {
                *pDst++=*pSrc++;
            }
            timer.Stop();
            x_printf("%d: time=%2.2f\n",counter,timer.ReadMs());
            counter++;
            eng_PageFlip();
        }
    }
#endif
    TestRenderInit();

    while (1) 
    {

        timer.Reset();
        timer.Start();
        input_UpdateState();

        TestRenderUpdate();
        TestRenderRender();
        {
            static s32 scrollcount=10;

            scrollcount--;
            if (scrollcount <=0)
            {
                scrollcount =15;
                x_printf("\n");
            }
        }

        status = card_Check("card00:",&Info);
        x_printfxy(0,1,"Status = %d(%s)\n",status,ErrorToString(status));
        if (status == CARD_ERR_CHANGED)
        {
            s32 resetflag;
            xtimer timer;

            x_printf("Memcard change detected\n");
            resetflag = FALSE;
            DirEntryCount=0;
            timer.Reset();
            timer.Start();
            while(1)
            {
                resetflag = card_Search("card00:*",&Directory[DirEntryCount],resetflag);
                if (resetflag != CARD_ERR_OK)
                    break;
                resetflag = TRUE;
                DirEntryCount++;
            }
            timer.Stop();
            x_printf("%d entries. Took %2.2fms to read.\n",DirEntryCount,timer.ReadMs());
        }
        else if (status != 0)
        {
            DirEntryCount=0;
        }

        x_printfxy(0,2,"Directory entries=%d",DirEntryCount);
        for (i=0;i<DirEntryCount;i++)
        {
            x_printfxy(0,3+i,"[%d] Name=%s, length=%d",i,Directory[i].Filename,Directory[i].Length);
        }

        if (input_WasPressed(INPUT_PS2_BTN_START))
        {
            *(u32 *)(1) = 0x4afb0001;
        }

        if (input_WasPressed(INPUT_PS2_BTN_SELECT))
        {
            ASSERTS(FALSE,"Test assert");
        }

        if (input_WasPressed(INPUT_PS2_BTN_L2))
        {
        }

        if (input_IsPressed(INPUT_PS2_BTN_R_STICK))
        {
        }

        if (input_WasPressed(INPUT_PS2_BTN_R1))
        {
            xbool create;

            while (1)
            {
                input_UpdateState();
                x_printfxy(1,10,"Confirm [X] create, [O] abort");
                if (input_WasPressed(INPUT_PS2_BTN_CROSS))
                {
                    create = TRUE;
                    break;
                }
                if (input_WasPressed(INPUT_PS2_BTN_CIRCLE))
                {
                    create = FALSE;
                    break;
                }
                eng_PageFlip();
            }
            if (create)
            {
                card_file *fp;
                xtimer timer;

                eng_PageFlip();
                x_printfxy(1,10,"Creating file....");
                eng_PageFlip();
                x_printfxy(1,10,"Creating file....");
                eng_PageFlip();

                timer.Reset();
                timer.Start();
                card_MkDir("BASLUS-BLAHBLAHBLAH");
                card_ChDir("BASLUS-BLAHBLAHBLAH");
                card_MkIcon("Tribes 2 for PS2");
                // First, create the icon.sys file which must be the
                // first within a directory. Or so I think.
                fp = card_Open("TEST-FILE","w");
                if (fp)
                {
                    card_Write(fp,Directory,102400);
                    card_Close(fp);
                }
                timer.Stop();
                x_printf("Took %2.2fms to create file\n",timer.ReadMs());

                fp = card_Open("TEST-FILE","r");
                if (fp)
                {
                    char *pBuffer;

                    timer.Reset();
                    timer.Start();
                    pBuffer = (char *)x_malloc(102400);
                    ASSERT(pBuffer);
                    card_Read(fp,pBuffer,102400);
                    x_free(pBuffer);
                    card_Close(fp);
                    x_printf("Took %2.2fms to read the file\n",timer.ReadMs());
                }
            }
        }

        if (input_WasPressed(INPUT_PS2_BTN_R2))
        {
        }

         if (input_WasPressed(INPUT_KBD_X))
        {
        }
        timer.Stop();
        x_printfxy(1,15,"Time = %2.2f",timer.ReadMs());
        eng_PageFlip();

    }

    // Shutdown engine
    eng_Kill();
}

//=========================================================================
