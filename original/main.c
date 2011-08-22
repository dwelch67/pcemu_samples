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

/* This is MAIN.C  This is where everything begins... */


#include "global.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "bios.h"
#include "vga.h"
#include "xstuff.h"
#include "hardware.h"
#include "vgahard.h"

BYTE *memory;
char *progname;

void exit_emu(void)
{
    disable();
    bios_off();
    vga_off();
    exit(0);
}


void check_error(char *msg, int line)
{
    if (msg)
        fprintf(stderr,"%s, line %d\n", msg, line);
}


void read_pcemurc(void)
{         /* This procedure is a bit of a hack :) - will be redone later */
    FILE *f1;
    char buffer[1024];  /* Maximum path length. Should really be a #define */
    char keyword[1024];
    char value[1024];
    int line = 0;
    int tmp;

    /* Try current directory first... */

    if ((f1 = fopen(".pcemurc","r")) == NULL)
    {           /* Try home directory */
        sprintf(buffer,"%s/.pcemurc", getenv("HOME"));
        if ((f1 = fopen(buffer,"r")) == NULL)
        {
            printf("Warning: .pcemurc not found - using compile time defaults\n");
            return;
        }
    }

    while (fgets(buffer,sizeof buffer,f1) != NULL)
    {
        line++;
        tmp = sscanf(buffer," %s %s", keyword, value);

        if (tmp == 0 || keyword[0] == '#')
            continue;
        if (tmp != 2)
        {
            check_error("Syntax error in .pcemu file", line);
            continue;
        }

        if (strcasecmp(keyword,"bootfile") == 0)
            check_error(set_boot_file(value), line);
        else if (strcasecmp(keyword,"boottype") == 0)
            check_error(set_boot_type(strtol(value, NULL,10)), line);
        else if (strcasecmp(keyword,"updatespeed") == 0)
            check_error(set_update_rate(strtol(value, NULL,10)), line);
        else if (strcasecmp(keyword,"cursorspeed") == 0)
            check_error(set_cursor_rate(strtol(value, NULL,10)), line);
        else
            check_error("Syntax error in .pcemu file", line);
    }
    fclose(f1);
}           


void main(int argc, char **argv)
{
    progname = (progname = strrchr(argv[0],'/')) ? progname : argv[0];

#ifndef BOOT    
    FILE *f1;
    if (argc != 2)
    {
        fprintf(stderr,"Format: %s testfile\n",progname);
        exit(1);
    }
#endif
    if (!(memory = (BYTE *)malloc(MEMORY_SIZE)))
    {
        fprintf(stderr,"Insufficient available memory\n");
        exit(1);
    }
#ifdef BOOT
    memset(memory,0, MEMORY_SIZE);
#else
    if (!(f1 = fopen(argv[1],"rb")))
    {
        fprintf(stderr,"Cannot open test file\n");
        exit(1);
    }


    fread(memory+0x800,1,MEMORY_SIZE-0x800,f1);
    fclose(f1);
#endif

    read_pcemurc();
    disable();

    start_X();
    init_cpu();
    init_bios();
    init_vga();
    init_timer();

    enable();

    execute();
    
    /* NOT REACHED */
}



