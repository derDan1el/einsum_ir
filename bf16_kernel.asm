
libxsmm_abi1_adl_bf16_nn_1x20x1_1_80_1_a1_b1_p0_br0_sa0_sb0_uh0_si0_tc-abid_avnni0_bvnni0_cvnni0.mxm:     file format binary


Disassembly of section .data:

0000000000000000 <.data>:
   0:	53                   	push   rbx
   1:	41 54                	push   r12
   3:	41 55                	push   r13
   5:	41 56                	push   r14
   7:	41 57                	push   r15
   9:	4c 8b ff             	mov    r15,rdi
   c:	49 8b 7f 20          	mov    rdi,QWORD PTR [r15+0x20]
  10:	49 8b 77 50          	mov    rsi,QWORD PTR [r15+0x50]
  14:	49 8b 97 80 00 00 00 	mov    rdx,QWORD PTR [r15+0x80]
  1b:	55                   	push   rbp
  1c:	48 8b ec             	mov    rbp,rsp
  1f:	48 81 ec c0 00 00 00 	sub    rsp,0xc0
  26:	49 ba c0 ff ff ff ff 	movabs r10,0xffffffffffffffc0
  2d:	ff ff ff 
  30:	49 23 e2             	and    rsp,r10
  33:	48 81 ec 40 00 00 00 	sub    rsp,0x40
  3a:	48 89 65 98          	mov    QWORD PTR [rbp-0x68],rsp
  3e:	48 81 ec 40 00 00 00 	sub    rsp,0x40
  45:	48 89 65 90          	mov    QWORD PTR [rbp-0x70],rsp
  49:	49 c7 c3 00 00 00 00 	mov    r11,0x0
  50:	49 81 c3 03 00 00 00 	add    r11,0x3
  57:	eb 20                	jmp    0x79
  59:	ff                   	(bad)  
  5a:	ff                   	(bad)  
  5b:	ff                   	(bad)  
  5c:	ff 00                	inc    DWORD PTR [rax]
	...
  76:	00 00                	add    BYTE PTR [rax],al
  78:	00 c5                	add    ch,al
  7a:	fc                   	cld    
  7b:	10 05 d8 ff ff ff    	adc    BYTE PTR [rip+0xffffffffffffffd8],al        # 0x59
  81:	4c 8b 7d 98          	mov    r15,QWORD PTR [rbp-0x68]
  85:	c4 81 7c 11 07       	vmovups YMMWORD PTR [r15],ymm0
  8a:	49 c7 c2 00 00 00 00 	mov    r10,0x0
  91:	49 81 c2 01 00 00 00 	add    r10,0x1
  98:	48 81 ec 20 00 00 00 	sub    rsp,0x20
  9f:	c4 01 15 ef ed       	vpxor  ymm13,ymm13,ymm13
  a4:	c4 61 7c 11 2c 24    	vmovups YMMWORD PTR [rsp],ymm13
  aa:	66 44 8b 32          	mov    r14w,WORD PTR [rdx]
  ae:	66 44 89 34 24       	mov    WORD PTR [rsp],r14w
  b3:	c4 61 7c 10 2c 24    	vmovups ymm13,YMMWORD PTR [rsp]
  b9:	48 81 c4 20 00 00 00 	add    rsp,0x20
  c0:	c4 02 7d 23 ed       	vpmovsxwd ymm13,xmm13
  c5:	c4 81 15 72 f5 10    	vpslld ymm13,ymm13,0x10
  cb:	48 81 ec 20 00 00 00 	sub    rsp,0x20
  d2:	c4 01 0d ef f6       	vpxor  ymm14,ymm14,ymm14
  d7:	c4 61 7c 11 34 24    	vmovups YMMWORD PTR [rsp],ymm14
  dd:	66 44 8b 72 02       	mov    r14w,WORD PTR [rdx+0x2]
  e2:	66 44 89 34 24       	mov    WORD PTR [rsp],r14w
  e7:	c4 61 7c 10 34 24    	vmovups ymm14,YMMWORD PTR [rsp]
  ed:	48 81 c4 20 00 00 00 	add    rsp,0x20
  f4:	c4 02 7d 23 f6       	vpmovsxwd ymm14,xmm14
  f9:	c4 81 0d 72 f6 10    	vpslld ymm14,ymm14,0x10
  ff:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 106:	c4 01 05 ef ff       	vpxor  ymm15,ymm15,ymm15
 10b:	c4 61 7c 11 3c 24    	vmovups YMMWORD PTR [rsp],ymm15
 111:	66 44 8b 72 04       	mov    r14w,WORD PTR [rdx+0x4]
 116:	66 44 89 34 24       	mov    WORD PTR [rsp],r14w
 11b:	c4 61 7c 10 3c 24    	vmovups ymm15,YMMWORD PTR [rsp]
 121:	48 81 c4 20 00 00 00 	add    rsp,0x20
 128:	c4 02 7d 23 ff       	vpmovsxwd ymm15,xmm15
 12d:	c4 81 05 72 f7 10    	vpslld ymm15,ymm15,0x10
 133:	c4 a2 7d 79 06       	vpbroadcastw ymm0,WORD PTR [rsi]
 138:	c4 a1 7d 72 f0 10    	vpslld ymm0,ymm0,0x10
 13e:	c4 a2 7d 79 8e a0 00 	vpbroadcastw ymm1,WORD PTR [rsi+0xa0]
 145:	00 00 
 147:	c4 a1 75 72 f1 10    	vpslld ymm1,ymm1,0x10
 14d:	c4 a2 7d 79 96 40 01 	vpbroadcastw ymm2,WORD PTR [rsi+0x140]
 154:	00 00 
 156:	c4 a1 6d 72 f2 10    	vpslld ymm2,ymm2,0x10
 15c:	48 81 c6 02 00 00 00 	add    rsi,0x2
 163:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 16a:	c4 a1 65 ef db       	vpxor  ymm3,ymm3,ymm3
 16f:	c4 e1 7c 11 1c 24    	vmovups YMMWORD PTR [rsp],ymm3
 175:	66 44 8b 37          	mov    r14w,WORD PTR [rdi]
 179:	66 44 89 34 24       	mov    WORD PTR [rsp],r14w
 17e:	c4 e1 7c 10 1c 24    	vmovups ymm3,YMMWORD PTR [rsp]
 184:	48 81 c4 20 00 00 00 	add    rsp,0x20
 18b:	c4 a2 7d 23 db       	vpmovsxwd ymm3,xmm3
 190:	c4 a1 65 72 f3 10    	vpslld ymm3,ymm3,0x10
 196:	c4 22 7d b8 eb       	vfmadd231ps ymm13,ymm0,ymm3
 19b:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 1a2:	c4 a1 65 ef db       	vpxor  ymm3,ymm3,ymm3
 1a7:	c4 e1 7c 11 1c 24    	vmovups YMMWORD PTR [rsp],ymm3
 1ad:	66 44 8b 37          	mov    r14w,WORD PTR [rdi]
 1b1:	66 44 89 34 24       	mov    WORD PTR [rsp],r14w
 1b6:	c4 e1 7c 10 1c 24    	vmovups ymm3,YMMWORD PTR [rsp]
 1bc:	48 81 c4 20 00 00 00 	add    rsp,0x20
 1c3:	c4 a2 7d 23 db       	vpmovsxwd ymm3,xmm3
 1c8:	c4 a1 65 72 f3 10    	vpslld ymm3,ymm3,0x10
 1ce:	c4 22 75 b8 f3       	vfmadd231ps ymm14,ymm1,ymm3
 1d3:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 1da:	c4 a1 65 ef db       	vpxor  ymm3,ymm3,ymm3
 1df:	c4 e1 7c 11 1c 24    	vmovups YMMWORD PTR [rsp],ymm3
 1e5:	66 44 8b 37          	mov    r14w,WORD PTR [rdi]
 1e9:	66 44 89 34 24       	mov    WORD PTR [rsp],r14w
 1ee:	c4 e1 7c 10 1c 24    	vmovups ymm3,YMMWORD PTR [rsp]
 1f4:	48 81 c4 20 00 00 00 	add    rsp,0x20
 1fb:	c4 a2 7d 23 db       	vpmovsxwd ymm3,xmm3
 200:	c4 a1 65 72 f3 10    	vpslld ymm3,ymm3,0x10
 206:	48 81 c7 02 00 00 00 	add    rdi,0x2
 20d:	c4 22 6d b8 fb       	vfmadd231ps ymm15,ymm2,ymm3
 212:	48 81 ee 02 00 00 00 	sub    rsi,0x2
 219:	48 81 ec a0 00 00 00 	sub    rsp,0xa0
 220:	c4 e1 7c 11 84 24 80 	vmovups YMMWORD PTR [rsp+0x80],ymm0
 227:	00 00 00 
 22a:	eb 20                	jmp    0x24c
 22c:	00 00                	add    BYTE PTR [rax],al
 22e:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 232:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 236:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 23a:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 23e:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 242:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 246:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 24a:	80 7f c5 fc          	cmp    BYTE PTR [rdi-0x3b],0xfc
 24e:	10 05 d8 ff ff ff    	adc    BYTE PTR [rip+0xffffffffffffffd8],al        # 0x22c
 254:	c4 e1 7c 11 44 24 60 	vmovups YMMWORD PTR [rsp+0x60],ymm0
 25b:	eb 20                	jmp    0x27d
 25d:	01 00                	add    DWORD PTR [rax],eax
 25f:	00 00                	add    BYTE PTR [rax],al
 261:	01 00                	add    DWORD PTR [rax],eax
 263:	00 00                	add    BYTE PTR [rax],al
 265:	01 00                	add    DWORD PTR [rax],eax
 267:	00 00                	add    BYTE PTR [rax],al
 269:	01 00                	add    DWORD PTR [rax],eax
 26b:	00 00                	add    BYTE PTR [rax],al
 26d:	01 00                	add    DWORD PTR [rax],eax
 26f:	00 00                	add    BYTE PTR [rax],al
 271:	01 00                	add    DWORD PTR [rax],eax
 273:	00 00                	add    BYTE PTR [rax],al
 275:	01 00                	add    DWORD PTR [rax],eax
 277:	00 00                	add    BYTE PTR [rax],al
 279:	01 00                	add    DWORD PTR [rax],eax
 27b:	00 00                	add    BYTE PTR [rax],al
 27d:	c5 fc 10 05 d8 ff ff 	vmovups ymm0,YMMWORD PTR [rip+0xffffffffffffffd8]        # 0x25d
 284:	ff 
 285:	c4 e1 7c 11 44 24 40 	vmovups YMMWORD PTR [rsp+0x40],ymm0
 28c:	eb 20                	jmp    0x2ae
 28e:	ff                   	(bad)  
 28f:	7f 00                	jg     0x291
 291:	00 ff                	add    bh,bh
 293:	7f 00                	jg     0x295
 295:	00 ff                	add    bh,bh
 297:	7f 00                	jg     0x299
 299:	00 ff                	add    bh,bh
 29b:	7f 00                	jg     0x29d
 29d:	00 ff                	add    bh,bh
 29f:	7f 00                	jg     0x2a1
 2a1:	00 ff                	add    bh,bh
 2a3:	7f 00                	jg     0x2a5
 2a5:	00 ff                	add    bh,bh
 2a7:	7f 00                	jg     0x2a9
 2a9:	00 ff                	add    bh,bh
 2ab:	7f 00                	jg     0x2ad
 2ad:	00 c5                	add    ch,al
 2af:	fc                   	cld    
 2b0:	10 05 d8 ff ff ff    	adc    BYTE PTR [rip+0xffffffffffffffd8],al        # 0x28e
 2b6:	c4 e1 7c 11 44 24 20 	vmovups YMMWORD PTR [rsp+0x20],ymm0
 2bd:	eb 20                	jmp    0x2df
 2bf:	00 01                	add    BYTE PTR [rcx],al
 2c1:	04 05                	add    al,0x5
 2c3:	08 09                	or     BYTE PTR [rcx],cl
 2c5:	0c 0d                	or     al,0xd
 2c7:	80 80 80 80 80 80 80 	add    BYTE PTR [rax-0x7f7f7f80],0x80
 2ce:	80 80 80 80 80 80 80 	add    BYTE PTR [rax-0x7f7f7f80],0x80
 2d5:	80 80 00 01 04 05 08 	add    BYTE PTR [rax+0x5040100],0x8
 2dc:	09 0c 0d c5 fc 10 05 	or     DWORD PTR [rcx*1+0x510fcc5],ecx
 2e3:	d8 ff                	fdivr  st,st(7)
 2e5:	ff                   	(bad)  
 2e6:	ff c4                	inc    esp
 2e8:	e1 7c                	loope  0x366
 2ea:	11 04 24             	adc    DWORD PTR [rsp],eax
 2ed:	c4 e1 7c 10 84 24 80 	vmovups ymm0,YMMWORD PTR [rsp+0x80]
 2f4:	00 00 00 
 2f7:	c4 e1 15 db 4c 24 60 	vpand  ymm1,ymm13,YMMWORD PTR [rsp+0x60]
 2fe:	c4 e1 75 76 4c 24 60 	vpcmpeqd ymm1,ymm1,YMMWORD PTR [rsp+0x60]
 305:	c4 81 6d 72 e5 10    	vpsrad ymm2,ymm13,0x10
 30b:	c4 e1 6d db 54 24 40 	vpand  ymm2,ymm2,YMMWORD PTR [rsp+0x40]
 312:	c4 e1 6d fe 54 24 20 	vpaddd ymm2,ymm2,YMMWORD PTR [rsp+0x20]
 319:	c4 81 6d fe d5       	vpaddd ymm2,ymm2,ymm13
 31e:	c4 81 7c 10 c5       	vmovups ymm0,ymm13
 323:	c4 a3 6d 4a c0 10    	vblendvps ymm0,ymm2,ymm0,ymm1
 329:	c4 a1 7d 72 e0 10    	vpsrad ymm0,ymm0,0x10
 32f:	c4 e2 7d 00 04 24    	vpshufb ymm0,ymm0,YMMWORD PTR [rsp]
 335:	c4 a3 7d 39 c1 01    	vextracti128 xmm1,ymm0,0x1
 33b:	c4 a1 75 eb c0       	vpor   ymm0,ymm1,ymm0
 340:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 347:	c4 e1 7c 11 04 24    	vmovups YMMWORD PTR [rsp],ymm0
 34d:	66 44 8b 34 24       	mov    r14w,WORD PTR [rsp]
 352:	66 44 89 32          	mov    WORD PTR [rdx],r14w
 356:	48 81 c4 20 00 00 00 	add    rsp,0x20
 35d:	c4 e1 0d db 4c 24 60 	vpand  ymm1,ymm14,YMMWORD PTR [rsp+0x60]
 364:	c4 e1 75 76 4c 24 60 	vpcmpeqd ymm1,ymm1,YMMWORD PTR [rsp+0x60]
 36b:	c4 81 6d 72 e6 10    	vpsrad ymm2,ymm14,0x10
 371:	c4 e1 6d db 54 24 40 	vpand  ymm2,ymm2,YMMWORD PTR [rsp+0x40]
 378:	c4 e1 6d fe 54 24 20 	vpaddd ymm2,ymm2,YMMWORD PTR [rsp+0x20]
 37f:	c4 81 6d fe d6       	vpaddd ymm2,ymm2,ymm14
 384:	c4 81 7c 10 c6       	vmovups ymm0,ymm14
 389:	c4 a3 6d 4a c0 10    	vblendvps ymm0,ymm2,ymm0,ymm1
 38f:	c4 a1 7d 72 e0 10    	vpsrad ymm0,ymm0,0x10
 395:	c4 e2 7d 00 04 24    	vpshufb ymm0,ymm0,YMMWORD PTR [rsp]
 39b:	c4 a3 7d 39 c1 01    	vextracti128 xmm1,ymm0,0x1
 3a1:	c4 a1 75 eb c0       	vpor   ymm0,ymm1,ymm0
 3a6:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 3ad:	c4 e1 7c 11 04 24    	vmovups YMMWORD PTR [rsp],ymm0
 3b3:	66 44 8b 34 24       	mov    r14w,WORD PTR [rsp]
 3b8:	66 44 89 72 02       	mov    WORD PTR [rdx+0x2],r14w
 3bd:	48 81 c4 20 00 00 00 	add    rsp,0x20
 3c4:	c4 e1 05 db 4c 24 60 	vpand  ymm1,ymm15,YMMWORD PTR [rsp+0x60]
 3cb:	c4 e1 75 76 4c 24 60 	vpcmpeqd ymm1,ymm1,YMMWORD PTR [rsp+0x60]
 3d2:	c4 81 6d 72 e7 10    	vpsrad ymm2,ymm15,0x10
 3d8:	c4 e1 6d db 54 24 40 	vpand  ymm2,ymm2,YMMWORD PTR [rsp+0x40]
 3df:	c4 e1 6d fe 54 24 20 	vpaddd ymm2,ymm2,YMMWORD PTR [rsp+0x20]
 3e6:	c4 81 6d fe d7       	vpaddd ymm2,ymm2,ymm15
 3eb:	c4 81 7c 10 c7       	vmovups ymm0,ymm15
 3f0:	c4 a3 6d 4a c0 10    	vblendvps ymm0,ymm2,ymm0,ymm1
 3f6:	c4 a1 7d 72 e0 10    	vpsrad ymm0,ymm0,0x10
 3fc:	c4 e2 7d 00 04 24    	vpshufb ymm0,ymm0,YMMWORD PTR [rsp]
 402:	c4 a3 7d 39 c1 01    	vextracti128 xmm1,ymm0,0x1
 408:	c4 a1 75 eb c0       	vpor   ymm0,ymm1,ymm0
 40d:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 414:	c4 e1 7c 11 04 24    	vmovups YMMWORD PTR [rsp],ymm0
 41a:	66 44 8b 34 24       	mov    r14w,WORD PTR [rsp]
 41f:	66 44 89 72 04       	mov    WORD PTR [rdx+0x4],r14w
 424:	48 81 c4 20 00 00 00 	add    rsp,0x20
 42b:	48 81 c4 a0 00 00 00 	add    rsp,0xa0
 432:	48 81 c2 02 00 00 00 	add    rdx,0x2
 439:	48 81 ef 00 00 00 00 	sub    rdi,0x0
 440:	49 81 fa 01 00 00 00 	cmp    r10,0x1
 447:	0f 8c 44 fc ff ff    	jl     0x91
 44d:	48 81 c2 04 00 00 00 	add    rdx,0x4
 454:	48 81 c6 e0 01 00 00 	add    rsi,0x1e0
 45b:	48 81 ef 02 00 00 00 	sub    rdi,0x2
 462:	49 81 fb 12 00 00 00 	cmp    r11,0x12
 469:	0f 8c e1 fb ff ff    	jl     0x50
 46f:	49 c7 c3 12 00 00 00 	mov    r11,0x12
 476:	49 81 c3 02 00 00 00 	add    r11,0x2
 47d:	eb 20                	jmp    0x49f
 47f:	ff                   	(bad)  
 480:	ff                   	(bad)  
 481:	ff                   	(bad)  
 482:	ff 00                	inc    DWORD PTR [rax]
	...
 49c:	00 00                	add    BYTE PTR [rax],al
 49e:	00 c5                	add    ch,al
 4a0:	fc                   	cld    
 4a1:	10 05 d8 ff ff ff    	adc    BYTE PTR [rip+0xffffffffffffffd8],al        # 0x47f
 4a7:	4c 8b 7d 98          	mov    r15,QWORD PTR [rbp-0x68]
 4ab:	c4 81 7c 11 07       	vmovups YMMWORD PTR [r15],ymm0
 4b0:	49 c7 c2 00 00 00 00 	mov    r10,0x0
 4b7:	49 81 c2 01 00 00 00 	add    r10,0x1
 4be:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 4c5:	c4 01 0d ef f6       	vpxor  ymm14,ymm14,ymm14
 4ca:	c4 61 7c 11 34 24    	vmovups YMMWORD PTR [rsp],ymm14
 4d0:	66 44 8b 32          	mov    r14w,WORD PTR [rdx]
 4d4:	66 44 89 34 24       	mov    WORD PTR [rsp],r14w
 4d9:	c4 61 7c 10 34 24    	vmovups ymm14,YMMWORD PTR [rsp]
 4df:	48 81 c4 20 00 00 00 	add    rsp,0x20
 4e6:	c4 02 7d 23 f6       	vpmovsxwd ymm14,xmm14
 4eb:	c4 81 0d 72 f6 10    	vpslld ymm14,ymm14,0x10
 4f1:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 4f8:	c4 01 05 ef ff       	vpxor  ymm15,ymm15,ymm15
 4fd:	c4 61 7c 11 3c 24    	vmovups YMMWORD PTR [rsp],ymm15
 503:	66 44 8b 72 02       	mov    r14w,WORD PTR [rdx+0x2]
 508:	66 44 89 34 24       	mov    WORD PTR [rsp],r14w
 50d:	c4 61 7c 10 3c 24    	vmovups ymm15,YMMWORD PTR [rsp]
 513:	48 81 c4 20 00 00 00 	add    rsp,0x20
 51a:	c4 02 7d 23 ff       	vpmovsxwd ymm15,xmm15
 51f:	c4 81 05 72 f7 10    	vpslld ymm15,ymm15,0x10
 525:	c4 a2 7d 79 06       	vpbroadcastw ymm0,WORD PTR [rsi]
 52a:	c4 a1 7d 72 f0 10    	vpslld ymm0,ymm0,0x10
 530:	c4 a2 7d 79 8e a0 00 	vpbroadcastw ymm1,WORD PTR [rsi+0xa0]
 537:	00 00 
 539:	c4 a1 75 72 f1 10    	vpslld ymm1,ymm1,0x10
 53f:	48 81 c6 02 00 00 00 	add    rsi,0x2
 546:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 54d:	c4 a1 6d ef d2       	vpxor  ymm2,ymm2,ymm2
 552:	c4 e1 7c 11 14 24    	vmovups YMMWORD PTR [rsp],ymm2
 558:	66 44 8b 37          	mov    r14w,WORD PTR [rdi]
 55c:	66 44 89 34 24       	mov    WORD PTR [rsp],r14w
 561:	c4 e1 7c 10 14 24    	vmovups ymm2,YMMWORD PTR [rsp]
 567:	48 81 c4 20 00 00 00 	add    rsp,0x20
 56e:	c4 a2 7d 23 d2       	vpmovsxwd ymm2,xmm2
 573:	c4 a1 6d 72 f2 10    	vpslld ymm2,ymm2,0x10
 579:	c4 22 7d b8 f2       	vfmadd231ps ymm14,ymm0,ymm2
 57e:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 585:	c4 a1 6d ef d2       	vpxor  ymm2,ymm2,ymm2
 58a:	c4 e1 7c 11 14 24    	vmovups YMMWORD PTR [rsp],ymm2
 590:	66 44 8b 37          	mov    r14w,WORD PTR [rdi]
 594:	66 44 89 34 24       	mov    WORD PTR [rsp],r14w
 599:	c4 e1 7c 10 14 24    	vmovups ymm2,YMMWORD PTR [rsp]
 59f:	48 81 c4 20 00 00 00 	add    rsp,0x20
 5a6:	c4 a2 7d 23 d2       	vpmovsxwd ymm2,xmm2
 5ab:	c4 a1 6d 72 f2 10    	vpslld ymm2,ymm2,0x10
 5b1:	48 81 c7 02 00 00 00 	add    rdi,0x2
 5b8:	c4 22 75 b8 fa       	vfmadd231ps ymm15,ymm1,ymm2
 5bd:	48 81 ee 02 00 00 00 	sub    rsi,0x2
 5c4:	48 81 ec a0 00 00 00 	sub    rsp,0xa0
 5cb:	c4 e1 7c 11 84 24 80 	vmovups YMMWORD PTR [rsp+0x80],ymm0
 5d2:	00 00 00 
 5d5:	eb 20                	jmp    0x5f7
 5d7:	00 00                	add    BYTE PTR [rax],al
 5d9:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 5dd:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 5e1:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 5e5:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 5e9:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 5ed:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 5f1:	80 7f 00 00          	cmp    BYTE PTR [rdi+0x0],0x0
 5f5:	80 7f c5 fc          	cmp    BYTE PTR [rdi-0x3b],0xfc
 5f9:	10 05 d8 ff ff ff    	adc    BYTE PTR [rip+0xffffffffffffffd8],al        # 0x5d7
 5ff:	c4 e1 7c 11 44 24 60 	vmovups YMMWORD PTR [rsp+0x60],ymm0
 606:	eb 20                	jmp    0x628
 608:	01 00                	add    DWORD PTR [rax],eax
 60a:	00 00                	add    BYTE PTR [rax],al
 60c:	01 00                	add    DWORD PTR [rax],eax
 60e:	00 00                	add    BYTE PTR [rax],al
 610:	01 00                	add    DWORD PTR [rax],eax
 612:	00 00                	add    BYTE PTR [rax],al
 614:	01 00                	add    DWORD PTR [rax],eax
 616:	00 00                	add    BYTE PTR [rax],al
 618:	01 00                	add    DWORD PTR [rax],eax
 61a:	00 00                	add    BYTE PTR [rax],al
 61c:	01 00                	add    DWORD PTR [rax],eax
 61e:	00 00                	add    BYTE PTR [rax],al
 620:	01 00                	add    DWORD PTR [rax],eax
 622:	00 00                	add    BYTE PTR [rax],al
 624:	01 00                	add    DWORD PTR [rax],eax
 626:	00 00                	add    BYTE PTR [rax],al
 628:	c5 fc 10 05 d8 ff ff 	vmovups ymm0,YMMWORD PTR [rip+0xffffffffffffffd8]        # 0x608
 62f:	ff 
 630:	c4 e1 7c 11 44 24 40 	vmovups YMMWORD PTR [rsp+0x40],ymm0
 637:	eb 20                	jmp    0x659
 639:	ff                   	(bad)  
 63a:	7f 00                	jg     0x63c
 63c:	00 ff                	add    bh,bh
 63e:	7f 00                	jg     0x640
 640:	00 ff                	add    bh,bh
 642:	7f 00                	jg     0x644
 644:	00 ff                	add    bh,bh
 646:	7f 00                	jg     0x648
 648:	00 ff                	add    bh,bh
 64a:	7f 00                	jg     0x64c
 64c:	00 ff                	add    bh,bh
 64e:	7f 00                	jg     0x650
 650:	00 ff                	add    bh,bh
 652:	7f 00                	jg     0x654
 654:	00 ff                	add    bh,bh
 656:	7f 00                	jg     0x658
 658:	00 c5                	add    ch,al
 65a:	fc                   	cld    
 65b:	10 05 d8 ff ff ff    	adc    BYTE PTR [rip+0xffffffffffffffd8],al        # 0x639
 661:	c4 e1 7c 11 44 24 20 	vmovups YMMWORD PTR [rsp+0x20],ymm0
 668:	eb 20                	jmp    0x68a
 66a:	00 01                	add    BYTE PTR [rcx],al
 66c:	04 05                	add    al,0x5
 66e:	08 09                	or     BYTE PTR [rcx],cl
 670:	0c 0d                	or     al,0xd
 672:	80 80 80 80 80 80 80 	add    BYTE PTR [rax-0x7f7f7f80],0x80
 679:	80 80 80 80 80 80 80 	add    BYTE PTR [rax-0x7f7f7f80],0x80
 680:	80 80 00 01 04 05 08 	add    BYTE PTR [rax+0x5040100],0x8
 687:	09 0c 0d c5 fc 10 05 	or     DWORD PTR [rcx*1+0x510fcc5],ecx
 68e:	d8 ff                	fdivr  st,st(7)
 690:	ff                   	(bad)  
 691:	ff c4                	inc    esp
 693:	e1 7c                	loope  0x711
 695:	11 04 24             	adc    DWORD PTR [rsp],eax
 698:	c4 e1 7c 10 84 24 80 	vmovups ymm0,YMMWORD PTR [rsp+0x80]
 69f:	00 00 00 
 6a2:	c4 e1 0d db 4c 24 60 	vpand  ymm1,ymm14,YMMWORD PTR [rsp+0x60]
 6a9:	c4 e1 75 76 4c 24 60 	vpcmpeqd ymm1,ymm1,YMMWORD PTR [rsp+0x60]
 6b0:	c4 81 6d 72 e6 10    	vpsrad ymm2,ymm14,0x10
 6b6:	c4 e1 6d db 54 24 40 	vpand  ymm2,ymm2,YMMWORD PTR [rsp+0x40]
 6bd:	c4 e1 6d fe 54 24 20 	vpaddd ymm2,ymm2,YMMWORD PTR [rsp+0x20]
 6c4:	c4 81 6d fe d6       	vpaddd ymm2,ymm2,ymm14
 6c9:	c4 81 7c 10 c6       	vmovups ymm0,ymm14
 6ce:	c4 a3 6d 4a c0 10    	vblendvps ymm0,ymm2,ymm0,ymm1
 6d4:	c4 a1 7d 72 e0 10    	vpsrad ymm0,ymm0,0x10
 6da:	c4 e2 7d 00 04 24    	vpshufb ymm0,ymm0,YMMWORD PTR [rsp]
 6e0:	c4 a3 7d 39 c1 01    	vextracti128 xmm1,ymm0,0x1
 6e6:	c4 a1 75 eb c0       	vpor   ymm0,ymm1,ymm0
 6eb:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 6f2:	c4 e1 7c 11 04 24    	vmovups YMMWORD PTR [rsp],ymm0
 6f8:	66 44 8b 34 24       	mov    r14w,WORD PTR [rsp]
 6fd:	66 44 89 32          	mov    WORD PTR [rdx],r14w
 701:	48 81 c4 20 00 00 00 	add    rsp,0x20
 708:	c4 e1 05 db 4c 24 60 	vpand  ymm1,ymm15,YMMWORD PTR [rsp+0x60]
 70f:	c4 e1 75 76 4c 24 60 	vpcmpeqd ymm1,ymm1,YMMWORD PTR [rsp+0x60]
 716:	c4 81 6d 72 e7 10    	vpsrad ymm2,ymm15,0x10
 71c:	c4 e1 6d db 54 24 40 	vpand  ymm2,ymm2,YMMWORD PTR [rsp+0x40]
 723:	c4 e1 6d fe 54 24 20 	vpaddd ymm2,ymm2,YMMWORD PTR [rsp+0x20]
 72a:	c4 81 6d fe d7       	vpaddd ymm2,ymm2,ymm15
 72f:	c4 81 7c 10 c7       	vmovups ymm0,ymm15
 734:	c4 a3 6d 4a c0 10    	vblendvps ymm0,ymm2,ymm0,ymm1
 73a:	c4 a1 7d 72 e0 10    	vpsrad ymm0,ymm0,0x10
 740:	c4 e2 7d 00 04 24    	vpshufb ymm0,ymm0,YMMWORD PTR [rsp]
 746:	c4 a3 7d 39 c1 01    	vextracti128 xmm1,ymm0,0x1
 74c:	c4 a1 75 eb c0       	vpor   ymm0,ymm1,ymm0
 751:	48 81 ec 20 00 00 00 	sub    rsp,0x20
 758:	c4 e1 7c 11 04 24    	vmovups YMMWORD PTR [rsp],ymm0
 75e:	66 44 8b 34 24       	mov    r14w,WORD PTR [rsp]
 763:	66 44 89 72 02       	mov    WORD PTR [rdx+0x2],r14w
 768:	48 81 c4 20 00 00 00 	add    rsp,0x20
 76f:	48 81 c4 a0 00 00 00 	add    rsp,0xa0
 776:	48 81 c2 02 00 00 00 	add    rdx,0x2
 77d:	48 81 ef 00 00 00 00 	sub    rdi,0x0
 784:	49 81 fa 01 00 00 00 	cmp    r10,0x1
 78b:	0f 8c 26 fd ff ff    	jl     0x4b7
 791:	48 81 c2 02 00 00 00 	add    rdx,0x2
 798:	48 81 c6 40 01 00 00 	add    rsi,0x140
 79f:	48 81 ef 02 00 00 00 	sub    rdi,0x2
 7a6:	49 81 fb 14 00 00 00 	cmp    r11,0x14
 7ad:	0f 8c c3 fc ff ff    	jl     0x476
 7b3:	48 8b e5             	mov    rsp,rbp
 7b6:	5d                   	pop    rbp
 7b7:	41 5f                	pop    r15
 7b9:	41 5e                	pop    r14
 7bb:	41 5d                	pop    r13
 7bd:	41 5c                	pop    r12
 7bf:	5b                   	pop    rbx
 7c0:	c3                   	ret    
