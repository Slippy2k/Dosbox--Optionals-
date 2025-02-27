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


/* Custom S3 VGA /////////////////////////////////////////////////////////////////////////////////////////////*/
typedef struct rgb24
{	
	protected:
	unsigned char byte[3];
	
	public:
	rgb24() {}
	rgb24(const rgb24& val) {
		*this = val; }

	operator int () const {
		return (byte[2]<<16)|(byte[1]<<8)|(byte[0]<<0); }
	int operator& (const rgb24& val) const	{
		return (char)*this & (char)val; }
} rgb24;
/* Custom S3 VGA /////////////////////////////////////////////////////////////////////////////////////////////*/