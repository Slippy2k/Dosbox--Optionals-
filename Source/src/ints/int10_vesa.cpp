/*
 *  Copyright (C) 2002-2018  The DOSBox Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#include <string.h>
#include <stddef.h>
#include <assert.h>  

#include "dosbox.h"
#include "callback.h"
#include "regs.h"
#include "mem.h"
#include "inout.h"
#include "int10.h"
#include "dos_inc.h"
#include "control.h"
#include "..\gui\version.h"

#define VESA_SUCCESS          0x00
#define VESA_FAIL             0x01
#define VESA_HW_UNSUPPORTED   0x02
#define VESA_MODE_UNSUPPORTED 0x03
// internal definition to pass to the caller
#define VESA_UNIMPLEMENTED    0xFF

static struct {
	Bitu setwindow;
	Bitu rmWindow;
	Bitu pmStart;
	Bitu pmWindow;
	Bitu pmPalette;
} callback = {0};

static char string_oem[]="S3 Inc. Trio64 Custom";
static char string_vendorname[]="DOSBox Development Team";
static char string_productname[]="DOSBox - The DOS Emulator";
static char string_productrev[]="DOSBox " VERSION" " DOSBOXREVISION;

#ifdef _MSC_VER
#pragma pack (1)
#endif
struct MODE_INFO{
	Bit16u ModeAttributes;
	Bit8u WinAAttributes;
	Bit8u WinBAttributes;
	Bit16u WinGranularity;
	Bit16u WinSize;
	Bit16u WinASegment;
	Bit16u WinBSegment;
	Bit32u WinFuncPtr;
	Bit16u BytesPerScanLine;
	Bit16u XResolution;
	Bit16u YResolution;
	Bit8u XCharSize;
	Bit8u YCharSize;
	Bit8u NumberOfPlanes;
	Bit8u BitsPerPixel;
	Bit8u NumberOfBanks;
	Bit8u MemoryModel;
	Bit8u BankSize;
	Bit8u NumberOfImagePages;
	Bit8u Reserved_page;
	Bit8u RedMaskSize;
	Bit8u RedMaskPos;
	Bit8u GreenMaskSize;
	Bit8u GreenMaskPos;
	Bit8u BlueMaskSize;
	Bit8u BlueMaskPos;
	Bit8u ReservedMaskSize;
	Bit8u ReservedMaskPos;
	Bit8u DirectColorModeInfo;
	Bit32u PhysBasePtr;
	Bit32u OffScreenMemOffset;
	Bit16u OffScreenMemSize;
	Bit8u Reserved[206];
} GCC_ATTRIBUTE(packed);
#ifdef _MSC_VER
#pragma pack()
#endif

Bit8u VESA_GetSVGAInformation(Bit16u seg,Bit16u off) {
	/* Fill 256 byte buffer with VESA information */
		
	PhysPt buffer=PhysMake(seg,off);
	Bitu i;
	bool vbe2=false;Bit16u vbe2_pos=256+off;
	Bitu id=mem_readd(buffer);
	if (((id==0x56424532)||(id==0x32454256)) && (!int10.vesa_oldvbe)) vbe2=true;
	if (vbe2) {
		for (i=0;i<0x200;i++) mem_writeb(buffer+i,0);		
	} else {
		for (i=0;i<0x100;i++) mem_writeb(buffer+i,0);
	}
	/* Fill common data */
	MEM_BlockWrite(buffer,(void *)"VESA",4);				//Identification
	if (!int10.vesa_oldvbe) mem_writew(buffer+0x04,0x200);	//Vesa version 2.0
	else mem_writew(buffer+0x04,0x102);						//Vesa version 1.2
	if (vbe2) {
		mem_writed(buffer+0x06,RealMake(seg,vbe2_pos));
		for (i=0;i<sizeof(string_oem);i++) real_writeb(seg,vbe2_pos++,string_oem[i]);
		mem_writew(buffer+0x14,0x200);					//VBE 2 software revision
		mem_writed(buffer+0x16,RealMake(seg,vbe2_pos));
		for (i=0;i<sizeof(string_vendorname);i++) real_writeb(seg,vbe2_pos++,string_vendorname[i]);
		mem_writed(buffer+0x1a,RealMake(seg,vbe2_pos));
		for (i=0;i<sizeof(string_productname);i++) real_writeb(seg,vbe2_pos++,string_productname[i]);
		mem_writed(buffer+0x1e,RealMake(seg,vbe2_pos));
		for (i=0;i<sizeof(string_productrev);i++) real_writeb(seg,vbe2_pos++,string_productrev[i]);
	} else {
		mem_writed(buffer+0x06,int10.rom.oemstring);	//Oemstring
	}
	mem_writed(buffer+0x0a,0x0);					//Capabilities and flags
	mem_writed(buffer+0x0e,int10.rom.vesa_modes);	//VESA Mode list
	mem_writew(buffer+0x12,(Bit16u)(vga.vmemsize/(64*1024))); // memory size in 64kb blocks
	return VESA_SUCCESS;
}

