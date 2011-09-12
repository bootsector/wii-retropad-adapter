/*
 * Copyright (c) 2011 Peter Brinkmann (peter.brinkmann@gmail.com)
 *
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * Bare-bones Nintendo Wiimote library for Arduino, tricking the Wiimote into
 * treating the Arduino as a Classic Controller.  Communication works both to and from
 * the Arduino.
 *
 * Using insights into the communication between Wiimote and Nunchuk found
 * at http://www.ccsinfo.com/forum/viewtopic.php?p=91094.  The wmgui tool that
 * comes with cwiid is good for experimenting and testing.
 *
 * Here's how to connect the Arduino to the Wiimote:
 *      Arduino    Wiimote
 *      GND        GND  (white cable)
 *      AREF       3.3V (red cable)
 *      A4         SDA  (green cable)
 *      A5         SCL  (yellow cable)
 *
 * It doesn't look like it's possible to power the Arduino from the Wiimote.
 *
 * Note that some online instructions for using a Wiimote extension 
 * with Arduino call for changes to twi.h.  
 * Fortunately, this modification is no longer
 * necessary.  Just use a current version of the Arduino development
 * environment.
 */

#ifndef __WIIMOTE_H__
#define __WIIMOTE_H__

#include <WProgram.h>
#include <Wire.h>
#include "wm_crypto.h"

// Identification sequence for Classic Controller
static byte idbuf[] = { 0x00, 0x00, 0xa4, 0x20, 0x01, 0x01 };

// Classic Controller calibration data
byte calbuf[] = { 0xFC, 0x04, 0x7E, 0xFC, 0x04, 0x7E, 0xFC, 0x04, 0x7E,
		0xFC, 0x04, 0x7E, 0x00, 0x00, 0x00, 0x00 }; // Checksum will be calculated later...

//byte calbuf[] = { 0xff, 0x00, 0x80, 0xff, 0x00, 0x80, 0xff, 0x00, 0x80, 0xff,
//		0x00, 0x80, 0x00, 0x00, 0x51, 0xa6 }; // Homebrews (libogc) don't seem to like this one...

static byte outbuf[6];
static byte *curbuf = outbuf;
static byte state = 0;
static byte crypt_setup_done = 0;

/*
 * This function pointer informs the Arduino that a byte has been
 * written to one of the registers corresponding to the Extension.
 * Specifically, if you assign the function foo to this pointer and
 * you write yy bytes starting at the register address 04a400xx of
 * the Wiimote, then this library will call foo(xx, yy).  The bytes
 * are written to the global array wiimote_registers.
 *
 * Note that count will always be less than eight.  If you write
 * eight or more bytes to the Wiimote, this callback will be
 * invoked several times.
 */
void (*wiimote_receive)(byte offset, byte count) = NULL;

/*
 * Callback for streaming bytes from the Arduino to the Wiimote.
 * If this function pointer is not null, then it will be called
 * right _after_ the current output buffer has been sent to the
 * Wiimote, and the implementing function has the choice of either
 * overwriting the given buffer and returning it or returning
 * another buffer that contains the next six bytes to be sent.
 *
 * The return value of this function becomes the new output buffer.
 * The idea is much like the one behind wiimote_swap_buffers.  You
 * probably want to use only one of them; either wiimote_stream
 * for streaming data, or wiimote_swap_buffers for more sporadic
 * updates.
 *
 * There is one exception to this rule: Since this function is
 * called after the current output buffer has been sent, the very
 * first buffer will be all zero unless you initialize it with
 * wiimote_set_byte or wiimote_swap_buffers.  (This approach may
 * seem awkward, but it minimizes delays in I2C communication, and
 * in most cases an all-zero initial buffer is not a problem.)
 */
byte *(*wiimote_stream)(byte *buffer) = NULL;

/*
 * Global array for storing and accessing register values written
 * by the Wiimote.  Those are the 256 registers corresponding to
 * the Extension, starting at 04a40000.
 */
byte wiimote_registers[0x100];

/*
 * Start Wiimote <-> Extension communication encryption,
 * if requested by the application.
 */

static void setup_encryption() {
	int i;

	for (i = 0x40; i <= 0x49; i++) {
		wm_rand[9 - (i - 0x40)] = wiimote_registers[i];
	}

	for (i = 0x4A; i <= 0x4F; i++) {
		wm_key[5 - (i - 0x4A)] = wiimote_registers[i];
	}

	wm_gentabs();

	crypt_setup_done = 1;
}

/*
 * Generic function for sending data via Wire.send().
 * Supports Wiimote encryption, if enabled.
 */
static void send_data(uint8_t* data, uint8_t size, uint8_t addr) {
	static uint8_t buffer[8];
	int i;

	if (wiimote_registers[0xF0] == 0xAA && crypt_setup_done) {
		for (i = 0; i < size; i++) {
			buffer[i] = (data[i] - wm_ft[(addr + i) % 8]) ^ wm_sb[(addr + i)
					% 8];
		}

		Wire.send(buffer, size);
	} else {
		Wire.send(data, size);
	}
}

