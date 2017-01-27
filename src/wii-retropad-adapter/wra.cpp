/*
 * Wii RetroPad Adapter - Nintendo Wiimote adapter for retro-controllers!
 * Copyright (c) 2011 Bruno Freitas - bruno@brunofreitas.com
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

#include <WProgram.h>

#include "WMExtension.h"
#include "PS2Pad.h"
#include "genesis.h"
#include "saturn.h"
#include "NESPad.h"
#include "GCPad.h"
#include "tg16.h"

// Classic Controller Buttons
int bdl = 0; // D-Pad Left state
int bdr = 0; // D-Pad Right state
int bdu = 0; // D-Pad Up state
int bdd = 0; // D-Pad Down state
int ba = 0; // A button state
int bb = 0; // B button state
int bx = 0; // X button state
int by = 0; // Y button state
int bl = 0; // L button state
int br = 0; // R button state
int bm = 0; // MINUS button state
int bp = 0; // PLUS button state
int bhome = 0; // HOME button state
int bzl = 0; // ZL button state
int bzr = 0; // ZR button state
int lt = 0; // L analog value
int rt = 0; // R analog value

// Analog Buttons
byte lx = WMExtension::get_calibration_byte(2);
byte ly = WMExtension::get_calibration_byte(5);
byte rx = WMExtension::get_calibration_byte(8);
byte ry = WMExtension::get_calibration_byte(11);

// Analog stick neutral radius
#define ANALOG_NEUTRAL_RADIUS 10

// Extension cable detection pins
#define DETPIN0 3  // DB9P2
#define DETPIN1	5  // DB9P4
#define DETPIN2	6  // DB9P6
#define DETPIN3	7  // DB9P7
#define DETPIN4	8  // DB9P9

// Possible values (as of today) returned by the detectPad() routine
// Normal pads
#define PAD_ARCADE		-1
#define PAD_GENESIS		0b00111
#define PAD_NES 		0b00110
#define PAD_SNES 		0b00101
#define PAD_PS2 		0b00100
#define PAD_GC	 		0b00011
#define PAD_N64			0b00010
#define PAD_NEOGEO		0b00001
#define PAD_WIICC		0b00000
// Extended pads (uses DB9 pin 4 and/or 2 for identification)
#define PAD_SATURN		0b01111
#define PAD_TG16		0b10111
#define PAD_DFU_DONGLE	0b01110 // Reserved for USBRA DFU dongle

/*
 * This is the new auto-detect function (non jumper based) which detects the extension
 * cable plugged in the DB9 port. It uses grounded pins from DB9 (4, 6, 7 and 9) for
 * the detection.
 *
 *  -1 - Arcade
 * 00111 - Sega Genesis (Default)
 * 00110 - NES
 * 00101 - SNES
 * 00100 - PS2
 * 00011 - Game Cube
 * 00010 - Nintendo 64
 * 00001 - Neo Geo
 * 00000 - Reserved 1
 * 01111 - Sega Saturn
 * 10111 - TurboGrafx 16
 */
int detectPad() {
	int pad;

	// Set pad/arcade detection pins as input, turning pull-ups on
	pinMode(DETPIN0, INPUT);
	digitalWrite(DETPIN0, HIGH);

	pinMode(DETPIN1, INPUT);
	digitalWrite(DETPIN1, HIGH);

	pinMode(DETPIN2, INPUT);
	digitalWrite(DETPIN2, HIGH);

	pinMode(DETPIN3, INPUT);
	digitalWrite(DETPIN3, HIGH);

	pinMode(DETPIN4, INPUT);
	digitalWrite(DETPIN4, HIGH);

	pad = (!digitalRead(DETPIN0) << 4) | (!digitalRead(DETPIN1) << 3) | (digitalRead(DETPIN2) << 2) | (digitalRead(DETPIN3) << 1) | (digitalRead(DETPIN4));

	if((pad >> 3) & 0b11) {
		switch(pad) {
		case 0b11011:
		case 0b10111:
			return PAD_TG16;
			break;
		case 0b11111:
		case 0b01111:
			return PAD_SATURN;
			break;
		case 0b11100:
			return PAD_PS2;
			break;
		default:
			return PAD_GENESIS;
			break;
		}
	}

	return (pad & 0b111);
}