Bit8u VESA_GetSVGAModeInformation(Bit16u mode,Bit16u seg,Bit16u off) {
	MODE_INFO minfo;
	memset(&minfo,0,sizeof(minfo));
	PhysPt buf=PhysMake(seg,off);
	Bitu pageSize;
	Bit8u modeAttributes;
	Bitu i=0;
	
	mode&=0x3fff;	// vbe2 compatible, ignore lfb and keep screen content bits
	if (mode<0x100) return 0x01;
	if (svga.accepts_mode) {
		if (!svga.accepts_mode(mode)) return 0x01;
	}
	while (ModeList_VGA[i].mode!=0xffff) {
		if (mode==ModeList_VGA[i].mode) goto foundit; else i++;
	}
	return VESA_FAIL;
foundit:
	/* Custom S3 VGA ////////////////////////////////////////////////////////////////*/	
	if (((int10.vesa_oldvbe ) && (ModeList_VGA[i].mode>0x120)) ||
		((int10.vesa_no24bpp) && (ModeList_VGA[i].type==M_LIN24))) return 0x01;
		
		VideoModeBlock * mblock;
		mblock=&ModeList_VGA[i];
		
			/* Variablen liegen in der Header Datei Int10.h. Im konstruktor.      */
			if ( int10.bModePatch0x153 )
			{
					
				if ( ( ModeList_VGA[i].mode==0x153 ) && ( ModeList_VGA[i].type==M_LIN4 ) )
				{	
					mblock=&ModeTableA_S3_Fixes[0];	
					#if defined(C_DEBUG)					
						LOG_MSG("VGA: Using S3 Screen Fix 320x240x8 --- (File %s, Line %d)",__FILE__,__LINE__);
					#endif
 					/* Fix für die demo Countdown. 320x240*8 zu 320x200*8, Siehe Int10_Modes.cpp */
				}
			}
			
			if ( int10.bModePatch0x166 )
			{
					
				if ( ( ModeList_VGA[i].mode==0x166 ) && ( ModeList_VGA[i].type==M_LIN8 ) )
				{	
					mblock=&ModeTableA_S3_Fixes[1];	
					#if defined(C_DEBUG)
						LOG_MSG("VGA: Using S3 Screen Fix 400x300x8 --- (File %s, Line %d)",__FILE__,__LINE__);
					#endif
					/* Fix für die Habitual Demo. 400x300*8 zu 320x200*8, Siehe Int10_Modes.cpp */
				}
			}			
					
			if ( int10.bVesaPatch32bit  || (int10.vesa_no24bpp) ){
					
				if ( ( ModeList_VGA[i].mode==0x10F ) && ( ModeList_VGA[i].type==M_LIN24 ) ){	
					mblock=&ModeTableA_S3_32Bit[0];						
				}
				if ( ( ModeList_VGA[i].mode==0x112 ) && ( ModeList_VGA[i].type==M_LIN24 ) ){	
					mblock=&ModeTableA_S3_32Bit[1];					
				}	
				if ( ( ModeList_VGA[i].mode==0x115 ) && ( ModeList_VGA[i].type==M_LIN24 ) ){	
					mblock=&ModeTableA_S3_32Bit[2];						
				}
				if ( ( ModeList_VGA[i].mode==0x118 ) && ( ModeList_VGA[i].type==M_LIN24 ) ){	
					mblock=&ModeTableA_S3_32Bit[3];						
				}	
				#if defined(C_DEBUG)
					LOG_MSG("VGA: Using Vesa VBE 1.2 32Bit Color Modes -- (File %s, Line %d)",__FILE__,__LINE__);				
				#endif
			}
    /* do not return information on deleted modes */
    if (mblock->type == M_ERROR) return 0x01;
	
	
	/* Custom S3 VGA ////////////////////////////////////////////////////////////////*/			
	switch (mblock->type) {
	case M_LIN4:
		pageSize = mblock->sheight * mblock->swidth/2;
		//var_write(&minfo.BytesPerScanLine,mblock->swidth/8);
		var_write(&minfo.BytesPerScanLine,((mblock->swidth+15U)/8U)&(~1U)); /* NTS: 4bpp requires even value due to VGA registers, round up */
		var_write(&minfo.NumberOfPlanes,0x4);
		var_write(&minfo.BitsPerPixel,4);
		var_write(&minfo.MemoryModel,3);	//ega planar mode
		modeAttributes = 0x1b;	// Color, graphics, no linear buffer	
		break;
	case M_LIN8:
		pageSize = mblock->sheight * mblock->swidth;
		var_write(&minfo.BytesPerScanLine,mblock->swidth);
		var_write(&minfo.NumberOfPlanes,0x1);
		var_write(&minfo.BitsPerPixel,8);
		var_write(&minfo.MemoryModel,4);		//packed pixel
		modeAttributes = 0x1b;	// Color, graphics
		if (!int10.vesa_nolfb) modeAttributes |= 0x80;	// linear framebuffer
		break;
	case M_LIN15:
		pageSize = mblock->sheight * mblock->swidth*2;
		var_write(&minfo.BytesPerScanLine,mblock->swidth*2);
		var_write(&minfo.NumberOfPlanes,0x1);
		var_write(&minfo.BitsPerPixel,15);
		var_write(&minfo.MemoryModel,6);	//HiColour
		var_write(&minfo.RedMaskSize,5);
		var_write(&minfo.RedMaskPos,10);
		var_write(&minfo.GreenMaskSize,5);
		var_write(&minfo.GreenMaskPos,5);
		var_write(&minfo.BlueMaskSize,5);
		var_write(&minfo.BlueMaskPos,0);
		var_write(&minfo.ReservedMaskSize,0x01);
		var_write(&minfo.ReservedMaskPos,0x0f);
		modeAttributes = 0x1b;	// Color, graphics
		if (!int10.vesa_nolfb) modeAttributes |= 0x80;	// linear framebuffer
		break;
	case M_LIN16:
		pageSize = mblock->sheight * mblock->swidth*2;
		var_write(&minfo.BytesPerScanLine,mblock->swidth*2);
		var_write(&minfo.NumberOfPlanes,0x1);
		var_write(&minfo.BitsPerPixel,16);
		var_write(&minfo.MemoryModel,6);	//HiColour
		var_write(&minfo.RedMaskSize,5);
		var_write(&minfo.RedMaskPos,11);
		var_write(&minfo.GreenMaskSize,6);
		var_write(&minfo.GreenMaskPos,5);
		var_write(&minfo.BlueMaskSize,5);
		var_write(&minfo.BlueMaskPos,0);
		modeAttributes = 0x1b;	// Color, graphics
		if (!int10.vesa_nolfb) modeAttributes |= 0x80;	// linear framebuffer
		break;
		/* Custom S3 VGA ////////////////////////////////////////////////////////////////*/			
	case M_LIN24:
		if (mode==0x212)
		{ // Mode 0x212 has 128 extra bytes per scan line for compatibility with Windows 640x480 24-bit S3 Trio drivers
			pageSize = mblock->sheight * (mblock->swidth*3+128);
			var_write(&minfo.BytesPerScanLine,mblock->swidth*3+128);
		} else {
			pageSize = mblock->sheight * mblock->swidth*3;
			var_write(&minfo.BytesPerScanLine,mblock->swidth*3);
		}
		var_write(&minfo.NumberOfPlanes,0x1);
		var_write(&minfo.BitsPerPixel,24);
		var_write(&minfo.MemoryModel,6);	//HiColour
		var_write(&minfo.RedMaskSize,8);
		var_write(&minfo.RedMaskPos,0x10);
		var_write(&minfo.GreenMaskSize,0x8);
		var_write(&minfo.GreenMaskPos,0x8);
		var_write(&minfo.BlueMaskSize,0x8);
		var_write(&minfo.BlueMaskPos,0x0);
		modeAttributes = 0x1b;	// Color, graphics
		if (!int10.vesa_nolfb) modeAttributes |= 0x80;	// linear framebuffer
		break;		
		/* Custom S3 VGA ////////////////////////////////////////////////////////////////*/
	case M_LIN32:
		pageSize = mblock->sheight * mblock->swidth*4;
		var_write(&minfo.BytesPerScanLine,mblock->swidth*4);
		var_write(&minfo.NumberOfPlanes,0x1);
		var_write(&minfo.BitsPerPixel,32);
		var_write(&minfo.MemoryModel,6);	//HiColour
		var_write(&minfo.RedMaskSize,8);
		var_write(&minfo.RedMaskPos,0x10);
		var_write(&minfo.GreenMaskSize,0x8);
		var_write(&minfo.GreenMaskPos,0x8);
		var_write(&minfo.BlueMaskSize,0x8);
		var_write(&minfo.BlueMaskPos,0x0);
		var_write(&minfo.ReservedMaskSize,0x8);
		var_write(&minfo.ReservedMaskPos,0x18);
		modeAttributes = 0x1b;	// Color, graphics
		if (!int10.vesa_nolfb) modeAttributes |= 0x80;	// linear framebuffer
		break;
	case M_TEXT:
		pageSize = 0;
		var_write(&minfo.BytesPerScanLine, mblock->twidth * 2);// <- Removed by Custom S3VGA
		/* Custom S3 VGA ///////////////////////////////////////////////////////////////////*/
		//pageSize = mblock->sheight/8 * mblock->swidth*2/mblock->cwidth;
		//var_write(&minfo.BytesPerScanLine,mblock->swidth*2/mblock->cwidth);		
		/* Custom S3 VGA //////////////////////////////////////////////////////////////////*/
		var_write(&minfo.NumberOfPlanes,0x4);
		var_write(&minfo.BitsPerPixel,4);
		var_write(&minfo.MemoryModel,0);	// text
		modeAttributes = 0x0f;	//Color, text, bios output
		break;
	default:
		return VESA_FAIL;
	}
	if (pageSize & 0xFFFF) {
		// It is documented that many applications assume 64k-aligned page sizes
		// VBETEST is one of them
		pageSize += 0x10000;
		pageSize &= ~0xFFFF;
	}
	Bitu pages = 0;
	if (pageSize > vga.vmemsize) {
		// mode not supported by current hardware configuration
		modeAttributes &= ~0x1;
	} else if (pageSize) {
		pages = (vga.vmemsize / pageSize)-1;
	}
	var_write(&minfo.NumberOfImagePages, pages);
	var_write(&minfo.ModeAttributes, modeAttributes);
	var_write(&minfo.WinAAttributes, 0x7);	// Exists/readable/writable

	if (mblock->type==M_TEXT) {
		var_write(&minfo.WinGranularity,32);
		var_write(&minfo.WinSize,32);
		var_write(&minfo.WinASegment,0xb800);
		var_write(&minfo.XResolution,mblock->twidth); 			//<- Removed by Custom S3VGA
		var_write(&minfo.YResolution,mblock->theight); 			//<- Removed by Custom S3VGA
		/* Custom S3 VGA //////////////////////////////////////////////////////////////////*/
		//var_write(&minfo.XResolution,mblock->swidth/mblock->cwidth);
		//var_write(&minfo.YResolution,mblock->sheight/mblock->cheight);
		/* Custom S3 VGA //////////////////////////////////////////////////////////////////*/
	} else {
		var_write(&minfo.WinGranularity,64);
		var_write(&minfo.WinSize,64);
		var_write(&minfo.WinASegment,0xa000);
		var_write(&minfo.XResolution,mblock->swidth);
		var_write(&minfo.YResolution,mblock->sheight);
	}
	var_write(&minfo.WinFuncPtr,int10.rom.set_window);
	var_write(&minfo.NumberOfBanks,0x1);
	var_write(&minfo.Reserved_page,0x1);
	var_write(&minfo.XCharSize,mblock->cwidth);
	var_write(&minfo.YCharSize,mblock->cheight);
	if (!int10.vesa_nolfb) var_write(&minfo.PhysBasePtr,S3_LFB_BASE);

	MEM_BlockWrite(buf,&minfo,sizeof(MODE_INFO));
	return VESA_SUCCESS;
}


