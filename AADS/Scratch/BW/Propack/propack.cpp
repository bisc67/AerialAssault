#include "x_files.hpp"
#include "entropy.hpp"
#include "stdlib.h"
#include "pp.h"

void Usage(void)
{
	x_printf("propack <option> <method> <blocksize> <in-file> <out-file>\n"
		   "   Option is -d to decompress\n"
		   "   Method is -m1 for method 1, -m2 for method 2\n"
		   "   Blocksize is -b<nn> where <nn> is the KB size\n"
		   "   when this is specified, method flag is or'ed with 0x80\n"
		   "   in the output file format.\n");
	exit(-1);
}

struct block_header
{
    u32     Offset;
    u32     Length;
};

#ifdef TARGET_PC
int main(s32 argc,char* argv[])
#else
void AppMain(s32 argc, char* argv[])
#endif
{

	const char* pInFile;
	const char* pOutFile;
	s32			Method;
	s32			BlockSize;
	s32			index;
	xbool		Decompress;
	u8*			pInData;
	u8*			pOutData;
	s32			InLength;
	s32			OutLength;
	s32			result;
    s32         BlockCount;
	X_FILE*		fp;
    X_FILE*     pOutFP;
    block_header* pBlockOffsets;

	pInFile=NULL;
	pOutFile=NULL;
	BlockSize = 0;
	Method = 1;
	Decompress = FALSE;

#ifdef TARGET_PC
	x_Init();
	index = 1;
	while (index < argc)
	{
		if (argv[index][0]=='-')
		{
			switch(argv[index][1])
			{
			case 'b':
			case 'B':
				BlockSize = atoi(&argv[index][2]);
				if ( (BlockSize <=0) || (BlockSize >= 64) )
					Usage();
				break;
			case 'm':
			case 'M':
				Method = atoi(&argv[index][2]);
				if ( ( Method != 1) && (Method !=2) )
					Usage();
				break;
			case 'd':
				Decompress = TRUE;
				break;
			default:
				Usage();
			}
		}
		else if (pInFile==NULL)
		{
			pInFile = argv[index];
		}
		else if (pOutFile==NULL)
		{
			pOutFile = argv[index];
		}
		else
		{
			Usage();
		}
		index++;
	}

	if ( (pInFile==NULL) || (pOutFile==NULL) )
	{
		Usage();
	}
#else
	pInFile = "meridian.rnc";
	pOutFile = "testfile.2";
	Method = 1;
	BlockSize = 0;
	Decompress=TRUE;
#endif
	x_printf("InFile=%s, OutFile=%s, Method=%d, BlockSize=%d\n",pInFile,pOutFile,Method,BlockSize);

	fp = x_fopen(pInFile,"rb");
	if (!fp)
	{
		x_printf("Error: Unable to open '%s' for input\n",pInFile);
	}

	InLength = x_flength(fp);

	pOutFP = x_fopen(pOutFile,"wb");
	if (!pOutFP)
	{
		x_printf("Error: Unable to open file '%s' for writing\n",pOutFile);
		exit(-1);
	}

	xtimer t;

	t.Reset();
	t.Start();
	if (Decompress)
	{
		rncfile* pData;
        s32 ReadLength;

        if (BlockSize == 0)
        {
            BlockSize = 32*1024;
        }
	    pInData = (u8*)x_malloc  ( BlockSize );
        // We alloc more space for our output data as the compress operation MAY increase
        // the block size if it can't be compressed!
        pOutData = (u8*)x_malloc (BlockSize * 2);
	    ASSERT(pInData);
        ASSERT(pOutData);

        while(InLength)
        {
            ReadLength = InLength;
            if (InLength > BlockSize)
            {
                InLength = BlockSize;
            }
        }


	    x_fread(pInData,InLength,1,fp);
	    x_fclose(fp);

        pData = (rncfile*)pInData;
		OutLength = BIGENDIANL(pData->UncompressedSize);
		InLength  = BIGENDIANL(pData->CompressedSize);

		pOutData = (u8*)x_malloc(OutLength+1024);
		ASSERT(pOutData);

		result = (u32)UnpackRNC(pData,pOutData);

		x_printf("Done. Original Size=%d, Decompressed Size=%d, Ratio=%d%%\n",InLength,OutLength,InLength*100/OutLength);
	}
	else
	{
        s32 ReadLength;
        u32 FileBlockStart;

        if (BlockSize == 0)
        {
            BlockSize = 32;
        }
        BlockSize *= 1024;
	    pInData = (u8*)x_malloc  ( BlockSize );
        // We alloc more space for our output data as the compress operation MAY increase
        // the block size if it can't be compressed!
        pOutData = (u8*)x_malloc (BlockSize * 2);
	    ASSERT(pInData);
        ASSERT(pOutData);

        BlockCount = (InLength+BlockSize-1)/BlockSize;
        pBlockOffsets = (block_header*)x_malloc(BlockCount * sizeof(block_header));
        ASSERT(pBlockOffsets);
        s32 BlockIndex = 0;

        FileBlockStart = x_ftell(pOutFP);
        // Write out the dummy block headers. These will be re-written once the
        // file compression is complete.
        x_fwrite(pBlockOffsets,sizeof(block_header),BlockCount,pOutFP);

        while(InLength)
        {
            ReadLength = InLength;
            if (InLength > BlockSize)
            {
                ReadLength = BlockSize;
            }
            ReadLength = x_fread(pInData,1,ReadLength,fp);
            OutLength = PackRNC(pInData,pOutData,ReadLength,Method);

            pBlockOffsets[BlockIndex].Offset = x_ftell(pOutFP);
            if (OutLength >= ReadLength)
            {
                pBlockOffsets[BlockIndex].Length = ReadLength;
                x_fwrite(pInData,InLength,1,pOutFP);

            }
            else
            {
                pBlockOffsets[BlockIndex].Length = OutLength;
                x_fwrite(pOutData,OutLength,1,pOutFP);
            }

            InLength -= ReadLength;
            BlockIndex++;
        }

        x_fseek(pOutFP,FileBlockStart,X_SEEK_SET);
        x_fwrite(pBlockOffsets,sizeof(block_header),BlockCount,pOutFP);
		x_printf("Done. Original Size=%d, Compressed Size=%d, Ratio=%d%%\n",InLength,OutLength,OutLength*100/InLength);
	}

    x_fclose(pOutFP);
	t.Stop();
	x_printf("Completed in %2.2f seconds\n",t.ReadSec());

#ifdef TARGET_PC
	return 0;
#endif
}