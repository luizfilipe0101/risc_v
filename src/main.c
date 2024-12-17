#include <stdio.h>
#include <stdlib.h>
#include "../libs/strhex.h"
#include "decoder.h"
#include "defines.h"
#include "log.h"
#include "memory_map.h"
#include "architecture.h"
#include "init.h"
#include "controller.h"


void debug(reg *register_file);


int main(int argc, char **argv)
{
  register int i;
  int instr_return = 0;

  int16_t PC = 0;
  int32_t instr = 0;
  reg register_file[32] = {0};

  
  // ******************* RUNNING *****************************
  
  PC += initialization(argv[1]);

  //log_gen(buf, size, MEMORY, register_file, instr);
    
  while(1)
  {
    ctrl_reg = 0;
    instr ^= instr;
    register_file[zr].sval ^= register_file[zr].sval;             /* Force value to zero simulating GND wired*/

    /* FETCH MEMORY */

    for(i = 0; i < 4; i++)
    {
      databus = MEMORY[PC + i].uval;
      instr |= (uint32_t)(databus << 8*i);
    }
      
    instr_return = check_type(instr, register_file, &PC);

    (ctrl_reg >> 15) & 1 ? (PC += fields[imm]) : (PC += 4);    // PC target

    if(instr_return == -99)
      break;
    
  }
  
  // ******************* END *********************************

  memmap(MEMORY);
        
  return 0;
}


void debug(reg *register_file)
{
  puts("*** Registers ***");
  printf("Zero: %d\n", register_file[0].sval);
  printf("ra: %d\n", register_file[1].sval);
  printf("sp: %d\n", register_file[2].sval);
  printf("gp: %d\n", register_file[3].sval);
  printf("tp: %d\n", register_file[4].sval);
  printf("t0: %d\n", register_file[5].sval);
  printf("t1: %d\n", register_file[6].sval);
  printf("t2: %d\n", register_file[7].sval);
  printf("x8 / s0 / fp: %d\n", register_file[8].sval);
  printf("s1: %d\n", register_file[9].sval);
  
  printf("a0: %d\n", register_file[10].sval);
  printf("a1: %d\n", register_file[11].sval);
  printf("a2: %d\n", register_file[12].sval);
  printf("a3: %d\n", register_file[13].sval);
  printf("a4: %d\n", register_file[14].sval);
  printf("a5: %d\n", register_file[15].sval);
  printf("a6: %d\n", register_file[16].sval);
  printf("a7: %d\n", register_file[17].sval);
  printf("s2: %d\n", register_file[18].sval);
  printf("s3: %d\n", register_file[19].sval);
  
  printf("s4: %d\n", register_file[20].sval);
  printf("s5: %d\n", register_file[21].sval);
  printf("s6: %d\n", register_file[22].sval);
  printf("s7: %d\n", register_file[23].sval);
  printf("s8: %d\n", register_file[24].sval);
  printf("s9: %d\n", register_file[25].sval);
  printf("s10: %d\n", register_file[26].sval);
  printf("s11: %d\n", register_file[27].sval);
  printf("t3: %d\n", register_file[28].sval);
  printf("t4: %d\n", register_file[29].sval);
  
  printf("t5: %d\n", register_file[30].sval);
  printf("t6: %d\n", register_file[31].sval);
}