Bit8u VESA_SetSVGAMode(Bit16u mode) {
	if (INT10_SetVideoMode(mode)) {
		int10.vesa_setmode=mode&0x7fff;
		return VESA_SUCCESS;
	}
	return VESA_FAIL;
}

Bit8u VESA_GetSVGAMode(Bit16u & mode) {
	if (int10.vesa_setmode!=0xffff) mode=int10.vesa_setmode;
	else mode=CurMode->mode;
	return VESA_SUCCESS;
}

Bit8u VESA_SetCPUWindow(Bit8u window,Bit8u address) {
	if (window) return VESA_FAIL;
	if (((Bit32u)(address)*64*1024<vga.vmemsize)) {
		IO_Write(0x3d4,0x6a);
		IO_Write(0x3d5,(Bit8u)address);
		return VESA_SUCCESS;
	} else return VESA_FAIL;
}

Bit8u VESA_GetCPUWindow(Bit8u window,Bit16u & address) {
	if (window) return VESA_FAIL;
	IO_Write(0x3d4,0x6a);
	address=IO_Read(0x3d5);
	return VESA_SUCCESS;
}


Bit8u VESA_SetPalette(PhysPt data,Bitu index,Bitu count,bool wait) {
//Structure is (vesa 3.0 doc): blue,green,red,alignment
	Bit8u r,g,b;
	if (index>255) return VESA_FAIL;
	if (index+count>256) return VESA_FAIL;

	// Wait for retrace if requested
	if (wait) CALLBACK_RunRealFar(RealSeg(int10.rom.wait_retrace),RealOff(int10.rom.wait_retrace));

	IO_Write(0x3c8,(Bit8u)index);
	while (count) {
		b = mem_readb(data++);
		g = mem_readb(data++);
		r = mem_readb(data++);
		data++;
		IO_Write(0x3c9,r);
		IO_Write(0x3c9,g);
		IO_Write(0x3c9,b);
		count--;
	}
	return VESA_SUCCESS;
}


