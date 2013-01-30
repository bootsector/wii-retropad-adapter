	.file	"GCPad.cpp"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.global __do_copy_data
	.global __do_clear_bss
	.section	.text._ZL10GCPad_sendPhh,"ax",@progbits
	.type	_ZL10GCPad_sendPhh, @function
_ZL10GCPad_sendPhh:
/* prologue: function */
/* frame size = 0 */
	movw r30,r24
	sbi 42-32,2
	ldi r25,lo8(-128)
.L2:
	cbi 43-32,2
	ld r24,Z
	and r24,r25
	mov r18,r25
	ldi r19,lo8(0)
	breq .L3
	sbi 43-32,2
	movw r24,r18
	asr r25
	ror r24
	mov r25,r24
	tst r24
	brne .L4
	adiw r30,1
	subi r22,lo8(-(-1))
	ldi r25,lo8(-128)
	rjmp .L5
.L4:
/* #APP */
 ;  61 "GCPad.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
.L5:
/* #APP */
 ;  64 "GCPad.cpp" 1
	nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
	rjmp .L6
.L3:
/* #APP */
 ;  66 "GCPad.cpp" 1
	nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
	movw r24,r18
	asr r25
	ror r24
	mov r25,r24
	tst r24
	brne .L7
	adiw r30,1
	subi r22,lo8(-(-1))
	ldi r25,lo8(-128)
	rjmp .L8
.L7:
/* #APP */
 ;  75 "GCPad.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
.L8:
	sbi 43-32,2
/* #APP */
 ;  80 "GCPad.cpp" 1
	nop
nop

 ;  0 "" 2
/* #NOAPP */
.L6:
	tst r22
	breq .+2
	rjmp .L2
/* #APP */
 ;  85 "GCPad.cpp" 1
	nop

 ;  0 "" 2
/* #NOAPP */
	cbi 43-32,2
/* #APP */
 ;  90 "GCPad.cpp" 1
	nop
nop
nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
	sbi 43-32,2
/* epilogue start */
	ret
	.size	_ZL10GCPad_sendPhh, .-_ZL10GCPad_sendPhh
	.section	.text._ZL10GCPad_recvPhh,"ax",@progbits
	.type	_ZL10GCPad_recvPhh, @function
_ZL10GCPad_recvPhh:
/* prologue: function */
/* frame size = 0 */
	movw r30,r24
	cbi 42-32,2
	sbi 43-32,2
	sts timeouted,__zero_reg__
.L12:
	ldi r24,lo8(64)
	rjmp .L13
.L15:
	subi r24,lo8(-(-1))
	brne .L13
	ldi r24,lo8(1)
	sts timeouted,r24
	ret
.L13:
	sbic 41-32,2
	rjmp .L15
/* #APP */
 ;  126 "GCPad.cpp" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
	in r24,41-32
	andi r24,lo8(4)
	st Z,r24
	subi r22,lo8(-(-1))
	breq .L17
.L18:
	sbis 41-32,2
	rjmp .L18
	adiw r30,1
	rjmp .L12
.L17:
	ret
	.size	_ZL10GCPad_recvPhh, .-_ZL10GCPad_recvPhh
	.section	.text._Z10GCPad_initbb,"ax",@progbits
.global	_Z10GCPad_initbb
	.type	_Z10GCPad_initbb, @function
_Z10GCPad_initbb:
	push r16
	push r17
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	mov r17,r24
	mov r16,r22
	tst r24
	breq .L22
/* #APP */
 ;  146 "GCPad.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
.L22:
	std Y+1,__zero_reg__
	movw r24,r28
	adiw r24,1
	ldi r22,lo8(1)
	call _ZL10GCPad_sendPhh
	ldi r24,lo8(raw_joy_data)
	ldi r25,hi8(raw_joy_data)
	ldi r22,lo8(24)
	call _ZL10GCPad_recvPhh
	tst r17
	breq .L23
/* #APP */
 ;  152 "GCPad.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L23:
	tst r16
	breq .L24
	ldi r30,lo8(raw_joy_data)
	ldi r31,hi8(raw_joy_data)
