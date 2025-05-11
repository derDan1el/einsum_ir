
libxsmm_abi1_neov2_bf16_nn_1x2x1_1_1_1_a1_b1_p0_br0_sa0_sb0_uh0_si0_tc-abid_avnni0_bvnni0_cvnni0.mxm:     file format binary


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
  54:	d2800049 	mov	x9, #0x2                   	// #2
  58:	252917e3 	whilelt	p3.b, xzr, x9
  5c:	2518e3e0 	ptrue	p0.b
  60:	d2800089 	mov	x9, #0x4                   	// #4
  64:	252917e1 	whilelt	p1.b, xzr, x9
  68:	d2800109 	mov	x9, #0x8                   	// #8
  6c:	252917e2 	whilelt	p2.b, xzr, x9
  70:	d2800049 	mov	x9, #0x2                   	// #2
  74:	252917e4 	whilelt	p4.b, xzr, x9
  78:	d2800047 	mov	x7, #0x2                   	// #2
  7c:	d2800026 	mov	x6, #0x1                   	// #1
  80:	a4a0b05e 	ld1h	{z30.h}, p4/z, [x2]
  84:	05b23bde 	uunpklo	z30.s, z30.h
  88:	04709fde 	lsl	z30.s, z30.s, #16
  8c:	91000842 	add	x2, x2, #0x2
  90:	a4a0b05f 	ld1h	{z31.h}, p4/z, [x2]
  94:	05b23bff 	uunpklo	z31.s, z31.h
  98:	04709fff 	lsl	z31.s, z31.s, #16
  9c:	d1000842 	sub	x2, x2, #0x2
  a0:	d280004b 	mov	x11, #0x2                   	// #2
  a4:	d29fffca 	mov	x10, #0xfffe                	// #65534
  a8:	f2bfffea 	movk	x10, #0xffff, lsl #16
  ac:	f2dfffea 	movk	x10, #0xffff, lsl #32
  b0:	f2ffffea 	movk	x10, #0xffff, lsl #48
  b4:	d2800009 	mov	x9, #0x0                   	// #0
  b8:	04a23042 	eor	z2.d, z2.d, z2.d
  bc:	a4a0ac00 	ld1h	{z0.h}, p3/z, [x0]
  c0:	05626001 	zip1	z1.h, z0.h, z2.h
  c4:	91000800 	add	x0, x0, #0x2
  c8:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
  cc:	91000821 	add	x1, x1, #0x2
  d0:	6460803e 	bfdot	z30.s, z1.h, z0.h
  d4:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]
  d8:	d1000021 	sub	x1, x1, #0x0
  dc:	6460803f 	bfdot	z31.s, z1.h, z0.h
  e0:	8b090000 	add	x0, x0, x9
  e4:	d1000800 	sub	x0, x0, #0x2
  e8:	d1000821 	sub	x1, x1, #0x2
  ec:	658aa3de 	bfcvt	z30.h, p0/m, z30.s
  f0:	057e6bde 	uzp1	z30.h, z30.h, z30.h
  f4:	e4a0f05e 	st1h	{z30.h}, p4, [x2]
  f8:	91000842 	add	x2, x2, #0x2
  fc:	658aa3ff 	bfcvt	z31.h, p0/m, z31.s
 100:	057f6bff 	uzp1	z31.h, z31.h, z31.h
 104:	e4a0f05f 	st1h	{z31.h}, p4, [x2]
 108:	d1000842 	sub	x2, x2, #0x2
 10c:	91000842 	add	x2, x2, #0x2
 110:	91000800 	add	x0, x0, #0x2
 114:	d10004c6 	sub	x6, x6, #0x1
 118:	b5fffb46 	cbnz	x6, 0x80
 11c:	91000842 	add	x2, x2, #0x2
 120:	d1000800 	sub	x0, x0, #0x2
 124:	91001021 	add	x1, x1, #0x4
 128:	d10008e7 	sub	x7, x7, #0x2
 12c:	b5fffa87 	cbnz	x7, 0x7c
 130:	910003bf 	mov	sp, x29
 134:	6d4b27e8 	ldp	d8, d9, [sp, #176]
 138:	6d4a2fea 	ldp	d10, d11, [sp, #160]
 13c:	6d4937ec 	ldp	d12, d13, [sp, #144]
 140:	6d483fee 	ldp	d14, d15, [sp, #128]
 144:	a9426ffa 	ldp	x26, x27, [sp, #32]
 148:	a94177fc 	ldp	x28, x29, [sp, #16]
 14c:	f94003fe 	ldr	x30, [sp]
 150:	910303ff 	add	sp, sp, #0xc0
 154:	d65f03c0 	ret
