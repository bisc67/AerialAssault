/*----------------------------------------------------------------------------
 * PROPACK compression 'C' source code
 *
 * Copyright (c) 1995 Rob Northen Computing, UK. All Rights Reserved.
 *
 * File: PACK.C
 *
 * Date: 12-APR-95
 *----------------------------------------------------------------------------*/

#include "x_files.hpp"
#include "pp.h"

u8 *InputBuffer,
     *InputPtrHi,
     *InputEnd,
     *OutputBuffer,
     RawBuff[9650],
     run9s_data[]  = { 0, 14, 8, 10, 18, 19, 22 },
     run9s_bits[]  = { 0, 4, 4, 4, 5, 5, 5 },
     bitseq_data[] = { 0, 6, 8, 9, 21, 23, 29, 31, 0x28, 0x29, 0x2c, 0x2d, 0x38, 0x39, 0x3c, 0x3d },
     bitseq_bits[] = { 1, 3, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6 };

u16 *DictLnk,
     *DictLen,
     *DictPtrF,
     *DictPtrL,
     *ValueBuffer,
     *ValuePtr,
     MaxPos,
     MaxLen,
     NewEntry,
     MatchPos,
     MatchLen,
     CRCTable[512],
     UnpackedDataCRC,
     PackedDataCRC,
     RawCount,
     BitBuff,
     RawBuffBytes,
     PackBlocks;

u32 InputLen,
     DoneLen,
     PackLen,
     UnpackBuffBytes,
     Counts;

void PackM1(void),
     PackM2(void),
     MakeValues(void),
     PutRawBytesM2(u16 n),
     PutHuffmanTable(HUFFMAN_tableptr Table, u16 n),
     PutPackedValue(u16 n, HUFFMAN_tableptr Table),
     PutPackedBits(u16 n, u8 Bits, u8 Method),
     PutPackedBitsM1(u16 n, u8 Bits),
     PutPackedBitsM2(u16 n, u8 Bits),
     PutPackedByteM1(u8 c),
     PutPackedByteM2(u8 c),
     BestMatch(void),
     GetMatch(void),
     InitDictionary(void),
     UpdateDictionary(u16 n),
     PutValue(u16 Val, HUFFMAN_tableptr Table),
     MakeCRCTable(void);

u8 BitLength(u16 n),
     GetUnpackedByte(void);

u32 PackRNC(u8 *Input, u8 *Output, u32 Len, u8 Method);

/*
int main(int argc, char *argv[])
{
    u8 *InputBuffer,
         *OutputBuffer,
         Method;

    int inhandle,
        outhandle;

    u32 UncompressedSize,
         CompressedSize;

    if (argc < 4) {
        printf("SYNTAX: PACK <Input File> <Output File> <Method>\n");
        exit(EXIT_FAILURE);
    }

    Method = (u8)atoi(argv[3]);

    if ((inhandle = open(argv[1], O_RDONLY | O_BINARY)) == -1) {
        printf("failed to open: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    UncompressedSize = filelength(inhandle);

    if ((InputBuffer = (u8 *)malloc(UncompressedSize)) == NULL) {
        printf("failed to allocate input buffer\n");
        exit(EXIT_FAILURE);
    }

    if (read(inhandle, InputBuffer, UncompressedSize) == -1) {
        printf("failed to read file\n");
        free((void *)InputBuffer);
        exit(EXIT_FAILURE);
    }

    if ((OutputBuffer = (u8 *)malloc(UncompressedSize + 20000)) == NULL) {
        printf("failed to allocate output buffer\n");
        free((void *)InputBuffer);
        exit(EXIT_FAILURE);
    }

    if ((CompressedSize = PackRNC(InputBuffer, OutputBuffer, UncompressedSize, Method)) == 0) {
        printf("failed to pack file\n");
        free((void *)InputBuffer);
        free((void *)OutputBuffer);
        exit(EXIT_FAILURE);
    }

    if ((outhandle = open(argv[2], O_RDWR | O_BINARY | O_CREAT)) == -1) {
        printf("failed to open: %s\n", argv[2]);
        free((void *)InputBuffer);
        free((void *)OutputBuffer);
        exit(EXIT_FAILURE);
    }

    if (write(outhandle, OutputBuffer, CompressedSize) != CompressedSize) {
        printf("failed to write file\n");
        free((void *)InputBuffer);
        free((void *)OutputBuffer);
        close(outhandle);
        remove(argv[2]);
        exit(EXIT_FAILURE);
    }

    free((void *)InputBuffer);
    free((void *)OutputBuffer);
    close(inhandle);
    close(outhandle);

    printf("success.\n");

    return(EXIT_SUCCESS);
}
*/

