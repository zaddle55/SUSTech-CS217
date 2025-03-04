
main:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 2f 00 00 	mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__@Base>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	ret    

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 82 2f 00 00    	push   0x2f82(%rip)        # 3fa8 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 83 2f 00 00 	bnd jmp *0x2f83(%rip)        # 3fb0 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nopl   (%rax)
    1030:	f3 0f 1e fa          	endbr64 
    1034:	68 00 00 00 00       	push   $0x0
    1039:	f2 e9 e1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    103f:	90                   	nop
    1040:	f3 0f 1e fa          	endbr64 
    1044:	68 01 00 00 00       	push   $0x1
    1049:	f2 e9 d1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    104f:	90                   	nop
    1050:	f3 0f 1e fa          	endbr64 
    1054:	68 02 00 00 00       	push   $0x2
    1059:	f2 e9 c1 ff ff ff    	bnd jmp 1020 <_init+0x20>
    105f:	90                   	nop

Disassembly of section .plt.got:

0000000000001060 <__cxa_finalize@plt>:
    1060:	f3 0f 1e fa          	endbr64 
    1064:	f2 ff 25 65 2f 00 00 	bnd jmp *0x2f65(%rip)        # 3fd0 <__cxa_finalize@GLIBC_2.2.5>
    106b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .plt.sec:

0000000000001070 <printf@plt>:
    1070:	f3 0f 1e fa          	endbr64 
    1074:	f2 ff 25 3d 2f 00 00 	bnd jmp *0x2f3d(%rip)        # 3fb8 <printf@GLIBC_2.2.5>
    107b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001080 <__cxa_atexit@plt>:
    1080:	f3 0f 1e fa          	endbr64 
    1084:	f2 ff 25 35 2f 00 00 	bnd jmp *0x2f35(%rip)        # 3fc0 <__cxa_atexit@GLIBC_2.2.5>
    108b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001090 <_ZNSt8ios_base4InitC1Ev@plt>:
    1090:	f3 0f 1e fa          	endbr64 
    1094:	f2 ff 25 2d 2f 00 00 	bnd jmp *0x2f2d(%rip)        # 3fc8 <_ZNSt8ios_base4InitC1Ev@GLIBCXX_3.4>
    109b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .text:

00000000000010a0 <_start>:
    10a0:	f3 0f 1e fa          	endbr64 
    10a4:	31 ed                	xor    %ebp,%ebp
    10a6:	49 89 d1             	mov    %rdx,%r9
    10a9:	5e                   	pop    %rsi
    10aa:	48 89 e2             	mov    %rsp,%rdx
    10ad:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    10b1:	50                   	push   %rax
    10b2:	54                   	push   %rsp
    10b3:	45 31 c0             	xor    %r8d,%r8d
    10b6:	31 c9                	xor    %ecx,%ecx
    10b8:	48 8d 3d ca 00 00 00 	lea    0xca(%rip),%rdi        # 1189 <main>
    10bf:	ff 15 13 2f 00 00    	call   *0x2f13(%rip)        # 3fd8 <__libc_start_main@GLIBC_2.34>
    10c5:	f4                   	hlt    
    10c6:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    10cd:	00 00 00 