Bit8u VESA_GetPalette(PhysPt data,Bitu index,Bitu count) {
	Bit8u r,g,b;
	if (index>255) return VESA_FAIL;
	if (index+count>256) return VESA_FAIL;
	IO_Write(0x3c7,(Bit8u)index);
	while (count) {
		r = IO_Read(0x3c9);
		g = IO_Read(0x3c9);
		b = IO_Read(0x3c9);
		mem_writeb(data++,b);
		mem_writeb(data++,g);
		mem_writeb(data++,r);
		data++;
		count--;
	}
	return VESA_SUCCESS;
}

// maximum offset for the S3 Trio64 is 10 bits
#define S3_MAX_OFFSET 0x3ff

Bit8u VESA_ScanLineLength(Bit8u subcall,Bit16u val, Bit16u & bytes,Bit16u & pixels,Bit16u & lines) {
	// offset register: virtual scanline length
	Bitu pixels_per_offset;
	Bitu bytes_per_offset = 8;
	Bitu vmemsize = vga.vmemsize;
	Bitu new_offset = vga.config.scan_len;
	Bitu screen_height = CurMode->sheight;

	switch (CurMode->type) {
	case M_TEXT:
		vmemsize = 0x8000;      // we have only the 32kB window here
		screen_height = CurMode->theight;
		pixels_per_offset = 16; // two characters each 8 pixels wide
		bytes_per_offset = 4;   // 2 characters + 2 attributes
		break;
	case M_LIN4:
		pixels_per_offset = 16;
		break;
	case M_LIN8:
		pixels_per_offset = 8;
		break;
	case M_LIN15:
	case M_LIN16:
		pixels_per_offset = 4;
		break;
		// /* Custom S3 VGA //////////////////////////////////////////////////////////////////*/		
	// case M_LIN24:
		// pixels_per_offset = 3;
		// break;		
		// /* Custom S3 VGA //////////////////////////////////////////////////////////////////*/		
	case M_LIN32:
		pixels_per_offset = 2;
		break;
	default:
		return VESA_MODE_UNSUPPORTED;
	}
	switch (subcall) {
	case 0x00: // set scan length in pixels
		new_offset = val / pixels_per_offset;
		if (val % pixels_per_offset) new_offset++;
		
		if (new_offset > S3_MAX_OFFSET)
			return VESA_HW_UNSUPPORTED; // scanline too long
		vga.config.scan_len = new_offset;
		VGA_CheckScanLength();
		break;

	case 0x01: // get current scanline length
		// implemented at the end of this function
		break;

	case 0x02: // set scan length in bytes
		new_offset = val / bytes_per_offset;
		if (val % bytes_per_offset) new_offset++;
		
		if (new_offset > S3_MAX_OFFSET)
			return VESA_HW_UNSUPPORTED; // scanline too long
		vga.config.scan_len = new_offset;
		VGA_CheckScanLength();
		break;

	case 0x03: // get maximum scan line length
		// the smaller of either the hardware maximum scanline length or
		// the limit to get full y resolution of this mode
		new_offset = S3_MAX_OFFSET;
		if ((new_offset * bytes_per_offset * screen_height) > vmemsize)
			new_offset = vmemsize / (bytes_per_offset * screen_height);
		break;

	default:
		return VESA_UNIMPLEMENTED;
	}

	// set up the return values
	bytes = (Bit16u)(new_offset * bytes_per_offset);
	pixels = (Bit16u)(new_offset * pixels_per_offset);
	if (!bytes)
		// return failure on division by zero
		// some real VESA BIOS implementations may crash here
		return VESA_FAIL;

	lines = (Bit16u)(vmemsize / bytes);
	
	if (CurMode->type==M_TEXT)
		lines *= CurMode->cheight;

	return VESA_SUCCESS;
}
Bit8u VESA_SetDisplayStart(Bit16u x,Bit16u y) {
	//TODO Maybe do things differently with lowres double line modes?	
	Bitu start;
	switch (CurMode->type) {
	case M_LIN4:
		start=vga.config.scan_len*16*y+x;
		vga.config.display_start=start/8;
		IO_Read(0x3da);
		IO_Write(0x3c0,0x13+32);
		IO_Write(0x3c0,start % 8);
		break;
	case M_LIN8:
		start=vga.config.scan_len*8*y+x;
		vga.config.display_start=start/4;
		IO_Read(0x3da);
		IO_Write(0x3c0,0x13+32);
		IO_Write(0x3c0,(start % 4)*2);
		break;
	case M_LIN16:
	case M_LIN15:
		start=vga.config.scan_len*8*y+x*2;
		vga.config.display_start=start/4;
		break;
		/* Custom S3 VGA //////////////////////////////////////////////////////////////////*/			
	case M_LIN24:
		start=(vga.config.scan_len*8)*y+x*3;
		start-=(x*3)%12;	// Align x to every 4 pixels to prevent color flickering when scrolling in VBETEST
		vga.config.display_start=start/4;
		break;		
		/* Custom S3 VGA //////////////////////////////////////////////////////////////////*/			
	case M_LIN32:
		start=vga.config.scan_len*8*y+x*4;
		vga.config.display_start=start/4;
		break;
	default:
		return 0x1;
	}
	return 0x00;
}
Bit8u VESA_SetDisplayStart(Bit16u x,Bit16u y,bool wait) {
	Bitu pixels_per_offset;
	Bitu panning_factor = 1;

	switch (CurMode->type) {
	case M_TEXT:
	case M_LIN4:
		pixels_per_offset = 16;
		break;
	case M_LIN8:
		panning_factor = 2; // the panning register ignores bit0 in this mode
		pixels_per_offset = 8;
		break;
	case M_LIN15:
	case M_LIN16:
		panning_factor = 2; // this may be DOSBox specific
		pixels_per_offset = 4;
		break;
		// /* Custom S3 VGA //////////////////////////////////////////////////////////////////*/		
	// case M_LIN24:
		// /* Custom S3 VGA //////////////////////////////////////////////////////////////////*/	
	case M_LIN32:
		pixels_per_offset = 2;
		break;
	default:
		return VESA_MODE_UNSUPPORTED;
	}
	// We would have to divide y by the character height for text modes and
	// write the remainder to the CRTC preset row scan register,
	// but VBE2 BIOSes that actually get that far also don't.
	// Only a VBE3 BIOS got it right.
	Bitu virtual_screen_width = vga.config.scan_len * pixels_per_offset;
	Bitu new_start_pixel = virtual_screen_width * y + x;
	Bitu new_crtc_start = new_start_pixel / (pixels_per_offset/2);
	Bitu new_panning = new_start_pixel % (pixels_per_offset/2);
	new_panning *= panning_factor;

	vga.config.display_start = new_crtc_start;
	
	// Setting the panning register is nice as it allows for super smooth
	// scrolling, but if we hit the retrace pulse there may be flicker as
	// panning and display start are latched at different times. 

	IO_Read(0x3da);              // reset attribute flipflop
	IO_Write(0x3c0,0x13 | 0x20); // panning register, screen on
	IO_Write(0x3c0,new_panning);

	// Wait for retrace if requested
	if (wait) CALLBACK_RunRealFar(RealSeg(int10.rom.wait_retrace),RealOff(int10.rom.wait_retrace));

	return VESA_SUCCESS;
}

