/*----------------------------------------------------------------------------
 * PROPACK Unpacker 'C' Source Code
 *
 * Copyright (c) 1995 Rob Northen Computing, UK. All Rights Reserved.
 *
 * File: UNPACK.C
 *
 * Date: 18.03.95
 *----------------------------------------------------------------------------*/

#include "x_files.hpp"
#include "pp.h"

u8 BitBuffM2,
     *OutputEnd;

u32 BitBuffM1;

u16 UnpackRNC(RNC_fileptr, u8 *OutputBuffer);
u16 UnpackMethod1(RNC_fileptr FilePtr, u8 *OutputBuffer);
u16 UnpackMethod2(RNC_fileptr FilePtr, u8 *OutputBuffer);
void InitUnpack(RNC_fileptr FilePtr, u8 *OutputBuffer);
u16 InputBitsM1(u8 n);
u16 InputBitsM2(u8 n);
void InputHuffmanTable(HUFFMAN_tableptr Table, u8 TableSize);
u16 InputValue(HUFFMAN_tableptr Table);
u16 InputLenM2(void);
u16 InputPosM2(void);

u16 UnpackRNC(RNC_fileptr FilePtr, u8 *OutputBuffer)
{
    u16	result;
	 char	*string;

	 string = (char *)&FilePtr->Id[0];

	 if ((string[0] != 'R') || (string[1] != 'N') || (string[2] != 'C'))
		 return RNCERROR_BADFILEID;


    switch (FilePtr->Method) {
        case 0:
				// Rob Northern's original lines - didn't compile
            // _fmemcpy(OutputBuffer, &FilePtr->Data, BIGENDIANL(FilePtr->UncompressedSize));
            x_memcpy(OutputBuffer, &FilePtr->Data, BIGENDIANL(FilePtr->UncompressedSize));
            result = RNCERROR_OK;
            break;

        case 1:
            result = UnpackMethod1(FilePtr, OutputBuffer);
            break;

        case 2:
            result = UnpackMethod2(FilePtr, OutputBuffer);
            break;

        default:
            result = RNCERROR_UNKNOWNMETHOD;
    }

    return result;
}

u16 UnpackMethod1(RNC_fileptr FilePtr, u8 *OutputBuffer)
{
    u8 *Ptr;

    u16 LoopCount,
         Len;

    InitUnpack(FilePtr, OutputBuffer);

    InputBitsM1(2); // lock and key bits

    while (OutputPtr < OutputEnd) {
        InputHuffmanTable(&RawHuffmanTable[0], 16);
        InputHuffmanTable(&PosHuffmanTable[0], 16);
        InputHuffmanTable(&LenHuffmanTable[0], 16);
        LoopCount = InputBitsM1(16);
        goto Start;

        Again:
            Ptr = OutputPtr - (InputValue(PosHuffmanTable) + 1);
            Len = InputValue(LenHuffmanTable) + 2;
            while (Len--)
                *OutputPtr++ = *Ptr++;
        Start:
            Len = InputValue(RawHuffmanTable);
            while (Len--)
                *OutputPtr++ = *InputPtr++;
            BitBuffM1 = ((((u32)*(InputPtr+2) << 16) + ((u16)*(InputPtr+1) << 8) + *InputPtr) << BitBuffBits)
                         + (BitBuffM1 & (1 << BitBuffBits) - 1);
            if (--LoopCount) goto Again;
    }

    return RNCERROR_OK;
}

