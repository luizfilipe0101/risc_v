#include <stdint.h>
#include <stdio.h>
#include "defines.h"


int32_t system_rv32i(reg *regs, mem *flash)
{
    switch(regs[a7].uval)
    {
        case 10: /* EXIT */
            puts("SUCCESSFUL EXECUTION");
            return -99;

        case 64: /* WRITE */
            register int i;

            printf(">>> ");
            
            for(i = 0; i < regs[a2].uval; i++)
                printf("%c", flash[(uint16_t)regs[a1].uval + i].uval);

            return 0;
            break;

        default:
            puts("PUTA MERDAAAAAAAAA!");
            break;
    }
    return 0;
}