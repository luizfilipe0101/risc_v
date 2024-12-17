#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "defines.h"
#include "../libs/strhex.h"
#include "architecture.h"


int checksum(char *stream, int size);
int16_t load_memory(char *rom, int len);


int initialization(char *firm)
{
    char *buf = NULL;
    char byte = 0;
    int  size = 0;
    int count = 0;
    int16_t entry = 0;

    FILE *rom = fopen(firm, "r");
    //FILE *mem_map = fopen("mem_map.txt", "w");

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
        }
        
    }

    if(checksum(buf, size))
    {
        puts("CHECKSUM ERROR");
        return 1;
    }else
    {
        puts("CHECKSUM OK");
    }

    entry = load_memory(buf, size);

    free(buf);
    fclose(rom);

    return entry;
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

int16_t load_memory(char *rom, int len)
{
  register int i;
  register int j;
  char read_byte[2] = {0};
  char get_addr[4] = {0};
  char get_status[2] = {0};
  int16_t entry = 0;
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
    entry = strhex(get_addr, 4); 
  } 
  
  for(i = 0; i < len; i++)
  {
    if(rom[i] == 58)
    {
      count_mem = 0;
    
      read_byte[0] = rom[i+1];
      read_byte[1] = rom[i+2];
      data_len = strhex(read_byte, 2);
      
      get_addr[0] = rom[i+3];
      get_addr[1] = rom[i+4];
      get_addr[2] = rom[i+5];
      get_addr[3] = rom[i+6];
      addr = strhex(get_addr, 4);
      
      get_status[0] = rom[i+7];
      get_status[1] = rom[i+8];
      status = strhex(get_status, 2);
      
      if(status == 0 || status == 4)
        for(j = 0; j < (data_len*2); j+=2)
        {
          read_byte[0] = rom[((i+j)+9)];
          read_byte[1] = rom[((i+j)+9)+1];
                    
          MEMORY[addr+count_mem].uval = (uint8_t)strhex(read_byte, 2);
                    
          count_mem += 1;
        }// end for j
      
    } //end if
    
  } //end for i

  return entry;
        
}// end load_memory