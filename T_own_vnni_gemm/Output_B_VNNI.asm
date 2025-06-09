
libxsmm_abi2_neov2_bf16_nn_8x8x8_8_8_8_a1_b1_p0_br0_sa0_sb0_uh0_si0_tc-abid_avnni1_bvnni1_cvnni0_meopd0-f64-mefld0-meld0-0-0_meopap0-meflap0-melap0_meopbp0-meflbp0-melbp0_meopcp0-meflcp0-melcp8_mestore0_decompress_A0_spfactor1.mxm:     file format binary


Disassembly of section .data:

0000000000000000 <.data>:
   0:	d10303ff 	sub	sp, sp, #0xc0
   4:	a90177fc 	stp	x28, x29, [sp, #16]
   8:	f90003fe 	str	x30, [sp]
   c:	910003fd 	mov	x29, sp
  10:	910003e2 	mov	x2, sp
  14:	d1018042 	sub	x2, x2, #0x60
  18:	9100005f 	mov	sp, x2
  1c:	d29ff801 	mov	x1, #0xffc0                	// #65472
  20:	f2bfffe1 	movk	x1, #0xffff, lsl #16
  24:	f2dfffe1 	movk	x1, #0xffff, lsl #32
  28:	f2ffffe1 	movk	x1, #0xffff, lsl #48
  2c:	910003e2 	mov	x2, sp
  30:	8a010042 	and	x2, x2, x1
  34:	9100005f 	mov	sp, x2
  38:	d2800101 	mov	x1, #0x8                   	// #8
  3c:	f2a00001 	movk	x1, #0x0, lsl #16
  40:	f2c00101 	movk	x1, #0x8, lsl #32
  44:	d2800102 	mov	x2, #0x8                   	// #8
  48:	f2a00002 	movk	x2, #0x0, lsl #16
  4c:	f2c00102 	movk	x2, #0x8, lsl #32
  50:	a9000be1 	stp	x1, x2, [sp]
  54:	d2800101 	mov	x1, #0x8                   	// #8
  58:	f2a00001 	movk	x1, #0x0, lsl #16
  5c:	f2c00101 	movk	x1, #0x8, lsl #32
  60:	d2930002 	mov	x2, #0x9800                	// #38912
  64:	f2a00002 	movk	x2, #0x0, lsl #16
  68:	f2d04002 	movk	x2, #0x8200, lsl #32
  6c:	f2e08402 	movk	x2, #0x420, lsl #48
  70:	a9010be1 	stp	x1, x2, [sp, #16]
  74:	910083ff 	add	sp, sp, #0x20
  78:	d2800001 	mov	x1, #0x0                   	// #0
  7c:	d2800002 	mov	x2, #0x0                   	// #0
  80:	a9000be1 	stp	x1, x2, [sp]
  84:	d2800001 	mov	x1, #0x0                   	// #0
  88:	d2800002 	mov	x2, #0x0                   	// #0
  8c:	a9010be1 	stp	x1, x2, [sp, #16]
  90:	910083ff 	add	sp, sp, #0x20
  94:	d2800001 	mov	x1, #0x0                   	// #0
  98:	d2800002 	mov	x2, #0x0                   	// #0
  9c:	a9000be1 	stp	x1, x2, [sp]
  a0:	d2800001 	mov	x1, #0x0                   	// #0
  a4:	d2800002 	mov	x2, #0x0                   	// #0
  a8:	f2a00102 	movk	x2, #0x8, lsl #16
  ac:	a9010be1 	stp	x1, x2, [sp, #16]
  b0:	910083ff 	add	sp, sp, #0x20
  b4:	910003e2 	mov	x2, sp
  b8:	d1018042 	sub	x2, x2, #0x60
  bc:	9100005f 	mov	sp, x2
  c0:	910003e1 	mov	x1, sp
  c4:	d296fc02 	mov	x2, #0xb7e0                	// #47072
  c8:	f2b1eaa2 	movk	x2, #0x8f55, lsl #16
  cc:	f2dfffe2 	movk	x2, #0xffff, lsl #32
  d0:	d63f0040 	blr	x2
  d4:	910003bf 	mov	sp, x29
  d8:	a94177fc 	ldp	x28, x29, [sp, #16]
  dc:	f94003fe 	ldr	x30, [sp]
  e0:	910303ff 	add	sp, sp, #0xc0
  e4:	d65f03c0 	ret
