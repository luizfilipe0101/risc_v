#include <stdint.h>
#include <stdio.h>
#include "defines.h"


int32_t system_rv32i(reg *regs)
{
    switch(regs[a7].uval)
    {
        case 10: /* EXIT */
            puts("SUCCESSFUL EXECUTION");
            return -99;

        case 64: /* WRITE */
            return 0;
            break;
    }
    return 0;
}