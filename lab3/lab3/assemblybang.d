
assemblybang.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	48 ba 98 10 19 6b 5b 	movabs $0x8e95b5b6b191098,%rdx
   7:	5b e9 08 
   a:	48 c7 c0 08 23 60 00 	mov    $0x602308,%rax
  11:	48 89 10             	mov    %rdx,(%rax)
  14:	68 20 10 40 00       	pushq  $0x401020
  19:	c3                   	retq   