// Genesis pad loop
void genesis_loop() {
	int button_data;

	genesis_init();

	for (;;) {
		button_data = genesis_read();

		bdl = button_data & GENESIS_LEFT;
		bdr = button_data & GENESIS_RIGHT;
		bdu = button_data & GENESIS_UP;
		bdd = button_data & GENESIS_DOWN;
		ba = button_data & GENESIS_C;
		bb = button_data & GENESIS_B;
		bx = button_data & GENESIS_Y;
		by = button_data & GENESIS_A;
		bl = button_data & GENESIS_X;
		br = button_data & GENESIS_Z;
		bm = button_data & GENESIS_MODE;
		bp = button_data & GENESIS_START;
		bhome = (bdu && bp); // UP + START == HOME

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);
	}
}

// NES pad loop
void nes_loop() {
	int button_data;

	NESPad::init();

	for (;;) {

		button_data = NESPad::read(8);

		bdl = button_data & 64;
		bdr = button_data & 128;
		bdu = button_data & 16;
		bdd = button_data & 32;
		ba = button_data & 1;
		bb = button_data & 2;
		bm = button_data & 4;
		bp = button_data & 8;
		bhome = (bm && bp); // SELECT + START == HOME

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);
	}
}

// SNES pad loop
void snes_loop() {
	int button_data;

	NESPad::init();

	for (;;) {
		button_data = NESPad::read(16);

		bdl = button_data & 64;
		bdr = button_data & 128;
		bdu = button_data & 16;
		bdd = button_data & 32;
		bb = button_data & 1;
		by = button_data & 2;
		bm = button_data & 4;
		bp = button_data & 8;
		ba = button_data & 256;
		bx = button_data & 512;
		bl = button_data & 1024;
		br = button_data & 2048;
		bhome = (bm && bp); // SELECT + START == HOME

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);
	}
}

// PS2 pad loop
void ps2_loop() {

	byte center_lx, center_ly, center_rx, center_ry;
	byte _lx, _ly, _rx, _ry;

	byte clx = WMExtension::get_calibration_byte(2);
	byte cly = WMExtension::get_calibration_byte(5);
	byte crx = WMExtension::get_calibration_byte(8);
	byte cry = WMExtension::get_calibration_byte(11);

	while (PS2Pad::init(false));

	PS2Pad::read();

	// If Pad mode is digital
	if(PS2Pad::PS2Pad_mode() == 4) {
		center_lx = clx;
		center_ly = cly;
		center_rx = crx;
		center_ry = cry;
	} else {
		center_lx = PS2Pad::stick(PSS_LX);
		center_ly = PS2Pad::stick(PSS_LY);
		center_rx = PS2Pad::stick(PSS_RX);
		center_ry = PS2Pad::stick(PSS_RY);
	}

	for (;;) {
		PS2Pad::read();

		bdl = PS2Pad::button(PSB_PAD_LEFT);
		bdr = PS2Pad::button(PSB_PAD_RIGHT);
		bdu = PS2Pad::button(PSB_PAD_UP);
		bdd = PS2Pad::button(PSB_PAD_DOWN);

		by = PS2Pad::button(PSB_SQUARE);
		bb = PS2Pad::button(PSB_CROSS);
		bx = PS2Pad::button(PSB_TRIANGLE);
		ba = PS2Pad::button(PSB_CIRCLE);

		bl = PS2Pad::button(PSB_L1);
		br = PS2Pad::button(PSB_R1);

		bzl = PS2Pad::button(PSB_L2);
		bzr = PS2Pad::button(PSB_R2);

		bm = PS2Pad::button(PSB_SELECT);
		bp = PS2Pad::button(PSB_START);

		bhome = (bm && bp); // SELECT + START == HOME

		// If Pad mode is Digital
		if(PS2Pad::PS2Pad_mode() == 4) {
			lx = clx;
			ly = cly;
			rx = crx;
			ry = cry;
		} else {
			_lx = PS2Pad::stick(PSS_LX);
			_ly = PS2Pad::stick(PSS_LY);
			_rx = PS2Pad::stick(PSS_RX);
			_ry = PS2Pad::stick(PSS_RY);

			if((_lx >= (center_lx - ANALOG_NEUTRAL_RADIUS)) && (_lx <= (center_lx + ANALOG_NEUTRAL_RADIUS))) {
				_lx = clx;
			}

			if((_ly >= (center_ly - ANALOG_NEUTRAL_RADIUS)) && (_ly <= (center_ly + ANALOG_NEUTRAL_RADIUS))) {
				_ly = cly;
			}


			if((_rx >= (center_rx - ANALOG_NEUTRAL_RADIUS/2)) && (_rx <= (center_rx + ANALOG_NEUTRAL_RADIUS/2))) {
				_rx = crx;
			}

			if((_ry >= (center_ry - ANALOG_NEUTRAL_RADIUS/2)) && (_ry <= (center_ry + ANALOG_NEUTRAL_RADIUS/2))) {
				_ry = cry;
			}

			lx = _lx;
			ly = ~_ly;
			rx = _rx;
			ry = ~_ry;

		}

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
					bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);
	}
}