u32 PackRNC(u8 *Input, u8 *Output, u32 Len, u8 Method)
{
    u16 i;

    InputBuffer     = Input;
    OutputBuffer    = Output;
    InputLen        = Len;

    PackLen         = 0;
    UnpackedDataCRC = 0;
    PackedDataCRC   = 0;
    UnpackBuffBytes = 0;
    PackBlocks      = 0;

    DoneLen         = 0;
    BitBuff         = 0;
    BitBuffBits     = 0;
    RawBuffBytes    = 0;

//    DictPtrF    = (u16 *)malloc(65536);
//    DictPtrL    = (u16 *)malloc(65536);
//    DictLnk     = (u16 *)malloc(65536);
//    DictLen     = (u16 *)malloc(65536);
//    ValueBuffer = (u16 *)malloc(65536);

	 DictPtrF = (u16*) x_malloc( 65536);
	 DictPtrL = (u16*) x_malloc( 65536);
	 DictLnk = (u16*)  x_malloc( 65536);
	 DictLen = (u16*)  x_malloc( 65536);
	 ValueBuffer = (u16*) x_malloc( 65536);


    if ((DictPtrF == NULL) || (DictPtrL == NULL) || (DictLnk == NULL) || (DictLen == NULL) || (ValueBuffer == NULL))
        goto PackRNCExit;

    MakeCRCTable();
    InputPtr  = InputBuffer;
    InputEnd  = InputBuffer + InputLen;
    OutputPtr = OutputBuffer;

    *OutputPtr++ = 'R';
    *OutputPtr++ = 'N';
    *OutputPtr++ = 'C';
    *OutputPtr++ = Method;
 
// Rob Northern's original lines - didn't compile
//	 *((u32 *)OutputPtr)++ = BIGENDIANL(InputLen);

#define WRITE_TYPE(ptr, value, type) { *((type*)ptr) = value; ptr = ((unsigned char*)ptr) + sizeof(type); }

	 WRITE_TYPE(OutputPtr, BIGENDIANL(InputLen), unsigned long)

	 
	 OutputPtr += 10;

    PutPackedBits(0, 1, Method); // 'locked' file bit
	PutPackedBits(0, 1, Method); // 'encrypted' file bit

	if (Method == 1)
		PackM1();
	else
		if (Method == 2)
			PackM2();

	for (i = 0; RawBuffBytes; RawBuffBytes--)
		PutPackedByteM1(RawBuff[i++]);

	if (UnpackBuffBytes > (InputLen - PackLen))
		UnpackBuffBytes -= (InputLen - PackLen);
	else
		UnpackBuffBytes = 0;

	if (Method == 2)
		UnpackBuffBytes += 2;

	OutputPtr = OutputBuffer + 8;


// Rob Northern's original lines - didn't compile
//	*((u32 *)OutputPtr)++ = BIGENDIANL(PackLen);
//	*((u16 *)OutputPtr)++ = BIGENDIANW(UnpackedDataCRC);
//	*((u16 *)OutputPtr)++ = BIGENDIANW(PackedDataCRC);
	
	WRITE_TYPE(OutputPtr, BIGENDIANL(PackLen), u32)
	WRITE_TYPE(OutputPtr, BIGENDIANW(UnpackedDataCRC), u16)
	WRITE_TYPE(OutputPtr, BIGENDIANW(PackedDataCRC),u16)


	*OutputPtr++ = (u8)UnpackBuffBytes;
	*OutputPtr++ = (u8)PackBlocks;

PackRNCExit:

//    if (DictPtrF)    free(DictPtrF);
//    if (DictPtrL)    free(DictPtrL);
//    if (DictLnk)     free(DictLnk);
//    if (DictLen)     free(DictLen);
//    if (ValueBuffer) free(ValueBuffer);

	 if (DictPtrF)
		 x_free( DictPtrF);
	 if (DictPtrL)
		 x_free( DictPtrL);
	 if (DictLnk)
		 x_free( DictLnk);
	 if (DictLen)
		 x_free( DictLen);
	 if (ValueBuffer)
		 x_free( ValueBuffer);
	 
	return PackLen + HEADERLEN;
}

