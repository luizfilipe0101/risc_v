#include "defines.h"
#include <stdio.h>
#include <stdint.h>


void memmap(mem *flash)
{

    register int i;

    FILE *mem_map = fopen("mem_map.txt", "w");

    if(mem_map)
    {
        for(i = 0; i < 0xFFFF; i++)
            fprintf(mem_map, "%.4X  --->   %.2X\n", i, (uint8_t)flash[i].uval);

        fclose(mem_map);
    }

}