u16 UnpackMethod2(RNC_fileptr FilePtr, u8 *OutputBuffer)
{
    u8 *Ptr;

    u16 Pos,
         Len;

    InitUnpack(FilePtr, OutputBuffer);

    InputBitsM2(2); // lock and key bits

    while (OutputPtr < OutputEnd) {
        for (;;) {
            while (InputBitsM2(1) == 0)
                *OutputPtr++ = *InputPtr++;
            if (InputBitsM2(1)) {
                if (InputBitsM2(1) == 0) {
                    Len = 2;
                    Pos = *InputPtr++ + 1;
                } else {
                    if (InputBitsM2(1) == 0)
                        Len = 3;
                    else {
                        if ((Len = *InputPtr++ + 8) == 8)
                            break;
                    }
                    Pos = InputPosM2();
                }
                Ptr = OutputPtr - Pos;
                while (Len--)
                    *OutputPtr++ = *Ptr++;
            }
            else {
                if ((Len = InputLenM2()) == 9) {
                    Len = (InputBitsM2(4) << 2) + 12;
                    while (Len--)
                        *OutputPtr++ = *InputPtr++;
                } else {
                    Ptr = OutputPtr - InputPosM2();
                    while (Len--)
                        *OutputPtr++ = *Ptr++;
                }
            }
        }
        InputBitsM2(1);
    }

    return RNCERROR_OK;
}

void InitUnpack(RNC_fileptr FilePtr, u8 *OutputBuffer)
{
    InputPtr = (u8 *)&FilePtr->Data;
    OutputPtr = OutputBuffer;
    OutputEnd = OutputBuffer + BIGENDIANL(FilePtr->UncompressedSize);
    BitBuffBits = 0;
}

u16 InputBitsM1(u8 n)
{
    u16 Bits = 0,
         BitMask = 1;

    while (n--) {
        if (BitBuffBits == 0) {
			BitBuffM1 = (InputPtr[0]<<8)|InputPtr[1];
            InputPtr += 2;
            BitBuffBits = 16;
        }
        if (BitBuffM1 & 1)
            Bits |= BitMask;
        BitMask <<= 1;
        BitBuffM1 >>= 1;
        BitBuffBits--;
    }

    return Bits;
}

u16 InputBitsM2(u8 n)
{
    u16 Bits = 0;

    while (n--) {
        if (BitBuffBits == 0) {
            BitBuffM2 = *InputPtr++;
            BitBuffBits = 8;
        }
        Bits <<= 1;
        if (BitBuffM2 & 0x80)
            Bits++;
        BitBuffM2 <<= 1;
        BitBuffBits--;
    }

    return Bits;
}

void InputHuffmanTable(HUFFMAN_tableptr Table, u8 TableSize)
{
    u8 n,
         i;

    HUFFMAN_tableptr tableptr;

    InitHuffmanTable(Table, TableSize);

    if ((n = (u8)InputBitsM1(5)) != 0) {
        if (n > 16)
            n = 16;
        tableptr = Table;
        for (i = 0; i < n; i++) {
            tableptr->CodeLen = InputBitsM1(4);
            tableptr++;
        }
        MakeHuffmanCodes(Table, n);
    }
}

u16 InputValue(HUFFMAN_tableptr Table)
{
    u8 Bits = 0;

    HUFFMAN_tableptr tableptr;

    tableptr = Table;

	while ( (tableptr->CodeLen == 0) || ((BitBuffM1 & (1 << tableptr->CodeLen) - 1) != tableptr->Code)) {
		tableptr++;
        Bits++;
    }

    InputBitsM1(tableptr->CodeLen);

    if (Bits < 2)
        return Bits;

    return InputBitsM1(Bits - 1) | (1 << (Bits - 1));
}

u16 InputLenM2(void)
{
    u16 Len = InputBitsM2(1) + 4;

    if (InputBitsM2(1) == 0)
        return Len;

    return ((Len - 1) << 1) + InputBitsM2(1);
}

u16 InputPosM2(void)
{
    u16 Pos = 0;

    if (InputBitsM2(1)) {
        Pos = InputBitsM2(1);
        if (InputBitsM2(1)) {
            Pos = ((Pos << 1) + InputBitsM2(1)) | 4;
            if (InputBitsM2(1) == 0)
                Pos = (Pos << 1) + InputBitsM2(1);
        } else
            if (Pos == 0)
                Pos = InputBitsM2(1) + 2;
    }

    return (Pos << 8) + *InputPtr++ + 1;
}

