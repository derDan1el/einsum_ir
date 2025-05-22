
libxsmm_abi1_neov2_bf16f32_nn_12x12x12_12_12_12_a1_b1_p0_br0_sa0_sb0_uh0_si0_tc-abid_avnni0_bvnni0_cvnni0.mxm:     file format binary


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
  58:	d105014a 	sub	x10, x10, #0x140
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
  bc:	d280018b 	mov	x11, #0xc                   	// #12
  c0:	d280018a 	mov	x10, #0xc                   	// #12
  c4:	7d400100 	ldr	h0, [x8]
  c8:	7c002520 	str	h0, [x9], #2
  cc:	91006108 	add	x8, x8, #0x18
  d0:	7d400100 	ldr	h0, [x8]
  d4:	7c002520 	str	h0, [x9], #2
  d8:	91006108 	add	x8, x8, #0x18
  dc:	7d400100 	ldr	h0, [x8]
  e0:	7c002520 	str	h0, [x9], #2
  e4:	91006108 	add	x8, x8, #0x18
  e8:	7d400100 	ldr	h0, [x8]
  ec:	7c002520 	str	h0, [x9], #2
  f0:	d1011908 	sub	x8, x8, #0x46
  f4:	d100054a 	sub	x10, x10, #0x1
  f8:	b5fffe6a 	cbnz	x10, 0xc4
  fc:	91012108 	add	x8, x8, #0x48
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
 13c:	d2800009 	mov	x9, #0x0                   	// #0
 140:	252917e1 	whilelt	p1.b, xzr, x9
 144:	d2800187 	mov	x7, #0xc                   	// #12
 148:	d2800186 	mov	x6, #0xc                   	// #12
 14c:	9100c049 	add	x9, x2, #0x30
 150:	85804046 	ldr	z6, [x2]
 154:	91004042 	add	x2, x2, #0x10
 158:	85804127 	ldr	z7, [x9]
 15c:	91004129 	add	x9, x9, #0x10
 160:	05e760c8 	zip1	z8.d, z6.d, z7.d
 164:	05e764c9 	zip2	z9.d, z6.d, z7.d
 168:	85804046 	ldr	z6, [x2]
 16c:	91004042 	add	x2, x2, #0x10
 170:	85804127 	ldr	z7, [x9]
 174:	91004129 	add	x9, x9, #0x10
 178:	05e760ca 	zip1	z10.d, z6.d, z7.d
 17c:	05e764cb 	zip2	z11.d, z6.d, z7.d
 180:	85804046 	ldr	z6, [x2]
 184:	91004042 	add	x2, x2, #0x10
 188:	85804127 	ldr	z7, [x9]
 18c:	91004129 	add	x9, x9, #0x10
 190:	05e760cc 	zip1	z12.d, z6.d, z7.d
 194:	05e764cd 	zip2	z13.d, z6.d, z7.d
 198:	9100c042 	add	x2, x2, #0x30
 19c:	9100c049 	add	x9, x2, #0x30
 1a0:	85804046 	ldr	z6, [x2]
 1a4:	91004042 	add	x2, x2, #0x10
 1a8:	85804127 	ldr	z7, [x9]
 1ac:	91004129 	add	x9, x9, #0x10
 1b0:	05e760d0 	zip1	z16.d, z6.d, z7.d
 1b4:	05e764d1 	zip2	z17.d, z6.d, z7.d
 1b8:	85804046 	ldr	z6, [x2]
 1bc:	91004042 	add	x2, x2, #0x10
 1c0:	85804127 	ldr	z7, [x9]
 1c4:	91004129 	add	x9, x9, #0x10
 1c8:	05e760d2 	zip1	z18.d, z6.d, z7.d
 1cc:	05e764d3 	zip2	z19.d, z6.d, z7.d
 1d0:	85804046 	ldr	z6, [x2]
 1d4:	91004042 	add	x2, x2, #0x10
 1d8:	85804127 	ldr	z7, [x9]
 1dc:	91004129 	add	x9, x9, #0x10
 1e0:	05e760d4 	zip1	z20.d, z6.d, z7.d
 1e4:	05e764d5 	zip2	z21.d, z6.d, z7.d
 1e8:	9100c042 	add	x2, x2, #0x30
 1ec:	9100c049 	add	x9, x2, #0x30
 1f0:	85804046 	ldr	z6, [x2]
 1f4:	91004042 	add	x2, x2, #0x10
 1f8:	85804127 	ldr	z7, [x9]
 1fc:	91004129 	add	x9, x9, #0x10
 200:	05e760d8 	zip1	z24.d, z6.d, z7.d
 204:	05e764d9 	zip2	z25.d, z6.d, z7.d
 208:	85804046 	ldr	z6, [x2]
 20c:	91004042 	add	x2, x2, #0x10
 210:	85804127 	ldr	z7, [x9]
 214:	91004129 	add	x9, x9, #0x10
 218:	05e760da 	zip1	z26.d, z6.d, z7.d
 21c:	05e764db 	zip2	z27.d, z6.d, z7.d
 220:	85804046 	ldr	z6, [x2]
 224:	91004042 	add	x2, x2, #0x10
 228:	85804127 	ldr	z7, [x9]
 22c:	91004129 	add	x9, x9, #0x10
 230:	05e760dc 	zip1	z28.d, z6.d, z7.d
 234:	05e764dd 	zip2	z29.d, z6.d, z7.d
 238:	9100c042 	add	x2, x2, #0x30
 23c:	d1048042 	sub	x2, x2, #0x120
 240:	d280030b 	mov	x11, #0x18                  	// #24
 244:	d280018a 	mov	x10, #0xc                   	// #12
 248:	d2800009 	mov	x9, #0x0                   	// #0
 24c:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 250:	91006021 	add	x1, x1, #0x18
 254:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 258:	91006021 	add	x1, x1, #0x18
 25c:	05e760c0 	zip1	z0.d, z6.d, z7.d
 260:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 264:	91006021 	add	x1, x1, #0x18
 268:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 26c:	91006021 	add	x1, x1, #0x18
 270:	05e760c2 	zip1	z2.d, z6.d, z7.d
 274:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 278:	91006021 	add	x1, x1, #0x18
 27c:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 280:	d101c021 	sub	x1, x1, #0x70
 284:	05e760c4 	zip1	z4.d, z6.d, z7.d
 288:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 28c:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 290:	91008000 	add	x0, x0, #0x20
 294:	6466e408 	bfmmla	z8.s, z0.h, z6.h
 298:	6467e409 	bfmmla	z9.s, z0.h, z7.h
 29c:	6466e450 	bfmmla	z16.s, z2.h, z6.h
 2a0:	6467e451 	bfmmla	z17.s, z2.h, z7.h
 2a4:	6466e498 	bfmmla	z24.s, z4.h, z6.h
 2a8:	6467e499 	bfmmla	z25.s, z4.h, z7.h
 2ac:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 2b0:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 2b4:	91008000 	add	x0, x0, #0x20
 2b8:	6466e40a 	bfmmla	z10.s, z0.h, z6.h
 2bc:	6467e40b 	bfmmla	z11.s, z0.h, z7.h
 2c0:	6466e452 	bfmmla	z18.s, z2.h, z6.h
 2c4:	6467e453 	bfmmla	z19.s, z2.h, z7.h
 2c8:	6466e49a 	bfmmla	z26.s, z4.h, z6.h
 2cc:	6467e49b 	bfmmla	z27.s, z4.h, z7.h
 2d0:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 2d4:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 2d8:	91008000 	add	x0, x0, #0x20
 2dc:	6466e40c 	bfmmla	z12.s, z0.h, z6.h
 2e0:	6467e40d 	bfmmla	z13.s, z0.h, z7.h
 2e4:	6466e454 	bfmmla	z20.s, z2.h, z6.h
 2e8:	6467e455 	bfmmla	z21.s, z2.h, z7.h
 2ec:	6466e49c 	bfmmla	z28.s, z4.h, z6.h
 2f0:	6467e49d 	bfmmla	z29.s, z4.h, z7.h
 2f4:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 2f8:	91006021 	add	x1, x1, #0x18
 2fc:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 300:	91006021 	add	x1, x1, #0x18
 304:	05e760c0 	zip1	z0.d, z6.d, z7.d
 308:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 30c:	91006021 	add	x1, x1, #0x18
 310:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 314:	91006021 	add	x1, x1, #0x18
 318:	05e760c2 	zip1	z2.d, z6.d, z7.d
 31c:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 320:	91006021 	add	x1, x1, #0x18
 324:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 328:	d101c021 	sub	x1, x1, #0x70
 32c:	05e760c4 	zip1	z4.d, z6.d, z7.d
 330:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 334:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 338:	91008000 	add	x0, x0, #0x20
 33c:	6466e408 	bfmmla	z8.s, z0.h, z6.h
 340:	6467e409 	bfmmla	z9.s, z0.h, z7.h
 344:	6466e450 	bfmmla	z16.s, z2.h, z6.h
 348:	6467e451 	bfmmla	z17.s, z2.h, z7.h
 34c:	6466e498 	bfmmla	z24.s, z4.h, z6.h
 350:	6467e499 	bfmmla	z25.s, z4.h, z7.h
 354:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 358:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 35c:	91008000 	add	x0, x0, #0x20
 360:	6466e40a 	bfmmla	z10.s, z0.h, z6.h
 364:	6467e40b 	bfmmla	z11.s, z0.h, z7.h
 368:	6466e452 	bfmmla	z18.s, z2.h, z6.h
 36c:	6467e453 	bfmmla	z19.s, z2.h, z7.h
 370:	6466e49a 	bfmmla	z26.s, z4.h, z6.h
 374:	6467e49b 	bfmmla	z27.s, z4.h, z7.h
 378:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 37c:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 380:	91008000 	add	x0, x0, #0x20
 384:	6466e40c 	bfmmla	z12.s, z0.h, z6.h
 388:	6467e40d 	bfmmla	z13.s, z0.h, z7.h
 38c:	6466e454 	bfmmla	z20.s, z2.h, z6.h
 390:	6467e455 	bfmmla	z21.s, z2.h, z7.h
 394:	6466e49c 	bfmmla	z28.s, z4.h, z6.h
 398:	6467e49d 	bfmmla	z29.s, z4.h, z7.h
 39c:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 3a0:	91006021 	add	x1, x1, #0x18
 3a4:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 3a8:	91006021 	add	x1, x1, #0x18
 3ac:	05e760c0 	zip1	z0.d, z6.d, z7.d
 3b0:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 3b4:	91006021 	add	x1, x1, #0x18
 3b8:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 3bc:	91006021 	add	x1, x1, #0x18
 3c0:	05e760c2 	zip1	z2.d, z6.d, z7.d
 3c4:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 3c8:	91006021 	add	x1, x1, #0x18
 3cc:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 3d0:	d101c021 	sub	x1, x1, #0x70
 3d4:	05e760c4 	zip1	z4.d, z6.d, z7.d
 3d8:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 3dc:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 3e0:	91008000 	add	x0, x0, #0x20
 3e4:	6466e408 	bfmmla	z8.s, z0.h, z6.h
 3e8:	6467e409 	bfmmla	z9.s, z0.h, z7.h
 3ec:	6466e450 	bfmmla	z16.s, z2.h, z6.h
 3f0:	6467e451 	bfmmla	z17.s, z2.h, z7.h
 3f4:	6466e498 	bfmmla	z24.s, z4.h, z6.h
 3f8:	6467e499 	bfmmla	z25.s, z4.h, z7.h
 3fc:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 400:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 404:	91008000 	add	x0, x0, #0x20
 408:	6466e40a 	bfmmla	z10.s, z0.h, z6.h
 40c:	6467e40b 	bfmmla	z11.s, z0.h, z7.h
 410:	6466e452 	bfmmla	z18.s, z2.h, z6.h
 414:	6467e453 	bfmmla	z19.s, z2.h, z7.h
 418:	6466e49a 	bfmmla	z26.s, z4.h, z6.h
 41c:	6467e49b 	bfmmla	z27.s, z4.h, z7.h
 420:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 424:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 428:	91008000 	add	x0, x0, #0x20
 42c:	6466e40c 	bfmmla	z12.s, z0.h, z6.h
 430:	6467e40d 	bfmmla	z13.s, z0.h, z7.h
 434:	6466e454 	bfmmla	z20.s, z2.h, z6.h
 438:	6467e455 	bfmmla	z21.s, z2.h, z7.h
 43c:	6466e49c 	bfmmla	z28.s, z4.h, z6.h
 440:	6467e49d 	bfmmla	z29.s, z4.h, z7.h
 444:	d1048000 	sub	x0, x0, #0x120
 448:	d1006021 	sub	x1, x1, #0x18
 44c:	9100c049 	add	x9, x2, #0x30
 450:	05e96906 	uzp1	z6.d, z8.d, z9.d
 454:	05e96d07 	uzp2	z7.d, z8.d, z9.d
 458:	e5804046 	str	z6, [x2]
 45c:	91004042 	add	x2, x2, #0x10
 460:	e5804127 	str	z7, [x9]
 464:	91004129 	add	x9, x9, #0x10
 468:	05eb6946 	uzp1	z6.d, z10.d, z11.d
 46c:	05eb6d47 	uzp2	z7.d, z10.d, z11.d
 470:	e5804046 	str	z6, [x2]
 474:	91004042 	add	x2, x2, #0x10
 478:	e5804127 	str	z7, [x9]
 47c:	91004129 	add	x9, x9, #0x10
 480:	05ed6986 	uzp1	z6.d, z12.d, z13.d
 484:	05ed6d87 	uzp2	z7.d, z12.d, z13.d
 488:	e5804046 	str	z6, [x2]
 48c:	91004042 	add	x2, x2, #0x10
 490:	e5804127 	str	z7, [x9]
 494:	91004129 	add	x9, x9, #0x10
 498:	9100c042 	add	x2, x2, #0x30
 49c:	9100c049 	add	x9, x2, #0x30
 4a0:	05f16a06 	uzp1	z6.d, z16.d, z17.d
 4a4:	05f16e07 	uzp2	z7.d, z16.d, z17.d
 4a8:	e5804046 	str	z6, [x2]
 4ac:	91004042 	add	x2, x2, #0x10
 4b0:	e5804127 	str	z7, [x9]
 4b4:	91004129 	add	x9, x9, #0x10
 4b8:	05f36a46 	uzp1	z6.d, z18.d, z19.d
 4bc:	05f36e47 	uzp2	z7.d, z18.d, z19.d
 4c0:	e5804046 	str	z6, [x2]
 4c4:	91004042 	add	x2, x2, #0x10
 4c8:	e5804127 	str	z7, [x9]
 4cc:	91004129 	add	x9, x9, #0x10
 4d0:	05f56a86 	uzp1	z6.d, z20.d, z21.d
 4d4:	05f56e87 	uzp2	z7.d, z20.d, z21.d
 4d8:	e5804046 	str	z6, [x2]
 4dc:	91004042 	add	x2, x2, #0x10
 4e0:	e5804127 	str	z7, [x9]
 4e4:	91004129 	add	x9, x9, #0x10
 4e8:	9100c042 	add	x2, x2, #0x30
 4ec:	9100c049 	add	x9, x2, #0x30
 4f0:	05f96b06 	uzp1	z6.d, z24.d, z25.d
 4f4:	05f96f07 	uzp2	z7.d, z24.d, z25.d
 4f8:	e5804046 	str	z6, [x2]
 4fc:	91004042 	add	x2, x2, #0x10
 500:	e5804127 	str	z7, [x9]
 504:	91004129 	add	x9, x9, #0x10
 508:	05fb6b46 	uzp1	z6.d, z26.d, z27.d
 50c:	05fb6f47 	uzp2	z7.d, z26.d, z27.d
 510:	e5804046 	str	z6, [x2]
 514:	91004042 	add	x2, x2, #0x10
 518:	e5804127 	str	z7, [x9]
 51c:	91004129 	add	x9, x9, #0x10
 520:	05fd6b86 	uzp1	z6.d, z28.d, z29.d
 524:	05fd6f87 	uzp2	z7.d, z28.d, z29.d
 528:	e5804046 	str	z6, [x2]
 52c:	91004042 	add	x2, x2, #0x10
 530:	e5804127 	str	z7, [x9]
 534:	91004129 	add	x9, x9, #0x10
 538:	9100c042 	add	x2, x2, #0x30
 53c:	d1048042 	sub	x2, x2, #0x120
 540:	9100c042 	add	x2, x2, #0x30
 544:	91018000 	add	x0, x0, #0x60
 548:	d10030c6 	sub	x6, x6, #0xc
 54c:	b5ffe006 	cbnz	x6, 0x14c
 550:	9103c042 	add	x2, x2, #0xf0
 554:	d1018000 	sub	x0, x0, #0x60
 558:	91024021 	add	x1, x1, #0x90
 55c:	d10018e7 	sub	x7, x7, #0x6
 560:	b5ffdf47 	cbnz	x7, 0x148
 564:	910003bf 	mov	sp, x29
 568:	6d4b27e8 	ldp	d8, d9, [sp, #176]
 56c:	6d4a2fea 	ldp	d10, d11, [sp, #160]
 570:	6d4937ec 	ldp	d12, d13, [sp, #144]
 574:	6d483fee 	ldp	d14, d15, [sp, #128]
 578:	a9426ffa 	ldp	x26, x27, [sp, #32]
 57c:	a94177fc 	ldp	x28, x29, [sp, #16]
 580:	f94003fe 	ldr	x30, [sp]
 584:	910303ff 	add	sp, sp, #0xc0
 588:	d65f03c0 	ret