void PackM1()
{
    u16 i;

    u32 InputDone = 0;

    MaxPos   = MAX_POS_M1;
    MaxLen   = MAX_LEN_M1;
    InitDictionary();

    while (InputDone < InputLen) {

        InitHuffmanTable(PosHuffmanTable, 16);
        InitHuffmanTable(LenHuffmanTable, 16);
        InitHuffmanTable(RawHuffmanTable, 16);

        MakeValues();

        MakeHuffmanTable(RawHuffmanTable, 16);
        MakeHuffmanTable(PosHuffmanTable, 16);
        MakeHuffmanTable(LenHuffmanTable, 16);

        PutHuffmanTable(RawHuffmanTable, 16);
        PutHuffmanTable(PosHuffmanTable, 16);
        PutHuffmanTable(LenHuffmanTable, 16);

        PutPackedBitsM1((u16)Counts, 16);

        InputPtr = InputBuffer + InputDone;

        while (Counts--) {
            RawCount = *ValuePtr++;
            InputDone += RawCount;
            PutPackedValue(RawCount, RawHuffmanTable);
            if (RawCount) {
                if (BitBuffBits)
                    while (RawCount--)
                        RawBuff[RawBuffBytes++] = GetUnpackedByte();
                else
                    while (RawCount--)
                        PutPackedByteM1(GetUnpackedByte());
            }
            if (Counts) {
                MatchLen = *ValuePtr++;
                MatchPos = *ValuePtr++;
                PutPackedValue(MatchPos, PosHuffmanTable);
                PutPackedValue(MatchLen, LenHuffmanTable);
                MatchLen  += MIN_LEN;
                InputDone += MatchLen;
                while (MatchLen--)
                    GetUnpackedByte();
            }
        }
        if (!BitBuffBits)
            for (i = 0; RawBuffBytes; RawBuffBytes--)
                PutPackedByteM1(RawBuff[i++]);
        PackBlocks++;
    }
    BitBuff >>= (16 - BitBuffBits);
    if (BitBuffBits || RawBuffBytes)
        PutPackedByteM1((u8)BitBuff);
    if ((BitBuffBits > 8) || RawBuffBytes)
        PutPackedByteM1(BitBuff >> 8);
}

