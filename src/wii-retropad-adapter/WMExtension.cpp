/*
* Wii RetroPad Adapter - Nintendo Wiimote adapter for retro-controllers!
* Copyright (c) 2011 Bruno Freitas - bootsector@ig.com.br
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

#include <WProgram.h>
#include <Wire.h>
#include "WMExtension.h"
#include "WMCrypt.h"

/* Classic Controller ID */
const byte WMExtension::id[6] = { 0x00, 0x00, 0xa4, 0x20, 0x01, 0x01 };

/*
 *  Classic Controller Calibration Data.
 * Last two bytes (checksum) will be calculated by the init() function
  */
byte WMExtension::calibration_data[16] = { 0xFC, 0x04, 0x7E, 0xFC, 0x04,
		0x7E, 0xFC, 0x04, 0x7E,	0xFC, 0x04, 0x7E, 0x00, 0x00, 0x00, 0x00 };

/* Address or command requested by the I2C Master Device (i.e., the Wiimote) */
byte WMExtension::state = 0;

/* Tells whether encryption was setup or not */
byte WMExtension::crypt_setup_done = 0;

/* Classic Controller buttons status */
byte WMExtension::buttons_data[6];

/* Classic Controller 256 data registers */
byte WMExtension::registers[0x100];

/*
 * Callback function pointer that will be called after the Wiimote has requested
 * buttons status (state == 0x00 on handle_request function).
 *
 * First buttons status requested by the Wiimote will be always zeroed, which
 * don't represent a problem.
 *
 * User should provide the callback function which should then call
 * WMExtension::set_button_data function that updates the buttons_data array.
 *
 * The updated array will be provided to the Wiimote when it requests buttons
 * status.
 *
 * This pointer is set when user calls WMExtension::set_button_data_callback
 * which will usually happen during program setup, before calling
 * WMExtension::init function.
 */
WMExtension::CBackPtr WMExtension::cbPtr = NULL;

/* Sets the buttons state update callback function */
void WMExtension::set_button_data_callback(CBackPtr cb) {
	WMExtension::cbPtr = cb;
}

/* Returns 1 of the 16 possible bytes from the calibration data array */
byte WMExtension::get_calibration_byte(int b) {
	if(b >= 0 && b <= 15)
		return WMExtension::calibration_data[b];
	else
		return 0;
}

/*
 * Setup Wiimote <-> Extension I2C communication encryption, if requested by
 * the application (game/homebrew).
 */
void WMExtension::setup_encryption() {
	int i;

	for (i = 0x40; i <= 0x49; i++) {
		WMCrypt::wm_rand[9 - (i - 0x40)] = WMExtension::registers[i];
	}

	for (i = 0x4A; i <= 0x4F; i++) {
		WMCrypt::wm_key[5 - (i - 0x4A)] = WMExtension::registers[i];
	}

	WMCrypt::wm_gentabs();

	WMExtension::crypt_setup_done = 1;
}

/*
 * Generic function for sending data via Wire.send().
 * Supports Wiimote encryption, if enabled.
  */
void WMExtension::send_data(uint8_t* data, uint8_t size, uint8_t addr) {
	static uint8_t buffer[8];
	int i;

	if (WMExtension::registers[0xF0] == 0xAA && WMExtension::crypt_setup_done) {
		for (i = 0; i < size; i++) {
			buffer[i] = (data[i] - WMCrypt::wm_ft[(addr + i) % 8]) ^ WMCrypt::wm_sb[(addr + i)
					% 8];
		}

		Wire.send(buffer, size);
	} else {
		Wire.send(data, size);
	}
}

/* I2C slave handler for data received from the Wiimote */
void WMExtension::receive_bytes(int count) {
	byte crypt_keys_received = 0;

	if (count == 1) {
		WMExtension::state = Wire.receive();
	} else if (count > 1) {
		byte addr = Wire.receive();
		byte curr = addr;

		for (int i = 1; i < count; i++) {
			byte d = Wire.receive();

			// Wii is trying to disable encryption...
			if(addr == 0xF0 && d == 0x55) {
				WMExtension::crypt_setup_done = 0;
			}

			if (WMExtension::registers[0xF0] == 0xAA && WMExtension::crypt_setup_done) {
				// Decrypt
				WMExtension::registers[curr] = (d ^ WMCrypt::wm_sb[curr % 8]) + WMCrypt::wm_ft[curr
						% 8];
				curr++;
			} else {
				WMExtension::registers[curr++] = d;
			}

			// Check if last crypt key setup byte was received...
			if (curr == 0x50) {
				crypt_keys_received = 1;
			}

		}
	}

	// Setup encryption
	if (crypt_keys_received) {
		WMExtension::setup_encryption();
	}
}

