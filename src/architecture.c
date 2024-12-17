#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "architecture.h"
#include "defines.h"

mem MEMORY[MEM_SIZE] = {0};
uint32_t databus = 0;
uint8_t PORTA = 0;
uint8_t PORTB = 0;
