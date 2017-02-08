/*
* Wii RetroPad Adapter - Nintendo Wiimote adapter for retro-controllers!
* Copyright (c) 2011 Bruno Freitas - bruno@brunofreitas.com
*
* Wiimote Extension class based on the arduino-wiimote library (c) by
* Peter Brinkmann: https://gitorious.org/randomstuff/arduino-wiimote
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

#ifndef WMEXTENSION_H_
#define WMEXTENSION_H_

#include <WProgram.h>

class WMExtension {

private:

	static const byte id[6];
	static byte calibration_data[16];
	static volatile byte address;
	static volatile byte crypt_setup_done;
	static byte registers[0x100];

	typedef void (*CBackPtr)();
	static CBackPtr cbPtr;

	static void setup_encryption();
	static void send_data(uint8_t* data, uint8_t addr);
	static void receive_bytes(int count);
	static void handle_request();

public:

	static void init();
	static void set_button_data_callback(CBackPtr cb);
	static void set_button_data(int bdl, int bdr, int bdu, int bdd,
		int ba, int bb, int bx, int by, int blt, int brt, int bminus, int bplus,
		int bhome, byte lx, byte ly, byte rx, byte ry, int bzl, int bzr, int lt, int rt);
	static byte get_calibration_byte(int b);
};


#endif /* WMEXTENSION_H_ */