void PackM2()
{
    u16 i;

    u32 InputDone = 0;

    MaxPos   = MAX_POS_M2;
    MaxLen   = MAX_LEN_M2;
    InitDictionary();

    while (InputDone < InputLen) {
        MakeValues();
        InputPtr = InputBuffer + InputDone;
        while (Counts--) {
            RawCount = *ValuePtr++;
            InputDone += RawCount;
            PutRawBytesM2(RawCount);
            if (Counts) {
                MatchLen = *ValuePtr++;
                MatchPos = *ValuePtr++;
                if (MatchLen == (2 - MIN_LEN)) {
                    PutPackedBitsM2(6, 3);
                    PutPackedByteM2((u8)MatchPos);
                } else {
                    if (MatchLen < (9 - MIN_LEN)) {
                        PutPackedBitsM2((u16) run9s_data[MatchLen], run9s_bits[MatchLen]);
                        PutPackedBitsM2((u16) bitseq_data[MatchPos >> 8], bitseq_bits[MatchPos >> 8]);
                        PutPackedByteM2(MatchPos & 0xFF);
                    } else {
                        PutPackedBitsM2(0xF, 4);
                        PutPackedByteM2(MatchLen - (8 - MIN_LEN));
                        PutPackedBitsM2((u16) bitseq_data[MatchPos >> 8], bitseq_bits[MatchPos >> 8]);
                        PutPackedByteM2(MatchPos & 0xFF);
                    }
                }
                MatchLen += MIN_LEN;
                InputDone += MatchLen;
                while (MatchLen--)
                    GetUnpackedByte();
            }
        }
        PutPackedBitsM2(0xF, 4);
        PutPackedByteM2(0);
        if (InputDone < InputLen)
            PutPackedBitsM2(1,1);
        else
            PutPackedBitsM2(0,1);
        if (!BitBuffBits)
            for (i = 0; RawBuffBytes; RawBuffBytes--)
                PutPackedByteM1(RawBuff[i++]);
        PackBlocks++;
    }
    BitBuff <<= (8 - BitBuffBits);
    if (BitBuffBits || RawBuffBytes)
        PutPackedByteM1((u8)BitBuff);
}

void MakeValues()
{
    Counts = RawCount = 0;

    ValuePtr = ValueBuffer;
    InputPtrHi = InputPtr + PACKBLOCKSIZE;

    if (InputPtrHi > InputEnd)
        InputPtrHi = InputEnd;

    while ( (InputPtr < (InputPtrHi - 1)) && (Counts < 0xFFFE)) {
        BestMatch();
        if (MatchLen >= MIN_LEN) {
            if (InputPtr + MatchLen > InputPtrHi)
                if (Counts)
                    break;
                else
                    MatchLen = InputPtrHi - InputPtr;
            PutValue(RawCount, RawHuffmanTable);
            PutValue(MatchLen - MIN_LEN, LenHuffmanTable);
            PutValue(MatchPos - 1, PosHuffmanTable);
            UpdateDictionary(MatchLen);
            Counts++;
            RawCount = 0;
        } else {
            UpdateDictionary(1);
            RawCount++;
        }
    }

    if ( (InputPtrHi == InputEnd) && (Counts != 0xFFFE) )
        RawCount += InputEnd - InputPtr;
    PutValue(RawCount, RawHuffmanTable);
    Counts++;
    ValuePtr = ValueBuffer;
}

void PutRawBytesM2(u16 n)
{
    u16 i;

    while (n) {
        if (n < 12)
            while (n) {
                PutPackedBitsM2(0, 1);
                PutPackedByteM2(GetUnpackedByte());
                n--;
            } else {
                while (n & 3) {
                    PutPackedBitsM2(0, 1);
                    PutPackedByteM2(GetUnpackedByte());
                    n--;
                }
                PutPackedBitsM2(0x17, 5);
                if (n >= 72) {
                    PutPackedBitsM2(15,4);
                    for (i = 0; i != 72; i++)
                        PutPackedByteM2(GetUnpackedByte());
                    n -= 72;
                } else {
                    PutPackedBitsM2(((n - 12) >> 2), 4);
                    for (; n; n--)
                        PutPackedByteM2(GetUnpackedByte());
                }
            }
    }
}

void PutValue(u16 Val, HUFFMAN_tableptr Table)
{
    Table[(Val > 1) ? BitLength(Val) : Val].Frequency++;
    *ValuePtr++ = Val;
}

