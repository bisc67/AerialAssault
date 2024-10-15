/*----------------------------------------------------------------------------
 * PROPACK Unpacker 'C' Source Code
 *
 * Copyright (c) 1995 Rob Northen Computing, UK. All Rights Reserved.
 *
 * File: HUFFMAN.C
 *
 * Date: 07-APR-95
 *----------------------------------------------------------------------------*/

#include "x_files.hpp"
#include "pp.h"

#ifdef TUROK_WIN32
#include <limits.h>
#endif


byte BitBuffBits,
     *InputPtr,
     *OutputPtr;

s16 FirstEntry,
	 SecondEntry;

huffman RawHuffmanTable[16],
		PosHuffmanTable[16],
		LenHuffmanTable[16];

void InitHuffmanTable(HUFFMAN_tableptr Table, byte TableSize);
void MakeHuffmanCodes(HUFFMAN_tableptr Table, byte n);
void MakeHuffmanTable(HUFFMAN_tableptr Table, byte n);
u16 FindLowest(HUFFMAN_tableptr Table, byte n);
u32 SwapBits(u32 InBits, byte n);

void InitHuffmanTable(HUFFMAN_tableptr Table, byte TableSize)
{
    while (TableSize--) {
        Table->Frequency = 0;
        Table->EntryPtr = U16_MAX;
        Table->Code = 0;
        Table->CodeLen = 0;
        Table++;
    }
}

void MakeHuffmanCodes(HUFFMAN_tableptr Table, byte n)
{
    s16 huff_bits = 1,
         i;

    u32 huff_code = 0,
         huff_base = 0x80000000;

    HUFFMAN_tableptr tableptr;

    while (huff_bits <= 16) {
		tableptr = Table;
        for (i = 0; i < n; i++) {
            if (tableptr->CodeLen == huff_bits) {
                tableptr->Code = SwapBits(huff_code / huff_base, (byte)huff_bits);
                huff_code += huff_base;
            }
            tableptr++;
        }
        huff_bits++;
        huff_base >>= 1;
    }
}

void MakeHuffmanTable(HUFFMAN_tableptr Table, byte n)
{
    s16 i,
         j,
         k;

    u32 temp[16];

    for (i = 0; i < 16; i++)
        temp[i] = Table[i].Frequency;

    for (i = j = 0; i < n; i++)
        if (Table[i].Frequency) {
			j++;
            k = i;
        }

    if (j == 0)
        return;

    if (j == 1) {
        Table[k].CodeLen++;
        return;
    }

    while (FindLowest(Table, n)) {
		Table[FirstEntry].Frequency += Table[SecondEntry].Frequency;
        Table[SecondEntry].Frequency = 0;
        Table[FirstEntry].CodeLen++;

        while (Table[FirstEntry].EntryPtr !=  U16_MAX) {
            FirstEntry = Table[FirstEntry].EntryPtr;
            Table[FirstEntry].CodeLen++;
        }

        Table[FirstEntry].EntryPtr = SecondEntry;
        Table[SecondEntry].CodeLen++;

        while (Table[SecondEntry].EntryPtr != U16_MAX) {
			SecondEntry = Table[SecondEntry].EntryPtr;
            Table[SecondEntry].CodeLen++;
        }
    }

    for (i = 0; i < 16; i++)
        Table[i].Frequency = temp[i];

    MakeHuffmanCodes(Table, n);
}

u16 FindLowest(HUFFMAN_tableptr Table, u8 n)
{
	byte Entry;

    u32 Freq,
         FirstFreq  = U32_MAX,
         SecondFreq = U32_MAX;

    for (Entry = 0; Entry < n; Entry++)
		if ((Freq = Table[Entry].Frequency) != 0)
			if (Freq < FirstFreq) {
				SecondFreq  = FirstFreq;
				SecondEntry = FirstEntry;
				FirstFreq   = Freq;
				FirstEntry  = Entry;
			} else
				if (Freq < SecondFreq) {
					SecondFreq  = Freq;
					SecondEntry = Entry;
				}
	if ((FirstFreq == U32_MAX) || (SecondFreq == U32_MAX))
		return 0;
	return 1;
}

u32 SwapBits(u32 InBits, byte n)
{
	u32 OutBits = 0;

	while (n--) {
        OutBits <<= 1;
        if (InBits & 1)
            OutBits |= 1;
        InBits >>= 1;
    }

    return OutBits;
}


