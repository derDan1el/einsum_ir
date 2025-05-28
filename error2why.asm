
libxsmm_abi1_neov2_bf16_nn_128x128x1024_128_1024_128_a1_b1_p0_br0_sa0_sb0_uh0_si0_tc-abid_avnni0_bvnni0_cvnni0.mxm:     file format binary


Disassembly of section .data:

0000000000000000 <.data>:
   0:   d10303ff    sub   sp, sp, #0xc0
   4:   6d0b27e8    stp   d8, d9, [sp, #176]
   8:   6d0a2fea    stp   d10, d11, [sp, #160]
   c:   6d0937ec    stp   d12, d13, [sp, #144]
  10:   6d083fee    stp   d14, d15, [sp, #128]
  14:   a9026ffa    stp   x26, x27, [sp, #32]
  18:   a90177fc    stp   x28, x29, [sp, #16]
  1c:   f90003fe    str   x30, [sp]
  20:   8a00000a    and   x10, x0, x0
  24:   f9401140    ldr   x0, [x10, #32]
  28:   f9402941    ldr   x1, [x10, #80]
  2c:   f9404142    ldr   x2, [x10, #128]
  30:   910003fd    mov   x29, sp
  34:   d10303ff    sub   sp, sp, #0xc0
  38:   d29ff80a    mov   x10, #0xffc0                   // #65472
  3c:   f2bfffea    movk   x10, #0xffff, lsl #16
  40:   f2dfffea    movk   x10, #0xffff, lsl #32
  44:   f2ffffea    movk   x10, #0xffff, lsl #48
  48:   910003e9    mov   x9, sp
  4c:   8a0a0129    and   x9, x9, x10
  50:   9100013f    mov   sp, x9
  54:   910003ea    mov   x10, sp
  58:   d141014a    sub   x10, x10, #0x40, lsl #12
  5c:   9100015f    mov   sp, x10
  60:   d10183a9    sub   x9, x29, #0x60
  64:   f900012a    str   x10, [x9]
  68:   910003ea    mov   x10, sp
  6c:   d102014a    sub   x10, x10, #0x80
  70:   9100015f    mov   sp, x10
  74:   d10223a9    sub   x9, x29, #0x88
  78:   f900012a    str   x10, [x9]
  7c:   d10303ff    sub   sp, sp, #0xc0
  80:   6d0b27e8    stp   d8, d9, [sp, #176]
  84:   6d0a2fea    stp   d10, d11, [sp, #160]
  88:   6d0937ec    stp   d12, d13, [sp, #144]
  8c:   6d083fee    stp   d14, d15, [sp, #128]
  90:   a9026ffa    stp   x26, x27, [sp, #32]
  94:   a90177fc    stp   x28, x29, [sp, #16]
  98:   f90003fe    str   x30, [sp]
  9c:   d10223ac    sub   x12, x29, #0x88
  a0:   f940018c    ldr   x12, [x12]
  a4:   f9001180    str   x0, [x12, #32]
  a8:   d10183aa    sub   x10, x29, #0x60
  ac:   f940014a    ldr   x10, [x10]
  b0:   f900298a    str   x10, [x12, #80]
  b4:   f9401188    ldr   x8, [x12, #32]
  b8:   f9402989    ldr   x9, [x12, #80]
  bc:   d280800b    mov   x11, #0x400                    // #1024
  c0:   d280100a    mov   x10, #0x80                     // #128
  c4:   3dc00100    ldr   q0, [x8]
  c8:   91040108    add   x8, x8, #0x100
  cc:   3dc00101    ldr   q1, [x8]
  d0:   91040108    add   x8, x8, #0x100
  d4:   3dc00102    ldr   q2, [x8]
  d8:   91040108    add   x8, x8, #0x100
  dc:   3dc00103    ldr   q3, [x8]
  e0:   4e413804    zip1   v4.8h, v0.8h, v1.8h
  e4:   4e433845    zip1   v5.8h, v2.8h, v3.8h
  e8:   4e417806    zip2   v6.8h, v0.8h, v1.8h
  ec:   4e437847    zip2   v7.8h, v2.8h, v3.8h
  f0:   4e853888    zip1   v8.4s, v4.4s, v5.4s
  f4:   4e857889    zip2   v9.4s, v4.4s, v5.4s
  f8:   4e8738ca    zip1   v10.4s, v6.4s, v7.4s
  fc:   4e8778cb    zip2   v11.4s, v6.4s, v7.4s
 100:   3c810528    str   q8, [x9], #16
 104:   3c810529    str   q9, [x9], #16
 108:   3c81052a    str   q10, [x9], #16
 10c:   3c81052b    str   q11, [x9], #16
 110:   d10bc108    sub   x8, x8, #0x2f0
 114:   d100214a    sub   x10, x10, #0x8
 118:   b5fffd6a    cbnz   x10, 0xc4
 11c:   910c0108    add   x8, x8, #0x300
 120:   d100116b    sub   x11, x11, #0x4
 124:   b5fffceb    cbnz   x11, 0xc0
 128:   d10183aa    sub   x10, x29, #0x60
 12c:   f940014a    ldr   x10, [x10]
 130:   91000140    add   x0, x10, #0x0
 134:   6d4b27e8    ldp   d8, d9, [sp, #176]
 138:   6d4a2fea    ldp   d10, d11, [sp, #160]
 13c:   6d4937ec    ldp   d12, d13, [sp, #144]
 140:   6d483fee    ldp   d14, d15, [sp, #128]
 144:   a9426ffa    ldp   x26, x27, [sp, #32]
 148:   a94177fc    ldp   x28, x29, [sp, #16]
 14c:   f94003fe    ldr   x30, [sp]
 150:   910303ff    add   sp, sp, #0xc0
 154:   2518e3e0    ptrue   p0.b
 158:   d2800009    mov   x9, #0x0                      // #0
 15c:   252917e1    whilelt   p1.b, xzr, x9
 160:   d2800109    mov   x9, #0x8                      // #8
 164:   252917e2    whilelt   p2.b, xzr, x9
 168:   d2800fc7    mov   x7, #0x7e                     // #126
 16c:   d2801006    mov   x6, #0x80                     // #128
 170:   91040049    add   x9, x2, #0x100
 174:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 178:   05b238c6    uunpklo   z6.s, z6.h
 17c:   04709cc6    lsl   z6.s, z6.s, #16
 180:   91002042    add   x2, x2, #0x8
 184:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 188:   05b238e7    uunpklo   z7.s, z7.h
 18c:   04709ce7    lsl   z7.s, z7.s, #16
 190:   91002129    add   x9, x9, #0x8
 194:   05e760c8    zip1   z8.d, z6.d, z7.d
 198:   05e764c9    zip2   z9.d, z6.d, z7.d
 19c:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 1a0:   05b238c6    uunpklo   z6.s, z6.h
 1a4:   04709cc6    lsl   z6.s, z6.s, #16
 1a8:   91002042    add   x2, x2, #0x8
 1ac:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 1b0:   05b238e7    uunpklo   z7.s, z7.h
 1b4:   04709ce7    lsl   z7.s, z7.s, #16
 1b8:   91002129    add   x9, x9, #0x8
 1bc:   05e760ca    zip1   z10.d, z6.d, z7.d
 1c0:   05e764cb    zip2   z11.d, z6.d, z7.d
 1c4:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 1c8:   05b238c6    uunpklo   z6.s, z6.h
 1cc:   04709cc6    lsl   z6.s, z6.s, #16
 1d0:   91002042    add   x2, x2, #0x8
 1d4:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 1d8:   05b238e7    uunpklo   z7.s, z7.h
 1dc:   04709ce7    lsl   z7.s, z7.s, #16
 1e0:   91002129    add   x9, x9, #0x8
 1e4:   05e760cc    zip1   z12.d, z6.d, z7.d
 1e8:   05e764cd    zip2   z13.d, z6.d, z7.d
 1ec:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 1f0:   05b238c6    uunpklo   z6.s, z6.h
 1f4:   04709cc6    lsl   z6.s, z6.s, #16
 1f8:   91002042    add   x2, x2, #0x8
 1fc:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 200:   05b238e7    uunpklo   z7.s, z7.h
 204:   04709ce7    lsl   z7.s, z7.s, #16
 208:   91002129    add   x9, x9, #0x8
 20c:   05e760ce    zip1   z14.d, z6.d, z7.d
 210:   05e764cf    zip2   z15.d, z6.d, z7.d
 214:   91078042    add   x2, x2, #0x1e0
 218:   91040049    add   x9, x2, #0x100
 21c:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 220:   05b238c6    uunpklo   z6.s, z6.h
 224:   04709cc6    lsl   z6.s, z6.s, #16
 228:   91002042    add   x2, x2, #0x8
 22c:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 230:   05b238e7    uunpklo   z7.s, z7.h
 234:   04709ce7    lsl   z7.s, z7.s, #16
 238:   91002129    add   x9, x9, #0x8
 23c:   05e760d0    zip1   z16.d, z6.d, z7.d
 240:   05e764d1    zip2   z17.d, z6.d, z7.d
 244:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 248:   05b238c6    uunpklo   z6.s, z6.h
 24c:   04709cc6    lsl   z6.s, z6.s, #16
 250:   91002042    add   x2, x2, #0x8
 254:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 258:   05b238e7    uunpklo   z7.s, z7.h
 25c:   04709ce7    lsl   z7.s, z7.s, #16
 260:   91002129    add   x9, x9, #0x8
 264:   05e760d2    zip1   z18.d, z6.d, z7.d
 268:   05e764d3    zip2   z19.d, z6.d, z7.d
 26c:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 270:   05b238c6    uunpklo   z6.s, z6.h
 274:   04709cc6    lsl   z6.s, z6.s, #16
 278:   91002042    add   x2, x2, #0x8
 27c:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 280:   05b238e7    uunpklo   z7.s, z7.h
 284:   04709ce7    lsl   z7.s, z7.s, #16
 288:   91002129    add   x9, x9, #0x8
 28c:   05e760d4    zip1   z20.d, z6.d, z7.d
 290:   05e764d5    zip2   z21.d, z6.d, z7.d
 294:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 298:   05b238c6    uunpklo   z6.s, z6.h
 29c:   04709cc6    lsl   z6.s, z6.s, #16
 2a0:   91002042    add   x2, x2, #0x8
 2a4:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 2a8:   05b238e7    uunpklo   z7.s, z7.h
 2ac:   04709ce7    lsl   z7.s, z7.s, #16
 2b0:   91002129    add   x9, x9, #0x8
 2b4:   05e760d6    zip1   z22.d, z6.d, z7.d
 2b8:   05e764d7    zip2   z23.d, z6.d, z7.d
 2bc:   91078042    add   x2, x2, #0x1e0
 2c0:   91040049    add   x9, x2, #0x100
 2c4:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 2c8:   05b238c6    uunpklo   z6.s, z6.h
 2cc:   04709cc6    lsl   z6.s, z6.s, #16
 2d0:   91002042    add   x2, x2, #0x8
 2d4:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 2d8:   05b238e7    uunpklo   z7.s, z7.h
 2dc:   04709ce7    lsl   z7.s, z7.s, #16
 2e0:   91002129    add   x9, x9, #0x8
 2e4:   05e760d8    zip1   z24.d, z6.d, z7.d
 2e8:   05e764d9    zip2   z25.d, z6.d, z7.d
 2ec:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 2f0:   05b238c6    uunpklo   z6.s, z6.h
 2f4:   04709cc6    lsl   z6.s, z6.s, #16
 2f8:   91002042    add   x2, x2, #0x8
 2fc:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 300:   05b238e7    uunpklo   z7.s, z7.h
 304:   04709ce7    lsl   z7.s, z7.s, #16
 308:   91002129    add   x9, x9, #0x8
 30c:   05e760da    zip1   z26.d, z6.d, z7.d
 310:   05e764db    zip2   z27.d, z6.d, z7.d
 314:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 318:   05b238c6    uunpklo   z6.s, z6.h
 31c:   04709cc6    lsl   z6.s, z6.s, #16
 320:   91002042    add   x2, x2, #0x8
 324:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 328:   05b238e7    uunpklo   z7.s, z7.h
 32c:   04709ce7    lsl   z7.s, z7.s, #16
 330:   91002129    add   x9, x9, #0x8
 334:   05e760dc    zip1   z28.d, z6.d, z7.d
 338:   05e764dd    zip2   z29.d, z6.d, z7.d
 33c:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 340:   05b238c6    uunpklo   z6.s, z6.h
 344:   04709cc6    lsl   z6.s, z6.s, #16
 348:   91002042    add   x2, x2, #0x8
 34c:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 350:   05b238e7    uunpklo   z7.s, z7.h
 354:   04709ce7    lsl   z7.s, z7.s, #16
 358:   91002129    add   x9, x9, #0x8
 35c:   05e760de    zip1   z30.d, z6.d, z7.d
 360:   05e764df    zip2   z31.d, z6.d, z7.d
 364:   91078042    add   x2, x2, #0x1e0
 368:   d1180042    sub   x2, x2, #0x600
 36c:   d281000b    mov   x11, #0x800                    // #2048
 370:   d280fe8a    mov   x10, #0x7f4                    // #2036
 374:   d2803809    mov   x9, #0x1c0                    // #448
 378:   d2808008    mov   x8, #0x400                    // #1024
 37c:   85c0e026    ld1rd   {z6.d}, p0/z, [x1]
 380:   91200021    add   x1, x1, #0x800
 384:   85c0e027    ld1rd   {z7.d}, p0/z, [x1]
 388:   91200021    add   x1, x1, #0x800
 38c:   05e760c0    zip1   z0.d, z6.d, z7.d
 390:   85c0e026    ld1rd   {z6.d}, p0/z, [x1]
 394:   91200021    add   x1, x1, #0x800
 398:   85c0e027    ld1rd   {z7.d}, p0/z, [x1]
 39c:   91200021    add   x1, x1, #0x800
 3a0:   05e760c2    zip1   z2.d, z6.d, z7.d
 3a4:   85c0e026    ld1rd   {z6.d}, p0/z, [x1]
 3a8:   91200021    add   x1, x1, #0x800
 3ac:   85c0e027    ld1rd   {z7.d}, p0/z, [x1]
 3b0:   d11fe021    sub   x1, x1, #0x7f8
 3b4:   d1400821    sub   x1, x1, #0x2, lsl #12
 3b8:   05e760c4    zip1   z4.d, z6.d, z7.d
 3bc:   a5e0a006    ld1d   {z6.d}, p0/z, [x0]
 3c0:   a5e1a007    ld1d   {z7.d}, p0/z, [x0, #1, mul vl]
 3c4:   91008000    add   x0, x0, #0x20
 3c8:   6466e408    bfmmla   z8.s, z0.h, z6.h
 3cc:   6467e409    bfmmla   z9.s, z0.h, z7.h
 3d0:   6466e450    bfmmla   z16.s, z2.h, z6.h
 3d4:   6467e451    bfmmla   z17.s, z2.h, z7.h
 3d8:   6466e498    bfmmla   z24.s, z4.h, z6.h
 3dc:   6467e499    bfmmla   z25.s, z4.h, z7.h
 3e0:   a5e0a006    ld1d   {z6.d}, p0/z, [x0]
 3e4:   a5e1a007    ld1d   {z7.d}, p0/z, [x0, #1, mul vl]
 3e8:   91008000    add   x0, x0, #0x20
 3ec:   6466e40a    bfmmla   z10.s, z0.h, z6.h
 3f0:   6467e40b    bfmmla   z11.s, z0.h, z7.h
 3f4:   6466e452    bfmmla   z18.s, z2.h, z6.h
 3f8:   6467e453    bfmmla   z19.s, z2.h, z7.h
 3fc:   6466e49a    bfmmla   z26.s, z4.h, z6.h
 400:   6467e49b    bfmmla   z27.s, z4.h, z7.h
 404:   a5e0a006    ld1d   {z6.d}, p0/z, [x0]
 408:   a5e1a007    ld1d   {z7.d}, p0/z, [x0, #1, mul vl]
 40c:   91008000    add   x0, x0, #0x20
 410:   6466e40c    bfmmla   z12.s, z0.h, z6.h
 414:   6467e40d    bfmmla   z13.s, z0.h, z7.h
 418:   6466e454    bfmmla   z20.s, z2.h, z6.h
 41c:   6467e455    bfmmla   z21.s, z2.h, z7.h
 420:   6466e49c    bfmmla   z28.s, z4.h, z6.h
 424:   6467e49d    bfmmla   z29.s, z4.h, z7.h
 428:   a5e0a006    ld1d   {z6.d}, p0/z, [x0]
 42c:   a5e1a007    ld1d   {z7.d}, p0/z, [x0, #1, mul vl]
 430:   91008000    add   x0, x0, #0x20
 434:   6466e40e    bfmmla   z14.s, z0.h, z6.h
 438:   6467e40f    bfmmla   z15.s, z0.h, z7.h
 43c:   6466e456    bfmmla   z22.s, z2.h, z6.h
 440:   6467e457    bfmmla   z23.s, z2.h, z7.h
 444:   6466e49e    bfmmla   z30.s, z4.h, z6.h
 448:   6467e49f    bfmmla   z31.s, z4.h, z7.h
 44c:   910e0000    add   x0, x0, #0x380
 450:   d1001108    sub   x8, x8, #0x4
 454:   b5fff948    cbnz   x8, 0x37c
 458:   d1410000    sub   x0, x0, #0x40, lsl #12
 45c:   d1200021    sub   x1, x1, #0x800
 460:   91040049    add   x9, x2, #0x100
 464:   05e96906    uzp1   z6.d, z8.d, z9.d
 468:   05e96d07    uzp2   z7.d, z8.d, z9.d
 46c:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 470:   056668c6    uzp1   z6.h, z6.h, z6.h
 474:   e4a0e846    st1h   {z6.h}, p2, [x2]
 478:   91002042    add   x2, x2, #0x8
 47c:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 480:   056768e7    uzp1   z7.h, z7.h, z7.h
 484:   e4a0e927    st1h   {z7.h}, p2, [x9]
 488:   91002129    add   x9, x9, #0x8
 48c:   05eb6946    uzp1   z6.d, z10.d, z11.d
 490:   05eb6d47    uzp2   z7.d, z10.d, z11.d
 494:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 498:   056668c6    uzp1   z6.h, z6.h, z6.h
 49c:   e4a0e846    st1h   {z6.h}, p2, [x2]
 4a0:   91002042    add   x2, x2, #0x8
 4a4:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 4a8:   056768e7    uzp1   z7.h, z7.h, z7.h
 4ac:   e4a0e927    st1h   {z7.h}, p2, [x9]
 4b0:   91002129    add   x9, x9, #0x8
 4b4:   05ed6986    uzp1   z6.d, z12.d, z13.d
 4b8:   05ed6d87    uzp2   z7.d, z12.d, z13.d
 4bc:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 4c0:   056668c6    uzp1   z6.h, z6.h, z6.h
 4c4:   e4a0e846    st1h   {z6.h}, p2, [x2]
 4c8:   91002042    add   x2, x2, #0x8
 4cc:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 4d0:   056768e7    uzp1   z7.h, z7.h, z7.h
 4d4:   e4a0e927    st1h   {z7.h}, p2, [x9]
 4d8:   91002129    add   x9, x9, #0x8
 4dc:   05ef69c6    uzp1   z6.d, z14.d, z15.d
 4e0:   05ef6dc7    uzp2   z7.d, z14.d, z15.d
 4e4:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 4e8:   056668c6    uzp1   z6.h, z6.h, z6.h
 4ec:   e4a0e846    st1h   {z6.h}, p2, [x2]
 4f0:   91002042    add   x2, x2, #0x8
 4f4:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 4f8:   056768e7    uzp1   z7.h, z7.h, z7.h
 4fc:   e4a0e927    st1h   {z7.h}, p2, [x9]
 500:   91002129    add   x9, x9, #0x8
 504:   91078042    add   x2, x2, #0x1e0
 508:   91040049    add   x9, x2, #0x100
 50c:   05f16a06    uzp1   z6.d, z16.d, z17.d
 510:   05f16e07    uzp2   z7.d, z16.d, z17.d
 514:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 518:   056668c6    uzp1   z6.h, z6.h, z6.h
 51c:   e4a0e846    st1h   {z6.h}, p2, [x2]
 520:   91002042    add   x2, x2, #0x8
 524:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 528:   056768e7    uzp1   z7.h, z7.h, z7.h
 52c:   e4a0e927    st1h   {z7.h}, p2, [x9]
 530:   91002129    add   x9, x9, #0x8
 534:   05f36a46    uzp1   z6.d, z18.d, z19.d
 538:   05f36e47    uzp2   z7.d, z18.d, z19.d
 53c:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 540:   056668c6    uzp1   z6.h, z6.h, z6.h
 544:   e4a0e846    st1h   {z6.h}, p2, [x2]
 548:   91002042    add   x2, x2, #0x8
 54c:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 550:   056768e7    uzp1   z7.h, z7.h, z7.h
 554:   e4a0e927    st1h   {z7.h}, p2, [x9]
 558:   91002129    add   x9, x9, #0x8
 55c:   05f56a86    uzp1   z6.d, z20.d, z21.d
 560:   05f56e87    uzp2   z7.d, z20.d, z21.d
 564:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 568:   056668c6    uzp1   z6.h, z6.h, z6.h
 56c:   e4a0e846    st1h   {z6.h}, p2, [x2]
 570:   91002042    add   x2, x2, #0x8
 574:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 578:   056768e7    uzp1   z7.h, z7.h, z7.h
 57c:   e4a0e927    st1h   {z7.h}, p2, [x9]
 580:   91002129    add   x9, x9, #0x8
 584:   05f76ac6    uzp1   z6.d, z22.d, z23.d
 588:   05f76ec7    uzp2   z7.d, z22.d, z23.d
 58c:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 590:   056668c6    uzp1   z6.h, z6.h, z6.h
 594:   e4a0e846    st1h   {z6.h}, p2, [x2]
 598:   91002042    add   x2, x2, #0x8
 59c:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 5a0:   056768e7    uzp1   z7.h, z7.h, z7.h
 5a4:   e4a0e927    st1h   {z7.h}, p2, [x9]
 5a8:   91002129    add   x9, x9, #0x8
 5ac:   91078042    add   x2, x2, #0x1e0
 5b0:   91040049    add   x9, x2, #0x100
 5b4:   05f96b06    uzp1   z6.d, z24.d, z25.d
 5b8:   05f96f07    uzp2   z7.d, z24.d, z25.d
 5bc:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 5c0:   056668c6    uzp1   z6.h, z6.h, z6.h
 5c4:   e4a0e846    st1h   {z6.h}, p2, [x2]
 5c8:   91002042    add   x2, x2, #0x8
 5cc:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 5d0:   056768e7    uzp1   z7.h, z7.h, z7.h
 5d4:   e4a0e927    st1h   {z7.h}, p2, [x9]
 5d8:   91002129    add   x9, x9, #0x8
 5dc:   05fb6b46    uzp1   z6.d, z26.d, z27.d
 5e0:   05fb6f47    uzp2   z7.d, z26.d, z27.d
 5e4:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 5e8:   056668c6    uzp1   z6.h, z6.h, z6.h
 5ec:   e4a0e846    st1h   {z6.h}, p2, [x2]
 5f0:   91002042    add   x2, x2, #0x8
 5f4:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 5f8:   056768e7    uzp1   z7.h, z7.h, z7.h
 5fc:   e4a0e927    st1h   {z7.h}, p2, [x9]
 600:   91002129    add   x9, x9, #0x8
 604:   05fd6b86    uzp1   z6.d, z28.d, z29.d
 608:   05fd6f87    uzp2   z7.d, z28.d, z29.d
 60c:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 610:   056668c6    uzp1   z6.h, z6.h, z6.h
 614:   e4a0e846    st1h   {z6.h}, p2, [x2]
 618:   91002042    add   x2, x2, #0x8
 61c:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 620:   056768e7    uzp1   z7.h, z7.h, z7.h
 624:   e4a0e927    st1h   {z7.h}, p2, [x9]
 628:   91002129    add   x9, x9, #0x8
 62c:   05ff6bc6    uzp1   z6.d, z30.d, z31.d
 630:   05ff6fc7    uzp2   z7.d, z30.d, z31.d
 634:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 638:   056668c6    uzp1   z6.h, z6.h, z6.h
 63c:   e4a0e846    st1h   {z6.h}, p2, [x2]
 640:   91002042    add   x2, x2, #0x8
 644:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 648:   056768e7    uzp1   z7.h, z7.h, z7.h
 64c:   e4a0e927    st1h   {z7.h}, p2, [x9]
 650:   91002129    add   x9, x9, #0x8
 654:   91078042    add   x2, x2, #0x1e0
 658:   d1180042    sub   x2, x2, #0x600
 65c:   91008042    add   x2, x2, #0x20
 660:   91020000    add   x0, x0, #0x80
 664:   d10040c6    sub   x6, x6, #0x10
 668:   b5ffd846    cbnz   x6, 0x170
 66c:   91140042    add   x2, x2, #0x500
 670:   d1100000    sub   x0, x0, #0x400
 674:   91400c21    add   x1, x1, #0x3, lsl #12
 678:   d10018e7    sub   x7, x7, #0x6
 67c:   b5ffd787    cbnz   x7, 0x16c
 680:   d2800047    mov   x7, #0x2                      // #2
 684:   d2801006    mov   x6, #0x80                     // #128
 688:   91040049    add   x9, x2, #0x100
 68c:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 690:   05b238c6    uunpklo   z6.s, z6.h
 694:   04709cc6    lsl   z6.s, z6.s, #16
 698:   91002042    add   x2, x2, #0x8
 69c:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 6a0:   05b238e7    uunpklo   z7.s, z7.h
 6a4:   04709ce7    lsl   z7.s, z7.s, #16
 6a8:   91002129    add   x9, x9, #0x8
 6ac:   05e760c8    zip1   z8.d, z6.d, z7.d
 6b0:   05e764c9    zip2   z9.d, z6.d, z7.d
 6b4:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 6b8:   05b238c6    uunpklo   z6.s, z6.h
 6bc:   04709cc6    lsl   z6.s, z6.s, #16
 6c0:   91002042    add   x2, x2, #0x8
 6c4:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 6c8:   05b238e7    uunpklo   z7.s, z7.h
 6cc:   04709ce7    lsl   z7.s, z7.s, #16
 6d0:   91002129    add   x9, x9, #0x8
 6d4:   05e760ca    zip1   z10.d, z6.d, z7.d
 6d8:   05e764cb    zip2   z11.d, z6.d, z7.d
 6dc:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 6e0:   05b238c6    uunpklo   z6.s, z6.h
 6e4:   04709cc6    lsl   z6.s, z6.s, #16
 6e8:   91002042    add   x2, x2, #0x8
 6ec:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 6f0:   05b238e7    uunpklo   z7.s, z7.h
 6f4:   04709ce7    lsl   z7.s, z7.s, #16
 6f8:   91002129    add   x9, x9, #0x8
 6fc:   05e760cc    zip1   z12.d, z6.d, z7.d
 700:   05e764cd    zip2   z13.d, z6.d, z7.d
 704:   a4a0a846    ld1h   {z6.h}, p2/z, [x2]
 708:   05b238c6    uunpklo   z6.s, z6.h
 70c:   04709cc6    lsl   z6.s, z6.s, #16
 710:   91002042    add   x2, x2, #0x8
 714:   a4a0a927    ld1h   {z7.h}, p2/z, [x9]
 718:   05b238e7    uunpklo   z7.s, z7.h
 71c:   04709ce7    lsl   z7.s, z7.s, #16
 720:   91002129    add   x9, x9, #0x8
 724:   05e760ce    zip1   z14.d, z6.d, z7.d
 728:   05e764cf    zip2   z15.d, z6.d, z7.d
 72c:   91078042    add   x2, x2, #0x1e0
 730:   d1080042    sub   x2, x2, #0x200
 734:   d281000b    mov   x11, #0x800                    // #2048
 738:   d280ff8a    mov   x10, #0x7fc                    // #2044
 73c:   d2803809    mov   x9, #0x1c0                    // #448
 740:   d2808008    mov   x8, #0x400                    // #1024
 744:   85c0e026    ld1rd   {z6.d}, p0/z, [x1]
 748:   91200021    add   x1, x1, #0x800
 74c:   85c0e027    ld1rd   {z7.d}, p0/z, [x1]
 750:   d11fe021    sub   x1, x1, #0x7f8
 754:   05e760c0    zip1   z0.d, z6.d, z7.d
 758:   a5e0a006    ld1d   {z6.d}, p0/z, [x0]
 75c:   a5e1a007    ld1d   {z7.d}, p0/z, [x0, #1, mul vl]
 760:   91008000    add   x0, x0, #0x20
 764:   6466e408    bfmmla   z8.s, z0.h, z6.h
 768:   6467e409    bfmmla   z9.s, z0.h, z7.h
 76c:   a5e0a006    ld1d   {z6.d}, p0/z, [x0]
 770:   a5e1a007    ld1d   {z7.d}, p0/z, [x0, #1, mul vl]
 774:   91008000    add   x0, x0, #0x20
 778:   6466e40a    bfmmla   z10.s, z0.h, z6.h
 77c:   6467e40b    bfmmla   z11.s, z0.h, z7.h
 780:   a5e0a006    ld1d   {z6.d}, p0/z, [x0]
 784:   a5e1a007    ld1d   {z7.d}, p0/z, [x0, #1, mul vl]
 788:   91008000    add   x0, x0, #0x20
 78c:   6466e40c    bfmmla   z12.s, z0.h, z6.h
 790:   6467e40d    bfmmla   z13.s, z0.h, z7.h
 794:   a5e0a006    ld1d   {z6.d}, p0/z, [x0]
 798:   a5e1a007    ld1d   {z7.d}, p0/z, [x0, #1, mul vl]
 79c:   91008000    add   x0, x0, #0x20
 7a0:   6466e40e    bfmmla   z14.s, z0.h, z6.h
 7a4:   6467e40f    bfmmla   z15.s, z0.h, z7.h
 7a8:   910e0000    add   x0, x0, #0x380
 7ac:   d1001108    sub   x8, x8, #0x4
 7b0:   b5fffca8    cbnz   x8, 0x744
 7b4:   d1410000    sub   x0, x0, #0x40, lsl #12
 7b8:   d1200021    sub   x1, x1, #0x800
 7bc:   91040049    add   x9, x2, #0x100
 7c0:   05e96906    uzp1   z6.d, z8.d, z9.d
 7c4:   05e96d07    uzp2   z7.d, z8.d, z9.d
 7c8:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 7cc:   056668c6    uzp1   z6.h, z6.h, z6.h
 7d0:   e4a0e846    st1h   {z6.h}, p2, [x2]
 7d4:   91002042    add   x2, x2, #0x8
 7d8:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 7dc:   056768e7    uzp1   z7.h, z7.h, z7.h
 7e0:   e4a0e927    st1h   {z7.h}, p2, [x9]
 7e4:   91002129    add   x9, x9, #0x8
 7e8:   05eb6946    uzp1   z6.d, z10.d, z11.d
 7ec:   05eb6d47    uzp2   z7.d, z10.d, z11.d
 7f0:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 7f4:   056668c6    uzp1   z6.h, z6.h, z6.h
 7f8:   e4a0e846    st1h   {z6.h}, p2, [x2]
 7fc:   91002042    add   x2, x2, #0x8
 800:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 804:   056768e7    uzp1   z7.h, z7.h, z7.h
 808:   e4a0e927    st1h   {z7.h}, p2, [x9]
 80c:   91002129    add   x9, x9, #0x8
 810:   05ed6986    uzp1   z6.d, z12.d, z13.d
 814:   05ed6d87    uzp2   z7.d, z12.d, z13.d
 818:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 81c:   056668c6    uzp1   z6.h, z6.h, z6.h
 820:   e4a0e846    st1h   {z6.h}, p2, [x2]
 824:   91002042    add   x2, x2, #0x8
 828:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 82c:   056768e7    uzp1   z7.h, z7.h, z7.h
 830:   e4a0e927    st1h   {z7.h}, p2, [x9]
 834:   91002129    add   x9, x9, #0x8
 838:   05ef69c6    uzp1   z6.d, z14.d, z15.d
 83c:   05ef6dc7    uzp2   z7.d, z14.d, z15.d
 840:   658aa0c6    bfcvt   z6.h, p0/m, z6.s
 844:   056668c6    uzp1   z6.h, z6.h, z6.h
 848:   e4a0e846    st1h   {z6.h}, p2, [x2]
 84c:   91002042    add   x2, x2, #0x8
 850:   658aa0e7    bfcvt   z7.h, p0/m, z7.s
 854:   056768e7    uzp1   z7.h, z7.h, z7.h
 858:   e4a0e927    st1h   {z7.h}, p2, [x9]
 85c:   91002129    add   x9, x9, #0x8
 860:   91078042    add   x2, x2, #0x1e0
 864:   d1080042    sub   x2, x2, #0x200
 868:   91008042    add   x2, x2, #0x20
 86c:   91020000    add   x0, x0, #0x80
 870:   d10040c6    sub   x6, x6, #0x10
 874:   b5fff0a6    cbnz   x6, 0x688
 878:   91040042    add   x2, x2, #0x100
 87c:   d1100000    sub   x0, x0, #0x400
 880:   91400421    add   x1, x1, #0x1, lsl #12
 884:   d10008e7    sub   x7, x7, #0x2
 888:   b5ffefe7    cbnz   x7, 0x684
 88c:   910003bf    mov   sp, x29
 890:   6d4b27e8    ldp   d8, d9, [sp, #176]
 894:   6d4a2fea    ldp   d10, d11, [sp, #160]
 898:   6d4937ec    ldp   d12, d13, [sp, #144]
 89c:   6d483fee    ldp   d14, d15, [sp, #128]
 8a0:   a9426ffa    ldp   x26, x27, [sp, #32]
 8a4:   a94177fc    ldp   x28, x29, [sp, #16]
 8a8:   f94003fe    ldr   x30, [sp]
 8ac:   910303ff    add   sp, sp, #0xc0
 8b0:   d65f03c0    ret
