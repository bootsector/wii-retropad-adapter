#include <WProgram.h>
#include "genesis.h"
#include "digitalWriteFast.h"

#define DB9P1 2
#define DB9P2 3
#define DB9P3 4
#define DB9P4 5
#define DB9P6 6
#define DB9P7 7
#define DB9P9 8

#define DELAY 14

void genesis_init() {
	pinModeFast(DB9P1, INPUT);
	pinModeFast(DB9P2, INPUT);
	pinModeFast(DB9P3, INPUT);
	pinModeFast(DB9P4, INPUT);
	pinModeFast(DB9P6, INPUT);
	pinModeFast(DB9P7, OUTPUT);
	pinModeFast(DB9P9, INPUT);

	digitalWriteFast(DB9P1, HIGH);
	digitalWriteFast(DB9P2, HIGH);
	digitalWriteFast(DB9P3, HIGH);
	digitalWriteFast(DB9P4, HIGH);
	digitalWriteFast(DB9P6, HIGH);
	digitalWriteFast(DB9P7, HIGH);
	digitalWriteFast(DB9P9, HIGH);
}

int genesis_read() {
	int retval;

	int extrabuttons = 0;
	int normalbuttons = 0;

	// Get D-PAD, B, C buttons state
	digitalWriteFast(DB9P7, HIGH);
	delayMicroseconds(DELAY);

	normalbuttons |= (!digitalReadFast(DB9P1) << 0);
	normalbuttons |= (!digitalReadFast(DB9P2) << 1);
	normalbuttons |= (!digitalReadFast(DB9P3) << 2);
	normalbuttons |= (!digitalReadFast(DB9P4) << 3);
	normalbuttons |= (!digitalReadFast(DB9P6) << 4);
	normalbuttons |= (!digitalReadFast(DB9P9) << 5);

	digitalWriteFast(DB9P7, LOW);
	delayMicroseconds(DELAY);

	// Get A and START buttons state
	normalbuttons |= (!digitalReadFast(DB9P6) << 6);
	normalbuttons |= (!digitalReadFast(DB9P9) << 7);

	digitalWriteFast(DB9P7, HIGH);
	delayMicroseconds(DELAY);
	digitalWriteFast(DB9P7, LOW);
	delayMicroseconds(DELAY);
	digitalWriteFast(DB9P7, HIGH);
	delayMicroseconds(DELAY);

	extrabuttons |= (!digitalReadFast(DB9P1) << 0);
	extrabuttons |= (!digitalReadFast(DB9P2) << 1);
	extrabuttons |= (!digitalReadFast(DB9P3) << 2);
	extrabuttons |= (!digitalReadFast(DB9P4) << 3);

	digitalWriteFast(DB9P7, LOW);
	delayMicroseconds(DELAY);
	digitalWriteFast(DB9P7, HIGH);
	delayMicroseconds(DELAY);
	digitalWriteFast(DB9P7, LOW);

	retval = normalbuttons | (extrabuttons << 8);

	// Delay needed for settling joystick down
	delay(2);

	return retval;
}
