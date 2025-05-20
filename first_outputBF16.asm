
libxsmm_abi1_neov2_bf16_nn_2x2x4_2_4_2_a1_b1_p0_br0_sa0_sb0_uh0_si0_tc-abid_avnni0_bvnni0_cvnni0.mxm:     file format binary


Disassembly of section .data:

0000000000000000 <.data>:
   0:	d10303ff 	sub	sp, sp, #0xc0
   4:	6d0b27e8 	stp	d8, d9, [sp, #176]
   8:	6d0a2fea 	stp	d10, d11, [sp, #160]
   c:	6d0937ec 	stp	d12, d13, [sp, #144]
  10:	6d083fee 	stp	d14, d15, [sp, #128]
  14:	a9026ffa 	stp	x26, x27, [sp, #32]
  18:	a90177fc 	stp	x28, x29, [sp, #16]
  1c:	f90003fe 	str	x30, [sp]
  20:	8a00000a 	and	x10, x0, x0
  24:	f9401140 	ldr	x0, [x10, #32]
  28:	f9402941 	ldr	x1, [x10, #80]
  2c:	f9404142 	ldr	x2, [x10, #128]
  30:	910003fd 	mov	x29, sp
  34:	d10303ff 	sub	sp, sp, #0xc0
  38:	d29ff80a 	mov	x10, #0xffc0                	// #65472
  3c:	f2bfffea 	movk	x10, #0xffff, lsl #16
  40:	f2dfffea 	movk	x10, #0xffff, lsl #32
  44:	f2ffffea 	movk	x10, #0xffff, lsl #48
  48:	910003e9 	mov	x9, sp
  4c:	8a0a0129 	and	x9, x9, x10
  50:	9100013f 	mov	sp, x9
  54:	910003ea 	mov	x10, sp
  58:	d101014a 	sub	x10, x10, #0x40
  5c:	9100015f 	mov	sp, x10
  60:	d10183a9 	sub	x9, x29, #0x60
  64:	f900012a 	str	x10, [x9]
  68:	910003ea 	mov	x10, sp
  6c:	d102014a 	sub	x10, x10, #0x80
  70:	9100015f 	mov	sp, x10
  74:	d10223a9 	sub	x9, x29, #0x88
  78:	f900012a 	str	x10, [x9]
  7c:	d10303ff 	sub	sp, sp, #0xc0
  80:	6d0b27e8 	stp	d8, d9, [sp, #176]
  84:	6d0a2fea 	stp	d10, d11, [sp, #160]
  88:	6d0937ec 	stp	d12, d13, [sp, #144]
  8c:	6d083fee 	stp	d14, d15, [sp, #128]
  90:	a9026ffa 	stp	x26, x27, [sp, #32]
  94:	a90177fc 	stp	x28, x29, [sp, #16]
  98:	f90003fe 	str	x30, [sp]
  9c:	d10223ac 	sub	x12, x29, #0x88
  a0:	f940018c 	ldr	x12, [x12]
  a4:	f9001180 	str	x0, [x12, #32]
  a8:	d10183aa 	sub	x10, x29, #0x60
  ac:	f940014a 	ldr	x10, [x10]
  b0:	f900298a 	str	x10, [x12, #80]
  b4:	f9401188 	ldr	x8, [x12, #32]
  b8:	f9402989 	ldr	x9, [x12, #80]
  bc:	d280008b 	mov	x11, #0x4                   	// #4
  c0:	d280004a 	mov	x10, #0x2                   	// #2
  c4:	7d400100 	ldr	h0, [x8]
  c8:	7c002520 	str	h0, [x9], #2
  cc:	91001108 	add	x8, x8, #0x4
  d0:	7d400100 	ldr	h0, [x8]
  d4:	7c002520 	str	h0, [x9], #2
  d8:	91001108 	add	x8, x8, #0x4
  dc:	7d400100 	ldr	h0, [x8]
  e0:	7c002520 	str	h0, [x9], #2
  e4:	91001108 	add	x8, x8, #0x4
  e8:	7d400100 	ldr	h0, [x8]
  ec:	7c002520 	str	h0, [x9], #2
  f0:	d1002908 	sub	x8, x8, #0xa
  f4:	d100054a 	sub	x10, x10, #0x1
  f8:	b5fffe6a 	cbnz	x10, 0xc4
  fc:	91003108 	add	x8, x8, #0xc
 100:	91000129 	add	x9, x9, #0x0
 104:	d100116b 	sub	x11, x11, #0x4
 108:	b5fffdcb 	cbnz	x11, 0xc0
 10c:	d10183aa 	sub	x10, x29, #0x60
 110:	f940014a 	ldr	x10, [x10]
 114:	91000140 	add	x0, x10, #0x0
 118:	6d4b27e8 	ldp	d8, d9, [sp, #176]
 11c:	6d4a2fea 	ldp	d10, d11, [sp, #160]
 120:	6d4937ec 	ldp	d12, d13, [sp, #144]
 124:	6d483fee 	ldp	d14, d15, [sp, #128]
 128:	a9426ffa 	ldp	x26, x27, [sp, #32]
 12c:	a94177fc 	ldp	x28, x29, [sp, #16]
 130:	f94003fe 	ldr	x30, [sp]
 134:	910303ff 	add	sp, sp, #0xc0
 138:	2518e3e0 	ptrue	p0.b
 13c:	d2800089 	mov	x9, #0x4                   	// #4
 140:	252917e1 	whilelt	p1.b, xzr, x9
 144:	d2800109 	mov	x9, #0x8                   	// #8
 148:	252917e2 	whilelt	p2.b, xzr, x9
 14c:	d2800209 	mov	x9, #0x10                  	// #16
 150:	252917e3 	whilelt	p3.b, xzr, x9
 154:	d2800047 	mov	x7, #0x2                   	// #2
 158:	d2800046 	mov	x6, #0x2                   	// #2
 15c:	91001049 	add	x9, x2, #0x4
 160:	a4a0a446 	ld1h	{z6.h}, p1/z, [x2]
 164:	05b238c6 	uunpklo	z6.s, z6.h
 168:	04709cc6 	lsl	z6.s, z6.s, #16
 16c:	91001042 	add	x2, x2, #0x4
 170:	a4a0a527 	ld1h	{z7.h}, p1/z, [x9]
 174:	05b238e7 	uunpklo	z7.s, z7.h
 178:	04709ce7 	lsl	z7.s, z7.s, #16
 17c:	91001129 	add	x9, x9, #0x4
 180:	05e760c8 	zip1	z8.d, z6.d, z7.d
 184:	05e764c9 	zip2	z9.d, z6.d, z7.d
 188:	91001042 	add	x2, x2, #0x4
 18c:	d1002042 	sub	x2, x2, #0x8
 190:	d280010b 	mov	x11, #0x8                   	// #8
 194:	d280008a 	mov	x10, #0x4                   	// #4
 198:	d2800009 	mov	x9, #0x0                   	// #0
 19c:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 1a0:	91002021 	add	x1, x1, #0x8
 1a4:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 1a8:	d1000021 	sub	x1, x1, #0x0
 1ac:	05e760c0 	zip1	z0.d, z6.d, z7.d
 1b0:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 1b4:	91004000 	add	x0, x0, #0x10
 1b8:	6466e408 	bfmmla	z8.s, z0.h, z6.h
 1bc:	d1004000 	sub	x0, x0, #0x10
 1c0:	d1002021 	sub	x1, x1, #0x8
 1c4:	91001049 	add	x9, x2, #0x4
 1c8:	05e96906 	uzp1	z6.d, z8.d, z9.d
 1cc:	05e96d07 	uzp2	z7.d, z8.d, z9.d
 1d0:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 1d4:	056668c6 	uzp1	z6.h, z6.h, z6.h
 1d8:	e4a0e446 	st1h	{z6.h}, p1, [x2]
 1dc:	91001042 	add	x2, x2, #0x4
 1e0:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 1e4:	056768e7 	uzp1	z7.h, z7.h, z7.h
 1e8:	e4a0e527 	st1h	{z7.h}, p1, [x9]
 1ec:	91001129 	add	x9, x9, #0x4
 1f0:	91001042 	add	x2, x2, #0x4
 1f4:	d1002042 	sub	x2, x2, #0x8
 1f8:	91001042 	add	x2, x2, #0x4
 1fc:	91004000 	add	x0, x0, #0x10
 200:	d10008c6 	sub	x6, x6, #0x2
 204:	b5fffac6 	cbnz	x6, 0x15c
 208:	91001042 	add	x2, x2, #0x4
 20c:	d1004000 	sub	x0, x0, #0x10
 210:	91004021 	add	x1, x1, #0x10
 214:	d10008e7 	sub	x7, x7, #0x2
 218:	b5fffa07 	cbnz	x7, 0x158
 21c:	910003bf 	mov	sp, x29
 220:	6d4b27e8 	ldp	d8, d9, [sp, #176]
 224:	6d4a2fea 	ldp	d10, d11, [sp, #160]
 228:	6d4937ec 	ldp	d12, d13, [sp, #144]
 22c:	6d483fee 	ldp	d14, d15, [sp, #128]
 230:	a9426ffa 	ldp	x26, x27, [sp, #32]
 234:	a94177fc 	ldp	x28, x29, [sp, #16]
 238:	f94003fe 	ldr	x30, [sp]
 23c:	910303ff 	add	sp, sp, #0xc0
 240:	d65f03c0 	ret