Bit8u VESA_GetDisplayStart(Bit16u & x,Bit16u & y) {
	Bitu pixels_per_offset;
	Bitu panning_factor = 1;

	switch (CurMode->type) {
	case M_TEXT:
		pixels_per_offset = 16;
		break;
	case M_LIN4:
		pixels_per_offset = 16;
		break;
	case M_LIN8:
		panning_factor = 2;
		pixels_per_offset = 8;
		break;
	case M_LIN15:
	case M_LIN16:
		panning_factor = 2;
		pixels_per_offset = 4;
		break;
		// /* Custom S3 VGA //////////////////////////////////////////////////////////////////*/		
	// case M_LIN24:		
		// /* Custom S3 VGA //////////////////////////////////////////////////////////////////*/	
	case M_LIN32:
		pixels_per_offset = 2;
		break;
	default:
		return VESA_MODE_UNSUPPORTED;
	}

	IO_Read(0x3da);              // reset attribute flipflop
	IO_Write(0x3c0,0x13 | 0x20); // panning register, screen on
	Bit8u panning = IO_Read(0x3c1);

	Bitu virtual_screen_width = vga.config.scan_len * pixels_per_offset;
	Bitu start_pixel = vga.config.display_start * (pixels_per_offset/2) 
		+ panning / panning_factor;
	
	y = start_pixel / virtual_screen_width;
	x = start_pixel % virtual_screen_width;
	return VESA_SUCCESS;
}