.L25:
	st Z+,__zero_reg__
	ldi r24,hi8(raw_joy_data+64)
	cpi r30,lo8(raw_joy_data+64)
	cpc r31,r24
	brne .L25
	ldi r30,lo8(gc_joy_data)
	ldi r31,hi8(gc_joy_data)
.L26:
	st Z+,__zero_reg__
	ldi r24,hi8(gc_joy_data+8)
	cpi r30,lo8(gc_joy_data+8)
	cpc r31,r24
	brne .L26
	sts n64_joy_data,__zero_reg__
	sts n64_joy_data+1,__zero_reg__
	sts n64_joy_data+2,__zero_reg__
	sts n64_joy_data+3,__zero_reg__
.L24:
	ldi r25,lo8(0)
	lds r24,timeouted
	tst r24
	brne .L27
	ldi r25,lo8(1)
.L27:
	mov r24,r25
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	pop r16
	ret
	.size	_Z10GCPad_initbb, .-_Z10GCPad_initbb
	.section	.text._Z10GCPad_datav,"ax",@progbits
.global	_Z10GCPad_datav
	.type	_Z10GCPad_datav, @function
_Z10GCPad_datav:
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
	lds r24,timeouted
	tst r24
	brne .L32
	ldi r30,lo8(gc_joy_data)
	ldi r31,hi8(gc_joy_data)
	ldi r20,lo8(7)
	ldi r21,hi8(7)
	ldi r28,lo8(0)
	ldi r29,hi8(0)
	ldi r16,lo8(1)
	ldi r17,hi8(1)
	rjmp .L33
.L37:
	ld r24,X
	ld r18,Z
	tst r24
	breq .L34
	movw r24,r16
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	or r18,r24
	rjmp .L40
.L34:
	movw r24,r16
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	com r24
	and r18,r24
.L40:
	st Z,r18
	subi r20,lo8(-(-1))
	sbci r21,hi8(-(-1))
	sbrs r21,7
	rjmp .L36
	ldi r20,lo8(7)
	ldi r21,hi8(7)
.L36:
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	adiw r26,1
	cpi r22,8
	cpc r23,__zero_reg__
	brne .L37
	adiw r28,8
	adiw r30,1
	cpi r28,64
	cpc r29,__zero_reg__
	breq .L32
.L33:
	movw r26,r28
	subi r26,lo8(-(raw_joy_data))
	sbci r27,hi8(-(raw_joy_data))
	ldi r22,lo8(0)
	ldi r23,hi8(0)
	rjmp .L37
.L32:
	ldi r24,lo8(gc_joy_data)
	ldi r25,hi8(gc_joy_data)
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	ret
	.size	_Z10GCPad_datav, .-_Z10GCPad_datav
	.section	.text._Z10GCPad_readb,"ax",@progbits
.global	_Z10GCPad_readb
	.type	_Z10GCPad_readb, @function
_Z10GCPad_readb:
	push r17
	push r29
	push r28
	rcall .
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
	mov r17,r24
	movw r26,r28
	adiw r26,1
	ldi r30,lo8(_ZZ10GCPad_readbE4C.10)
	ldi r31,hi8(_ZZ10GCPad_readbE4C.10)
	ldi r24,lo8(3)
.L42:
	ld r0,Z+
	st X+,r0
	subi r24,lo8(-(-1))
	brne .L42
	tst r17
	breq .L43
/* #APP */
 ;  202 "GCPad.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
.L43:
	movw r24,r28
	adiw r24,1
	ldi r22,lo8(3)
	call _ZL10GCPad_sendPhh
	ldi r24,lo8(raw_joy_data)
	ldi r25,hi8(raw_joy_data)
	ldi r22,lo8(64)
	call _ZL10GCPad_recvPhh
	tst r17
	breq .L44
/* #APP */
 ;  208 "GCPad.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L44:
	ldi r25,lo8(0)
	lds r24,timeouted
	tst r24
	brne .L45
	ldi r25,lo8(1)
.L45:
	mov r24,r25
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	ret
	.size	_Z10GCPad_readb, .-_Z10GCPad_readb
	.section	.text._Z11N64Pad_datav,"ax",@progbits
.global	_Z11N64Pad_datav
	.type	_Z11N64Pad_datav, @function
