#ifndef GENESIS_H_
#define GENESIS_H_

void genesis_init();
int genesis_read();

#define GENESIS_UP 0x01
#define GENESIS_DOWN 0x02
#define GENESIS_LEFT 0x04
#define GENESIS_RIGHT 0x08
#define GENESIS_A 0x40
#define GENESIS_B 0x10
#define GENESIS_C 0x20
#define GENESIS_X 0x400
#define GENESIS_Y 0x200
#define GENESIS_Z 0x100
#define GENESIS_START 0x80
#define GENESIS_MODE 0x800

#endif /* GENESIS_H_ */
