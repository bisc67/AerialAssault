#include "x_types.hpp"
#include "x_files.hpp"
#include "encode.hpp"
#include "library/encvag.h"
#include "memory.h"
#include "../../support/audiomgr/audio.hpp"

#include "main.hpp"

s32 pc_Encode(s32 Length, u8* pOutBuffer, s8* pFilename,t_DecodeHeader *pHeader, s32 packageId)
{
    // The length of data.
    s32 len = strlen((const char *)pFilename);
    s32 looplen = sizeof(pHeader->LoopStart) + sizeof(pHeader->LoopEnd);
    u8* bytes;
    u8* Buffer;
    Buffer = pOutBuffer;
    bytes = (u8 *)pFilename;
    
    // Copy the name of fill where the data is supposed to be.
    for (s32 i = 0; i < len; i++)
        Buffer[i] = bytes[i];

    // We only want to the looping information for the effect package.
    if (packageId == 2)
    {
        x_memcpy(&Buffer[i] ,&pHeader->LoopStart, sizeof(u32));
        i += sizeof(u32);
        x_memcpy(&Buffer[i] ,&pHeader->LoopEnd, sizeof(u32));
        return (len+looplen);
    }
    
    return len;
}