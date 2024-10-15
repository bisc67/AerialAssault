/*----------------------------------------------------------------------------
 * PROPACK compression/decompression 'C' header file
 *
 * Copyright (c) 1995 Rob Northen Computing, UK. All Rights Reserved.
 *
 * File: PP.H
 *
 * Date: 7-APR-95
 *----------------------------------------------------------------------------*/

#include "x_target.hpp"

#define RNCERROR_OK             0
#define RNCERROR_BADFILEID      -1
#define RNCERROR_UNKNOWNMETHOD  -2

#define HEADERLEN				18
#define CODE_SIZE				32768
#define PACKBLOCKSIZE			0x3000
#define	CRCPOLY					0xA001
#define MIN_LEN					2
#define MIN_POS					1024

#define MAX_POS_M1				32768
#define MAX_LEN_M1				4096
#define MAX_RAW_M1				65535

#define MAX_POS_M2				4096
#define MAX_LEN_M2				255
#define MAX_RAW_M2				72

#ifdef LITTLE_ENDIAN

#define BIGENDIANW(n)           (((n&0xFF)<<8)+((n&0xFF00)>>8))
#define BIGENDIANL(n)           ((BIGENDIANW(n)<<16)+BIGENDIANW((n>>16)))

#else

#define BIGENDIANW(n)           (n)
#define BIGENDIANL(n)           (n)

#endif

#define GETBIGENDIANW(x)        (((u16)*(x)<<8)+(u8)*(x+1))
#define UPDATE_CRC(r,c)         r=CRCTable[((u8)(r)^(u8)(c))&0xFF]^(r>>8)

struct rncfile {
    u8 Id[3];
    u8 Method;
    u32 UncompressedSize;
    u32 CompressedSize;
    u8 Reserved[6];
    u8 Data[0];
};

typedef struct huffman {
	u32 Frequency;
	u16 EntryPtr;
	u32 Code;
	u16 CodeLen;
} huffman;

typedef struct rncfile *RNC_fileptr;
typedef struct huffman *HUFFMAN_tableptr;

extern huffman RawHuffmanTable[],
			   PosHuffmanTable[],
			   LenHuffmanTable[];

extern void InitHuffmanTable(HUFFMAN_tableptr Table, u8 TableSize),
			MakeHuffmanCodes(HUFFMAN_tableptr Table, u8 n),
			MakeHuffmanTable(HUFFMAN_tableptr Table, u8 n);
extern u16 FindLowest(HUFFMAN_tableptr Table, u8 n);
extern u32 SwapBits(u32 InBits, u8 n);

extern u8 BitBuffBits,
			*InputPtr,
			*OutputPtr;

extern u32 PackRNC(u8 *Input, u8 *Output, u32 Len, u8 Method);
extern u16 UnpackRNC(RNC_fileptr FilePtr, u8 *OutputBuffer);
//extern void Propack_UnpackM1(void *FilePtr, void *OutputBuffer);
//extern void Propack_UnpackM2(void *FilePtr, void *OutputBuffer);

typedef struct s_Propack
{
	u8	Id[3];						// MUST be RNC
	u8	Method;						// MUST be 0,1,2 (will have 0x80 added if indexed type)
	u32	UnpackedSize;
	u32	PackedSize;					// Including size of ALL data
	u16	BlockCount;					// Indexed method block count
	u8	BlockSize;					// When method is indexed, this gives us the block size in KB.
	u8	Reserved[3];
	u8	Data[0];
} t_Propack;


/* Prototypes for unpack routines in assembly - Note: these just do the raw unpacking
   everything else is done at a level above
*/

void	Propack_UnpackM1(void *Source, void *Dest,u32 CompLen,u32 UncompLen);
void	Propack_UnpackM2(void *Source, void *Dest,u32 CompLen,u32 UncompLen);

u16 UnpackMethod1(RNC_fileptr FilePtr, u8 *OutputBuffer);
u16 UnpackMethod2(RNC_fileptr FilePtr, u8 *OutputBuffer);
