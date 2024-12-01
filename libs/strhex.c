#include <math.h>
#include <stdint.h>

uint8_t ascii_to(char chr);

uint16_t strhex(char *num, int len)
{
  register int i;
  uint8_t result = 0;
  int power = len;
  
  for(i = 0; i < len; i++)
    result += ascii_to(num[i]) * pow(16, --power);
    
  return result;
}

uint8_t ascii_to(char chr)
{
  switch(chr)
  {
    case(48):
      return 0;
      
    case(49):
      return 1;
      
    case(50):
      return 2;
      
    case(51):
      return 3;
      
    case(52):
      return 4;
      
    case(53):
      return 5;
      
    case(54):
      return 6;
      
    case(55):
      return 7;
      
    case(56):
      return 8;
      
    case(57):
      return 9;
      
    case(65):
      return 10;
      
    case(66):
      return 11;
      
    case(67):
      return 12;
      
    case(68):
      return 13;
      
    case(69):
      return 14;
      
    case(70):
      return 15;
      
    default:
      return 0;
  }

}
