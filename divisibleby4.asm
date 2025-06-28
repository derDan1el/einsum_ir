
libxsmm_abi1_neov2_bf16_nn_128x128x256_128_256_128_a1_b1_p0_br0_sa0_sb0_uh0_si0_tc-abid_avnni1_bvnni0_cvnni0.mxm:     file format binary


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
  54:	2518e3e0 	ptrue	p0.b
  58:	d2800009 	mov	x9, #0x0                   	// #0
  5c:	252917e1 	whilelt	p1.b, xzr, x9
  60:	d2800109 	mov	x9, #0x8                   	// #8
  64:	252917e2 	whilelt	p2.b, xzr, x9
  68:	d2800fc7 	mov	x7, #0x7e                  	// #126
  6c:	d2801006 	mov	x6, #0x80                  	// #128
  70:	91040049 	add	x9, x2, #0x100
  74:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
  78:	05b238c6 	uunpklo	z6.s, z6.h
  7c:	04709cc6 	lsl	z6.s, z6.s, #16
  80:	91002042 	add	x2, x2, #0x8
  84:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
  88:	05b238e7 	uunpklo	z7.s, z7.h
  8c:	04709ce7 	lsl	z7.s, z7.s, #16
  90:	91002129 	add	x9, x9, #0x8
  94:	05e760c8 	zip1	z8.d, z6.d, z7.d
  98:	05e764c9 	zip2	z9.d, z6.d, z7.d
  9c:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
  a0:	05b238c6 	uunpklo	z6.s, z6.h
  a4:	04709cc6 	lsl	z6.s, z6.s, #16
  a8:	91002042 	add	x2, x2, #0x8
  ac:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
  b0:	05b238e7 	uunpklo	z7.s, z7.h
  b4:	04709ce7 	lsl	z7.s, z7.s, #16
  b8:	91002129 	add	x9, x9, #0x8
  bc:	05e760ca 	zip1	z10.d, z6.d, z7.d
  c0:	05e764cb 	zip2	z11.d, z6.d, z7.d
  c4:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
  c8:	05b238c6 	uunpklo	z6.s, z6.h
  cc:	04709cc6 	lsl	z6.s, z6.s, #16
  d0:	91002042 	add	x2, x2, #0x8
  d4:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
  d8:	05b238e7 	uunpklo	z7.s, z7.h
  dc:	04709ce7 	lsl	z7.s, z7.s, #16
  e0:	91002129 	add	x9, x9, #0x8
  e4:	05e760cc 	zip1	z12.d, z6.d, z7.d
  e8:	05e764cd 	zip2	z13.d, z6.d, z7.d
  ec:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
  f0:	05b238c6 	uunpklo	z6.s, z6.h
  f4:	04709cc6 	lsl	z6.s, z6.s, #16
  f8:	91002042 	add	x2, x2, #0x8
  fc:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 100:	05b238e7 	uunpklo	z7.s, z7.h
 104:	04709ce7 	lsl	z7.s, z7.s, #16
 108:	91002129 	add	x9, x9, #0x8
 10c:	05e760ce 	zip1	z14.d, z6.d, z7.d
 110:	05e764cf 	zip2	z15.d, z6.d, z7.d
 114:	91078042 	add	x2, x2, #0x1e0
 118:	91040049 	add	x9, x2, #0x100
 11c:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 120:	05b238c6 	uunpklo	z6.s, z6.h
 124:	04709cc6 	lsl	z6.s, z6.s, #16
 128:	91002042 	add	x2, x2, #0x8
 12c:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 130:	05b238e7 	uunpklo	z7.s, z7.h
 134:	04709ce7 	lsl	z7.s, z7.s, #16
 138:	91002129 	add	x9, x9, #0x8
 13c:	05e760d0 	zip1	z16.d, z6.d, z7.d
 140:	05e764d1 	zip2	z17.d, z6.d, z7.d
 144:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 148:	05b238c6 	uunpklo	z6.s, z6.h
 14c:	04709cc6 	lsl	z6.s, z6.s, #16
 150:	91002042 	add	x2, x2, #0x8
 154:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 158:	05b238e7 	uunpklo	z7.s, z7.h
 15c:	04709ce7 	lsl	z7.s, z7.s, #16
 160:	91002129 	add	x9, x9, #0x8
 164:	05e760d2 	zip1	z18.d, z6.d, z7.d
 168:	05e764d3 	zip2	z19.d, z6.d, z7.d
 16c:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 170:	05b238c6 	uunpklo	z6.s, z6.h
 174:	04709cc6 	lsl	z6.s, z6.s, #16
 178:	91002042 	add	x2, x2, #0x8
 17c:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 180:	05b238e7 	uunpklo	z7.s, z7.h
 184:	04709ce7 	lsl	z7.s, z7.s, #16
 188:	91002129 	add	x9, x9, #0x8
 18c:	05e760d4 	zip1	z20.d, z6.d, z7.d
 190:	05e764d5 	zip2	z21.d, z6.d, z7.d
 194:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 198:	05b238c6 	uunpklo	z6.s, z6.h
 19c:	04709cc6 	lsl	z6.s, z6.s, #16
 1a0:	91002042 	add	x2, x2, #0x8
 1a4:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 1a8:	05b238e7 	uunpklo	z7.s, z7.h
 1ac:	04709ce7 	lsl	z7.s, z7.s, #16
 1b0:	91002129 	add	x9, x9, #0x8
 1b4:	05e760d6 	zip1	z22.d, z6.d, z7.d
 1b8:	05e764d7 	zip2	z23.d, z6.d, z7.d
 1bc:	91078042 	add	x2, x2, #0x1e0
 1c0:	91040049 	add	x9, x2, #0x100
 1c4:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 1c8:	05b238c6 	uunpklo	z6.s, z6.h
 1cc:	04709cc6 	lsl	z6.s, z6.s, #16
 1d0:	91002042 	add	x2, x2, #0x8
 1d4:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 1d8:	05b238e7 	uunpklo	z7.s, z7.h
 1dc:	04709ce7 	lsl	z7.s, z7.s, #16
 1e0:	91002129 	add	x9, x9, #0x8
 1e4:	05e760d8 	zip1	z24.d, z6.d, z7.d
 1e8:	05e764d9 	zip2	z25.d, z6.d, z7.d
 1ec:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 1f0:	05b238c6 	uunpklo	z6.s, z6.h
 1f4:	04709cc6 	lsl	z6.s, z6.s, #16
 1f8:	91002042 	add	x2, x2, #0x8
 1fc:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 200:	05b238e7 	uunpklo	z7.s, z7.h
 204:	04709ce7 	lsl	z7.s, z7.s, #16
 208:	91002129 	add	x9, x9, #0x8
 20c:	05e760da 	zip1	z26.d, z6.d, z7.d
 210:	05e764db 	zip2	z27.d, z6.d, z7.d
 214:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 218:	05b238c6 	uunpklo	z6.s, z6.h
 21c:	04709cc6 	lsl	z6.s, z6.s, #16
 220:	91002042 	add	x2, x2, #0x8
 224:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 228:	05b238e7 	uunpklo	z7.s, z7.h
 22c:	04709ce7 	lsl	z7.s, z7.s, #16
 230:	91002129 	add	x9, x9, #0x8
 234:	05e760dc 	zip1	z28.d, z6.d, z7.d
 238:	05e764dd 	zip2	z29.d, z6.d, z7.d
 23c:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 240:	05b238c6 	uunpklo	z6.s, z6.h
 244:	04709cc6 	lsl	z6.s, z6.s, #16
 248:	91002042 	add	x2, x2, #0x8
 24c:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 250:	05b238e7 	uunpklo	z7.s, z7.h
 254:	04709ce7 	lsl	z7.s, z7.s, #16
 258:	91002129 	add	x9, x9, #0x8
 25c:	05e760de 	zip1	z30.d, z6.d, z7.d
 260:	05e764df 	zip2	z31.d, z6.d, z7.d
 264:	91078042 	add	x2, x2, #0x1e0
 268:	d1180042 	sub	x2, x2, #0x600
 26c:	d280400b 	mov	x11, #0x200                 	// #512
 270:	d2803e8a 	mov	x10, #0x1f4                 	// #500
 274:	d2803809 	mov	x9, #0x1c0                 	// #448
 278:	d2802008 	mov	x8, #0x100                 	// #256
 27c:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 280:	91080021 	add	x1, x1, #0x200
 284:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 288:	91080021 	add	x1, x1, #0x200
 28c:	05e760c0 	zip1	z0.d, z6.d, z7.d
 290:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 294:	91080021 	add	x1, x1, #0x200
 298:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 29c:	91080021 	add	x1, x1, #0x200
 2a0:	05e760c2 	zip1	z2.d, z6.d, z7.d
 2a4:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 2a8:	91080021 	add	x1, x1, #0x200
 2ac:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 2b0:	d127e021 	sub	x1, x1, #0x9f8
 2b4:	05e760c4 	zip1	z4.d, z6.d, z7.d
 2b8:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 2bc:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 2c0:	91008000 	add	x0, x0, #0x20
 2c4:	6466e408 	bfmmla	z8.s, z0.h, z6.h
 2c8:	6467e409 	bfmmla	z9.s, z0.h, z7.h
 2cc:	6466e450 	bfmmla	z16.s, z2.h, z6.h
 2d0:	6467e451 	bfmmla	z17.s, z2.h, z7.h
 2d4:	6466e498 	bfmmla	z24.s, z4.h, z6.h
 2d8:	6467e499 	bfmmla	z25.s, z4.h, z7.h
 2dc:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 2e0:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 2e4:	91008000 	add	x0, x0, #0x20
 2e8:	6466e40a 	bfmmla	z10.s, z0.h, z6.h
 2ec:	6467e40b 	bfmmla	z11.s, z0.h, z7.h
 2f0:	6466e452 	bfmmla	z18.s, z2.h, z6.h
 2f4:	6467e453 	bfmmla	z19.s, z2.h, z7.h
 2f8:	6466e49a 	bfmmla	z26.s, z4.h, z6.h
 2fc:	6467e49b 	bfmmla	z27.s, z4.h, z7.h
 300:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 304:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 308:	91008000 	add	x0, x0, #0x20
 30c:	6466e40c 	bfmmla	z12.s, z0.h, z6.h
 310:	6467e40d 	bfmmla	z13.s, z0.h, z7.h
 314:	6466e454 	bfmmla	z20.s, z2.h, z6.h
 318:	6467e455 	bfmmla	z21.s, z2.h, z7.h
 31c:	6466e49c 	bfmmla	z28.s, z4.h, z6.h
 320:	6467e49d 	bfmmla	z29.s, z4.h, z7.h
 324:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 328:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 32c:	91008000 	add	x0, x0, #0x20
 330:	6466e40e 	bfmmla	z14.s, z0.h, z6.h
 334:	6467e40f 	bfmmla	z15.s, z0.h, z7.h
 338:	6466e456 	bfmmla	z22.s, z2.h, z6.h
 33c:	6467e457 	bfmmla	z23.s, z2.h, z7.h
 340:	6466e49e 	bfmmla	z30.s, z4.h, z6.h
 344:	6467e49f 	bfmmla	z31.s, z4.h, z7.h
 348:	910e0000 	add	x0, x0, #0x380
 34c:	d1001108 	sub	x8, x8, #0x4
 350:	b5fff968 	cbnz	x8, 0x27c
 354:	d1404000 	sub	x0, x0, #0x10, lsl #12
 358:	d1080021 	sub	x1, x1, #0x200
 35c:	91040049 	add	x9, x2, #0x100
 360:	05e96906 	uzp1	z6.d, z8.d, z9.d
 364:	05e96d07 	uzp2	z7.d, z8.d, z9.d
 368:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 36c:	056668c6 	uzp1	z6.h, z6.h, z6.h
 370:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 374:	91002042 	add	x2, x2, #0x8
 378:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 37c:	056768e7 	uzp1	z7.h, z7.h, z7.h
 380:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 384:	91002129 	add	x9, x9, #0x8
 388:	05eb6946 	uzp1	z6.d, z10.d, z11.d
 38c:	05eb6d47 	uzp2	z7.d, z10.d, z11.d
 390:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 394:	056668c6 	uzp1	z6.h, z6.h, z6.h
 398:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 39c:	91002042 	add	x2, x2, #0x8
 3a0:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 3a4:	056768e7 	uzp1	z7.h, z7.h, z7.h
 3a8:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 3ac:	91002129 	add	x9, x9, #0x8
 3b0:	05ed6986 	uzp1	z6.d, z12.d, z13.d
 3b4:	05ed6d87 	uzp2	z7.d, z12.d, z13.d
 3b8:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 3bc:	056668c6 	uzp1	z6.h, z6.h, z6.h
 3c0:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 3c4:	91002042 	add	x2, x2, #0x8
 3c8:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 3cc:	056768e7 	uzp1	z7.h, z7.h, z7.h
 3d0:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 3d4:	91002129 	add	x9, x9, #0x8
 3d8:	05ef69c6 	uzp1	z6.d, z14.d, z15.d
 3dc:	05ef6dc7 	uzp2	z7.d, z14.d, z15.d
 3e0:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 3e4:	056668c6 	uzp1	z6.h, z6.h, z6.h
 3e8:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 3ec:	91002042 	add	x2, x2, #0x8
 3f0:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 3f4:	056768e7 	uzp1	z7.h, z7.h, z7.h
 3f8:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 3fc:	91002129 	add	x9, x9, #0x8
 400:	91078042 	add	x2, x2, #0x1e0
 404:	91040049 	add	x9, x2, #0x100
 408:	05f16a06 	uzp1	z6.d, z16.d, z17.d
 40c:	05f16e07 	uzp2	z7.d, z16.d, z17.d
 410:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 414:	056668c6 	uzp1	z6.h, z6.h, z6.h
 418:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 41c:	91002042 	add	x2, x2, #0x8
 420:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 424:	056768e7 	uzp1	z7.h, z7.h, z7.h
 428:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 42c:	91002129 	add	x9, x9, #0x8
 430:	05f36a46 	uzp1	z6.d, z18.d, z19.d
 434:	05f36e47 	uzp2	z7.d, z18.d, z19.d
 438:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 43c:	056668c6 	uzp1	z6.h, z6.h, z6.h
 440:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 444:	91002042 	add	x2, x2, #0x8
 448:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 44c:	056768e7 	uzp1	z7.h, z7.h, z7.h
 450:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 454:	91002129 	add	x9, x9, #0x8
 458:	05f56a86 	uzp1	z6.d, z20.d, z21.d
 45c:	05f56e87 	uzp2	z7.d, z20.d, z21.d
 460:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 464:	056668c6 	uzp1	z6.h, z6.h, z6.h
 468:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 46c:	91002042 	add	x2, x2, #0x8
 470:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 474:	056768e7 	uzp1	z7.h, z7.h, z7.h
 478:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 47c:	91002129 	add	x9, x9, #0x8
 480:	05f76ac6 	uzp1	z6.d, z22.d, z23.d
 484:	05f76ec7 	uzp2	z7.d, z22.d, z23.d
 488:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 48c:	056668c6 	uzp1	z6.h, z6.h, z6.h
 490:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 494:	91002042 	add	x2, x2, #0x8
 498:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 49c:	056768e7 	uzp1	z7.h, z7.h, z7.h
 4a0:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 4a4:	91002129 	add	x9, x9, #0x8
 4a8:	91078042 	add	x2, x2, #0x1e0
 4ac:	91040049 	add	x9, x2, #0x100
 4b0:	05f96b06 	uzp1	z6.d, z24.d, z25.d
 4b4:	05f96f07 	uzp2	z7.d, z24.d, z25.d
 4b8:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 4bc:	056668c6 	uzp1	z6.h, z6.h, z6.h
 4c0:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 4c4:	91002042 	add	x2, x2, #0x8
 4c8:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 4cc:	056768e7 	uzp1	z7.h, z7.h, z7.h
 4d0:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 4d4:	91002129 	add	x9, x9, #0x8
 4d8:	05fb6b46 	uzp1	z6.d, z26.d, z27.d
 4dc:	05fb6f47 	uzp2	z7.d, z26.d, z27.d
 4e0:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 4e4:	056668c6 	uzp1	z6.h, z6.h, z6.h
 4e8:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 4ec:	91002042 	add	x2, x2, #0x8
 4f0:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 4f4:	056768e7 	uzp1	z7.h, z7.h, z7.h
 4f8:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 4fc:	91002129 	add	x9, x9, #0x8
 500:	05fd6b86 	uzp1	z6.d, z28.d, z29.d
 504:	05fd6f87 	uzp2	z7.d, z28.d, z29.d
 508:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 50c:	056668c6 	uzp1	z6.h, z6.h, z6.h
 510:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 514:	91002042 	add	x2, x2, #0x8
 518:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 51c:	056768e7 	uzp1	z7.h, z7.h, z7.h
 520:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 524:	91002129 	add	x9, x9, #0x8
 528:	05ff6bc6 	uzp1	z6.d, z30.d, z31.d
 52c:	05ff6fc7 	uzp2	z7.d, z30.d, z31.d
 530:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 534:	056668c6 	uzp1	z6.h, z6.h, z6.h
 538:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 53c:	91002042 	add	x2, x2, #0x8
 540:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 544:	056768e7 	uzp1	z7.h, z7.h, z7.h
 548:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 54c:	91002129 	add	x9, x9, #0x8
 550:	91078042 	add	x2, x2, #0x1e0
 554:	d1180042 	sub	x2, x2, #0x600
 558:	91008042 	add	x2, x2, #0x20
 55c:	91020000 	add	x0, x0, #0x80
 560:	d10040c6 	sub	x6, x6, #0x10
 564:	b5ffd866 	cbnz	x6, 0x70
 568:	91140042 	add	x2, x2, #0x500
 56c:	d1100000 	sub	x0, x0, #0x400
 570:	91300021 	add	x1, x1, #0xc00
 574:	d10018e7 	sub	x7, x7, #0x6
 578:	b5ffd7a7 	cbnz	x7, 0x6c
 57c:	d2800047 	mov	x7, #0x2                   	// #2
 580:	d2801006 	mov	x6, #0x80                  	// #128
 584:	91040049 	add	x9, x2, #0x100
 588:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 58c:	05b238c6 	uunpklo	z6.s, z6.h
 590:	04709cc6 	lsl	z6.s, z6.s, #16
 594:	91002042 	add	x2, x2, #0x8
 598:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 59c:	05b238e7 	uunpklo	z7.s, z7.h
 5a0:	04709ce7 	lsl	z7.s, z7.s, #16
 5a4:	91002129 	add	x9, x9, #0x8
 5a8:	05e760c8 	zip1	z8.d, z6.d, z7.d
 5ac:	05e764c9 	zip2	z9.d, z6.d, z7.d
 5b0:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 5b4:	05b238c6 	uunpklo	z6.s, z6.h
 5b8:	04709cc6 	lsl	z6.s, z6.s, #16
 5bc:	91002042 	add	x2, x2, #0x8
 5c0:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 5c4:	05b238e7 	uunpklo	z7.s, z7.h
 5c8:	04709ce7 	lsl	z7.s, z7.s, #16
 5cc:	91002129 	add	x9, x9, #0x8
 5d0:	05e760ca 	zip1	z10.d, z6.d, z7.d
 5d4:	05e764cb 	zip2	z11.d, z6.d, z7.d
 5d8:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 5dc:	05b238c6 	uunpklo	z6.s, z6.h
 5e0:	04709cc6 	lsl	z6.s, z6.s, #16
 5e4:	91002042 	add	x2, x2, #0x8
 5e8:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 5ec:	05b238e7 	uunpklo	z7.s, z7.h
 5f0:	04709ce7 	lsl	z7.s, z7.s, #16
 5f4:	91002129 	add	x9, x9, #0x8
 5f8:	05e760cc 	zip1	z12.d, z6.d, z7.d
 5fc:	05e764cd 	zip2	z13.d, z6.d, z7.d
 600:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]
 604:	05b238c6 	uunpklo	z6.s, z6.h
 608:	04709cc6 	lsl	z6.s, z6.s, #16
 60c:	91002042 	add	x2, x2, #0x8
 610:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]
 614:	05b238e7 	uunpklo	z7.s, z7.h
 618:	04709ce7 	lsl	z7.s, z7.s, #16
 61c:	91002129 	add	x9, x9, #0x8
 620:	05e760ce 	zip1	z14.d, z6.d, z7.d
 624:	05e764cf 	zip2	z15.d, z6.d, z7.d
 628:	91078042 	add	x2, x2, #0x1e0
 62c:	d1080042 	sub	x2, x2, #0x200
 630:	d280400b 	mov	x11, #0x200                 	// #512
 634:	d2803f8a 	mov	x10, #0x1fc                 	// #508
 638:	d2803809 	mov	x9, #0x1c0                 	// #448
 63c:	d2802008 	mov	x8, #0x100                 	// #256
 640:	85c0e026 	ld1rd	{z6.d}, p0/z, [x1]
 644:	91080021 	add	x1, x1, #0x200
 648:	85c0e027 	ld1rd	{z7.d}, p0/z, [x1]
 64c:	d107e021 	sub	x1, x1, #0x1f8
 650:	05e760c0 	zip1	z0.d, z6.d, z7.d
 654:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 658:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 65c:	91008000 	add	x0, x0, #0x20
 660:	6466e408 	bfmmla	z8.s, z0.h, z6.h
 664:	6467e409 	bfmmla	z9.s, z0.h, z7.h
 668:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 66c:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 670:	91008000 	add	x0, x0, #0x20
 674:	6466e40a 	bfmmla	z10.s, z0.h, z6.h
 678:	6467e40b 	bfmmla	z11.s, z0.h, z7.h
 67c:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 680:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 684:	91008000 	add	x0, x0, #0x20
 688:	6466e40c 	bfmmla	z12.s, z0.h, z6.h
 68c:	6467e40d 	bfmmla	z13.s, z0.h, z7.h
 690:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]
 694:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]
 698:	91008000 	add	x0, x0, #0x20
 69c:	6466e40e 	bfmmla	z14.s, z0.h, z6.h
 6a0:	6467e40f 	bfmmla	z15.s, z0.h, z7.h
 6a4:	910e0000 	add	x0, x0, #0x380
 6a8:	d1001108 	sub	x8, x8, #0x4
 6ac:	b5fffca8 	cbnz	x8, 0x640
 6b0:	d1404000 	sub	x0, x0, #0x10, lsl #12
 6b4:	d1080021 	sub	x1, x1, #0x200
 6b8:	91040049 	add	x9, x2, #0x100
 6bc:	05e96906 	uzp1	z6.d, z8.d, z9.d
 6c0:	05e96d07 	uzp2	z7.d, z8.d, z9.d
 6c4:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 6c8:	056668c6 	uzp1	z6.h, z6.h, z6.h
 6cc:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 6d0:	91002042 	add	x2, x2, #0x8
 6d4:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 6d8:	056768e7 	uzp1	z7.h, z7.h, z7.h
 6dc:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 6e0:	91002129 	add	x9, x9, #0x8
 6e4:	05eb6946 	uzp1	z6.d, z10.d, z11.d
 6e8:	05eb6d47 	uzp2	z7.d, z10.d, z11.d
 6ec:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 6f0:	056668c6 	uzp1	z6.h, z6.h, z6.h
 6f4:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 6f8:	91002042 	add	x2, x2, #0x8
 6fc:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 700:	056768e7 	uzp1	z7.h, z7.h, z7.h
 704:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 708:	91002129 	add	x9, x9, #0x8
 70c:	05ed6986 	uzp1	z6.d, z12.d, z13.d
 710:	05ed6d87 	uzp2	z7.d, z12.d, z13.d
 714:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 718:	056668c6 	uzp1	z6.h, z6.h, z6.h
 71c:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 720:	91002042 	add	x2, x2, #0x8
 724:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 728:	056768e7 	uzp1	z7.h, z7.h, z7.h
 72c:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 730:	91002129 	add	x9, x9, #0x8
 734:	05ef69c6 	uzp1	z6.d, z14.d, z15.d
 738:	05ef6dc7 	uzp2	z7.d, z14.d, z15.d
 73c:	658aa0c6 	bfcvt	z6.h, p0/m, z6.s
 740:	056668c6 	uzp1	z6.h, z6.h, z6.h
 744:	e4a0e846 	st1h	{z6.h}, p2, [x2]
 748:	91002042 	add	x2, x2, #0x8
 74c:	658aa0e7 	bfcvt	z7.h, p0/m, z7.s
 750:	056768e7 	uzp1	z7.h, z7.h, z7.h
 754:	e4a0e927 	st1h	{z7.h}, p2, [x9]
 758:	91002129 	add	x9, x9, #0x8
 75c:	91078042 	add	x2, x2, #0x1e0
 760:	d1080042 	sub	x2, x2, #0x200
 764:	91008042 	add	x2, x2, #0x20
 768:	91020000 	add	x0, x0, #0x80
 76c:	d10040c6 	sub	x6, x6, #0x10
 770:	b5fff0a6 	cbnz	x6, 0x584
 774:	91040042 	add	x2, x2, #0x100
 778:	d1100000 	sub	x0, x0, #0x400
 77c:	91100021 	add	x1, x1, #0x400
 780:	d10008e7 	sub	x7, x7, #0x2
 784:	b5ffefe7 	cbnz	x7, 0x580
 788:	910003bf 	mov	sp, x29
 78c:	6d4b27e8 	ldp	d8, d9, [sp, #176]
 790:	6d4a2fea 	ldp	d10, d11, [sp, #160]
 794:	6d4937ec 	ldp	d12, d13, [sp, #144]
 798:	6d483fee 	ldp	d14, d15, [sp, #128]
 79c:	a9426ffa 	ldp	x26, x27, [sp, #32]
 7a0:	a94177fc 	ldp	x28, x29, [sp, #16]
 7a4:	f94003fe 	ldr	x30, [sp]
 7a8:	910303ff 	add	sp, sp, #0xc0
 7ac:	d65f03c0 	ret
