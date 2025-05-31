
libxsmm_abi1_neov2_bf16_nn_5x5x5_5_5_5_a1_b1_p0_br0_sa0_sb0_uh0_si0_tc-abid_avnni0_bvnni0_cvnni0.mxm:     file format binary


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
  54:	d2800149 	mov	x9, #0xa                   	// #10
  58:	252917e3 	whilelt	p3.b, xzr, x9
  5c:	2518e3e0 	ptrue	p0.b
  60:	d2800089 	mov	x9, #0x4                   	// #4
  64:	252917e1 	whilelt	p1.b, xzr, x9
  68:	d2800109 	mov	x9, #0x8                   	// #8
  6c:	252917e2 	whilelt	p2.b, xzr, x9
  70:	d2800049 	mov	x9, #0x2                   	// #2
  74:	252917e4 	whilelt	p4.b, xzr, x9
  78:	d28000a7 	mov	x7, #0x5                   	// #5
  7c:	d28000a6 	mov	x6, #0x5                   	// #5
  80:	a4a0a856 	ld1h	{z22.h}, p2/z, [x2]
  84:	05b23ad6 	uunpklo	z22.s, z22.h
  88:	04709ed6 	lsl	z22.s, z22.s, #16
  8c:	91002042 	add	x2, x2, #0x8
  90:	a4a0b057 	ld1h	{z23.h}, p4/z, [x2]
  94:	05b23af7 	uunpklo	z23.s, z23.h
  98:	04709ef7 	lsl	z23.s, z23.s, #16
  9c:	91000842 	add	x2, x2, #0x2
  a0:	a4a0a858 	ld1h	{z24.h}, p2/z, [x2]
  a4:	05b23b18 	uunpklo	z24.s, z24.h
  a8:	04709f18 	lsl	z24.s, z24.s, #16
  ac:	91002042 	add	x2, x2, #0x8
  b0:	a4a0b059 	ld1h	{z25.h}, p4/z, [x2]
  b4:	05b23b39 	uunpklo	z25.s, z25.h
  b8:	04709f39 	lsl	z25.s, z25.s, #16
  bc:	91000842 	add	x2, x2, #0x2
  c0:	a4a0a85a 	ld1h	{z26.h}, p2/z, [x2]
  c4:	05b23b5a 	uunpklo	z26.s, z26.h
  c8:	04709f5a 	lsl	z26.s, z26.s, #16
  cc:	91002042 	add	x2, x2, #0x8
  d0:	a4a0b05b 	ld1h	{z27.h}, p4/z, [x2]
  d4:	05b23b7b 	uunpklo	z27.s, z27.h
  d8:	04709f7b 	lsl	z27.s, z27.s, #16
  dc:	91000842 	add	x2, x2, #0x2
  e0:	a4a0a85c 	ld1h	{z28.h}, p2/z, [x2]
  e4:	05b23b9c 	uunpklo	z28.s, z28.h
  e8:	04709f9c 	lsl	z28.s, z28.s, #16
  ec:	91002042 	add	x2, x2, #0x8
  f0:	a4a0b05d 	ld1h	{z29.h}, p4/z, [x2]
  f4:	05b23bbd 	uunpklo	z29.s, z29.h
  f8:	04709fbd 	lsl	z29.s, z29.s, #16
  fc:	91000842 	add	x2, x2, #0x2
 100:	a4a0a85e 	ld1h	{z30.h}, p2/z, [x2]
 104:	05b23bde 	uunpklo	z30.s, z30.h
 108:	04709fde 	lsl	z30.s, z30.s, #16
 10c:	91002042 	add	x2, x2, #0x8
 110:	a4a0b05f 	ld1h	{z31.h}, p4/z, [x2]
 114:	05b23bff 	uunpklo	z31.s, z31.h
 118:	04709fff 	lsl	z31.s, z31.s, #16
 11c:	d100c042 	sub	x2, x2, #0x30
 120:	d280014b 	mov	x11, #0xa                   	// #10
 124:	d280000a 	mov	x10, #0x0                   	// #0
 128:	d2800009 	mov	x9, #0x0                   	// #0
 12c:	04a33063 	eor	z3.d, z3.d, z3.d
 130:	a4a0ac00 	ld1h	{z0.h}, p3/z, [x0]
 134:	05636001 	zip1	z1.h, z0.h, z3.h
 138:	05636402 	zip2	z2.h, z0.h, z3.h
 13c:	91002800 	add	x0, x0, #0xa
 140:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 144:	91002821 	add	x1, x1, #0xa
 148:	64608036 	bfdot	z22.s, z1.h, z0.h
 14c:	64608057 	bfdot	z23.s, z2.h, z0.h
 150:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 154:	91002821 	add	x1, x1, #0xa
 158:	64608038 	bfdot	z24.s, z1.h, z0.h
 15c:	64608059 	bfdot	z25.s, z2.h, z0.h
 160:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 164:	91002821 	add	x1, x1, #0xa
 168:	6460803a 	bfdot	z26.s, z1.h, z0.h
 16c:	6460805b 	bfdot	z27.s, z2.h, z0.h
 170:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 174:	91002821 	add	x1, x1, #0xa
 178:	6460803c 	bfdot	z28.s, z1.h, z0.h
 17c:	6460805d 	bfdot	z29.s, z2.h, z0.h
 180:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 184:	d1009821 	sub	x1, x1, #0x26
 188:	6460803e 	bfdot	z30.s, z1.h, z0.h
 18c:	6460805f 	bfdot	z31.s, z2.h, z0.h
 190:	8b090000 	add	x0, x0, x9
 194:	04a33063 	eor	z3.d, z3.d, z3.d
 198:	a4a0ac00 	ld1h	{z0.h}, p3/z, [x0]
 19c:	05636001 	zip1	z1.h, z0.h, z3.h
 1a0:	05636402 	zip2	z2.h, z0.h, z3.h
 1a4:	91002800 	add	x0, x0, #0xa
 1a8:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 1ac:	91002821 	add	x1, x1, #0xa
 1b0:	64608036 	bfdot	z22.s, z1.h, z0.h
 1b4:	64608057 	bfdot	z23.s, z2.h, z0.h
 1b8:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 1bc:	91002821 	add	x1, x1, #0xa
 1c0:	64608038 	bfdot	z24.s, z1.h, z0.h
 1c4:	64608059 	bfdot	z25.s, z2.h, z0.h
 1c8:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 1cc:	91002821 	add	x1, x1, #0xa
 1d0:	6460803a 	bfdot	z26.s, z1.h, z0.h
 1d4:	6460805b 	bfdot	z27.s, z2.h, z0.h
 1d8:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 1dc:	91002821 	add	x1, x1, #0xa
 1e0:	6460803c 	bfdot	z28.s, z1.h, z0.h
 1e4:	6460805d 	bfdot	z29.s, z2.h, z0.h
 1e8:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 1ec:	d1009821 	sub	x1, x1, #0x26
 1f0:	6460803e 	bfdot	z30.s, z1.h, z0.h
 1f4:	6460805f 	bfdot	z31.s, z2.h, z0.h
 1f8:	8b090000 	add	x0, x0, x9
 1fc:	04a33063 	eor	z3.d, z3.d, z3.d
 200:	a4a0ac00 	ld1h	{z0.h}, p3/z, [x0]
 204:	05636001 	zip1	z1.h, z0.h, z3.h
 208:	05636402 	zip2	z2.h, z0.h, z3.h
 20c:	91002800 	add	x0, x0, #0xa
 210:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 214:	91002821 	add	x1, x1, #0xa
 218:	64608036 	bfdot	z22.s, z1.h, z0.h
 21c:	64608057 	bfdot	z23.s, z2.h, z0.h
 220:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 224:	91002821 	add	x1, x1, #0xa
 228:	64608038 	bfdot	z24.s, z1.h, z0.h
 22c:	64608059 	bfdot	z25.s, z2.h, z0.h
 230:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 234:	91002821 	add	x1, x1, #0xa
 238:	6460803a 	bfdot	z26.s, z1.h, z0.h
 23c:	6460805b 	bfdot	z27.s, z2.h, z0.h
 240:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 244:	91002821 	add	x1, x1, #0xa
 248:	6460803c 	bfdot	z28.s, z1.h, z0.h
 24c:	6460805d 	bfdot	z29.s, z2.h, z0.h
 250:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
 254:	d1009821 	sub	x1, x1, #0x26
 258:	6460803e 	bfdot	z30.s, z1.h, z0.h
 25c:	6460805f 	bfdot	z31.s, z2.h, z0.h
 260:	8b090000 	add	x0, x0, x9
 264:	d100c800 	sub	x0, x0, #0x32
 268:	d1002821 	sub	x1, x1, #0xa
 26c:	658aa2d6 	bfcvt	z22.h, p0/m, z22.s
 270:	05766ad6 	uzp1	z22.h, z22.h, z22.h
 274:	e4a0e856 	st1h	{z22.h}, p2, [x2]
 278:	91002042 	add	x2, x2, #0x8
 27c:	658aa2f7 	bfcvt	z23.h, p0/m, z23.s
 280:	05776af7 	uzp1	z23.h, z23.h, z23.h
 284:	e4a0f057 	st1h	{z23.h}, p4, [x2]
 288:	91000842 	add	x2, x2, #0x2
 28c:	658aa318 	bfcvt	z24.h, p0/m, z24.s
 290:	05786b18 	uzp1	z24.h, z24.h, z24.h
 294:	e4a0e858 	st1h	{z24.h}, p2, [x2]
 298:	91002042 	add	x2, x2, #0x8
 29c:	658aa339 	bfcvt	z25.h, p0/m, z25.s
 2a0:	05796b39 	uzp1	z25.h, z25.h, z25.h
 2a4:	e4a0f059 	st1h	{z25.h}, p4, [x2]
 2a8:	91000842 	add	x2, x2, #0x2
 2ac:	658aa35a 	bfcvt	z26.h, p0/m, z26.s
 2b0:	057a6b5a 	uzp1	z26.h, z26.h, z26.h
 2b4:	e4a0e85a 	st1h	{z26.h}, p2, [x2]
 2b8:	91002042 	add	x2, x2, #0x8
 2bc:	658aa37b 	bfcvt	z27.h, p0/m, z27.s
 2c0:	057b6b7b 	uzp1	z27.h, z27.h, z27.h
 2c4:	e4a0f05b 	st1h	{z27.h}, p4, [x2]
 2c8:	91000842 	add	x2, x2, #0x2
 2cc:	658aa39c 	bfcvt	z28.h, p0/m, z28.s
 2d0:	057c6b9c 	uzp1	z28.h, z28.h, z28.h
 2d4:	e4a0e85c 	st1h	{z28.h}, p2, [x2]
 2d8:	91002042 	add	x2, x2, #0x8
 2dc:	658aa3bd 	bfcvt	z29.h, p0/m, z29.s
 2e0:	057d6bbd 	uzp1	z29.h, z29.h, z29.h
 2e4:	e4a0f05d 	st1h	{z29.h}, p4, [x2]
 2e8:	91000842 	add	x2, x2, #0x2
 2ec:	658aa3de 	bfcvt	z30.h, p0/m, z30.s
 2f0:	057e6bde 	uzp1	z30.h, z30.h, z30.h
 2f4:	e4a0e85e 	st1h	{z30.h}, p2, [x2]
 2f8:	91002042 	add	x2, x2, #0x8
 2fc:	658aa3ff 	bfcvt	z31.h, p0/m, z31.s
 300:	057f6bff 	uzp1	z31.h, z31.h, z31.h
 304:	e4a0f05f 	st1h	{z31.h}, p4, [x2]
 308:	d100c042 	sub	x2, x2, #0x30
 30c:	91002842 	add	x2, x2, #0xa
 310:	91002800 	add	x0, x0, #0xa
 314:	d10014c6 	sub	x6, x6, #0x5
 318:	b5ffeb46 	cbnz	x6, 0x80
 31c:	9100a042 	add	x2, x2, #0x28
 320:	d1002800 	sub	x0, x0, #0xa
 324:	9100c821 	add	x1, x1, #0x32
 328:	d10014e7 	sub	x7, x7, #0x5
 32c:	b5ffea87 	cbnz	x7, 0x7c
 330:	910003bf 	mov	sp, x29
 334:	6d4b27e8 	ldp	d8, d9, [sp, #176]
 338:	6d4a2fea 	ldp	d10, d11, [sp, #160]
 33c:	6d4937ec 	ldp	d12, d13, [sp, #144]
 340:	6d483fee 	ldp	d14, d15, [sp, #128]
 344:	a9426ffa 	ldp	x26, x27, [sp, #32]
 348:	a94177fc 	ldp	x28, x29, [sp, #16]
 34c:	f94003fe 	ldr	x30, [sp]
 350:	910303ff 	add	sp, sp, #0xc0
 354:	d65f03c0 	ret
