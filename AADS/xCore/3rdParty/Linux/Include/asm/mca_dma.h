#ifndef MCA_DMA_H
#define MCA_DMA_H

#include <asm/io.h>
#include <linux/ioport.h>

/*
 * Microchannel specific DMA stuff.  DMA on an MCA machine is fairly similar to
 *   standard PC dma, but it certainly has its quirks.  DMA register addresses
 *   are in a different place and there are some added functions.  Most of this
 *   should be pretty obvious on inspection.  Note that the user must divide
 *   count by 2 when using 16-bit dma; that is not handled by these functions.
 *
 * Ramen Noodles are yummy.
 * 
 *  1998 Tymm Twillman <tymm@computer.org>  
 */

/*
 * Registers that are used by the DMA controller; FN is the function register 
 *   (tell the controller what to do) and EXE is the execution register (how
 *   to do it)
 */

#define MCA_DMA_REG_FN  0x18
#define MCA_DMA_REG_EXE 0x1A 

/*
 * Functions that the DMA controller can do
 */

#define MCA_DMA_FN_SET_IO       0x00
#define MCA_DMA_FN_SET_ADDR     0x20
#define MCA_DMA_FN_GET_ADDR     0x30
#define MCA_DMA_FN_SET_COUNT    0x40
#define MCA_DMA_FN_GET_COUNT    0x50
#define MCA_DMA_FN_GET_STATUS   0x60
#define MCA_DMA_FN_SET_MODE     0x70
#define MCA_DMA_FN_SET_ARBUS    0x80
#define MCA_DMA_FN_MASK         0x90
#define MCA_DMA_FN_RESET_MASK   0xA0
#define MCA_DMA_FN_MASTER_CLEAR 0xD0

/*
 * Modes (used by setting MCA_DMA_FN_MODE in the function register)
 * 
 * Note that the MODE_READ is read from memory (write to device), and
 *   MODE_WRITE is vice-versa.  
 */

#define MCA_DMA_MODE_XFER  0x04  /* read by default */
#define MCA_DMA_MODE_READ  0x04  /* same as XFER */
#define MCA_DMA_MODE_WRITE 0x08  /* OR with MODE_XFER to use */
#define MCA_DMA_MODE_IO    0x01  /* DMA from IO register */
#define MCA_DMA_MODE_16    0x40  /* 16 bit xfers */



static __inline__ void mca_enable_dma(unsigned int dmanr)
{
	outb(MCA_DMA_FN_RESET_MASK | dmanr, MCA_DMA_REG_FN);
}

static __inline__ void mca_disable_dma(unsigned int dmanr)
{
	outb(MCA_DMA_FN_MASK | dmanr, MCA_DMA_REG_FN);
}

static __inline__ void mca_set_dma_addr(unsigned int dmanr, unsigned int a)
{
	outb(MCA_DMA_FN_SET_ADDR | dmanr, MCA_DMA_REG_FN);
	outb(a & 0xff, MCA_DMA_REG_EXE);
	outb((a >> 8) & 0xff, MCA_DMA_REG_EXE);
	outb((a >> 16) & 0xff, MCA_DMA_REG_EXE);
}

static __inline__ unsigned int mca_get_dma_addr(unsigned int dmanr)
{
	unsigned int addr;

	outb(MCA_DMA_FN_GET_ADDR | dmanr, MCA_DMA_REG_FN);
	addr = inb(MCA_DMA_REG_EXE);
	addr |= inb(MCA_DMA_REG_EXE) << 8;
	addr |= inb(MCA_DMA_REG_EXE) << 16;  

	return addr;
}

static __inline__ void mca_set_dma_count(unsigned int dmanr, unsigned int count)
{
	count--;  /* transfers one more than count -- correct for this */

	outb(MCA_DMA_FN_SET_COUNT | dmanr, MCA_DMA_REG_FN);
	outb(count & 0xff, MCA_DMA_REG_EXE);
	outb((count >> 8) & 0xff, MCA_DMA_REG_EXE);
}

static __inline__ unsigned int mca_get_dma_residue(unsigned int dmanr)
{
	unsigned short count;

	outb(MCA_DMA_FN_GET_COUNT | dmanr, MCA_DMA_REG_FN);
	count = 1 + inb(MCA_DMA_REG_EXE);
	count += inb(MCA_DMA_REG_EXE) << 8;

	return count;
}

static __inline__ void mca_set_dma_io(unsigned int dmanr, unsigned int io_addr)
{
	/*
	 * DMA from a port address -- set the io address
	 */
	
	outb(MCA_DMA_FN_SET_IO | dmanr, MCA_DMA_REG_FN);
	outb(io_addr & 0xff, MCA_DMA_REG_EXE);
	outb((io_addr >>  8) & 0xff, MCA_DMA_REG_EXE);
}

static __inline__ void mca_set_dma_mode(unsigned int dmanr, unsigned int mode)
{
	outb(MCA_DMA_FN_SET_MODE | dmanr, MCA_DMA_REG_FN);
	outb(mode, MCA_DMA_REG_EXE);
}

#endif MCA_DMA_H
