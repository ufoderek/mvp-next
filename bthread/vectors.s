/* --====================================================================--
 * File Name : vectors.s
 * File Version : 1.0.0
 *-------------------------------------------------------------------------
 * Release Time : 2010/10/1 p.m. 03:42:34
 * Author       : Yuan-Ting Jiang <bill0822@gmail.com>
 * Note         : first release
 *-------------------------------------------------------------------------
 * Revision     : None
 *-------------------------------------------------------------------------
 * Release Information : NCKU EE CASLAB
 * ------------------------------------------------------------------------
 * Purpose             : Interrupt vector table
 * --====================================================================-- */

/* In isr.c */
.extern  RST_Handler		@ Reset handler   
.extern  UND_Handler		@ Undefined handler
.extern  SWI_Handler		@ Software interrupt handler
.extern  PRE_Handler		@ Prefetch abort handler
.extern  DAB_Handler		@ Data abort handler
.extern  IRQ_Handler		@ Interrupt request handler
.extern  FIQ_Handler		@ Fast interrupt request handler

.text     
        mov     PC, #0x40
        @LDR     PC, RST_Addr
        LDR     PC, UND_Addr
        LDR     PC, SWI_Addr
        LDR     PC, PRE_Addr
        LDR     PC, DAB_Addr
        NOP
        LDR     PC, IRQ_Addr           
        @LDR     PC, [PC, #-0xff0]       @ Load VECTADDR into PC (VIC)
        LDR     PC, FIQ_Addr
        


RST_Addr:	.long      RST_Handler
UND_Addr:	.long      UND_Handler
SWI_Addr:	.long      SWI_Handler
PRE_Addr:	.long      PRE_Handler
DAB_Addr:	.long      DAB_Handler
			.long      0               @ Reserved vector
IRQ_Addr:	.long      IRQ_Handler
FIQ_Addr:	.long      FIQ_Handler

.end

        