u8 GetUnpackedByte(void)
{
    u8 c = *InputPtr++;

    UPDATE_CRC(UnpackedDataCRC, c);

    DoneLen++;

    return c;
}

void PutHuffmanTable(HUFFMAN_tableptr Table, u16 n)
{
    u16 i = n;

    while (i--)
        if (Table[i].CodeLen)
            break;
        else
            n--;
    PutPackedBitsM1(n, 5);
    for (i = 0; i < n; i++)
        PutPackedBitsM1(Table[i].CodeLen, 4);
}

void PutPackedValue(u16 n, HUFFMAN_tableptr Table)
{
    u16 Bits;

    Bits = (n > 1) ? BitLength(n) : n;
    PutPackedBitsM1( (u16)Table[Bits].Code, (u8)Table[Bits].CodeLen );
    if (Bits > 1)
        PutPackedBitsM1(n - (1 << (Bits-1)), Bits-1);
}

void PutPackedBits(u16 n, u8 Bits, u8 Method)
{
    if (Method == 2)
        PutPackedBitsM2(n, Bits);
    else
        PutPackedBitsM1(n, Bits);
}
                
void PutPackedBitsM1(u16 n, u8 Bits)
{
    u16 i;

    while (Bits--) {
        BitBuff >>= 1;
        if (n & 1)
            BitBuff |= 0x8000;
        n >>= 1;
        if (++BitBuffBits == 16) {
            PutPackedByteM1((u8)BitBuff);
            PutPackedByteM1((u8)(BitBuff >> 8));
            for (i = 0; RawBuffBytes; RawBuffBytes--)
                PutPackedByteM1(RawBuff[i++]);
            if (DoneLen > PackLen)
                if ((DoneLen - PackLen) > UnpackBuffBytes)
                    UnpackBuffBytes = DoneLen - PackLen;
            BitBuff = BitBuffBits = 0;
        }
    }
}

void PutPackedBitsM2(u16 n, u8 Bits)
{
    u16 Mask,
         i;

    Mask = 1 << (Bits-1);

    while (Bits--) {
        BitBuff <<= 1;
        if (n & Mask)
            BitBuff++;
        Mask >>= 1;
        if (++BitBuffBits == 8) {
            PutPackedByteM1((u8)BitBuff);
            for (i = 0; RawBuffBytes; RawBuffBytes--)
                PutPackedByteM1(RawBuff[i++]);
            if (DoneLen > PackLen)
                if ((DoneLen - PackLen) > UnpackBuffBytes)
                    UnpackBuffBytes = DoneLen - PackLen;
            BitBuff = BitBuffBits = 0;
        }
    }
}

void PutPackedByteM1(u8 c)
{
    *OutputPtr++ = c;
    UPDATE_CRC(PackedDataCRC, c);
    PackLen++;
}

void PutPackedByteM2(u8 c)
{
    if (BitBuffBits)
        RawBuff[RawBuffBytes++] = c;
    else
        PutPackedByteM1(c);
}

void BestMatch()
{
    u16 Pos,
         Len;

    GetMatch();

    if (MatchLen >= MIN_LEN && ((InputPtrHi - InputPtr) > 2)) {
        Pos = MatchPos;
        Len = MatchLen;
        NewEntry++;
        InputPtr++;
        GetMatch();
        NewEntry--;
        InputPtr--;
        if (MatchLen > Len)
            MatchLen = 1;
        else {
            MatchPos = Pos;
            MatchLen = Len;
        }
    }
}