void gc_loop_helper(void) {
	GCPad_read(false);
}

void gc_loop() {
	byte *button_data;

	byte center_lx, center_ly, center_rx, center_ry;
	byte _lx, _ly, _rx, _ry;

	byte clx = WMExtension::get_calibration_byte(2);
	byte cly = WMExtension::get_calibration_byte(5);
	byte crx = WMExtension::get_calibration_byte(8);
	byte cry = WMExtension::get_calibration_byte(11);

	while(!GCPad_init(true, true)) {
		delayMicroseconds(10000);
	}

	delayMicroseconds(10000);

	GCPad_read(true);

	button_data = GCPad_data();

	center_lx = button_data[2];
	center_ly = button_data[3];
	center_rx = button_data[4];
	center_ry = button_data[5];

	WMExtension::set_button_data_callback(gc_loop_helper);

	for(;;) {
		button_data = GCPad_data();

		bdl = button_data[1] & 0x01;
		bdr = button_data[1] & 0x02;
		bdu = button_data[1] & 0x08;
		bdd = button_data[1] & 0x04;

		by = button_data[0] & 0x08;
		bb = button_data[0] & 0x02;
		bx = button_data[0] & 0x04;
		ba = button_data[0] & 0x01;

		bp = button_data[0] & 0x10;

		bl = button_data[1] & 0x40;
		br = button_data[1] & 0x20;

		bzl = bzr = button_data[1] & 0x10;

		bhome = (bdu && bp); // UP + START == HOME

		_lx = button_data[2];
		_ly = button_data[3];
		_rx = button_data[4];
		_ry = button_data[5];

		if((_lx >= (center_lx - ANALOG_NEUTRAL_RADIUS)) && (_lx <= (center_lx + ANALOG_NEUTRAL_RADIUS))) {
			_lx = clx;
		}

		if((_ly >= (center_ly - ANALOG_NEUTRAL_RADIUS)) && (_ly <= (center_ly + ANALOG_NEUTRAL_RADIUS))) {
			_ly = cly;
		}

		if((_rx >= (center_rx - ANALOG_NEUTRAL_RADIUS/2)) && (_rx <= (center_rx + ANALOG_NEUTRAL_RADIUS/2))) {
			_rx = crx;
		}

		if((_ry >= (center_ry - ANALOG_NEUTRAL_RADIUS/2)) && (_ry <= (center_ry + ANALOG_NEUTRAL_RADIUS/2))) {
			_ry = cry;
		}

		lx = _lx;
		ly = _ly;
		rx = _rx;
		ry = _ry;

		lt = button_data[6]; //map(button_data[6], 0, 255, 0, 31);
		rt = button_data[7]; //map(button_data[7], 0, 255, 0, 31);

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);

	}
}

void n64_loop_helper(void) {
	N64Pad_read(false);
}

