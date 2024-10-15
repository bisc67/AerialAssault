#include "x_files.hpp"
#include "../zlib/zlib.h"
#include "stdlib.h"


char* SIGNATURE="PCK0";
char* EXTENSION=".pck";

struct block_offsets
{
    s32     Offset;
    u16     BlockLength;
    u16     Spare;
};

void main(s32 argc, char** argv)
{

    s32             error;
    byte*           pSourceBuff;
    byte*           pDestBuff;

    X_FILE*         pSourceFile;
    X_FILE*         pDestFile;
    block_offsets*  pOffsetTable;
    s32             BlockCount;
    s32             BlockSize;
    s32             BlockIndex;
    s32             DestSize;
    s32             SourceSize;
    s32             BlockFileOffset;
    s32             FileLength;
    s32             LengthRemain;
    char*           pInFilename=NULL;
    char*           pOutFilename=NULL;
    char            OutFileBuffer[256];
    u32             CurrentSrcLength;
    u32             CurrentDstLength;
    xbool           Decompress;
    xtimer          t;

    x_Init();

    BlockSize = 0;
    Decompress = FALSE;

    argv++;
    argc--;
    while (argc)
    {
        if (argv[0][0]=='-')
        {
            switch(argv[0][1])
            {
            case 'b':
                BlockSize = x_atoi(&argv[0][2]);
                break;
            case 'd':
                Decompress = TRUE;
                break;
            }
        }
        else
        {
            if (!pInFilename)
            {
                pInFilename=argv[0];
            }
            else
            {
                if (pOutFilename)
                {
                    x_printf("Usage: blockpack [-b<blocksize>] <infile> [<outfile>]\n");
                    exit(-1);
                }
                pOutFilename = argv[0];
            }
        }

        argv++;
        argc--;
    }

    if (BlockSize == 0)
    {
        BlockSize = 32;
    }

    if (pInFilename==NULL)
    {
        x_printf("Usage: blockpack [-b<blocksize>] <infile> [<outfile>]\n");
        exit(-1);
    }

    if (pOutFilename==NULL)
    {
        pOutFilename=OutFileBuffer;
        if (Decompress)
        {
            char* pEnd;
            x_strcpy(OutFileBuffer,pInFilename);
            pEnd = pInFilename+x_strlen(pInFilename)-x_strlen(EXTENSION);
            if (x_strcmp(pEnd,EXTENSION)==0)
            {
                *pEnd = 0;
            }
            else
            {
                x_strcat(OutFileBuffer,".upk");
            }
        }
        else
        {
            x_sprintf(OutFileBuffer,"%s%s",pInFilename,EXTENSION);
        }
    }

    BlockSize *= 1024;

    // We just allocate a twice as much since the dest data 'could' be bigger
    // than the source.
    pSourceBuff = (byte*)x_malloc(BlockSize * 2);
    pDestBuff   = (byte*)x_malloc(BlockSize * 2);
    ASSERT(pSourceBuff);
    ASSERT(pDestBuff);

    pSourceFile = x_fopen(pInFilename,"rb");
    if (!pSourceFile)
    {
        x_printf("Error: Unable to open file '%s' for reading\n",pInFilename);
        exit(-1);
    }

    pDestFile = x_fopen(pOutFilename,"wb");
    if (!pDestFile)
    {
        x_printf("Error: Unable to open file '%s' for writing\n",pOutFilename);
        exit(-1);
    }

    t.Start();

    if (Decompress)
    {
        char Buffer[4];

        x_fread(Buffer,4,1,pSourceFile);
        if (x_memcmp(Buffer,SIGNATURE,4)!=0)
        {
            x_printf("Error: Invalid signature for a decompress\n");
        }
        x_fread(&FileLength,sizeof(s32),1,pSourceFile);
        x_fread(&BlockSize,sizeof(s16),1,pSourceFile);

        BlockCount = (FileLength+BlockSize-1)/BlockSize;
        pOffsetTable = (block_offsets*)x_malloc(sizeof(block_offsets)*BlockCount);
        ASSERT(pOffsetTable);
        BlockIndex = 0;

        x_fread(pOffsetTable,sizeof(block_offsets),BlockCount,pSourceFile);
        LengthRemain = FileLength;

        CurrentSrcLength = 0;
        CurrentDstLength = 0;

        while(LengthRemain)
        {
            if (pOffsetTable[BlockIndex].Offset != x_ftell(pSourceFile) )
            {
                x_printf("Error: Invalid block table\n");
                exit(-1);
            }
            SourceSize  = pOffsetTable[BlockIndex].BlockLength;
            DestSize    = MIN(LengthRemain,BlockSize);
            if (x_fread(pSourceBuff,1,SourceSize,pSourceFile)!=SourceSize)
            {
                x_printf("Error: Ran out of file\n");
                exit(-1);
            }
            if (SourceSize == DestSize)
            {
                x_fwrite(pSourceBuff,1,SourceSize,pDestFile);
            }
            else
            {
                error       = uncompress(pDestBuff,(unsigned long*)&DestSize,pSourceBuff,SourceSize);
                if (error)
                {
                    x_printf("Error: Decompress failed\n");
                    exit(-1);
                }
                x_fwrite(pDestBuff,1,DestSize,pDestFile);
            }

            LengthRemain -= DestSize;
            BlockIndex++;
            CurrentSrcLength += SourceSize;
            CurrentDstLength += DestSize;
            x_printf("\r%d/%d(%d)",CurrentDstLength,CurrentSrcLength,FileLength);
        }
    }
    else
    {
        FileLength = x_flength(pSourceFile);
        BlockCount = (FileLength+BlockSize-1)/BlockSize;
        pOffsetTable = (block_offsets*)x_malloc(sizeof(block_offsets)*BlockCount);
        ASSERT(pOffsetTable);
        BlockIndex = 0;
        x_memset(pOffsetTable,0,sizeof(block_offsets)*BlockCount);

        x_fwrite(SIGNATURE,4,1,pDestFile);
        x_fwrite(&FileLength,sizeof(s32),1,pDestFile);
        x_fwrite(&BlockSize,sizeof(s16),1,pDestFile);

        BlockFileOffset = x_ftell(pDestFile);
        x_fwrite(pOffsetTable,sizeof(block_offsets),BlockCount,pDestFile);
        LengthRemain = FileLength;
        CurrentSrcLength = 0;
        CurrentDstLength = 0;

        while(LengthRemain)
        {
            SourceSize  = MIN(LengthRemain,BlockSize);
            DestSize    = BlockSize;
            SourceSize  = x_fread(pSourceBuff,1,SourceSize,pSourceFile);
            error       = compress(pDestBuff,(unsigned long*)&DestSize,pSourceBuff,SourceSize);

            // If we didn't actually make the file any smaller, then just do a block copy
            // without any compression at all
            pOffsetTable[BlockIndex].Offset = x_ftell(pDestFile);
            if (DestSize >= SourceSize)
            {
                pOffsetTable[BlockIndex].BlockLength = SourceSize;
                x_fwrite(pSourceBuff,1,SourceSize,pDestFile);
            }
            else
            {
                pOffsetTable[BlockIndex].BlockLength = DestSize;
                x_fwrite(pDestBuff,1,DestSize,pDestFile);
            }

            LengthRemain -= SourceSize;
            BlockIndex++;
            CurrentSrcLength += SourceSize;
            CurrentDstLength += DestSize;
            x_printf("\r%d/%d(%d)",CurrentDstLength,CurrentSrcLength,FileLength);
        }

        x_fseek(pDestFile,BlockFileOffset,X_SEEK_SET);
        x_fwrite(pOffsetTable,sizeof(block_offsets),BlockCount,pDestFile);
    }

    t.Stop();
    x_printf("\n\nTook %2.2f seconds for %d bytes, %d bytes/sec\n",t.ReadSec(),FileLength,(s32)((f32)FileLength/t.ReadSec()));
    x_fclose(pDestFile);
    x_fclose(pSourceFile);

    x_free(pDestBuff);
    x_free(pSourceBuff);
    x_free(pOffsetTable);

    x_Kill();
}