00000000000010d0 <deregister_tm_clones>:
    10d0:	48 8d 3d 39 2f 00 00 	lea    0x2f39(%rip),%rdi        # 4010 <__TMC_END__>
    10d7:	48 8d 05 32 2f 00 00 	lea    0x2f32(%rip),%rax        # 4010 <__TMC_END__>
    10de:	48 39 f8             	cmp    %rdi,%rax
    10e1:	74 15                	je     10f8 <deregister_tm_clones+0x28>
    10e3:	48 8b 05 f6 2e 00 00 	mov    0x2ef6(%rip),%rax        # 3fe0 <_ITM_deregisterTMCloneTable@Base>
    10ea:	48 85 c0             	test   %rax,%rax
    10ed:	74 09                	je     10f8 <deregister_tm_clones+0x28>
    10ef:	ff e0                	jmp    *%rax
    10f1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    10f8:	c3                   	ret    
    10f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001100 <register_tm_clones>:
    1100:	48 8d 3d 09 2f 00 00 	lea    0x2f09(%rip),%rdi        # 4010 <__TMC_END__>
    1107:	48 8d 35 02 2f 00 00 	lea    0x2f02(%rip),%rsi        # 4010 <__TMC_END__>
    110e:	48 29 fe             	sub    %rdi,%rsi
    1111:	48 89 f0             	mov    %rsi,%rax
    1114:	48 c1 ee 3f          	shr    $0x3f,%rsi
    1118:	48 c1 f8 03          	sar    $0x3,%rax
    111c:	48 01 c6             	add    %rax,%rsi
    111f:	48 d1 fe             	sar    %rsi
    1122:	74 14                	je     1138 <register_tm_clones+0x38>
    1124:	48 8b 05 c5 2e 00 00 	mov    0x2ec5(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable@Base>
    112b:	48 85 c0             	test   %rax,%rax
    112e:	74 08                	je     1138 <register_tm_clones+0x38>
    1130:	ff e0                	jmp    *%rax
    1132:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    1138:	c3                   	ret    
    1139:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001140 <__do_global_dtors_aux>:
    1140:	f3 0f 1e fa          	endbr64 
    1144:	80 3d c5 2e 00 00 00 	cmpb   $0x0,0x2ec5(%rip)        # 4010 <__TMC_END__>
    114b:	75 2b                	jne    1178 <__do_global_dtors_aux+0x38>
    114d:	55                   	push   %rbp
    114e:	48 83 3d 7a 2e 00 00 	cmpq   $0x0,0x2e7a(%rip)        # 3fd0 <__cxa_finalize@GLIBC_2.2.5>
    1155:	00 
    1156:	48 89 e5             	mov    %rsp,%rbp
    1159:	74 0c                	je     1167 <__do_global_dtors_aux+0x27>
    115b:	48 8b 3d a6 2e 00 00 	mov    0x2ea6(%rip),%rdi        # 4008 <__dso_handle>
    1162:	e8 f9 fe ff ff       	call   1060 <__cxa_finalize@plt>
    1167:	e8 64 ff ff ff       	call   10d0 <deregister_tm_clones>
    116c:	c6 05 9d 2e 00 00 01 	movb   $0x1,0x2e9d(%rip)        # 4010 <__TMC_END__>
    1173:	5d                   	pop    %rbp
    1174:	c3                   	ret    
    1175:	0f 1f 00             	nopl   (%rax)
    1178:	c3                   	ret    
    1179:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001180 <frame_dummy>:
    1180:	f3 0f 1e fa          	endbr64 
    1184:	e9 77 ff ff ff       	jmp    1100 <register_tm_clones>

0000000000001189 <main>:
    1189:	f3 0f 1e fa          	endbr64 
    118d:	55                   	push   %rbp
    118e:	48 89 e5             	mov    %rsp,%rbp
    1191:	48 83 ec 10          	sub    $0x10,%rsp
    1195:	c7 45 f8 ff ff ff 7f 	movl   $0x7fffffff,-0x8(%rbp)
    119c:	c7 45 fc 01 00 00 00 	movl   $0x1,-0x4(%rbp)
    11a3:	8b 55 fc             	mov    -0x4(%rbp),%edx
    11a6:	8b 45 f8             	mov    -0x8(%rbp),%eax
    11a9:	89 d6                	mov    %edx,%esi
    11ab:	89 c7                	mov    %eax,%edi
    11ad:	e8 8c 00 00 00       	call   123e <_Z3addii>
    11b2:	89 c6                	mov    %eax,%esi
    11b4:	48 8d 05 49 0e 00 00 	lea    0xe49(%rip),%rax        # 2004 <_IO_stdin_used+0x4>
    11bb:	48 89 c7             	mov    %rax,%rdi
    11be:	b8 00 00 00 00       	mov    $0x0,%eax
    11c3:	e8 a8 fe ff ff       	call   1070 <printf@plt>
    11c8:	b8 00 00 00 00       	mov    $0x0,%eax
    11cd:	c9                   	leave  
    11ce:	c3                   	ret    

00000000000011cf <_Z41__static_initialization_and_destruction_0ii>:
    11cf:	f3 0f 1e fa          	endbr64 
    11d3:	55                   	push   %rbp
    11d4:	48 89 e5             	mov    %rsp,%rbp
    11d7:	48 83 ec 10          	sub    $0x10,%rsp
    11db:	89 7d fc             	mov    %edi,-0x4(%rbp)
    11de:	89 75 f8             	mov    %esi,-0x8(%rbp)
    11e1:	83 7d fc 01          	cmpl   $0x1,-0x4(%rbp)
    11e5:	75 3b                	jne    1222 <_Z41__static_initialization_and_destruction_0ii+0x53>
    11e7:	81 7d f8 ff ff 00 00 	cmpl   $0xffff,-0x8(%rbp)
    11ee:	75 32                	jne    1222 <_Z41__static_initialization_and_destruction_0ii+0x53>
    11f0:	48 8d 05 1a 2e 00 00 	lea    0x2e1a(%rip),%rax        # 4011 <_ZStL8__ioinit>
    11f7:	48 89 c7             	mov    %rax,%rdi
    11fa:	e8 91 fe ff ff       	call   1090 <_ZNSt8ios_base4InitC1Ev@plt>
    11ff:	48 8d 05 02 2e 00 00 	lea    0x2e02(%rip),%rax        # 4008 <__dso_handle>
    1206:	48 89 c2             	mov    %rax,%rdx
    1209:	48 8d 05 01 2e 00 00 	lea    0x2e01(%rip),%rax        # 4011 <_ZStL8__ioinit>
    1210:	48 89 c6             	mov    %rax,%rsi
    1213:	48 8b 05 de 2d 00 00 	mov    0x2dde(%rip),%rax        # 3ff8 <_ZNSt8ios_base4InitD1Ev@GLIBCXX_3.4>
    121a:	48 89 c7             	mov    %rax,%rdi
    121d:	e8 5e fe ff ff       	call   1080 <__cxa_atexit@plt>
    1222:	90                   	nop
    1223:	c9                   	leave  
    1224:	c3                   	ret    

0000000000001225 <_GLOBAL__sub_I_main>:
    1225:	f3 0f 1e fa          	endbr64 
    1229:	55                   	push   %rbp
    122a:	48 89 e5             	mov    %rsp,%rbp
    122d:	be ff ff 00 00       	mov    $0xffff,%esi
    1232:	bf 01 00 00 00       	mov    $0x1,%edi
    1237:	e8 93 ff ff ff       	call   11cf <_Z41__static_initialization_and_destruction_0ii>
    123c:	5d                   	pop    %rbp
    123d:	c3                   	ret    

000000000000123e <_Z3addii>:
    123e:	f3 0f 1e fa          	endbr64 
    1242:	55                   	push   %rbp
    1243:	48 89 e5             	mov    %rsp,%rbp
    1246:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1249:	89 75 f8             	mov    %esi,-0x8(%rbp)
    124c:	8b 55 fc             	mov    -0x4(%rbp),%edx
    124f:	8b 45 f8             	mov    -0x8(%rbp),%eax
    1252:	01 d0                	add    %edx,%eax
    1254:	5d                   	pop    %rbp
    1255:	c3                   	ret    

Disassembly of section .fini:

0000000000001258 <_fini>:
    1258:	f3 0f 1e fa          	endbr64 
    125c:	48 83 ec 08          	sub    $0x8,%rsp
    1260:	48 83 c4 08          	add    $0x8,%rsp
    1264:	c3                   	ret    