static void receive_bytes(int count) {
	byte crypt_keys_received = 0;

	if (count == 1) {
		state = Wire.receive();
	} else if (count > 1) {
		byte addr = Wire.receive();
		byte curr = addr;

		for (int i = 1; i < count; i++) {
			byte d = Wire.receive();

			// Wii is trying to disable encryption...
			if(addr == 0xF0 && d == 0x55) {
				crypt_setup_done = 0;
			}

			if (wiimote_registers[0xF0] == 0xAA && crypt_setup_done) {
				// Decrypt
				wiimote_registers[curr] = (d ^ wm_sb[curr % 8]) + wm_ft[curr
						% 8];
				curr++;
			} else {
				wiimote_registers[curr++] = d;
			}

			// Check if last crypt key setup byte was received...
			if (curr == 0x50) {
				crypt_keys_received = 1;
			}

		}
		if (wiimote_receive) {
			wiimote_receive(addr, (byte) count - 1);
		}
	}

	// Setup encryption
	if (crypt_keys_received) {
		setup_encryption();
	}

}

static void handle_request() {

	static byte last_state = 0xFF;
	static byte offset = 0;

	switch (state) {

	case 0x00:
		send_data(curbuf, 6, 0x00);

		if (wiimote_stream) {
			curbuf = wiimote_stream(curbuf);
		}

		break;

	case 0xFA:
		send_data(wiimote_registers + state, 6, state);

		break;

	default:
		if(last_state == state) {
			offset += 8;
		} else {
			last_state = state;
			offset = 0;
		}

		send_data(wiimote_registers + state + offset, 8, state + offset);
		break;
	}
}

/*
 * Sets the given value of the current buffer.
 *
 * index of the value to be set, 0 <= index < 6
 * value new value of the byte at the given index
 *
 * Make sure to read the documentation of wiimote_stream if you
 * intend to use both wiimote_stream and this function.
 */
void wiimote_set_byte(int index, byte value) {
	curbuf[index] = value;
}

/*
 * Makes the given buffer the current buffer and returns the
 * previous buffer.  Use this function if you want to change
 * several values at once.
 *
 * buffer must be a byte buffer of length at least 6.
 *
 * Returns the previous buffer, suitable for reuse.
 *
 * Make sure to read the documentation of wiimote_stream if you
 * intend to use both wiimote_stream and this function.
 */
byte *wiimote_swap_buffers(byte *buffer) {
	byte *tmp = curbuf;
	curbuf = buffer;
	return tmp;
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
void wiimote_write_buffer(byte *buffer, int bdl, int bdr, int bdu, int bdd,
		int ba, int bb, int bx, int by, int blt, int brt, int bminus, int bplus,
		int bhome, byte lx, byte ly, byte rx, byte ry, int bzl, int bzr) {

	buffer[0] = ((rx & 0x18) << 3) | (lx & 0x3F);
	buffer[1] = ((rx & 0x06) << 5) | (ly & 0x3F);
	buffer[2] = ((rx & 0x01) << 7) | (ry & 0x1F);
	buffer[3] = 0x00;

	buffer[4] = ((bdr ? 1 : 0) << 7) | ((bdd ? 1 : 0) << 6) | ((blt ? 1 : 0)
			<< 5) | ((bminus ? 1 : 0) << 4) | ((bplus ? 1 : 0) << 2)
			| ((brt ? 1 : 0) << 1) | ((bhome ? 1 : 0) << 3);

	buffer[5] = ((bb ? 1 : 0) << 6) | ((by ? 1 : 0) << 5) | ((ba ? 1 : 0)
			<< 4) | ((bx ? 1 : 0) << 3) | ((bdl ? 1 : 0) << 1) | (bdu ? 1
			: 0) | ((bzl ? 1 : 0) << 7) | ((bzr ? 1 : 0) << 2);

	buffer[4] = ~buffer[4];
	buffer[5] = ~buffer[5];
}


/*
 * Initializes Wiimote connection. Call this function in your
 * setup function.
 */
void wiimote_init() {
	byte calchecksum = 0;

	memset(wiimote_registers, 0xFF, 0x100);

	// Set extension id on registers
	for (int i = 0xFA; i <= 0xFF; i++) {
		wiimote_registers[i] = idbuf[i - 0xFA];
	}

	// Fix calibration data checksum, just in case...
	for(int i = 0; i < 14; i++) {
		calchecksum += calbuf[i];
	}
	calbuf[14] = calchecksum + 0x55;
	calbuf[15] = calchecksum + 0xAA;

	// Set calibration data on registers
	for (int i = 0x20; i <= 0x2F; i++) {
		wiimote_registers[i] = calbuf[i - 0x20]; // 0x20
		wiimote_registers[i + 0x10] = calbuf[i - 0x20]; // 0x30
	}

	// Initialize curbuf, otherwise, "Up+Right locked" bug...
	wiimote_write_buffer(curbuf, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, calbuf[2]>>2, calbuf[5]>>2, calbuf[8]>>3, calbuf[11]>>3, 0, 0);

	// Encryption disabled by default
	wiimote_registers[0xF0] = 0x55;
	wiimote_registers[0xFB] = 0x00;

	// Join I2C bus
	Wire.begin(0x52);

	Wire.onReceive(receive_bytes);
	Wire.onRequest(handle_request);
}


#endif
