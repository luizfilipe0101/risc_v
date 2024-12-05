#include <stdio.h>
#include <stdlib.h>
#include "../libs/strhex.h"
#include "decoder.h"
#include "defines.h"


int checksum(char *stream, int size);
void load_memory(char *rom, int len, int *entry);
void debug(void);


mem flash[0xFFFF] = {0};
//uint8_t flash[0xFFFF] = {0};

reg register_file[32] = {0};

int main(int argc, char **argv)
{
  register int i;
  char *buf = NULL;
  char byte = 0;
  int  size = 0;
  int count = 0;
  
  int entry_point = 0;
  int16_t PC = 0;
  uint32_t instr = 0;
  
  int instr_return = 0;
  
  
  FILE *rom = fopen(argv[1], "r");
  
  if(!rom)
    return 1;

  /* Get buffer's size to MALLOC without special characters*/
  while(1)
  {
    byte = fgetc(rom);
    if(byte == EOF)
      break;
    
    if(byte != 10 && byte != 13)
      size++;
    
  }
  
  buf = malloc(size);
  rewind(rom);
  
  /* Remove new_line and start_line bytes*/
  while(1)
  {
    byte = fgetc(rom);
    if(byte == EOF)
      break;
    
    if(byte != 10 && byte != 13)
    {
      buf[count] = byte;
      count++;
      //printf("%c", byte);
    }
    
  }
  
  !checksum(buf, size) ? puts("CHECKSUM: OK!") : exit(1);
  load_memory(buf, size, &entry_point);

  
  free(buf);
  fclose(rom);
  

  printf("Addr 16 = %.4X\n", flash[16].uval);
  
  // ******************* RUNNING *****************************
  
  PC += entry_point;
  
  putchar(10);
    
  //exit(0);
    
  while(1)
  {
    instr = 0;
    
    instr |= flash[PC].uval << 0;
    instr |= flash[PC + 1].uval << 8;
    instr |= flash[PC + 2].uval << 8*2;
    instr |= flash[PC + 3].uval << 8*3;
    
    for(i = 31; i >=0; i--)
      (instr >> i & 1) ? putchar('1') : putchar('0');
      
    putchar(10);
      
    instr_return = check_type(instr, flash, register_file);
    
    switch(instr_return)
    {
      case 0:
        PC += 4;
        debug();
        break;
        
      case 1:
        exit(1);
        break;
        
      default:
        PC += (int16_t)instr_return;     /* Branch or jump */
        break;
    }
    
  }
  
  // ******************* END *********************************
        
  return 0;
}


int checksum(char *stream, int size)
{
  register int i;
  register int j;
  
  int data_len = 0;                 /* Quantidade de bytes de instrução*/
  int checksum = 0;
  uint32_t sum = 0;
  
  char data_byte[2] = {0};
  char get_checksum[2] = {0};
  
  uint8_t   mask_sum = 0xFF;
  uint8_t result_sum = 0;
  
  for(i = 0; i < size; i++)
    if(stream[i] == 58)
    {
      data_byte[0] = stream[i+1];
      data_byte[1] = stream[i+2];
      data_len = strhex(data_byte, 2);
      
      get_checksum[0] = stream[(i+9)+(data_len*2)];
      get_checksum[1] = stream[(i+9)+(data_len*2)+1];
      checksum = strhex(get_checksum, 2);
      
      for(j = 1; j < (9+(data_len*2)); j+=2)
      {
        data_byte[0] = stream[i+j];
        data_byte[1] = stream[(i+j)+1];
        sum += strhex(data_byte, 2);
      }
      
      result_sum = sum & mask_sum;
      result_sum = ~result_sum + 1;

      sum = 0;
      
      if(result_sum != checksum)
        return 1;
      
    }
      
  return 0;
}


void load_memory(char *rom, int len, int *entry)
{
  register int i;
  register int j;
  register int k;
  char read_byte[2] = {0};
  char get_addr[4] = {0};
  char get_status[2] = {0};
  int status = 0;
  int data_len = 0;
  int addr = 0; 
  
  int count_mem = 0;
  
  /* Get entry point */
  for(i = 0; i < 4; i++)
  {
    get_addr[0] = rom[3+i];
    get_addr[1] = rom[3+i];
    get_addr[2] = rom[3+i];
    get_addr[3] = rom[3+i];
    *entry = strhex(get_addr, 4); 
  } 
  
  for(i = 0; i < len; i++)
  {
    if(rom[i] == 58)
    {
      count_mem = 0;
    
      read_byte[0] = rom[i+1];
      read_byte[1] = rom[i+2];
      data_len = strhex(read_byte, 2);
      //printf("data_len %d = %d\n", i, data_len);
      
      get_addr[0] = rom[i+3];
      get_addr[1] = rom[i+4];
      get_addr[2] = rom[i+5];
      get_addr[3] = rom[i+6];
      addr = strhex(get_addr, 4);
      //printf("Address: %.4X\n", addr);
      
      get_status[0] = rom[i+7];
      get_status[1] = rom[i+8];
      status = strhex(get_status, 2);
      
      if(status == 0 || status == 4)
        for(j = 0; j < (data_len*2); j+=2)
        {
          read_byte[0] = rom[((i+j)+9)];
          read_byte[1] = rom[((i+j)+9)+1];
          
          //printf("Byte: %.4X\n", strhex(read_byte, 2));
          for(k = 7; k >= 0; k--)
            //(strhex(read_byte, 2) >> k) & 1 ? putchar('1') : putchar('0');
          //putchar(10);
          
          flash[addr+count_mem].uval = (uint8_t)strhex(read_byte, 2);
                    
          count_mem += 1;
        }// end for j
      
    } //end if
    
  } //end for i
        
}// end load_memory

void debug(void)
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