static Bitu VESA_SetWindow(void) {
	if (reg_bh) reg_ah=VESA_GetCPUWindow(reg_bl,reg_dx);
	else reg_ah=VESA_SetCPUWindow(reg_bl,(Bit8u)reg_dx);
	reg_al=0x4f;
	return CBRET_NONE;
}

static Bitu VESA_PMSetWindow(void) {
	IO_Write(0x3d4,0x6a);
	IO_Write(0x3d5,reg_dl);
	return CBRET_NONE;
}
static Bitu VESA_PMSetPalette(void) {
	PhysPt data=SegPhys(es)+reg_edi;
	Bit32u count=reg_cx;
	IO_Write(0x3c8,reg_dl);
	do {
		IO_Write(0x3c9,mem_readb(data+2));
		IO_Write(0x3c9,mem_readb(data+1));
		IO_Write(0x3c9,mem_readb(data));
		data+=4;
	} while (--count);
	return CBRET_NONE;
}
static Bitu VESA_PMSetStart(void) {
	Bit32u start = (reg_dx << 16) | reg_cx;
	vga.config.display_start = start;
	return CBRET_NONE;
}




Bitu INT10_WriteVESAModeList(Bitu max_modes) {
	
	Bitu mode_wptr   	 = int10.rom.vesa_modes;
	Bitu i		     	 = 0;
	Bitu modecount   	 = 0;
	int bpp				 = 0;
	int10.LogingCountMode= true;
	bool canuse_mode     = false;
	int cnt				 = 0;
	
	if ( (int10.nModeListLog) && (int10.LogingCountMode) ){
		
		if (int10.nModeListCut > 0){
			LOG_MSG("VGA: Video Modes Capped to %d. Get Mode at 0x100",int10.nModeListCut);
		}else {
			LOG_MSG("VGA: Getting all Video Modes at 0x100");
		}				
	}
	
	while (ModeList_VGA[i].mode!=0xffff) {
		
		if (ModeList_VGA[i].type == M_ERROR)
		{
            /* ignore */
        }
        else
		{	
			if (!svga.accepts_mode)
			{
				canuse_mode=true;
			}
			else if (svga.accepts_mode(ModeList_VGA[i].mode))
			{
				canuse_mode=true;
			}				
		}
		
        if (canuse_mode &&  int10.nModeListCut > 0 && (unsigned int)modecount >= (unsigned int)int10.nModeListCut)
            canuse_mode = false;
						
        if (ModeList_VGA[i].mode>=0x100 && canuse_mode) {
			if (((!int10.vesa_oldvbe) || (ModeList_VGA[i].mode<=0x120)) &&
				((!int10.vesa_no24bpp) || (ModeList_VGA[i].type!=M_LIN24))) {

				if (int10.nModeListLog){					
					if 	( (canuse_mode) && (int10.LogingCountMode == true) ){
						switch(ModeList_VGA[i].type)
						{
							case M_TEXT:{bpp = 2;break;}
							case M_LIN4:{bpp = 4;break;}
							case M_LIN8:{bpp = 8;break;}
							case M_LIN15:{bpp = 15;break;}	
							case M_LIN16:{bpp = 16;break;}
							case M_LIN24:{bpp = 24;break;}			
							case M_LIN32:{bpp = 32;break;}						
						}
						LOG_MSG("VGA: Mode Available %#04x: [ %d x %d x %dbit ] -- (%d,%d)",ModeList_VGA[i].mode,ModeList_VGA[i].swidth,ModeList_VGA[i].sheight,bpp, cnt,i);
					}
					cnt += 1;
				}
                phys_writew(PhysMake(0xc000,mode_wptr),ModeList_VGA[i].mode);
                mode_wptr+=2;
                modecount++;
            }
        }
        i++;
	}	
	
    assert(modecount <= int10.rom.vesa_alloc_modes); /* do not overrun the buffer */
	
    /* after the buffer, is 0xFFFF */
    phys_writew(PhysMake(0xc000,mode_wptr),0xFFFF);
    mode_wptr+=2;

	if 	( (canuse_mode) && (int10.LogingCountMode == true) ){
		int10.LogingCountMode = false;
	}
    return modecount;	
}