void n64_loop() {
	byte *button_data;
	bool swap_l_z = false;

	byte center_lx, center_ly;
	byte _lx, _ly, _rx, _ry;

	byte clx = WMExtension::get_calibration_byte(2);
	byte cly = WMExtension::get_calibration_byte(5);
	byte crx = WMExtension::get_calibration_byte(8);
	byte cry = WMExtension::get_calibration_byte(11);

	while(!GCPad_init(true, true)) {
		delayMicroseconds(10000);
	}

	delayMicroseconds(10000);

	N64Pad_read(true);

	button_data = N64Pad_data();

	center_lx = ((button_data[2] >= 128) ? button_data[2] - 128 : button_data[2] + 128);
	center_ly = ((button_data[3] >= 128) ? button_data[3] - 128 : button_data[3] + 128);

	// If plugged in with L pressed, L and Z buttons will be swapped (for Zelda games' sake!)
	if (button_data[1] & 0x20) {
		swap_l_z = true;
	}

	WMExtension::set_button_data_callback(n64_loop_helper);

	for(;;) {
		button_data = N64Pad_data();

		bdl = button_data[0] & 0x02;
		bdr = button_data[0] & 0x01;
		bdu = button_data[0] & 0x08;
		bdd = button_data[0] & 0x04;

		bb = button_data[0] & 0x40;
		ba = button_data[0] & 0x80;

		bp = button_data[0] & 0x10;

		br = button_data[1] & 0x10;

		if (!swap_l_z) {
			bl = button_data[1] & 0x20;
			bzl = bzr = button_data[0] & 0x20;
		} else {
			bl = button_data[0] & 0x20;
			bzl = bzr = button_data[1] & 0x20;
		}

		bhome = (bdu && bp); // UP + START == HOME

		_ry = cry;

		if(button_data[1] & 0x08) { // C Up
			_ry = 30;
		} else if(button_data[1] & 0x04) { // C Down
			_ry = 1;
		}

		_rx = crx;

		if(button_data[1] & 0x02) { // C Left
			_rx = 1;
		} else if(button_data[1] & 0x01) { // C Right
			_rx = 30;
		}

		by = button_data[1] & 0x02; // Y == C Left
		bx = button_data[1] & 0x01; // X == C Right

		_lx = ((button_data[2] >= 128) ? button_data[2] - 128 : button_data[2] + 128);
		_ly = ((button_data[3] >= 128) ? button_data[3] - 128 : button_data[3] + 128);

		if((_lx >= (center_lx - ANALOG_NEUTRAL_RADIUS)) && (_lx <= (center_lx + ANALOG_NEUTRAL_RADIUS))) {
			_lx = clx;
		}

		if((_ly >= (center_ly - ANALOG_NEUTRAL_RADIUS)) && (_ly <= (center_ly + ANALOG_NEUTRAL_RADIUS))) {
			_ly = cly;
		}

		lx = _lx;
		ly = _ly;
		rx = _rx;
		ry = _ry;

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);

	}
}

void neogeo_loop() {
	int button_data;

	NESPad::init();

	for (;;) {
		button_data = NESPad::read(16);

		bdl = button_data & 0x02;
		bdr = button_data & 0x800;
		bdu = button_data & 0x04;
		bdd = button_data & 0x1000;
		bb = button_data & 0x01;
		by = button_data & 0x8000;
		bm = button_data & 0x100;
		bp = button_data & 0x4000;
		ba = button_data & 0x400;
		bx = button_data & 0x200; // D button is also 0x2000
		bhome = (bm && bp); // SELECT + START == HOME

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);
	}
}

void saturn_loop() {
	int button_data;

	saturn_init();

	for (;;) {
		button_data = saturn_read();

		bdl = button_data & SATURN_LEFT;
		bdr = button_data & SATURN_RIGHT;
		bdu = button_data & SATURN_UP;
		bdd = button_data & SATURN_DOWN;
		ba = button_data & SATURN_C;
		bb = button_data & SATURN_B;
		bx = button_data & SATURN_Y;
		by = button_data & SATURN_A;
		bl = button_data & SATURN_X;
		br = button_data & SATURN_Z;

		bzl = button_data & SATURN_L;
		bzr = button_data & SATURN_R;

		bp = button_data & SATURN_START;

		bhome = (bdu && bp); // UP + START == HOME

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);
	}
}

// TG16 pad loop
void tg16_loop() {
	int button_data;

	tg16_init();

	for (;;) {

		button_data = tg16_read();

		bdl = button_data & (1 << TG16_LEFT);
		bdr = button_data & (1 << TG16_RIGHT);
		bdu = button_data & (1 << TG16_UP);
		bdd = button_data & (1 << TG16_DOWN);
		ba = button_data & (1 << TG16_I);
		bb = button_data & (1 << TG16_II);
		bm = button_data & (1 << TG16_SELECT);
		bp = button_data & (1 << TG16_RUN);
		bhome = (bm && bp); // SELECT + START == HOME

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr, lt, rt);
	}
}

void unsupported_pad(void) {
	for(;;);
}

void setup() {
	// Prepare wiimote communications
	WMExtension::init();
}

void loop() {
	// Select pad loop based on pad auto-detection routine. Genesis pad is the default.
	switch (detectPad()) {
	case PAD_NES:
		nes_loop();
		break;
	case PAD_SNES:
		snes_loop();
		break;
	case PAD_PS2:
		ps2_loop();
		break;
	case PAD_GC:
		gc_loop();
		break;
	case PAD_N64:
		n64_loop();
		break;
	case PAD_NEOGEO:
		neogeo_loop();
		break;
	case PAD_SATURN:
		saturn_loop();
		break;
	case PAD_TG16:
		tg16_loop();
		break;
	case PAD_WIICC:
		unsupported_pad();
		break;
	default:
		genesis_loop();
		break;
	}
}
