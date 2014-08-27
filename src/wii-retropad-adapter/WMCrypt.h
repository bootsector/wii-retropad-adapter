/*
* Wii RetroPad Adapter - Nintendo Wiimote adapter for retro-controllers!
* Copyright (c) 2011 Bruno Freitas - bruno@brunofreitas.com
* Copyright (C) 2008-2009 Héctor Martín <hector@marcansoft.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WMCRYPT_H_
#define WMCRYPT_H_

#include <avr/pgmspace.h>

class WMCrypt {

public:
	static unsigned char wm_ft[8];
	static unsigned char wm_sb[8];
	static void wiimote_gen_key(const unsigned char* const keydata);

private:
	static const unsigned char ans_tbl[7][6] PROGMEM;
	static const unsigned char tsbox[256] PROGMEM;
	static const unsigned char sboxes[8][256] PROGMEM;

	static unsigned char ror8(const unsigned char a, const unsigned char b);
	static void genkey(const unsigned char* const rand, const unsigned char idx, unsigned char* const key);
	static void gentabs(const unsigned char* const rand, const unsigned char* const key, const unsigned char idx, unsigned char* const ft, unsigned char* const sb);

};

#endif /* WMCRYPT_H_ */
