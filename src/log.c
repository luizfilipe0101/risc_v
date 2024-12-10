#include <stdio.h>
#include <stdint.h>
#include "defines.h"

char *instr_name(int32_t opcode);

int log_gen(char *rom, int romsize, mem *flash, reg *regs, uint32_t instr)
{
    register int i;
    //register int j;
    char chr = 0;

    FILE *log;

    log = fopen("log.txt", "w");
    
    if(!log)
        return 1;

    fputs("####################### FIRMWARE #######################", log);
    fputc('\n', log);

    for(i = 0; i<romsize; i++)
    {
        chr = rom[i];
        chr == 58 ? fputc(10, log) : fputc(chr, log);  
    }
    fputs("\n\n", log);
    fputs("########################################################", log);


    fputs("\n\n", log);

    fputs("####################### MEMORY #########################", log);
    fputs("\n\n", log);

    for(i = 0; i < 0xFFFF; i++)
    {
        if(i < 30)
            fprintf(log, "0x%.5X ---> %.4X\n", i, flash[i].uval);
        
        if(i >= 0xF82F && i <= 0xF84D)
            fprintf(log, "0x%.5X ---> %.4X\n", i, flash[i].uval);
    }

    fputs("\n\n", log);
    fputs("########################################################", log);

    fputs("\n\n", log);

    fputs("####################### INSTRUCTION ####################", log);
    fputs("\n\n", log);

    fclose(log);

    return 0;

}// end log_gen

int log_instr(int32_t instr, uint32_t type, int32_t *fields, reg *regs)
{
   /*
    rd    0
    func3 1
    rs1   2
    rs2   3
    imm   4
    func7 5
  */

    register int i;
    int name = 0;

    FILE *log;

    log = fopen("log.txt", "a");

    fputs("***** Registers *****\n", log);

    fprintf(log, "Zr: %.4X  ", regs[0].uval);
    fprintf(log, "ra: %.4X  ", regs[1].uval);
    fprintf(log, "sp: %.4X  ", regs[2].uval);
    fprintf(log, "gp: %.4X  ", regs[3].uval);
    fprintf(log, "tp: %.4X  ", regs[4].uval);
    fprintf(log, "x8 / s0 / fp: %.4X     \n", regs[8].uval);
    
    fprintf(log, "a0: %.4X  ", regs[10].uval);
    fprintf(log, "a1: %.4X  ", regs[11].uval);
    fprintf(log, "a2: %.4X  ", regs[12].uval);
    fprintf(log, "a3: %.4X  ", regs[13].uval);
    fprintf(log, "a4: %.4X  ", regs[14].uval);
    fprintf(log, "a5: %.4X  ", regs[15].uval);
    fprintf(log, "a6: %.4X  ", regs[16].uval);
    fprintf(log, "a7: %.4X\n", regs[17].uval);

    fprintf(log, "s1: %.4X  ", regs[9].uval);
    fprintf(log, "s2: %.4X  ", regs[18].uval);
    fprintf(log, "s3: %.4X  ", regs[19].uval);
    fprintf(log, "s4: %.4X  ", regs[20].uval);
    fprintf(log, "s5: %.4X  ", regs[21].uval);
    fprintf(log, "s6: %.4X  ", regs[22].uval);
    fprintf(log, "s7: %.4X  ", regs[23].uval);
    fprintf(log, "s8: %.4X  ", regs[24].uval);
    fprintf(log, "s9: %.4X  ", regs[25].uval);
    fprintf(log, "s10: %.4X ", regs[26].uval);
    fprintf(log, "s11: %.4X\n", regs[27].uval);

    fprintf(log, "t0: %.8X  ", regs[5].uval);
    fprintf(log, "t1: %.8X   ", regs[6].uval);
    fprintf(log, "t2: %.4X   ", regs[7].uval);
    fprintf(log, "t3: %.4X   ", regs[28].uval);
    fprintf(log, "t4: %.4X   ", regs[29].uval);
    fprintf(log, "t5: %.4X   ", regs[30].uval);
    fprintf(log, "t6: %.4X   ", regs[31].uval);
    fputc(10, log);
    fputs("*********************", log);
    fputc(10, log);

    switch(type)
    {
        case rtype:
            break;

        case utype:
            for(i = 31; i >= 0; i--)
            {
                switch(i)
                {
                    case 31:
                        fputc('[', log);
                    break;

                    case 11:
                        fputc(']', log);
                        fputc('[', log);
                    break;

                    case 6:
                        fputc(']', log);
                        fputc('[', log);
                    break;
                }
                (instr >> i & 1) ? fputc('1', log) : fputc('0', log);
            }

            fputc(']', log);
            fputc(10, log);
            fprintf(log, "rd: %d\n", fields[0]);
            fprintf(log, "imm: %d\n", fields[4]);

            name |= instr & 0x7F;

            if(name == 55)
                fprintf(log, "[%s]\n", "lui");
            else    
                fprintf(log, "[%s]\n", "auipc");

            break;

        case ltype:
        case itype:
            for(i = 31; i >= 0; i--)
            {
                switch(i)
                {
                    case 31:
                        fputc('[', log);
                    break;

                    case 19:
                        fputc(']', log);
                        fputc('[', log);
                    break;

                    case 14:
                        fputc(']', log);
                        fputc('[', log);
                    break;

                    case 11:
                        fputc(']', log);
                        fputc('[', log);
                    break;

                    case 6:
                        fputc(']', log);
                        fputc('[', log);
                    break;
                }
                (instr >> i & 1) ? fputc('1', log) : fputc('0', log);
            }
            fputc(']', log);
            fputs("\n", log);
            fprintf(log, "rd: %d\n", fields[0]);
            fprintf(log, "func3: %d\n", fields[1]);
            fprintf(log, "rs1: %d\n", fields[2]);
            fprintf(log, "imm: %d\n", fields[4]);
            fprintf(log, "[%s]\n", instr_name(instr));
            break;

        case stype:
            for(i = 31; i >= 0; i--)
            {
                switch(i)
                {
                    case 31:
                        fputc('[', log);
                    break;

                    case 24:
                        fputc(']', log);
                        fputc('[', log);
                    break;

                    case 19:
                        fputc(']', log);
                        fputc('[', log);
                    break;

                    case 14:
                        fputc(']', log);
                        fputc('[', log);
                    break;

                    case 11:
                        fputc(']', log);
                        fputc('[', log);
                    break;

                    case 6:
                        fputc(']', log);
                        fputc('[', log);
                    break;
                }
                (instr >> i & 1) ? fputc('1', log) : fputc('0', log);
            }

            fputc(']', log);
            fputs("\n", log);
            fprintf(log, "imm_rd: %d\n", fields[0]);
            fprintf(log, "func3: %d\n", fields[1]);
            fprintf(log, "rs1: %d\n", fields[2]);
            fprintf(log, "rs2: %d\n", fields[3]);
            fprintf(log, "imm: %d\n", fields[4]);
            fprintf(log, "[%s]\n", instr_name(instr));

            break;
    }

    fputs("\n", log);
    fclose(log);
    return 0;
}

