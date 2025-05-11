
libxsmm_abi1_neov2_bf16_nn_1x2x1_1_1_1_a1_b1_p0_br0_sa0_sb0_uh0_si0_tc-abid_avnni0_bvnni0_cvnni0.mxm:     file format binary


Disassembly of section .data:

0000000000000000 <.data>:
   0:	d10303ff 	sub	sp, sp, #0xc0              //man reserviert sich 192 Byte Speicher
   4:	6d0b27e8 	stp	d8, d9, [sp, #176]         //stp : store pair: speichert die zwei register ab an [sp, #176] : sp + 176 byte : also 192 - 176 = 16 Byte : 2 Mal 8 byte was den d8 und d9 entspricht. zuerst d8 dann d9
   8:	6d0a2fea 	stp	d10, d11, [sp, #160]       //-||- 
   c:	6d0937ec 	stp	d12, d13, [sp, #144]
  10:	6d083fee 	stp	d14, d15, [sp, #128]
  14:	a9026ffa 	stp	x26, x27, [sp, #32]
  18:	a90177fc 	stp	x28, x29, [sp, #16]     //man erkennt das die 192 byte nur dafür waren. diese müssen am ende wieder zurückgeschriebenwerden
  1c:	f90003fe 	str	x30, [sp]
  20:	8a00000a 	and	x10, x0, x0          //kopiert  x0 in x10 weil x0 && x0 = x0 -> x10, in x0 soll  Kernel-Descriptor-Struktur (die alle Inputs/Outputs enthält). sein 
  24:	f9401140 	ldr	x0, [x10, #32]       //ldr (load Register) man nimmt sich den inhalt von x10 da steht die adresse des  Kernel-Descriptor-Struktur drinne darauf dann 32 byte addiert ist die adresse von A[0] (das sind 8 byte)
  28:	f9402941 	ldr	x1, [x10, #80]       // zeiger auf matrix B
  2c:	f9404142 	ldr	x2, [x10, #128]      //zeiger auf matrix C
  30:	910003fd 	mov	x29, sp              //speichert den stackframe pointer
  34:	d10303ff 	sub	sp, sp, #0xc0        //noch mehr platz reservieren
  38:	d29ff80a 	mov	x10, #0xffc0                	// #65472Wir wollen später den Stack-Pointer auf ein 64-Byte-Vielfaches abrunden. Dafür brauchen wir eine Maske, die die niedrigsten 6 Bits löscht (2⁶ = 64):
  3c:	f2bfffea 	movk	x10, #0xffff, lsl #16     // lsl sthet für logical shift left dh 16 bits wird ffff nach links verschoben bevor sie ins zielregister geschoben werden aber schreibt nur die 16 bits des immediates und belässt alle anderen bits so wie ise waren
  40:	f2dfffea 	movk	x10, #0xffff, lsl #32
  44:	f2ffffea 	movk	x10, #0xffff, lsl #48  //ergebnis ist 0xffff_ffff_ffff_ffc0 : das macht man weil man den speicher alligned will gleich auf 64 byte
  48:	910003e9 	mov	x9, sp
  4c:	8a0a0129 	and	x9, x9, x10       // man löscht die niedrigsten 6 bits auf 0 und rundet also ab. damit wird es nochmal verschoben aber x9 enthält nun eine 64 byte grenze was für SVE optimal ist
  50:	9100013f 	mov	sp, x9            //man speichert den alligned speicherort wieder in sp
  54:	d2800049 	mov	x9, #0x2                   	// #2 // in sp steht nun die tolle adresse und wir verwenden x9 weiter um eine 2 zu speichern.
  58:	252917e3 	whilelt	p3.b, xzr, x9        //  setzt 2 bits von links auf 1 der rest ist 0 , man sagt welche "lanes" values aktiv sind in einem Vektor
  5c:	2518e3e0 	ptrue	p0.b                 //setzt alle bits im predicate register p0 auf 1 p0.b heißt jeder btyte bekoimmt ein bit auf 1 gesetzt
  60:	d2800089 	mov	x9, #0x4                   	// #4   //in x9 kommt die 4
  64:	252917e1 	whilelt	p1.b, xzr, x9              //in p1 werden die ersten 4 auf 1 rest 0
  68:	d2800109 	mov	x9, #0x8                   	// #8  setze x9 auf 8
  6c:	252917e2 	whilelt	p2.b, xzr, x9           // p2 bekommt die ersten 8 auf 1
  70:	d2800049 	mov	x9, #0x2                   	// #2    in x9 geht wieder 2
  74:	252917e4 	whilelt	p4.b, xzr, x9              // in p4 werden die ersten 2 auf 1
  78:	d2800047 	mov	x7, #0x2                   	// #2  in x7 die 2
  7c:	d2800026 	mov	x6, #0x1                   	// #1 in x6 die 1
  80:	a4a0b05e 	ld1h	{z30.h}, p4/z, [x2]           //(ersten 2 werte aus C : C00, C01)z ist skalierbare vektorregister die 128 bit - 512 bit breit sein können, man sagt nun das die ersten beiden elemente aus dem C tensor in die z30 vektorlane geladen werden .h sagt es sind halfwords 16 bit und p4 sagt das man 2 lädt und p4/z sagt das man den rest auf 0 setzen soll derlane.  Alle inaktiven Lanes werden Null gesetzt
  //auch der C tensor hat BF16 werte welche in z30 reingeladen werdne die ersten beiden
  84:	05b23bde 	uunpklo	z30.s, z30.h         // uunpklo :"unsigneed unpack low" die 16 bits werden in die unteren 16 bit eines 32 bit slots z30.s[i] kopiert die oberen 16 mit 0en aufgefüllt
  88:	04709fde 	lsl	z30.s, z30.s, #16       //lsl hatte ich vorhin schon und shiften die 16 bit nun 16 nach oben, niederen 16 bits sind 0
  8c:	91000842 	add	x2, x2, #0x2            //auf outputtensoradresse x2 wird 2 byte draufaddiert also geht man ein BF16 weiter (?)
  90:	a4a0b05f 	ld1h	{z31.h}, p4/z, [x2]   // in z31 ist nun C(0,1), C(1,0) geladen weil x2 ja um 2 byte erhöht = 16bit
  94:	05b23bff 	uunpklo	z31.s, z31.h         //es wird wieder verteilt
  98:	04709fff 	lsl	z31.s, z31.s, #16       //es wird wieder auf die ersten 16 bit verschoben
  9c:	d1000842 	sub	x2, x2, #0x2            // es werden 2 abgezogen nun ist x2 wieder auf dem C (0,0)
  a0:	d280004b 	mov	x11, #0x2                   	// #2
  a4:	d29fffca 	mov	x10, #0xfffe                	// #65534
  a8:	f2bfffea 	movk	x10, #0xffff, lsl #16
  ac:	f2dfffea 	movk	x10, #0xffff, lsl #32
  b0:	f2ffffea 	movk	x10, #0xffff, lsl #48         //x10 ← 0xFFFF_FFFF_FFFF_FFFE
  b4:	d2800009 	mov	x9, #0x0                   	// #0
  b8:	04a23042 	eor	z2.d, z2.d, z2.d              //z2.d wird ge xor ed also stehen nur noch 0en drinne in z2.d
  bc:	a4a0ac00 	ld1h	{z0.h}, p3/z, [x0]            //lade die ersten beiden werte aus dem A tensor in z0
  c0:	05626001 	zip1	z1.h, z0.h, z2.h              //zip1 ist eine funktion die die elemente aus z0 und z2 abwechselnd in z1 speichert zuerst daselement aus z0 das erste. dann das erste von z2 dahinter dann das zweite von z0 und dann das zweite von z2 usw
  c4:	91000800 	add	x0, x0, #0x2                  //x0 zeigt auf A01
  c8:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]            //da p0 nur 1 ist werden alle werte von B die reinpassen in x1 gepsiechert
  cc:	91000821 	add	x1, x1, #0x2                  //x2 zeigt auf B01
  d0:	6460803e 	bfdot	z30.s, z1.h, z0.h          //Es wird gerechnen : A00 x B00 + 0 x B00 auf C00 akkumuliert u. C01 += A01 x B00 + 0 x B00 (zweite rechnung ist müll)
  d4:	84c0a020 	ld1rh	{z0.h}, p0/z, [x1]         //bei B wurde vorhin ein element weitergegangen: jetzt wird alles mit dem element mit index 1 gefüllt
  d8:	d1000021 	sub	x1, x1, #0x0               //nop
  dc:	6460803f 	bfdot	z31.s, z1.h, z0.h         //ganz komisch C01 += A00 x B01 + 0 x B01 u. C01+=A01 x B01 + 0 x B01 (zweite rechnung ist wieder müll)
  e0:	8b090000 	add	x0, x0, x9                //nop
  e4:	d1000800 	sub	x0, x0, #0x2              // x0 zeigt auf A00 adr
  e8:	d1000821 	sub	x1, x1, #0x2              // x1 zeigt auf B00 adr 
  ec:	658aa3de 	bfcvt	z30.h, p0/m, z30.s        //BFloat16 ConVerT alle werte werden auf BF16 convertiert auch die 0er
  f0:	057e6bde 	uzp1	z30.h, z30.h, z30.h       //unzip Quelle N : [ N0, N1, N2, N3, N4, N5, N6, N7 ] Quelle M : [ M0, M1, M2, M3, M4, M5, M6, M7 ] ergibt: [N0,  N2,  N4,  N6,M0,  M2,  M4,  M6] die geraden indize von N und dann die geraden von M : resultat : Es steht nur noch ein C00 in dem vektor vorne und danach sonst nur 0en
  f4:	e4a0f05e 	st1h	{z30.h}, p4, [x2]         //Store one halfword : speichert die ersten 2 werte aus z30 ab x2 rein: der erste wert ist das teilergebnis C00 auf das A00 und B00 draufaddiert wurde und die zweite zahl die abgespeichert wird ist eine 0. Man denkt sich jetzt: "aber dann verliere ich den wert an stelle C01! ... das ist aber nicht der fall da dieser noch in z31 ist (auf das auch schon das teilergebnis addiert wurde aus A00 x B01)
  f8:	91000842 	add	x2, x2, #0x2              // x2 zeigt nun auf C01
  fc:	658aa3ff 	bfcvt	z31.h, p0/m, z31.s        //alles wird auf bf16 gerundet.
 100:	057f6bff 	uzp1	z31.h, z31.h, z31.h       //erstes element ist C01 der rest sind 0er
 104:	e4a0f05f 	st1h	{z31.h}, p4, [x2]         //an adresse C01 kommt das C01 was berechnet wurde aber es kommt auch eine 0 an adresse C10 ?!?!?
 108:	d1000842 	sub	x2, x2, #0x2              //x2 zeigt auf C00
 10c:	91000842 	add	x2, x2, #0x2              //x2 zeigt auf C01
 110:	91000800 	add	x0, x0, #0x2              //x0 zeigt auf A01
 114:	d10004c6 	sub	x6, x6, #0x1              //x6 hat nun den wert 0
 118:	b5fffb46 	cbnz	x6, 0x80                  //nicht erfüllt (wenn ungleich null springe zu 0x80 instruction count)
 11c:	91000842 	add	x2, x2, #0x2              //x2 zeigt auf C10
 120:	d1000800 	sub	x0, x0, #0x2              //x0 zeigt auf A00
 124:	91001021 	add	x1, x1, #0x4              //x1 zeigt auf B10
 128:	d10008e7 	sub	x7, x7, #0x2              //x7 ist 0
 12c:	b5fffa87 	cbnz	x7, 0x7c                  //nicht erfüllt muss != 0 sein aber ist 0
 130:	910003bf 	mov	sp, x29                   //in sp ist nun startsp -192 byte
 134:	6d4b27e8 	ldp	d8, d9, [sp, #176]        //ab hier werden nur noch daten wieder zurückgeladen
 138:	6d4a2fea 	ldp	d10, d11, [sp, #160]
 13c:	6d4937ec 	ldp	d12, d13, [sp, #144]
 140:	6d483fee 	ldp	d14, d15, [sp, #128]
 144:	a9426ffa 	ldp	x26, x27, [sp, #32]
 148:	a94177fc 	ldp	x28, x29, [sp, #16]
 14c:	f94003fe 	ldr	x30, [sp]                 //nimm adresse von sp und lade in x30
 150:	910303ff 	add	sp, sp, #0xc0             //sp wird um 192 byte nach oben verschoben
 154:	d65f03c0 	ret
