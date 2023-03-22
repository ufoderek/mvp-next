/*------------------------------------------------------------------------------
   *--  This confidential and proprietary software may be used only as
   *--  authorised by a licensing agreement from ARM Limited
   *--    (C) COPYRIGHT 2001 ARM Limited
   *--        ALL RIGHTS RESERVED
   *--  The entire notice above must be reproduced on all authorised
   *--  copies and copies may only be made to the extent permitted
   *--  by a licensing agreement from ARM Limited.
   *--
   *------------------------------------------------------------------------------
   *--  Version and Release Control Information:
   *--
   *--  File Name : tube.h,v
   *--  File Revision : 1.4
   *--
   *--  Release Information : AUK-REL1v0
   *--
 *---------------------------------------------------------------------------*/
/* Purpose             : Tube registers                                     */
/*                                                                          */
/****************************************************************************/

#ifndef __tube_h
#define __tube_h

extern volatile unsigned Tube; //@0

#define EOT 0x4

//Data
#define TUBEDataMask 15U

#endif/* defined(__tube_h) */