void INT10_SetupVESA(void) {

	Section_prop *section = static_cast<Section_prop *>(control->GetSection("dosbox"));	
	int10.nModeListCut 	  = section->Get_int("CutVesaResolutionIndex");
	int10.nModeListLog    = section->Get_bool("LogVesaResolutionIndex");	
	int10.bModePatch0x153 = section->Get_bool("S3Screen_Fix_320x240x8");	
	int10.bModePatch0x166 = section->Get_bool("S3Screen_Fix_400x300x8");
	int10.bVesaPatch32bit = section->Get_bool("VesaVbe1.2_32Bit_Modes");
	
	if (int10.bModePatch0x153){	LOG_MSG("VGA: Using S3 Screen Fix 320x240x8\n");	}
	if (int10.bModePatch0x166){	LOG_MSG("VGA: Using S3 Screen Fix 400x300x8\n");	}	
	if (int10.bVesaPatch32bit){	LOG_MSG("VGA: Using Vesa 1.2 32Bit Color Modes\n");	}	
	
	Section_prop *sRender = static_cast<Section_prop *>(control->GetSection("render"));	
	int10.bExtraVGA_Debug = sRender->Get_bool("debug");
	int10.bAspectRatio	  = sRender->Get_bool("aspect");
	
	Bitu i		     = 0;
	Bitu VESAModeList= 0;
	
	/* BUGFIX: Generating VESA BIOS data when machine=ega or machine=vgaonly is dumb.
	 * Stop wasting ROM space! --J.C. */	
	if (svgaCard == SVGA_None) return;
	
	/* Put the mode list somewhere in memory */
	int10.rom.vesa_alloc_modes = ~0;
	
	int10.rom.vesa_modes	   = RealMake(0xc000,int10.rom.used);
	VESAModeList               = INT10_WriteVESAModeList(0xFFFF);  	/* <-- Max Mode List 0xFFFF */
	
	int10.rom.vesa_alloc_modes = (Bit16u)VESAModeList;
	int10.rom.used 			  += VESAModeList * 2u;
	
	phys_writew(PhysMake(0xc000,int10.rom.used),0xffff);
	
	int10.rom.used			  += 2;
	int10.rom.oemstring		   = RealMake(0xc000,int10.rom.used);
	Bitu len=(Bitu)(strlen(string_oem)+1);
	
	for (i=0;i<len;i++) {
		phys_writeb(0xc0000+int10.rom.used++,string_oem[i]);
	}


	/* Prepare the real mode interface */
	int10.rom.wait_retrace=RealMake(0xc000,int10.rom.used);
	int10.rom.used += (Bit16u)CALLBACK_Setup(0, NULL, CB_VESA_WAIT, PhysMake(0xc000,int10.rom.used), "");
	callback.rmWindow=CALLBACK_Allocate();
	int10.rom.set_window=RealMake(0xc000,int10.rom.used);
	int10.rom.used += (Bit16u)CALLBACK_Setup(callback.rmWindow,
											 VESA_SetWindow, CB_RETF,
											 PhysMake(0xc000,int10.rom.used), "VESA Real Set Window");
	
	
	/* Prepare the pmode interface */
	int10.rom.pmode_interface  = RealMake(0xc000,int10.rom.used);
	int10.rom.used 			  += 8;															/* <-- Skip the byte later used for offsets */
	
	/* PM Set Window call */
	int10.rom.pmode_interface_window = int10.rom.used - RealOff( int10.rom.pmode_interface );
	phys_writew( Real2Phys(int10.rom.pmode_interface) + 0, int10.rom.pmode_interface_window );
	callback.pmWindow	= CALLBACK_Allocate();
	int10.rom.used 	   += (Bit16u)CALLBACK_Setup(callback.pmWindow,
											 VESA_PMSetWindow, CB_RETN,
											 PhysMake(0xc000,int10.rom.used),"VESA PM Set Window");
	
	/* PM Set start call */
	int10.rom.pmode_interface_start = int10.rom.used - RealOff( int10.rom.pmode_interface );
	phys_writew( Real2Phys(int10.rom.pmode_interface) + 2, int10.rom.pmode_interface_start);
	callback.pmStart		        = CALLBACK_Allocate();
	
	/* Custom S3 VGA //////////////////////////////////////////////////////////////////*/	
	int10.rom.used 				   += (Bit16u)CALLBACK_Setup(callback.pmStart,
											 VESA_PMSetStart, CB_VESA_START,
											 PhysMake(0xc000,int10.rom.used), "VESA PM Set Start");	
	/* Custom S3 VGA //////////////////////////////////////////////////////////////////*/	
	
	/* PM Set Palette call */
	int10.rom.pmode_interface_palette = int10.rom.used - RealOff( int10.rom.pmode_interface );
	phys_writew( Real2Phys(int10.rom.pmode_interface) + 4, int10.rom.pmode_interface_palette);
	callback.pmPalette=CALLBACK_Allocate();
	int10.rom.used += (Bit16u)CALLBACK_Setup(0, NULL, CB_VESA_PM, PhysMake(0xc000,int10.rom.used), "");
	int10.rom.used += (Bit16u)CALLBACK_Setup(callback.pmPalette,
											 VESA_PMSetPalette, CB_RETN,
											 PhysMake(0xc000,int10.rom.used), "VESA PM Set Palette");
											 
	/* Finalize the size and clear the required ports pointer */
	phys_writew( Real2Phys(int10.rom.pmode_interface) + 6, 0);
	int10.rom.pmode_interface_size=int10.rom.used - RealOff( int10.rom.pmode_interface );
}
