
libxsmm_abi1_neov2_bf16_nn_128x128x258_128_258_128_a1_b1_p0_br0_sa0_sb0_uh0_si0_tc-abid_avnni0_bvnni0_cvnni0.mxm:     file format binary


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
  54:	d2800009 	mov	x9, #0x0                   	// #0
  58:	252917e3 	whilelt	p3.b, xzr, x9
  5c:	2518e3e0 	ptrue	p0.b
  60:	d2800009 	mov	x9, #0x0                   	// #0
  64:	252917e1 	whilelt	p1.b, xzr, x9
  68:	d2800109 	mov	x9, #0x8                   	// #8
  6c:	252917e2 	whilelt	p2.b, xzr, x9
  70:	d2800009 	mov	x9, #0x0                   	// #0
  74:	252917e4 	whilelt	p4.b, xzr, x9
  78:	d2800fc7 	mov	x7, #0x7e                  	// #126
  7c:	d2801006 	mov	x6, #0x80                  	// #128
  80:	a4a0a848 	ld1h	{z8.h}, p2/z, [x2]
  84:	05b23908 	uunpklo	z8.s, z8.h
  88:	04709d08 	lsl	z8.s, z8.s, #16
  8c:	91002042 	add	x2, x2, #0x8
  90:	a4a0a849 	ld1h	{z9.h}, p2/z, [x2]
  94:	05b23929 	uunpklo	z9.s, z9.h
  98:	04709d29 	lsl	z9.s, z9.s, #16
  9c:	91002042 	add	x2, x2, #0x8
  a0:	a4a0a84a 	ld1h	{z10.h}, p2/z, [x2]
  a4:	05b2394a 	uunpklo	z10.s, z10.h
  a8:	04709d4a 	lsl	z10.s, z10.s, #16
  ac:	91002042 	add	x2, x2, #0x8
  b0:	a4a0a84b 	ld1h	{z11.h}, p2/z, [x2]
  b4:	05b2396b 	uunpklo	z11.s, z11.h
  b8:	04709d6b 	lsl	z11.s, z11.s, #16
  bc:	9103a042 	add	x2, x2, #0xe8
  c0:	a4a0a84c 	ld1h	{z12.h}, p2/z, [x2]
  c4:	05b2398c 	uunpklo	z12.s, z12.h
  c8:	04709d8c 	lsl	z12.s, z12.s, #16
  cc:	91002042 	add	x2, x2, #0x8
  d0:	a4a0a84d 	ld1h	{z13.h}, p2/z, [x2]
  d4:	05b239ad 	uunpklo	z13.s, z13.h
  d8:	04709dad 	lsl	z13.s, z13.s, #16
  dc:	91002042 	add	x2, x2, #0x8
  e0:	a4a0a84e 	ld1h	{z14.h}, p2/z, [x2]
  e4:	05b239ce 	uunpklo	z14.s, z14.h
  e8:	04709dce 	lsl	z14.s, z14.s, #16
  ec:	91002042 	add	x2, x2, #0x8
  f0:	a4a0a84f 	ld1h	{z15.h}, p2/z, [x2]
  f4:	05b239ef 	uunpklo	z15.s, z15.h
  f8:	04709def 	lsl	z15.s, z15.s, #16
  fc:	9103a042 	add	x2, x2, #0xe8
 100:	a4a0a850 	ld1h	{z16.h}, p2/z, [x2]
 104:	05b23a10 	uunpklo	z16.s, z16.h
 108:	04709e10 	lsl	z16.s, z16.s, #16
 10c:	91002042 	add	x2, x2, #0x8
 110:	a4a0a851 	ld1h	{z17.h}, p2/z, [x2]
 114:	05b23a31 	uunpklo	z17.s, z17.h
 118:	04709e31 	lsl	z17.s, z17.s, #16
 11c:	91002042 	add	x2, x2, #0x8
 120:	a4a0a852 	ld1h	{z18.h}, p2/z, [x2]
 124:	05b23a52 	uunpklo	z18.s, z18.h
 128:	04709e52 	lsl	z18.s, z18.s, #16
 12c:	91002042 	add	x2, x2, #0x8
 130:	a4a0a853 	ld1h	{z19.h}, p2/z, [x2]
 134:	05b23a73 	uunpklo	z19.s, z19.h
 138:	04709e73 	lsl	z19.s, z19.s, #16
 13c:	9103a042 	add	x2, x2, #0xe8
 140:	a4a0a854 	ld1h	{z20.h}, p2/z, [x2]
 144:	05b23a94 	uunpklo	z20.s, z20.h
 148:	04709e94 	lsl	z20.s, z20.s, #16
 14c:	91002042 	add	x2, x2, #0x8
 150:	a4a0a855 	ld1h	{z21.h}, p2/z, [x2]
 154:	05b23ab5 	uunpklo	z21.s, z21.h
 158:	04709eb5 	lsl	z21.s, z21.s, #16
 15c:	91002042 	add	x2, x2, #0x8
 160:	a4a0a856 	ld1h	{z22.h}, p2/z, [x2]
 164:	05b23ad6 	uunpklo	z22.s, z22.h
 168:	04709ed6 	lsl	z22.s, z22.s, #16
 16c:	91002042 	add	x2, x2, #0x8
 170:	a4a0a857 	ld1h	{z23.h}, p2/z, [x2]
 174:	05b23af7 	uunpklo	z23.s, z23.h
 178:	04709ef7 	lsl	z23.s, z23.s, #16
 17c:	9103a042 	add	x2, x2, #0xe8
 180:	a4a0a858 	ld1h	{z24.h}, p2/z, [x2]
 184:	05b23b18 	uunpklo	z24.s, z24.h
 188:	04709f18 	lsl	z24.s, z24.s, #16
 18c:	91002042 	add	x2, x2, #0x8
 190:	a4a0a859 	ld1h	{z25.h}, p2/z, [x2]
 194:	05b23b39 	uunpklo	z25.s, z25.h
 198:	04709f39 	lsl	z25.s, z25.s, #16
 19c:	91002042 	add	x2, x2, #0x8
 1a0:	a4a0a85a 	ld1h	{z26.h}, p2/z, [x2]
 1a4:	05b23b5a 	uunpklo	z26.s, z26.h
 1a8:	04709f5a 	lsl	z26.s, z26.s, #16
 1ac:	91002042 	add	x2, x2, #0x8
 1b0:	a4a0a85b 	ld1h	{z27.h}, p2/z, [x2]
 1b4:	05b23b7b 	uunpklo	z27.s, z27.h
 1b8:	04709f7b 	lsl	z27.s, z27.s, #16
 1bc:	9103a042 	add	x2, x2, #0xe8
 1c0:	a4a0a85c 	ld1h	{z28.h}, p2/z, [x2]
 1c4:	05b23b9c 	uunpklo	z28.s, z28.h
 1c8:	04709f9c 	lsl	z28.s, z28.s, #16
 1cc:	91002042 	add	x2, x2, #0x8
 1d0:	a4a0a85d 	ld1h	{z29.h}, p2/z, [x2]
 1d4:	05b23bbd 	uunpklo	z29.s, z29.h
 1d8:	04709fbd 	lsl	z29.s, z29.s, #16
 1dc:	91002042 	add	x2, x2, #0x8
 1e0:	a4a0a85e 	ld1h	{z30.h}, p2/z, [x2]
 1e4:	05b23bde 	uunpklo	z30.s, z30.h
 1e8:	04709fde 	lsl	z30.s, z30.s, #16
 1ec:	91002042 	add	x2, x2, #0x8
 1f0:	a4a0a85f 	ld1h	{z31.h}, p2/z, [x2]
 1f4:	05b23bff 	uunpklo	z31.s, z31.h
 1f8:	04709fff 	lsl	z31.s, z31.s, #16
 1fc:	d1146042 	sub	x2, x2, #0x518
 200:	d280408b 	mov	x11, #0x204                 	// #516
 204:	d2803f0a 	mov	x10, #0x1f8                 	// #504
 208:	d2803c09 	mov	x9, #0x1e0                 	// #480
 20c:	d2802008 	mov	x8, #0x100                 	// #256
 210:	85804000 	ldr	z0, [x0]
 214:	91040000 	add	x0, x0, #0x100
 218:	85804005 	ldr	z5, [x0]
 21c:	05656001 	zip1	z1.h, z0.h, z5.h
 220:	05656402 	zip2	z2.h, z0.h, z5.h
 224:	d103c000 	sub	x0, x0, #0xf0
 228:	85804000 	ldr	z0, [x0]
 22c:	91040000 	add	x0, x0, #0x100
 230:	85804005 	ldr	z5, [x0]
 234:	05656003 	zip1	z3.h, z0.h, z5.h
 238:	05656404 	zip2	z4.h, z0.h, z5.h
 23c:	d103c000 	sub	x0, x0, #0xf0
 240:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 244:	91081021 	add	x1, x1, #0x204
 248:	64608028 	bfdot	z8.s, z1.h, z0.h
 24c:	64608049 	bfdot	z9.s, z2.h, z0.h
 250:	6460806a 	bfdot	z10.s, z3.h, z0.h
 254:	6460808b 	bfdot	z11.s, z4.h, z0.h
 258:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 25c:	91081021 	add	x1, x1, #0x204
 260:	6460802c 	bfdot	z12.s, z1.h, z0.h
 264:	6460804d 	bfdot	z13.s, z2.h, z0.h
 268:	6460806e 	bfdot	z14.s, z3.h, z0.h
 26c:	6460808f 	bfdot	z15.s, z4.h, z0.h
 270:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 274:	91081021 	add	x1, x1, #0x204
 278:	64608030 	bfdot	z16.s, z1.h, z0.h
 27c:	64608051 	bfdot	z17.s, z2.h, z0.h
 280:	64608072 	bfdot	z18.s, z3.h, z0.h
 284:	64608093 	bfdot	z19.s, z4.h, z0.h
 288:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 28c:	91081021 	add	x1, x1, #0x204
 290:	64608034 	bfdot	z20.s, z1.h, z0.h
 294:	64608055 	bfdot	z21.s, z2.h, z0.h
 298:	64608076 	bfdot	z22.s, z3.h, z0.h
 29c:	64608097 	bfdot	z23.s, z4.h, z0.h
 2a0:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 2a4:	91081021 	add	x1, x1, #0x204
 2a8:	64608038 	bfdot	z24.s, z1.h, z0.h
 2ac:	64608059 	bfdot	z25.s, z2.h, z0.h
 2b0:	6460807a 	bfdot	z26.s, z3.h, z0.h
 2b4:	6460809b 	bfdot	z27.s, z4.h, z0.h
 2b8:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 2bc:	d1284021 	sub	x1, x1, #0xa10
 2c0:	6460803c 	bfdot	z28.s, z1.h, z0.h
 2c4:	6460805d 	bfdot	z29.s, z2.h, z0.h
 2c8:	6460807e 	bfdot	z30.s, z3.h, z0.h
 2cc:	6460809f 	bfdot	z31.s, z4.h, z0.h
 2d0:	8b090000 	add	x0, x0, x9
 2d4:	85804000 	ldr	z0, [x0]
 2d8:	91040000 	add	x0, x0, #0x100
 2dc:	85804005 	ldr	z5, [x0]
 2e0:	05656001 	zip1	z1.h, z0.h, z5.h
 2e4:	05656402 	zip2	z2.h, z0.h, z5.h
 2e8:	d103c000 	sub	x0, x0, #0xf0
 2ec:	85804000 	ldr	z0, [x0]
 2f0:	91040000 	add	x0, x0, #0x100
 2f4:	85804005 	ldr	z5, [x0]
 2f8:	05656003 	zip1	z3.h, z0.h, z5.h
 2fc:	05656404 	zip2	z4.h, z0.h, z5.h
 300:	d103c000 	sub	x0, x0, #0xf0
 304:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 308:	91081021 	add	x1, x1, #0x204
 30c:	64608028 	bfdot	z8.s, z1.h, z0.h
 310:	64608049 	bfdot	z9.s, z2.h, z0.h
 314:	6460806a 	bfdot	z10.s, z3.h, z0.h
 318:	6460808b 	bfdot	z11.s, z4.h, z0.h
 31c:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 320:	91081021 	add	x1, x1, #0x204
 324:	6460802c 	bfdot	z12.s, z1.h, z0.h
 328:	6460804d 	bfdot	z13.s, z2.h, z0.h
 32c:	6460806e 	bfdot	z14.s, z3.h, z0.h
 330:	6460808f 	bfdot	z15.s, z4.h, z0.h
 334:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 338:	91081021 	add	x1, x1, #0x204
 33c:	64608030 	bfdot	z16.s, z1.h, z0.h
 340:	64608051 	bfdot	z17.s, z2.h, z0.h
 344:	64608072 	bfdot	z18.s, z3.h, z0.h
 348:	64608093 	bfdot	z19.s, z4.h, z0.h
 34c:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 350:	91081021 	add	x1, x1, #0x204
 354:	64608034 	bfdot	z20.s, z1.h, z0.h
 358:	64608055 	bfdot	z21.s, z2.h, z0.h
 35c:	64608076 	bfdot	z22.s, z3.h, z0.h
 360:	64608097 	bfdot	z23.s, z4.h, z0.h
 364:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 368:	91081021 	add	x1, x1, #0x204
 36c:	64608038 	bfdot	z24.s, z1.h, z0.h
 370:	64608059 	bfdot	z25.s, z2.h, z0.h
 374:	6460807a 	bfdot	z26.s, z3.h, z0.h
 378:	6460809b 	bfdot	z27.s, z4.h, z0.h
 37c:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 380:	d1284021 	sub	x1, x1, #0xa10
 384:	6460803c 	bfdot	z28.s, z1.h, z0.h
 388:	6460805d 	bfdot	z29.s, z2.h, z0.h
 38c:	6460807e 	bfdot	z30.s, z3.h, z0.h
 390:	6460809f 	bfdot	z31.s, z4.h, z0.h
 394:	8b090000 	add	x0, x0, x9
 398:	85804000 	ldr	z0, [x0]
 39c:	91040000 	add	x0, x0, #0x100
 3a0:	85804005 	ldr	z5, [x0]
 3a4:	05656001 	zip1	z1.h, z0.h, z5.h
 3a8:	05656402 	zip2	z2.h, z0.h, z5.h
 3ac:	d103c000 	sub	x0, x0, #0xf0
 3b0:	85804000 	ldr	z0, [x0]
 3b4:	91040000 	add	x0, x0, #0x100
 3b8:	85804005 	ldr	z5, [x0]
 3bc:	05656003 	zip1	z3.h, z0.h, z5.h
 3c0:	05656404 	zip2	z4.h, z0.h, z5.h
 3c4:	d103c000 	sub	x0, x0, #0xf0
 3c8:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 3cc:	91081021 	add	x1, x1, #0x204
 3d0:	64608028 	bfdot	z8.s, z1.h, z0.h
 3d4:	64608049 	bfdot	z9.s, z2.h, z0.h
 3d8:	6460806a 	bfdot	z10.s, z3.h, z0.h
 3dc:	6460808b 	bfdot	z11.s, z4.h, z0.h
 3e0:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 3e4:	91081021 	add	x1, x1, #0x204
 3e8:	6460802c 	bfdot	z12.s, z1.h, z0.h
 3ec:	6460804d 	bfdot	z13.s, z2.h, z0.h
 3f0:	6460806e 	bfdot	z14.s, z3.h, z0.h
 3f4:	6460808f 	bfdot	z15.s, z4.h, z0.h
 3f8:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 3fc:	91081021 	add	x1, x1, #0x204
 400:	64608030 	bfdot	z16.s, z1.h, z0.h
 404:	64608051 	bfdot	z17.s, z2.h, z0.h
 408:	64608072 	bfdot	z18.s, z3.h, z0.h
 40c:	64608093 	bfdot	z19.s, z4.h, z0.h
 410:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 414:	91081021 	add	x1, x1, #0x204
 418:	64608034 	bfdot	z20.s, z1.h, z0.h
 41c:	64608055 	bfdot	z21.s, z2.h, z0.h
 420:	64608076 	bfdot	z22.s, z3.h, z0.h
 424:	64608097 	bfdot	z23.s, z4.h, z0.h
 428:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 42c:	91081021 	add	x1, x1, #0x204
 430:	64608038 	bfdot	z24.s, z1.h, z0.h
 434:	64608059 	bfdot	z25.s, z2.h, z0.h
 438:	6460807a 	bfdot	z26.s, z3.h, z0.h
 43c:	6460809b 	bfdot	z27.s, z4.h, z0.h
 440:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 444:	d1284021 	sub	x1, x1, #0xa10
 448:	6460803c 	bfdot	z28.s, z1.h, z0.h
 44c:	6460805d 	bfdot	z29.s, z2.h, z0.h
 450:	6460807e 	bfdot	z30.s, z3.h, z0.h
 454:	6460809f 	bfdot	z31.s, z4.h, z0.h
 458:	8b090000 	add	x0, x0, x9
 45c:	85804000 	ldr	z0, [x0]
 460:	91040000 	add	x0, x0, #0x100
 464:	85804005 	ldr	z5, [x0]
 468:	05656001 	zip1	z1.h, z0.h, z5.h
 46c:	05656402 	zip2	z2.h, z0.h, z5.h
 470:	d103c000 	sub	x0, x0, #0xf0
 474:	85804000 	ldr	z0, [x0]
 478:	91040000 	add	x0, x0, #0x100
 47c:	85804005 	ldr	z5, [x0]
 480:	05656003 	zip1	z3.h, z0.h, z5.h
 484:	05656404 	zip2	z4.h, z0.h, z5.h
 488:	d103c000 	sub	x0, x0, #0xf0
 48c:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 490:	91081021 	add	x1, x1, #0x204
 494:	64608028 	bfdot	z8.s, z1.h, z0.h
 498:	64608049 	bfdot	z9.s, z2.h, z0.h
 49c:	6460806a 	bfdot	z10.s, z3.h, z0.h
 4a0:	6460808b 	bfdot	z11.s, z4.h, z0.h
 4a4:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 4a8:	91081021 	add	x1, x1, #0x204
 4ac:	6460802c 	bfdot	z12.s, z1.h, z0.h
 4b0:	6460804d 	bfdot	z13.s, z2.h, z0.h
 4b4:	6460806e 	bfdot	z14.s, z3.h, z0.h
 4b8:	6460808f 	bfdot	z15.s, z4.h, z0.h
 4bc:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 4c0:	91081021 	add	x1, x1, #0x204
 4c4:	64608030 	bfdot	z16.s, z1.h, z0.h
 4c8:	64608051 	bfdot	z17.s, z2.h, z0.h
 4cc:	64608072 	bfdot	z18.s, z3.h, z0.h
 4d0:	64608093 	bfdot	z19.s, z4.h, z0.h
 4d4:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 4d8:	91081021 	add	x1, x1, #0x204
 4dc:	64608034 	bfdot	z20.s, z1.h, z0.h
 4e0:	64608055 	bfdot	z21.s, z2.h, z0.h
 4e4:	64608076 	bfdot	z22.s, z3.h, z0.h
 4e8:	64608097 	bfdot	z23.s, z4.h, z0.h
 4ec:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 4f0:	91081021 	add	x1, x1, #0x204
 4f4:	64608038 	bfdot	z24.s, z1.h, z0.h
 4f8:	64608059 	bfdot	z25.s, z2.h, z0.h
 4fc:	6460807a 	bfdot	z26.s, z3.h, z0.h
 500:	6460809b 	bfdot	z27.s, z4.h, z0.h
 504:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 508:	d1284021 	sub	x1, x1, #0xa10
 50c:	6460803c 	bfdot	z28.s, z1.h, z0.h
 510:	6460805d 	bfdot	z29.s, z2.h, z0.h
 514:	6460807e 	bfdot	z30.s, z3.h, z0.h
 518:	6460809f 	bfdot	z31.s, z4.h, z0.h
 51c:	8b090000 	add	x0, x0, x9
 520:	d1002108 	sub	x8, x8, #0x8
 524:	b5ffe768 	cbnz	x8, 0x210
 528:	85804000 	ldr	z0, [x0]
 52c:	91040000 	add	x0, x0, #0x100
 530:	85804005 	ldr	z5, [x0]
 534:	05656001 	zip1	z1.h, z0.h, z5.h
 538:	05656402 	zip2	z2.h, z0.h, z5.h
 53c:	d103c000 	sub	x0, x0, #0xf0
 540:	85804000 	ldr	z0, [x0]
 544:	91040000 	add	x0, x0, #0x100
 548:	85804005 	ldr	z5, [x0]
 54c:	05656003 	zip1	z3.h, z0.h, z5.h
 550:	05656404 	zip2	z4.h, z0.h, z5.h
 554:	d103c000 	sub	x0, x0, #0xf0
 558:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 55c:	91081021 	add	x1, x1, #0x204
 560:	64608028 	bfdot	z8.s, z1.h, z0.h
 564:	64608049 	bfdot	z9.s, z2.h, z0.h
 568:	6460806a 	bfdot	z10.s, z3.h, z0.h
 56c:	6460808b 	bfdot	z11.s, z4.h, z0.h
 570:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 574:	91081021 	add	x1, x1, #0x204
 578:	6460802c 	bfdot	z12.s, z1.h, z0.h
 57c:	6460804d 	bfdot	z13.s, z2.h, z0.h
 580:	6460806e 	bfdot	z14.s, z3.h, z0.h
 584:	6460808f 	bfdot	z15.s, z4.h, z0.h
 588:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 58c:	91081021 	add	x1, x1, #0x204
 590:	64608030 	bfdot	z16.s, z1.h, z0.h
 594:	64608051 	bfdot	z17.s, z2.h, z0.h
 598:	64608072 	bfdot	z18.s, z3.h, z0.h
 59c:	64608093 	bfdot	z19.s, z4.h, z0.h
 5a0:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 5a4:	91081021 	add	x1, x1, #0x204
 5a8:	64608034 	bfdot	z20.s, z1.h, z0.h
 5ac:	64608055 	bfdot	z21.s, z2.h, z0.h
 5b0:	64608076 	bfdot	z22.s, z3.h, z0.h
 5b4:	64608097 	bfdot	z23.s, z4.h, z0.h
 5b8:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 5bc:	91081021 	add	x1, x1, #0x204
 5c0:	64608038 	bfdot	z24.s, z1.h, z0.h
 5c4:	64608059 	bfdot	z25.s, z2.h, z0.h
 5c8:	6460807a 	bfdot	z26.s, z3.h, z0.h
 5cc:	6460809b 	bfdot	z27.s, z4.h, z0.h
 5d0:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 5d4:	d1284021 	sub	x1, x1, #0xa10
 5d8:	6460803c 	bfdot	z28.s, z1.h, z0.h
 5dc:	6460805d 	bfdot	z29.s, z2.h, z0.h
 5e0:	6460807e 	bfdot	z30.s, z3.h, z0.h
 5e4:	6460809f 	bfdot	z31.s, z4.h, z0.h
 5e8:	8b090000 	add	x0, x0, x9
 5ec:	d1080000 	sub	x0, x0, #0x200
 5f0:	d1404000 	sub	x0, x0, #0x10, lsl #12
 5f4:	d1081021 	sub	x1, x1, #0x204
 5f8:	658aa108 	bfcvt	z8.h, p0/m, z8.s
 5fc:	05686908 	uzp1	z8.h, z8.h, z8.h
 600:	e4a0e848 	st1h	{z8.h}, p2, [x2]
 604:	91002042 	add	x2, x2, #0x8
 608:	658aa129 	bfcvt	z9.h, p0/m, z9.s
 60c:	05696929 	uzp1	z9.h, z9.h, z9.h
 610:	e4a0e849 	st1h	{z9.h}, p2, [x2]
 614:	91002042 	add	x2, x2, #0x8
 618:	658aa14a 	bfcvt	z10.h, p0/m, z10.s
 61c:	056a694a 	uzp1	z10.h, z10.h, z10.h
 620:	e4a0e84a 	st1h	{z10.h}, p2, [x2]
 624:	91002042 	add	x2, x2, #0x8
 628:	658aa16b 	bfcvt	z11.h, p0/m, z11.s
 62c:	056b696b 	uzp1	z11.h, z11.h, z11.h
 630:	e4a0e84b 	st1h	{z11.h}, p2, [x2]
 634:	9103a042 	add	x2, x2, #0xe8
 638:	658aa18c 	bfcvt	z12.h, p0/m, z12.s
 63c:	056c698c 	uzp1	z12.h, z12.h, z12.h
 640:	e4a0e84c 	st1h	{z12.h}, p2, [x2]
 644:	91002042 	add	x2, x2, #0x8
 648:	658aa1ad 	bfcvt	z13.h, p0/m, z13.s
 64c:	056d69ad 	uzp1	z13.h, z13.h, z13.h
 650:	e4a0e84d 	st1h	{z13.h}, p2, [x2]
 654:	91002042 	add	x2, x2, #0x8
 658:	658aa1ce 	bfcvt	z14.h, p0/m, z14.s
 65c:	056e69ce 	uzp1	z14.h, z14.h, z14.h
 660:	e4a0e84e 	st1h	{z14.h}, p2, [x2]
 664:	91002042 	add	x2, x2, #0x8
 668:	658aa1ef 	bfcvt	z15.h, p0/m, z15.s
 66c:	056f69ef 	uzp1	z15.h, z15.h, z15.h
 670:	e4a0e84f 	st1h	{z15.h}, p2, [x2]
 674:	9103a042 	add	x2, x2, #0xe8
 678:	658aa210 	bfcvt	z16.h, p0/m, z16.s
 67c:	05706a10 	uzp1	z16.h, z16.h, z16.h
 680:	e4a0e850 	st1h	{z16.h}, p2, [x2]
 684:	91002042 	add	x2, x2, #0x8
 688:	658aa231 	bfcvt	z17.h, p0/m, z17.s
 68c:	05716a31 	uzp1	z17.h, z17.h, z17.h
 690:	e4a0e851 	st1h	{z17.h}, p2, [x2]
 694:	91002042 	add	x2, x2, #0x8
 698:	658aa252 	bfcvt	z18.h, p0/m, z18.s
 69c:	05726a52 	uzp1	z18.h, z18.h, z18.h
 6a0:	e4a0e852 	st1h	{z18.h}, p2, [x2]
 6a4:	91002042 	add	x2, x2, #0x8
 6a8:	658aa273 	bfcvt	z19.h, p0/m, z19.s
 6ac:	05736a73 	uzp1	z19.h, z19.h, z19.h
 6b0:	e4a0e853 	st1h	{z19.h}, p2, [x2]
 6b4:	9103a042 	add	x2, x2, #0xe8
 6b8:	658aa294 	bfcvt	z20.h, p0/m, z20.s
 6bc:	05746a94 	uzp1	z20.h, z20.h, z20.h
 6c0:	e4a0e854 	st1h	{z20.h}, p2, [x2]
 6c4:	91002042 	add	x2, x2, #0x8
 6c8:	658aa2b5 	bfcvt	z21.h, p0/m, z21.s
 6cc:	05756ab5 	uzp1	z21.h, z21.h, z21.h
 6d0:	e4a0e855 	st1h	{z21.h}, p2, [x2]
 6d4:	91002042 	add	x2, x2, #0x8
 6d8:	658aa2d6 	bfcvt	z22.h, p0/m, z22.s
 6dc:	05766ad6 	uzp1	z22.h, z22.h, z22.h
 6e0:	e4a0e856 	st1h	{z22.h}, p2, [x2]
 6e4:	91002042 	add	x2, x2, #0x8
 6e8:	658aa2f7 	bfcvt	z23.h, p0/m, z23.s
 6ec:	05776af7 	uzp1	z23.h, z23.h, z23.h
 6f0:	e4a0e857 	st1h	{z23.h}, p2, [x2]
 6f4:	9103a042 	add	x2, x2, #0xe8
 6f8:	658aa318 	bfcvt	z24.h, p0/m, z24.s
 6fc:	05786b18 	uzp1	z24.h, z24.h, z24.h
 700:	e4a0e858 	st1h	{z24.h}, p2, [x2]
 704:	91002042 	add	x2, x2, #0x8
 708:	658aa339 	bfcvt	z25.h, p0/m, z25.s
 70c:	05796b39 	uzp1	z25.h, z25.h, z25.h
 710:	e4a0e859 	st1h	{z25.h}, p2, [x2]
 714:	91002042 	add	x2, x2, #0x8
 718:	658aa35a 	bfcvt	z26.h, p0/m, z26.s
 71c:	057a6b5a 	uzp1	z26.h, z26.h, z26.h
 720:	e4a0e85a 	st1h	{z26.h}, p2, [x2]
 724:	91002042 	add	x2, x2, #0x8
 728:	658aa37b 	bfcvt	z27.h, p0/m, z27.s
 72c:	057b6b7b 	uzp1	z27.h, z27.h, z27.h
 730:	e4a0e85b 	st1h	{z27.h}, p2, [x2]
 734:	9103a042 	add	x2, x2, #0xe8
 738:	658aa39c 	bfcvt	z28.h, p0/m, z28.s
 73c:	057c6b9c 	uzp1	z28.h, z28.h, z28.h
 740:	e4a0e85c 	st1h	{z28.h}, p2, [x2]
 744:	91002042 	add	x2, x2, #0x8
 748:	658aa3bd 	bfcvt	z29.h, p0/m, z29.s
 74c:	057d6bbd 	uzp1	z29.h, z29.h, z29.h
 750:	e4a0e85d 	st1h	{z29.h}, p2, [x2]
 754:	91002042 	add	x2, x2, #0x8
 758:	658aa3de 	bfcvt	z30.h, p0/m, z30.s
 75c:	057e6bde 	uzp1	z30.h, z30.h, z30.h
 760:	e4a0e85e 	st1h	{z30.h}, p2, [x2]
 764:	91002042 	add	x2, x2, #0x8
 768:	658aa3ff 	bfcvt	z31.h, p0/m, z31.s
 76c:	057f6bff 	uzp1	z31.h, z31.h, z31.h
 770:	e4a0e85f 	st1h	{z31.h}, p2, [x2]
 774:	d1146042 	sub	x2, x2, #0x518
 778:	91008042 	add	x2, x2, #0x20
 77c:	91008000 	add	x0, x0, #0x20
 780:	d10040c6 	sub	x6, x6, #0x10
 784:	b5ffc7e6 	cbnz	x6, 0x80
 788:	91140042 	add	x2, x2, #0x500
 78c:	d1040000 	sub	x0, x0, #0x100
 790:	91306021 	add	x1, x1, #0xc18
 794:	d10018e7 	sub	x7, x7, #0x6
 798:	b5ffc727 	cbnz	x7, 0x7c
 79c:	d2800047 	mov	x7, #0x2                   	// #2
 7a0:	d2801006 	mov	x6, #0x80                  	// #128
 7a4:	a4a0a858 	ld1h	{z24.h}, p2/z, [x2]
 7a8:	05b23b18 	uunpklo	z24.s, z24.h
 7ac:	04709f18 	lsl	z24.s, z24.s, #16
 7b0:	91002042 	add	x2, x2, #0x8
 7b4:	a4a0a859 	ld1h	{z25.h}, p2/z, [x2]
 7b8:	05b23b39 	uunpklo	z25.s, z25.h
 7bc:	04709f39 	lsl	z25.s, z25.s, #16
 7c0:	91002042 	add	x2, x2, #0x8
 7c4:	a4a0a85a 	ld1h	{z26.h}, p2/z, [x2]
 7c8:	05b23b5a 	uunpklo	z26.s, z26.h
 7cc:	04709f5a 	lsl	z26.s, z26.s, #16
 7d0:	91002042 	add	x2, x2, #0x8
 7d4:	a4a0a85b 	ld1h	{z27.h}, p2/z, [x2]
 7d8:	05b23b7b 	uunpklo	z27.s, z27.h
 7dc:	04709f7b 	lsl	z27.s, z27.s, #16
 7e0:	9103a042 	add	x2, x2, #0xe8
 7e4:	a4a0a85c 	ld1h	{z28.h}, p2/z, [x2]
 7e8:	05b23b9c 	uunpklo	z28.s, z28.h
 7ec:	04709f9c 	lsl	z28.s, z28.s, #16
 7f0:	91002042 	add	x2, x2, #0x8
 7f4:	a4a0a85d 	ld1h	{z29.h}, p2/z, [x2]
 7f8:	05b23bbd 	uunpklo	z29.s, z29.h
 7fc:	04709fbd 	lsl	z29.s, z29.s, #16
 800:	91002042 	add	x2, x2, #0x8
 804:	a4a0a85e 	ld1h	{z30.h}, p2/z, [x2]
 808:	05b23bde 	uunpklo	z30.s, z30.h
 80c:	04709fde 	lsl	z30.s, z30.s, #16
 810:	91002042 	add	x2, x2, #0x8
 814:	a4a0a85f 	ld1h	{z31.h}, p2/z, [x2]
 818:	05b23bff 	uunpklo	z31.s, z31.h
 81c:	04709fff 	lsl	z31.s, z31.s, #16
 820:	d1046042 	sub	x2, x2, #0x118
 824:	d280408b 	mov	x11, #0x204                 	// #516
 828:	d280400a 	mov	x10, #0x200                 	// #512
 82c:	d2803c09 	mov	x9, #0x1e0                 	// #480
 830:	d2802008 	mov	x8, #0x100                 	// #256
 834:	85804000 	ldr	z0, [x0]
 838:	91040000 	add	x0, x0, #0x100
 83c:	85804005 	ldr	z5, [x0]
 840:	05656001 	zip1	z1.h, z0.h, z5.h
 844:	05656402 	zip2	z2.h, z0.h, z5.h
 848:	d103c000 	sub	x0, x0, #0xf0
 84c:	85804000 	ldr	z0, [x0]
 850:	91040000 	add	x0, x0, #0x100
 854:	85804005 	ldr	z5, [x0]
 858:	05656003 	zip1	z3.h, z0.h, z5.h
 85c:	05656404 	zip2	z4.h, z0.h, z5.h
 860:	d103c000 	sub	x0, x0, #0xf0
 864:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 868:	91081021 	add	x1, x1, #0x204
 86c:	64608038 	bfdot	z24.s, z1.h, z0.h
 870:	64608059 	bfdot	z25.s, z2.h, z0.h
 874:	6460807a 	bfdot	z26.s, z3.h, z0.h
 878:	6460809b 	bfdot	z27.s, z4.h, z0.h
 87c:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 880:	d1080021 	sub	x1, x1, #0x200
 884:	6460803c 	bfdot	z28.s, z1.h, z0.h
 888:	6460805d 	bfdot	z29.s, z2.h, z0.h
 88c:	6460807e 	bfdot	z30.s, z3.h, z0.h
 890:	6460809f 	bfdot	z31.s, z4.h, z0.h
 894:	8b090000 	add	x0, x0, x9
 898:	85804000 	ldr	z0, [x0]
 89c:	91040000 	add	x0, x0, #0x100
 8a0:	85804005 	ldr	z5, [x0]
 8a4:	05656001 	zip1	z1.h, z0.h, z5.h
 8a8:	05656402 	zip2	z2.h, z0.h, z5.h
 8ac:	d103c000 	sub	x0, x0, #0xf0
 8b0:	85804000 	ldr	z0, [x0]
 8b4:	91040000 	add	x0, x0, #0x100
 8b8:	85804005 	ldr	z5, [x0]
 8bc:	05656003 	zip1	z3.h, z0.h, z5.h
 8c0:	05656404 	zip2	z4.h, z0.h, z5.h
 8c4:	d103c000 	sub	x0, x0, #0xf0
 8c8:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 8cc:	91081021 	add	x1, x1, #0x204
 8d0:	64608038 	bfdot	z24.s, z1.h, z0.h
 8d4:	64608059 	bfdot	z25.s, z2.h, z0.h
 8d8:	6460807a 	bfdot	z26.s, z3.h, z0.h
 8dc:	6460809b 	bfdot	z27.s, z4.h, z0.h
 8e0:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 8e4:	d1080021 	sub	x1, x1, #0x200
 8e8:	6460803c 	bfdot	z28.s, z1.h, z0.h
 8ec:	6460805d 	bfdot	z29.s, z2.h, z0.h
 8f0:	6460807e 	bfdot	z30.s, z3.h, z0.h
 8f4:	6460809f 	bfdot	z31.s, z4.h, z0.h
 8f8:	8b090000 	add	x0, x0, x9
 8fc:	85804000 	ldr	z0, [x0]
 900:	91040000 	add	x0, x0, #0x100
 904:	85804005 	ldr	z5, [x0]
 908:	05656001 	zip1	z1.h, z0.h, z5.h
 90c:	05656402 	zip2	z2.h, z0.h, z5.h
 910:	d103c000 	sub	x0, x0, #0xf0
 914:	85804000 	ldr	z0, [x0]
 918:	91040000 	add	x0, x0, #0x100
 91c:	85804005 	ldr	z5, [x0]
 920:	05656003 	zip1	z3.h, z0.h, z5.h
 924:	05656404 	zip2	z4.h, z0.h, z5.h
 928:	d103c000 	sub	x0, x0, #0xf0
 92c:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 930:	91081021 	add	x1, x1, #0x204
 934:	64608038 	bfdot	z24.s, z1.h, z0.h
 938:	64608059 	bfdot	z25.s, z2.h, z0.h
 93c:	6460807a 	bfdot	z26.s, z3.h, z0.h
 940:	6460809b 	bfdot	z27.s, z4.h, z0.h
 944:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 948:	d1080021 	sub	x1, x1, #0x200
 94c:	6460803c 	bfdot	z28.s, z1.h, z0.h
 950:	6460805d 	bfdot	z29.s, z2.h, z0.h
 954:	6460807e 	bfdot	z30.s, z3.h, z0.h
 958:	6460809f 	bfdot	z31.s, z4.h, z0.h
 95c:	8b090000 	add	x0, x0, x9
 960:	85804000 	ldr	z0, [x0]
 964:	91040000 	add	x0, x0, #0x100
 968:	85804005 	ldr	z5, [x0]
 96c:	05656001 	zip1	z1.h, z0.h, z5.h
 970:	05656402 	zip2	z2.h, z0.h, z5.h
 974:	d103c000 	sub	x0, x0, #0xf0
 978:	85804000 	ldr	z0, [x0]
 97c:	91040000 	add	x0, x0, #0x100
 980:	85804005 	ldr	z5, [x0]
 984:	05656003 	zip1	z3.h, z0.h, z5.h
 988:	05656404 	zip2	z4.h, z0.h, z5.h
 98c:	d103c000 	sub	x0, x0, #0xf0
 990:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 994:	91081021 	add	x1, x1, #0x204
 998:	64608038 	bfdot	z24.s, z1.h, z0.h
 99c:	64608059 	bfdot	z25.s, z2.h, z0.h
 9a0:	6460807a 	bfdot	z26.s, z3.h, z0.h
 9a4:	6460809b 	bfdot	z27.s, z4.h, z0.h
 9a8:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 9ac:	d1080021 	sub	x1, x1, #0x200
 9b0:	6460803c 	bfdot	z28.s, z1.h, z0.h
 9b4:	6460805d 	bfdot	z29.s, z2.h, z0.h
 9b8:	6460807e 	bfdot	z30.s, z3.h, z0.h
 9bc:	6460809f 	bfdot	z31.s, z4.h, z0.h
 9c0:	8b090000 	add	x0, x0, x9
 9c4:	d1002108 	sub	x8, x8, #0x8
 9c8:	b5fff368 	cbnz	x8, 0x834
 9cc:	85804000 	ldr	z0, [x0]
 9d0:	91040000 	add	x0, x0, #0x100
 9d4:	85804005 	ldr	z5, [x0]
 9d8:	05656001 	zip1	z1.h, z0.h, z5.h
 9dc:	05656402 	zip2	z2.h, z0.h, z5.h
 9e0:	d103c000 	sub	x0, x0, #0xf0
 9e4:	85804000 	ldr	z0, [x0]
 9e8:	91040000 	add	x0, x0, #0x100
 9ec:	85804005 	ldr	z5, [x0]
 9f0:	05656003 	zip1	z3.h, z0.h, z5.h
 9f4:	05656404 	zip2	z4.h, z0.h, z5.h
 9f8:	d103c000 	sub	x0, x0, #0xf0
 9fc:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 a00:	91081021 	add	x1, x1, #0x204
 a04:	64608038 	bfdot	z24.s, z1.h, z0.h
 a08:	64608059 	bfdot	z25.s, z2.h, z0.h
 a0c:	6460807a 	bfdot	z26.s, z3.h, z0.h
 a10:	6460809b 	bfdot	z27.s, z4.h, z0.h
 a14:	8540c020 	ld1rw	{z0.s}, p0/z, [x1]
 a18:	d1080021 	sub	x1, x1, #0x200
 a1c:	6460803c 	bfdot	z28.s, z1.h, z0.h
 a20:	6460805d 	bfdot	z29.s, z2.h, z0.h
 a24:	6460807e 	bfdot	z30.s, z3.h, z0.h
 a28:	6460809f 	bfdot	z31.s, z4.h, z0.h
 a2c:	8b090000 	add	x0, x0, x9
 a30:	d1080000 	sub	x0, x0, #0x200
 a34:	d1404000 	sub	x0, x0, #0x10, lsl #12
 a38:	d1081021 	sub	x1, x1, #0x204
 a3c:	658aa318 	bfcvt	z24.h, p0/m, z24.s
 a40:	05786b18 	uzp1	z24.h, z24.h, z24.h
 a44:	e4a0e858 	st1h	{z24.h}, p2, [x2]
 a48:	91002042 	add	x2, x2, #0x8
 a4c:	658aa339 	bfcvt	z25.h, p0/m, z25.s
 a50:	05796b39 	uzp1	z25.h, z25.h, z25.h
 a54:	e4a0e859 	st1h	{z25.h}, p2, [x2]
 a58:	91002042 	add	x2, x2, #0x8
 a5c:	658aa35a 	bfcvt	z26.h, p0/m, z26.s
 a60:	057a6b5a 	uzp1	z26.h, z26.h, z26.h
 a64:	e4a0e85a 	st1h	{z26.h}, p2, [x2]
 a68:	91002042 	add	x2, x2, #0x8
 a6c:	658aa37b 	bfcvt	z27.h, p0/m, z27.s
 a70:	057b6b7b 	uzp1	z27.h, z27.h, z27.h
 a74:	e4a0e85b 	st1h	{z27.h}, p2, [x2]
 a78:	9103a042 	add	x2, x2, #0xe8
 a7c:	658aa39c 	bfcvt	z28.h, p0/m, z28.s
 a80:	057c6b9c 	uzp1	z28.h, z28.h, z28.h
 a84:	e4a0e85c 	st1h	{z28.h}, p2, [x2]
 a88:	91002042 	add	x2, x2, #0x8
 a8c:	658aa3bd 	bfcvt	z29.h, p0/m, z29.s
 a90:	057d6bbd 	uzp1	z29.h, z29.h, z29.h
 a94:	e4a0e85d 	st1h	{z29.h}, p2, [x2]
 a98:	91002042 	add	x2, x2, #0x8
 a9c:	658aa3de 	bfcvt	z30.h, p0/m, z30.s
 aa0:	057e6bde 	uzp1	z30.h, z30.h, z30.h
 aa4:	e4a0e85e 	st1h	{z30.h}, p2, [x2]
 aa8:	91002042 	add	x2, x2, #0x8
 aac:	658aa3ff 	bfcvt	z31.h, p0/m, z31.s
 ab0:	057f6bff 	uzp1	z31.h, z31.h, z31.h
 ab4:	e4a0e85f 	st1h	{z31.h}, p2, [x2]
 ab8:	d1046042 	sub	x2, x2, #0x118
 abc:	91008042 	add	x2, x2, #0x20
 ac0:	91008000 	add	x0, x0, #0x20
 ac4:	d10040c6 	sub	x6, x6, #0x10
 ac8:	b5ffe6e6 	cbnz	x6, 0x7a4
 acc:	91040042 	add	x2, x2, #0x100
 ad0:	d1040000 	sub	x0, x0, #0x100
 ad4:	91102021 	add	x1, x1, #0x408
 ad8:	d10008e7 	sub	x7, x7, #0x2
 adc:	b5ffe627 	cbnz	x7, 0x7a0
 ae0:	910003bf 	mov	sp, x29
 ae4:	6d4b27e8 	ldp	d8, d9, [sp, #176]
 ae8:	6d4a2fea 	ldp	d10, d11, [sp, #160]
 aec:	6d4937ec 	ldp	d12, d13, [sp, #144]
 af0:	6d483fee 	ldp	d14, d15, [sp, #128]
 af4:	a9426ffa 	ldp	x26, x27, [sp, #32]
 af8:	a94177fc 	ldp	x28, x29, [sp, #16]
 afc:	f94003fe 	ldr	x30, [sp]
 b00:	910303ff 	add	sp, sp, #0xc0
 b04:	d65f03c0 	ret