char *instr_name(int32_t opcode)
{
    int32_t code = 0;
    int tmp = 0;

    code |= opcode & 0x7F;
    tmp  |= (opcode >> 12) & 0x7;
    code |= (tmp << 7);

    printf("NUMBER: %d\n", code);

    switch(code)
    {
        case 55:
            return "lui";
        break;

        case 23:
            return "auipc";
        break;

        case 110:
            return "jal";
        break;

        case 0x67:
            return "jalr ";
        break;

        case 0x63:
            return "beq";
        break;

        case 0xE3:
            return "bne";
        break;

        case 0x263:
            return "blt";
        break;

        case 0x2E3:
            return "bge";
        break;

        case 0x363:
            return "bltu";
        break;

        case 0x3E3:
            return "bgeu";

        case 0x3:
            return "lb";
        break;

        case 0x83:
            return "lh";
        break;

        case 0x103:
            return "lw";
        break;

        case 0x203:
            return "lbu";
        break;

        case 0x283:
            return "lhu ";
        break;

        case 0x23:
            return "sb";
        break;

        case 0xA3:
            return "sh";
        break;

        case 0x123:
            return "sw";
        break;

        case 0x13:
            return "addi";
        break;

        case 0x113:
            return "slti";
        break;

        case 0x193:
            return "sltiu";
        break;

        case 0x213:
            return "xori";
        break;

        case 0x313:
            return "ori";
        break;

        case 0x393:
            return "andi";
        break;

        case 0x93:
            return "slli";
        break;

        case 0x293:
            return "srli";
        break;

        case 0x8293:
            return "srai";
        break;
    }
    return "";
}