/* I2C slave handler for data request from the Wiimote */
void WMExtension::handle_request() {

	static byte last_state = 0xFF;
	static byte offset = 0;

	switch (WMExtension::state) {

	case 0x00:
		WMExtension::send_data(WMExtension::buttons_data, 6, 0x00);

		if(WMExtension::cbPtr) {
			WMExtension::cbPtr();
		}

		break;

	case 0xFA:
		WMExtension::send_data(WMExtension::registers + WMExtension::state, 6, WMExtension::state);

		break;

	default:
		if(last_state == WMExtension::state) {
			offset += 8;
		} else {
			last_state = WMExtension::state;
			offset = 0;
		}

		WMExtension::send_data(WMExtension::registers + WMExtension::state + offset, 8, WMExtension::state + offset);
		break;
	}
}

/*
 * Takes joystick, and button values and encodes them
 * into a buffer.
 *
 * Classic Controller
 *
 * Buffer encoding details:
 * http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller
 */
void WMExtension::set_button_data(int bdl, int bdr, int bdu, int bdd,
		int ba, int bb, int bx, int by, int blt, int brt, int bminus, int bplus,
		int bhome, byte lx, byte ly, byte rx, byte ry, int bzl, int bzr) {

	WMExtension::buttons_data[0] = ((rx & 0x18) << 3) | (lx & 0x3F);
	WMExtension::buttons_data[1] = ((rx & 0x06) << 5) | (ly & 0x3F);
	WMExtension::buttons_data[2] = ((rx & 0x01) << 7) | (ry & 0x1F);
	WMExtension::buttons_data[3] = 0x00;

	WMExtension::buttons_data[4] = ((bdr ? 1 : 0) << 7) | ((bdd ? 1 : 0) << 6) | ((blt ? 1 : 0)
			<< 5) | ((bminus ? 1 : 0) << 4) | ((bplus ? 1 : 0) << 2)
			| ((brt ? 1 : 0) << 1) | ((bhome ? 1 : 0) << 3);

	WMExtension::buttons_data[5] = ((bb ? 1 : 0) << 6) | ((by ? 1 : 0) << 5) | ((ba ? 1 : 0)
			<< 4) | ((bx ? 1 : 0) << 3) | ((bdl ? 1 : 0) << 1) | (bdu ? 1
			: 0) | ((bzl ? 1 : 0) << 7) | ((bzr ? 1 : 0) << 2);

	WMExtension::buttons_data[4] = ~WMExtension::buttons_data[4];
	WMExtension::buttons_data[5] = ~WMExtension::buttons_data[5];
}

/*
 * Initializes Wiimote connection. Call this function in your
 * setup function.
 */
void WMExtension::init() {
	byte calchecksum = 0;

	memset(WMExtension::registers, 0xFF, 0x100);

	// Set extension id on registers
	for (int i = 0xFA; i <= 0xFF; i++) {
		WMExtension::registers[i] = WMExtension::id[i - 0xFA];
	}

	// Fix calibration data checksum, just in case...
	for(int i = 0; i < 14; i++) {
		calchecksum += WMExtension::calibration_data[i];
	}
	WMExtension::calibration_data[14] = calchecksum + 0x55;
	WMExtension::calibration_data[15] = calchecksum + 0xAA;

	// Set calibration data on registers
	for (int i = 0x20; i <= 0x2F; i++) {
		WMExtension::registers[i] = WMExtension::calibration_data[i - 0x20]; // 0x20
		WMExtension::registers[i + 0x10] = WMExtension::calibration_data[i - 0x20]; // 0x30
	}

	// Initialize buttons_data, otherwise, "Up+Right locked" bug...
	WMExtension::set_button_data(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, WMExtension::calibration_data[2]>>2, WMExtension::calibration_data[5]>>2, WMExtension::calibration_data[8]>>3, WMExtension::calibration_data[11]>>3, 0, 0);

	// Encryption disabled by default
	WMExtension::registers[0xF0] = 0x55;
	WMExtension::registers[0xFB] = 0x00;

	// Join I2C bus
	Wire.begin(0x52);

	Wire.onReceive(WMExtension::receive_bytes);
	Wire.onRequest(WMExtension::handle_request);
}
