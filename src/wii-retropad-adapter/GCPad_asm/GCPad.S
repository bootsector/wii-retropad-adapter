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
 ;  59 "GCPad.cpp" 1
	nop
nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
.L5:
/* #APP */
 ;  62 "GCPad.cpp" 1
	nop
nop
nop
nop

 ;  0 "" 2
/* #NOAPP */
	rjmp .L6
.L3:
/* #APP */
 ;  64 "GCPad.cpp" 1
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
 ;  73 "GCPad.cpp" 1
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
 ;  78 "GCPad.cpp" 1
	nop
nop

 ;  0 "" 2
/* #NOAPP */
.L6:
	tst r22
	breq .+2
	rjmp .L2
/* #APP */
 ;  83 "GCPad.cpp" 1
	nop

 ;  0 "" 2
/* #NOAPP */
	cbi 43-32,2
/* #APP */
 ;  88 "GCPad.cpp" 1
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
	ldi r30,lo8(gc_joy_data)
	ldi r31,hi8(gc_joy_data)
	ldi r20,lo8(7)
	ldi r21,hi8(7)
	ldi r28,lo8(0)
	ldi r29,hi8(0)
	ldi r16,lo8(1)
	ldi r17,hi8(1)
	rjmp .L12
.L16:
	ld r24,X
	ld r18,Z
	tst r24
	breq .L13
	movw r24,r16
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	or r18,r24
	rjmp .L20
.L13:
	movw r24,r16
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	com r24
	and r18,r24
.L20:
	st Z,r18
	subi r20,lo8(-(-1))
	sbci r21,hi8(-(-1))
	sbrs r21,7
	rjmp .L15
	ldi r20,lo8(7)
	ldi r21,hi8(7)
.L15:
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	adiw r26,1
	cpi r22,8
	cpc r23,__zero_reg__
	brne .L16
	adiw r28,8
	adiw r30,1
	cpi r28,64
	cpc r29,__zero_reg__
	breq .L17
.L12:
	movw r26,r28
	subi r26,lo8(-(raw_joy_data))
	sbci r27,hi8(-(raw_joy_data))
	ldi r22,lo8(0)
	ldi r23,hi8(0)
	rjmp .L16
.L17:
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
	ldi r30,lo8(_ZZ10GCPad_readbE3C.9)
	ldi r31,hi8(_ZZ10GCPad_readbE3C.9)
	ldi r24,lo8(3)
.L22:
	ld r0,Z+
	st X+,r0
	subi r24,lo8(-(-1))
	brne .L22
	tst r17
	breq .L23
/* #APP */
 ;  191 "GCPad.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
.L23:
	movw r24,r28
	adiw r24,1
	ldi r22,lo8(3)
	call _ZL10GCPad_sendPhh
	cbi 42-32,2
	sbi 43-32,2
	ldi r30,lo8(raw_joy_data)
	ldi r31,hi8(raw_joy_data)
.L34:
	sbic 41-32,2
	rjmp .L34
/* #APP */
 ;  115 "GCPad.cpp" 1
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
nop

 ;  0 "" 2
/* #NOAPP */
	in r24,41-32
	andi r24,lo8(4)
	st Z,r24
	ldi r24,hi8(raw_joy_data+63)
	cpi r30,lo8(raw_joy_data+63)
	cpc r31,r24
	breq .L25
.L30:
	sbis 41-32,2
	rjmp .L30
	adiw r30,1
	rjmp .L34
.L25:
	tst r17
	breq .L28
/* #APP */
 ;  197 "GCPad.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L28:
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
	ldi r30,lo8(n64_joy_data)
	ldi r31,hi8(n64_joy_data)
	ldi r20,lo8(7)
	ldi r21,hi8(7)
	ldi r28,lo8(0)
	ldi r29,hi8(0)
	ldi r16,lo8(1)
	ldi r17,hi8(1)
	rjmp .L36
.L40:
	ld r24,X
	ld r18,Z
	tst r24
	breq .L37
	movw r24,r16
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	or r18,r24
	rjmp .L44
