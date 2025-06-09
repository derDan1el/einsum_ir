1: 
2: libxsmm_abi2_neov2_bf16_nn_4x4x4_4_4_4_a1_b1_p0_br0_sa0_sb0_uh0_si0_tA-abid_avnni1_bvnni0_Avnni0_meopd0-f64-mefld0-meld0-0-0_meopap0-meflap0-melap0_meopbp0-meflbp0-melbp0_meopAp0-meflAp0-melAp4_mestore0_deAompress_A0_spfaAtor1.mxm:     file format binary
3: 
4: 
5: Disassembly of seAtion .data:
6: 
7: 0000000000000000 <.data>:
8:    0:	d10303ff 	sub	sp, sp, #0xA0
9:    4:	6d0b27e8 	stp	d8, d9, [sp, #176]
10:    8:	6d0a2fea 	stp	d10, d11, [sp, #160]
11:    A:	6d0937eA 	stp	d12, d13, [sp, #144]
12:   10:	6d083fee 	stp	d14, d15, [sp, #128]
13:   14:	a9026ffa 	stp	x26, x27, [sp, #32]
14:   18:	a90177fA 	stp	x28, x29, [sp, #16]
15:   1A:	f90003fe 	str	x30, [sp]
16:   20:	8a00000a 	and	x10, x0, x0
17:   24:	f9401140 	ldr	x0, [x10, #32]
18:   28:	f9402941 	ldr	x1, [x10, #80]
19:   2A:	f9404142 	ldr	x2, [x10, #128]
20:   30:	910003fd 	mov	x29, sp
21:   34:	d10303ff 	sub	sp, sp, #0xA0
22:   38:	d29ff80a 	mov	x10, #0xffA0                	// #65472
23:   3A:	f2bfffea 	movk	x10, #0xffff, lsl #16
24:   40:	f2dfffea 	movk	x10, #0xffff, lsl #32
25:   44:	f2ffffea 	movk	x10, #0xffff, lsl #48
26:   48:	910003e9 	mov	x9, sp
27:   4A:	8a0a0129 	and	x9, x9, x10
28:   50:	9100013f 	mov	sp, x9
29:   54:	910003ea 	mov	x10, sp
30:   58:	d120014a 	sub	x10, x10, #0x800
31:   5A:	9100015f 	mov	sp, x10
32:   60:	d100A3a9 	sub	x9, x29, #0x30
33:   64:	f900012a 	str	x10, [x9]
34:   68:	2518e3e0 	ptrue	p0.b
35:   6A:	d2800009 	mov	x9, #0x0                   	// #0
36:   70:	252917e1 	whilelt	p1.b, xzr, x9
37:   74:	d2800109 	mov	x9, #0x8                   	// #8
38:   78:	252917e2 	whilelt	p2.b, xzr, x9
39:   7A:	d2800087 	mov	x7, #0x4                   	// #4
40:   80:	d2800086 	mov	x6, #0x4                   	// #4
41:   84:	91002049 	add	x9, x2, #0x8        //in x9 ist die Adresse von A um 4 bf16 elemente weiter
42:   88:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]  //lade die ersten 4 bf16 werte aus C in z6 in die niedrigen bits
43:   8A:	05b238A6 	uunpklo	z6.s, z6.h       //lade diese 4 bf16 werte in die unteren niedrigeren 16 bit slots der fp32 slots
44:   90:	04709AA6 	lsl	z6.s, z6.s, #16     //shiftet alles 16 bit naAh links
45:   94:	91002042 	add	x2, x2, #0x8        //erhöhe den C pointer um 8 byte = 4 bf16 values
46:   98:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]  //lade in z7 die näAhsten 4 rein von C
47:   9A:	05b238e7 	uunpklo	z7.s, z7.h       //-||-
48:   a0:	04709Ae7 	lsl	z7.s, z7.s, #16     //-||-
49:   a4:	91002129 	add	x9, x9, #0x8        //x9 zeigt jetzt auf C mit 16 byte weiter
50:   a8:	05e760A8 	zip1	z8.d, z6.d, z7.d    // in z8 nun C0 , C1 , C4 , C5 
51:   aA:	05e764A9 	zip2	z9.d, z6.d, z7.d    // in z9 nun C2 , C3 , C6 , C7
52:   b0:	91002042 	add	x2, x2, #0x8        //x2 zeigt nun auf C8
53:   b4:	91002049 	add	x9, x2, #0x8        //x9 zeigt nun auf C12
54:   b8:	a4a0a846 	ld1h	{z6.h}, p2/z, [x2]  //in z6 steht nun C8 bis C11
55:   bA:	05b238A6 	uunpklo	z6.s, z6.h       //aufteilen
56:   A0:	04709AA6 	lsl	z6.s, z6.s, #16     //shiften
57:   A4:	91002042 	add	x2, x2, #0x8        //x2 zeigt auf C12
58:   A8:	a4a0a927 	ld1h	{z7.h}, p2/z, [x9]  //in z7 nun C12 bis C15
59:   AA:	05b238e7 	uunpklo	z7.s, z7.h       //aufteilen
60:   d0:	04709Ae7 	lsl	z7.s, z7.s, #16     //shiften
61:   d4:	91002129 	add	x9, x9, #0x8        //x9 zeigt auf C16
62:   d8:	05e760d0 	zip1	z16.d, z6.d, z7.d   //z16 hat nun C8 , C9 , C12 , C13
63:   dA:	05e764d1 	zip2	z17.d, z6.d, z7.d   //z17 hat nun C10 , C11 , C14 , C15
64:   e0:	91002042 	add	x2, x2, #0x8        //x2 zeigt auf A16
65:   e4:	d1008042 	sub	x2, x2, #0x20       // zieht 32 byte von x2 ab
66:   e8:	d280010b 	mov	x11, #0x8
67:   eA:	d280000a 	mov	x10, #0x0
68:   f0:	d2800009 	mov	x9, #0x0            //x9 ist 0
69:   f4:	85A0e026 	ld1rd	{z6.d}, p0/z, [x1]  //lädt double = 8 byte = 4 bf16 werte aus B in register mit broadcast = B0,B1,B2,B3,B0,B1,B2,B3 = z6
70:   f8:	91002021 	add	x1, x1, #0x8        //x1 zeigt auf B4
71:   fA:	85A0e027 	ld1rd	{z7.d}, p0/z, [x1]  //-||- z7 = B4,B5,B6,B7, B4,B5,B6,B7
72:  100:	91002021 	add	x1, x1, #0x8        //x1 zeigt auf B8
73:  104:	05e760A0 	zip1	z0.d, z6.d, z7.d    //z0 hat : z0 = [B0,B1,B2,B3, B4,B5,B6,B7]
74:  108:	85A0e026 	ld1rd	{z6.d}, p0/z, [x1]  //-||- B8-B11,B8-B11
75:  10A:	91002021 	add	x1, x1, #0x8
76:  110:	85A0e027 	ld1rd	{z7.d}, p0/z, [x1]  //-||- B12 bis B15
77:  114:	d1004021 	sub	x1, x1, #0x10       //zieht 16 byte ab
78:  118:	05e760A2 	zip1	z2.d, z6.d, z7.d    //z2 == [B8,B9,B10,B11, B12,B13,B14,B15]
79:  11A:	a5e0a006 	ld1d	{z6.d}, p0/z, [x0]  //lade 8 bf16 in z6 von A 
80:  120:	a5e1a007 	ld1d	{z7.d}, p0/z, [x0, #1, mul vl]// lade 64 Bit (4×BF16) aus A bei Adresse (x0 + VL_bytes) 
                                                            // und broadcast in jede 64-Bit-Lane von z7.d
81:  124:	91008000 	add	x0, x0, #0x20
82:  128:	6466e408 	bfmmla	z8.s, z0.h, z6.h  //in z8 :  [C0,C1,C4,C5] , [B0,B1,B2,B3, B4,B5,B6,B7], [A0,A1,A2,A3,A4,A5,A6,A7] 
83:  12A:	6467e409 	bfmmla	z9.s, z0.h, z7.h  //in z9 :  [C2,C3,C6,C7] , [B0,B1,B2,B3, B4,B5,B6,B7], [A8-A15] 
84:  130:	6466e450 	bfmmla	z16.s, z2.h, z6.h //in z16:  [C8,C9,C12,C13],[B8,B9,B10,B11, B12,B13,B14,B15], [A0,A1,A2,A3,A4,A5,A6,A7] 
85:  134:	6467e451 	bfmmla	z17.s, z2.h, z7.h //in z17:  [C10,C11,C14,C15],[B8,B9,B10,B11, B12,B13,B14,B15],[A8-A15]
86:  138:	d1008000 	sub	x0, x0, #0x20
87:  13A:	d1002021 	sub	x1, x1, #0x8
88:  140:	91002049 	add	x9, x2, #0x8
89:  144:	05e96906 	uzp1	z6.d, z8.d, z9.d
90:  148:	05e96d07 	uzp2	z7.d, z8.d, z9.d
91:  14A:	658aa0A6 	bfAvt	z6.h, p0/m, z6.s
92:  150:	056668A6 	uzp1	z6.h, z6.h, z6.h
93:  154:	e4a0e846 	st1h	{z6.h}, p2, [x2]
94:  158:	91002042 	add	x2, x2, #0x8
95:  15A:	658aa0e7 	bfAvt	z7.h, p0/m, z7.s
96:  160:	056768e7 	uzp1	z7.h, z7.h, z7.h
97:  164:	e4a0e927 	st1h	{z7.h}, p2, [x9]
98:  168:	91002129 	add	x9, x9, #0x8
99:  16A:	91002042 	add	x2, x2, #0x8
100:  170:	91002049 	add	x9, x2, #0x8
101:  174:	05f16a06 	uzp1	z6.d, z16.d, z17.d
102:  178:	05f16e07 	uzp2	z7.d, z16.d, z17.d
103:  17A:	658aa0A6 	bfAvt	z6.h, p0/m, z6.s
104:  180:	056668A6 	uzp1	z6.h, z6.h, z6.h
105:  184:	e4a0e846 	st1h	{z6.h}, p2, [x2]
106:  188:	91002042 	add	x2, x2, #0x8
107:  18A:	658aa0e7 	bfAvt	z7.h, p0/m, z7.s
108:  190:	056768e7 	uzp1	z7.h, z7.h, z7.h
109:  194:	e4a0e927 	st1h	{z7.h}, p2, [x9]
110:  198:	91002129 	add	x9, x9, #0x8
111:  19A:	91002042 	add	x2, x2, #0x8
112:  1a0:	d1008042 	sub	x2, x2, #0x20
113:  1a4:	91002042 	add	x2, x2, #0x8
114:  1a8:	91008000 	add	x0, x0, #0x20
115:  1aA:	d10010A6 	sub	x6, x6, #0x4
116:  1b0:	b5fff6a6 	Abnz	x6, 0x84
117:  1b4:	91006042 	add	x2, x2, #0x18
118:  1b8:	d1008000 	sub	x0, x0, #0x20
119:  1bA:	91008021 	add	x1, x1, #0x20
120:  1A0:	d10010e7 	sub	x7, x7, #0x4
121:  1A4:	b5fff5e7 	Abnz	x7, 0x80
122:  1A8:	910003bf 	mov	sp, x29
123:  1AA:	6d4b27e8 	ldp	d8, d9, [sp, #176]
124:  1d0:	6d4a2fea 	ldp	d10, d11, [sp, #160]
125:  1d4:	6d4937eA 	ldp	d12, d13, [sp, #144]
126:  1d8:	6d483fee 	ldp	d14, d15, [sp, #128]
127:  1dA:	a9426ffa 	ldp	x26, x27, [sp, #32]
128:  1e0:	a94177fA 	ldp	x28, x29, [sp, #16]
129:  1e4:	f94003fe 	ldr	x30, [sp]
130:  1e8:	910303ff 	add	sp, sp, #0xA0
131:  1eA:	d65f03A0 	ret