void GetMatch()
{
    u8 *s1 = InputPtr +1,
         *s2 = 0;

    u16 Entry,
         Pos,
         Len = 0,
         RunLen;

    MatchPos = 0;
    MatchLen = 1;

    while ((*s1 == *InputPtr) && (s1 < InputEnd))
        s1++;

    RunLen = s1 - InputPtr;

    Entry = DictPtrF[GETBIGENDIANW(InputPtr) & 0x7FFF];

    while (Entry != MaxPos) {
        Pos = (NewEntry > Entry) ? (NewEntry - Entry) : (MaxPos - (Entry - NewEntry));
        if ((GETBIGENDIANW(InputPtr - Pos) == GETBIGENDIANW(InputPtr))) {
            Len = DictLen[Entry];
            if (Len > Pos) {
                Pos = 1;
                Len = RunLen;
            } else {
                if (Len > RunLen) {
                    Pos -= (Len - RunLen);
                    Len = RunLen;
                }
                if (Len == RunLen) {
                    s1 = InputPtr + Len;
                    s2 = s1 - Pos;
                    while ((*s1 == *s2) && (s1 < InputEnd)) {
                        s1++;
                        s2++;
                    }
                    Len = s1 - InputPtr;
                }
            }
            if (Len > MaxLen)
                Len = MaxLen;
            if (Len >= MatchLen) {
                MatchLen = Len;
                MatchPos = Pos;
                if (MatchLen == MaxLen)
                    break;
            }
        }

        Entry = DictLnk[Entry];
    }

    if ((MatchLen == MIN_LEN) && (MatchPos > 0x100)) {
        MatchLen = 1;
        MatchPos = 0;
    }
}

void InitDictionary(void)
{
    u16 i;

    for (i = 0; i < MaxPos; i++) {
        DictLnk[i] = i;
        DictLen[i] = 0;
    }

    for (i = 0; i < CODE_SIZE; i++) {
        DictPtrF[i] = MaxPos;
        DictPtrL[i] = MaxPos;
    }

    NewEntry = 0;
}

void UpdateDictionary(u16 n)
{
    u8 *s;

    u16 n1,
         RunLen;

    while (n) {
        if (DictLnk[NewEntry] != NewEntry) {
            n1 = GETBIGENDIANW(InputPtr - MaxPos) & 0x7FFF;
            DictPtrF[n1] = DictLnk[NewEntry];
            if (DictLnk[NewEntry] == MaxPos)
                DictPtrL[n1] = MaxPos;
        }
        DictLnk[NewEntry] = MaxPos;
        n1 = GETBIGENDIANW(InputPtr) & 0x7FFF;
        if (DictPtrF[n1] == MaxPos)
            DictPtrF[n1] = NewEntry;
        else
            DictLnk[DictPtrL[n1]] = NewEntry;
        DictPtrL[n1] = NewEntry;

        s = InputPtr + 1;
        while ((*s == *InputPtr) && (s < InputEnd))
            s++;
        RunLen = s - InputPtr;
        DictLen[NewEntry] = RunLen;

        if (++NewEntry == MaxPos)
            NewEntry = 0;
        n--;
        InputPtr++;
        RunLen--;

        while (n && (RunLen > 1)) {
            DictLen[NewEntry] = RunLen;
            if (DictLnk[NewEntry] != NewEntry) {
                n1 = GETBIGENDIANW(InputPtr - MaxPos) & 0x7FFF;
                DictPtrF[n1] = DictLnk[NewEntry];
                if (DictLnk[NewEntry] == MaxPos)
                    DictPtrL[n1] = MaxPos;
                DictLnk[NewEntry] = NewEntry;
            }
            if (++NewEntry == MaxPos)
                NewEntry = 0;
            n--;
            RunLen--;
            InputPtr++;
        }
    }
}

u8 BitLength(u16 n)
{
    u8 Bits;

    for (Bits = 1; n >>= 1; Bits++);

    return Bits;
}

void MakeCRCTable(void)
{
    u16 i,
         j,
         k;

    for (i = 0; i < 256; i++) {
        for (k = i, j = 8; j; --j)
            k = (k & 1) ? ((k >> 1) ^ CRCPOLY) : (k >> 1);
        CRCTable[i] = k;
    }
}

