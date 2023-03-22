/* --====================================================================--
 * File Name : isr.h
 * File Version : 1.0.0
 **-------------------------------------------------------------------------
 * Release Time : 2010/10/1 p.m. 04:12:25
 * Author       : Yuan-Ting Jiang <bill0822@gmail.com>
 * Note         : first release
 **-------------------------------------------------------------------------
 * Revision     : None
 **-------------------------------------------------------------------------
 * Release Information : NCKU EE CASLAB
 * ------------------------------------------------------------------------
 * Purpose             : Interrupt service routine header
 * --====================================================================-- */

#ifndef _ISR_H_
#define _ISR_H_

extern void RST_Handler(void);
extern void UND_Handler(void) __attribute__((interrupt("UNDEF")));
extern void SWI_Handler(void) __attribute__((interrupt("SWI")));
extern void PRE_Handler(void) __attribute__((interrupt("ABORT")));
extern void DAB_Handler(void) __attribute__((interrupt("ABORT")));
extern void IRQ_Handler(void) __attribute__((interrupt("IRQ")));
extern void FIQ_Handler(void) __attribute__((interrupt("FIQ")));

extern void TimerAHB1IntHandler(void);
extern void TimerAHB2IntHandler(void);

#endif