_Z11N64Pad_datav:
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
	lds r24,timeouted
	tst r24
	brne .L49
	ldi r30,lo8(n64_joy_data)
	ldi r31,hi8(n64_joy_data)
	ldi r20,lo8(7)
	ldi r21,hi8(7)
	ldi r28,lo8(0)
	ldi r29,hi8(0)
	ldi r16,lo8(1)
	ldi r17,hi8(1)
	rjmp .L50
.L54:
	ld r24,X
	ld r18,Z
	tst r24
	breq .L51
	movw r24,r16
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	or r18,r24
	rjmp .L57
.L51:
	movw r24,r16
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	com r24
	and r18,r24
.L57:
	st Z,r18
	subi r20,lo8(-(-1))
	sbci r21,hi8(-(-1))
	sbrs r21,7
	rjmp .L53
	ldi r20,lo8(7)
	ldi r21,hi8(7)
.L53:
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	adiw r26,1
	cpi r22,8
	cpc r23,__zero_reg__
	brne .L54
	adiw r28,8
	adiw r30,1
	cpi r28,32
	cpc r29,__zero_reg__
	breq .L49
.L50:
	movw r26,r28
	subi r26,lo8(-(raw_joy_data))
	sbci r27,hi8(-(raw_joy_data))
	ldi r22,lo8(0)
	ldi r23,hi8(0)
	rjmp .L54
.L49:
	ldi r24,lo8(n64_joy_data)
	ldi r25,hi8(n64_joy_data)
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	ret
	.size	_Z11N64Pad_datav, .-_Z11N64Pad_datav
	.section	.text._Z11N64Pad_readb,"ax",@progbits
.global	_Z11N64Pad_readb
	.type	_Z11N64Pad_readb, @function
_Z11N64Pad_readb:
	push r17
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	mov r17,r24
	tst r24
	breq .L59
/* #APP */
 ;  244 "GCPad.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
.L59:
	ldi r24,lo8(1)
	std Y+1,r24
	movw r24,r28
	adiw r24,1
	ldi r22,lo8(1)
	call _ZL10GCPad_sendPhh
	ldi r24,lo8(raw_joy_data)
	ldi r25,hi8(raw_joy_data)
	ldi r22,lo8(32)
	call _ZL10GCPad_recvPhh
	tst r17
	breq .L60
/* #APP */
 ;  250 "GCPad.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L60:
	ldi r25,lo8(0)
	lds r24,timeouted
	tst r24
	brne .L61
	ldi r25,lo8(1)
.L61:
	mov r24,r25
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	ret
	.size	_Z11N64Pad_readb, .-_Z11N64Pad_readb
	.section	.text._Z15GCPad_timeoutedv,"ax",@progbits
.global	_Z15GCPad_timeoutedv
	.type	_Z15GCPad_timeoutedv, @function
_Z15GCPad_timeoutedv:
/* prologue: function */
/* frame size = 0 */
	lds r24,timeouted
	cpse r24,__zero_reg__
	ldi r24,lo8(1)
.L64:
	ret
	.size	_Z15GCPad_timeoutedv, .-_Z15GCPad_timeoutedv
.global	raw_joy_data
	.section	.bss.raw_joy_data,"aw",@nobits
	.type	raw_joy_data, @object
	.size	raw_joy_data, 64
raw_joy_data:
	.skip 64,0
.global	gc_joy_data
	.section	.bss.gc_joy_data,"aw",@nobits
	.type	gc_joy_data, @object
	.size	gc_joy_data, 8
gc_joy_data:
	.skip 8,0
.global	n64_joy_data
	.section	.bss.n64_joy_data,"aw",@nobits
	.type	n64_joy_data, @object
	.size	n64_joy_data, 4
n64_joy_data:
	.skip 4,0
.global	timeouted
	.section	.bss.timeouted,"aw",@nobits
	.type	timeouted, @object
	.size	timeouted, 1
timeouted:
	.skip 1,0
	.section	.rodata._ZZ10GCPad_readbE4C.10,"a",@progbits
	.type	_ZZ10GCPad_readbE4C.10, @object
	.size	_ZZ10GCPad_readbE4C.10, 3
_ZZ10GCPad_readbE4C.10:
	.byte	64
	.byte	3
	.byte	0