.L37:
	movw r24,r16
	mov r0,r20
	rjmp 2f
1:	lsl r24
	rol r25
2:	dec r0
	brpl 1b
	com r24
	and r18,r24
.L44:
	st Z,r18
	subi r20,lo8(-(-1))
	sbci r21,hi8(-(-1))
	sbrs r21,7
	rjmp .L39
	ldi r20,lo8(7)
	ldi r21,hi8(7)
.L39:
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	adiw r26,1
	cpi r22,8
	cpc r23,__zero_reg__
	brne .L40
	adiw r28,8
	adiw r30,1
	cpi r28,32
	cpc r29,__zero_reg__
	breq .L41
.L36:
	movw r26,r28
	subi r26,lo8(-(raw_joy_data))
	sbci r27,hi8(-(raw_joy_data))
	ldi r22,lo8(0)
	ldi r23,hi8(0)
	rjmp .L40
.L41:
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
	breq .L46
/* #APP */
 ;  228 "GCPad.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
.L46:
	ldi r24,lo8(1)
	std Y+1,r24
	movw r24,r28
	adiw r24,1
	ldi r22,lo8(1)
	call _ZL10GCPad_sendPhh
	cbi 42-32,2
	sbi 43-32,2
	ldi r30,lo8(raw_joy_data)
	ldi r31,hi8(raw_joy_data)
.L56:
	sbic 41-32,2
	rjmp .L56
/* #APP */
 ;  115 "GCPad.cpp" 1
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
nop

 ;  0 "" 2
/* #NOAPP */
	in r24,41-32
	andi r24,lo8(4)
	st Z,r24
	ldi r24,hi8(raw_joy_data+31)
	cpi r30,lo8(raw_joy_data+31)
	cpc r31,r24
	breq .L48
.L53:
	sbis 41-32,2
	rjmp .L53
	adiw r30,1
	rjmp .L56
.L48:
	tst r17
	breq .L51
/* #APP */
 ;  234 "GCPad.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L51:
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	ret
	.size	_Z11N64Pad_readb, .-_Z11N64Pad_readb
	.section	.text._Z10GCPad_initv,"ax",@progbits
.global	_Z10GCPad_initv
	.type	_Z10GCPad_initv, @function
_Z10GCPad_initv:
	push r17
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	ldi r30,lo8(raw_joy_data)
	ldi r31,hi8(raw_joy_data)
.L58:
	st Z+,__zero_reg__
	ldi r24,hi8(raw_joy_data+64)
	cpi r30,lo8(raw_joy_data+64)
	cpc r31,r24
	brne .L58
	ldi r30,lo8(gc_joy_data)
	ldi r31,hi8(gc_joy_data)
.L59:
	st Z+,__zero_reg__
	ldi r24,hi8(gc_joy_data+8)
	cpi r30,lo8(gc_joy_data+8)
	cpc r31,r24
	brne .L59
	sts n64_joy_data,__zero_reg__
	sts n64_joy_data+1,__zero_reg__
	sts n64_joy_data+2,__zero_reg__
	sts n64_joy_data+3,__zero_reg__
/* #APP */
 ;  146 "GCPad.cpp" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	std Y+1,__zero_reg__
	movw r24,r28
	adiw r24,1
	ldi r22,lo8(1)
	call _ZL10GCPad_sendPhh
	cbi 42-32,2
	sbi 43-32,2
	ldi r17,lo8(64)
.L61:
	sbis 41-32,2
	rjmp .L60
	subi r17,lo8(-(-1))
	brne .L61
.L60:
/* #APP */
 ;  155 "GCPad.cpp" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(500)
	ldi r25,hi8(500)
	call delayMicroseconds
	mov r24,r17
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	ret
	.size	_Z10GCPad_initv, .-_Z10GCPad_initv
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
	.section	.rodata._ZZ10GCPad_readbE3C.9,"a",@progbits
	.type	_ZZ10GCPad_readbE3C.9, @object
	.size	_ZZ10GCPad_readbE3C.9, 3
_ZZ10GCPad_readbE3C.9:
	.byte	64
	.byte	3
	.byte	0
