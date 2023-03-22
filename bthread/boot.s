/****************************************************************************************************
 * the test application program bootup code for MVP
 * every test APP should use this booting header to initilize the MVP
 *
 * Shin-Ying Lee(sing@casmail.ee.ncku.edu.tw)
 * Computer Architecture & System Laboratory
 * Dept. of Electrical Engineering & Inst. of Computer and Communication Engineering
 * National Cheng Kung University, Tainan, Taiwan
 * 2009.12.04
 ****************************************************************************************************/

/* importing external data */
.extern main

/* export global labels and variables */
.global _start

/* text(code) area directive */
.code 32
.text
.section .start, "x", "a"	@ executable & allocatable section
.align 4

/* the code entry */
_start:
	mov r0, #0x0000
	mrs r1, cpsr
	and r1, r1, #0xff00		@ disable IRQ & FIQ, then switch to SYS mode
	orr r1, r1, #0x00df
	msr cpsr_c, r1

/* to setup the stack section */

.equ Mode_USR, 0x10
.equ Mode_FIQ, 0x11
.equ Mode_IRQ, 0x12
.equ Mode_SVC, 0x13
.equ Mode_ABT, 0x17
.equ Mode_UND, 0x1B

.equ I_Bit, 0x80 @ when I bit is set, IRQ is disabled
.equ F_Bit, 0x40 @ when F bit is set, FIQ is disabled

.equ len_FIQ_stack, 256
.equ len_IRQ_stack, 256
.equ len_SVC_stack, 256
.equ len_ABT_stack, 64
.equ len_UND_stack, 64
.equ len_USR_stack, 8192

.equ offset_USR_stack, 0
.equ offset_FIQ_stack, (offset_USR_stack + len_USR_stack)
.equ offset_IRQ_stack, (offset_FIQ_stack + len_FIQ_stack)
.equ offset_SVC_stack, (offset_IRQ_stack + len_IRQ_stack)
.equ offset_ABT_stack, (offset_SVC_stack + len_SVC_stack)
.equ offset_UND_stack, (offset_ABT_stack + len_ABT_stack)
.equ offset_USR_stack, (offset_UND_stack + len_UND_stack)



.equ Top_of_stack_0, 0x08000000 @ 128MB
.equ Top_of_stack_1, 0x07000000 @ 112MB
.equ Top_of_stack_2, 0x06000000 @ 96MB
.equ Top_of_stack_3, 0x05000000 @ 80MB

init_stack_part1:
	mrc p15, 0, r6, c0, c0, 5
	
	mov r0, #Top_of_stack_3
	cmp r6 , #0x3
	beq init_stack_part2
	
	mov r0, #Top_of_stack_2
	cmp r6 , #0x2
	beq init_stack_part2
	
	mov r0, #Top_of_stack_1
	cmp r6 , #0x1
	beq init_stack_part2

	mov r0, #Top_of_stack_0
	cmp r6 , #0x0
	beq init_stack_part2

	nop
	b .

init_stack_part2:
	sub sp, r0, #offset_USR_stack @ set user/sys mode stack pointer
	
	msr cpsr_c, #Mode_FIQ|I_Bit|F_Bit @ set fiq mode stack pointer
	sub sp, r0, #offset_FIQ_stack
	
	msr cpsr_c, #Mode_IRQ|I_Bit|F_Bit @ set irq mode stack pointer
	sub sp, r0, #offset_IRQ_stack
	
	msr cpsr_c, #Mode_SVC|I_Bit|F_Bit @ set svc mode stack pointer
	sub sp, r0, #offset_SVC_stack
	
	msr cpsr_c, #Mode_ABT|I_Bit|F_Bit @ set abt mode stack pointer
	sub sp, r0, #offset_ABT_stack
	
	msr cpsr_c, #Mode_UND|I_Bit|F_Bit @ set und mode stack pointer
	sub sp, r0, #offset_UND_stack



/* to setup the BBS section */
init_bss:
	ldr r0, start_bss
	ldr r1, length_bss
	eor r2, r2, r2			@ $r2 = 0

bss_clear_loop:
	subs r1, r1, #4
	bmi bss_clear_end
	str r2, [r0]
	b bss_clear_loop

bss_clear_end:

/* MMU enable */	
mmu_enable:
	mrc p15, 0, r2, c1, c0, 0 
	mov r1, #0x00000001
	orr r2, r2, r1
	mcr p15, 0, r2, c1, c0, 0 @ Enable MMU

	
/* I-Cache enable */
icache_enable:
	mrc p15, 0, r2, c1, c0, 0 
	mov r1, #0x00001000
	orr r2, r2, r1
	mcr p15, 0, r2, c1, c0, 0 @ Enable Icache

/* I-Cache enable */	
dcache_enable:
	mrc p15, 0, r2, c1, c0, 0 
	mov r1, #0x00000004
	orr r2, r2, r1
	mcr p15, 0, r2, c1, c0, 0 @ Enable Dcache
	
/****************************************************************************************************
 * the initialization process is done now
 * switch to USR mode and then go to the entry of the test program
 ****************************************************************************************************/
go:
	mrs r5, cpsr
	and r5, r5, #~0x00ff
	orr r5, r5, #0x0010
	msr cpsr_c, r5
	bl main

/****************************************************************************************************
 * the test program is also done now
 * terminate and shutdown the system through semihosting
 ****************************************************************************************************/
stop:
	swi 0x00ffffff
	nop
	b .

/****************************************************************************************************
 * the linker parameters and static local data
 ****************************************************************************************************/
start_text:
	.word _start_text		@ start address of text(code) section

length_text:
	.word _length_text		@ length of text(code) section

start_rodata:
	.word _start_rodata		@ start address of read only data section

length_rodata:
	.word _length_rodata	@ length of read only data section

start_data:
	.word _start_data		@ start address of data section

length_data:
	.word _length_data		@ length of data section

start_bss:
	.word _start_bss		@ start address of BSS section

length_bss:
	.word _length_bss		@ length of BSS section

/* end of boot code */
.size _start, . - _start
.end
	
