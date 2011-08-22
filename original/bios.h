/****************************************************************************
*                                                                           *
*                            Third Year Project                             *
*                                                                           *
*                            An IBM PC Emulator                             *
*                          For Unix and X Windows                           *
*                                                                           *
*                             By David Hedley                               *
*                                                                           *
*                                                                           *
* This program is Copyrighted.  Consult the file COPYRIGHT for more details *
*                                                                           *
****************************************************************************/

/* This is BIOS.H  It contains definitions for the BIOS functions */


#ifndef BIOS_H
#define BIOS_H

#include "mytypes.h"

#define BOOT

enum {
    INT9 = 1,
    INT_SERIAL,
    INT_PRINTER,
    INT_KEYBOARD,
    INT_EXTENDED,
    INT_BASIC,
    INT_REBOOT,
    INT_EQUIPMENT,
    INT_MEMORY,
    INT_TIME,
    INT_DISK,
    INT_E6,
    INT_2F,
    INT_VIDEO
};

void init_bios(void);
void init_timer(void);
void bios_off(void);
void set_int(unsigned, BYTE *, unsigned, unsigned, BYTE *, unsigned);

void disable(void);
void enable(void);

void put_scancode(BYTE *code, int count);

BYTE read_port60(void);

void loc(void);

char *set_boot_file(char *);
char *set_boot_type(int);

#endif
