/*
 *  CS3421 Assignment 4
 *  Name: Ryan Becotte
 *
 */
#include <stdio.h>
#include <stdlib.h>

union instruction {  //union for instruction hex values
	unsigned int hex;
	struct {
		unsigned int funct:6;
		unsigned int shamt:5;
		unsigned int rd:5;
		unsigned int rt:5;
		unsigned int rs:5;
		unsigned int opcode:6;
	} r_type;
	struct {
		int imm:16;
		unsigned int rt:5;
		unsigned int rs:5;
		unsigned int opcode:6;
	} i_type;
	struct {
		int imm:26;
		unsigned int opcode:6;
	} j_type;
};

int printInst(union instruction, FILE*);  //prints instruction
char registers[32][6] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9","$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

int main(int argc, char **argv) {
	char testinput[12];
	int textsize;  //number of instructions
	int datasize;  //number of data words
	int i;
	union instruction *instrs;  //array of instructions
	FILE *inputf = fopen(argv[1], "r");  //input file
	FILE *outputf = fopen("log.txt", "w+");  //output file
	
	fgets(testinput, 12, inputf);
	sscanf(testinput, "%d %d", &textsize, &datasize);  //gets textsize and datasize
	fprintf(outputf, "insts:\n");

	instrs = malloc(textsize * sizeof(union instruction));  //allocates memory for instruction array
	for (i = 0; i < textsize; i++){  //gets instructions from input file
		fgets(testinput, 12, inputf);
		sscanf(testinput, "%x", &(instrs[i].hex));
		fprintf(outputf, "%5d: ", i);
		if (printInst(instrs[i], outputf) == -1){
			fprintf(stderr, "Error: Illegal Instruction\n");
			free(instrs);
			return 0;
		}
	}
	fprintf(outputf, "\n");

	int *data;  //array for data words
	int dataval;
	data = malloc(datasize * sizeof(int));  //allocates memory for data array
	for (i = 0; i < datasize; i++){  //gets data words from input file
		fgets(testinput, 12, inputf);
		sscanf(testinput, "%x", &dataval);
		data[i] = dataval;
	}

	i = textsize;
	int j;
	fprintf(outputf, "data:\n");  //prints data words
        for (j = 0; j < datasize; j++){
                fprintf(outputf, "%5d: %d", i, data[j]);
        }
	fprintf(outputf, "\n\n");

	int regs[32] = {0};
	regs[28] = textsize;  //gp
	int hi = 0;
	int lo = 0;
	unsigned int pc = 0;
	long long int temp;

	while (1 == 1){
		if (pc >= textsize || pc < 0){
			fprintf(stderr, "Error: Illegal Instruction Address\n");
			free(instrs);
			free(data);
			return 0;
		}
		fprintf(outputf, "PC: %d\n", pc);
		fprintf(outputf, "inst: ");
		printInst(instrs[pc], outputf);
		fprintf(outputf, "\n");
		if (instrs[pc].r_type.opcode == 0){  //r type instructions
			if (instrs[pc].r_type.funct == 33){  //addu
				regs[instrs[pc].r_type.rd] = regs[instrs[pc].r_type.rs] + regs[instrs[pc].r_type.rt];
			} else if (instrs[pc].r_type.funct == 36){  //and
				regs[instrs[pc].r_type.rd] = regs[instrs[pc].r_type.rs] & regs[instrs[pc].r_type.rt];
			} else if (instrs[pc].r_type.funct == 26){  //div
				if (regs[instrs[pc].r_type.rt] == 0){
					fprintf(stderr, "Error: Divide By Zero\n");
					free(instrs);
					free(data);
					return 0;
				} else {
					lo = regs[instrs[pc].r_type.rs] / regs[instrs[pc].r_type.rt];
					hi = regs[instrs[pc].r_type.rs] % regs[instrs[pc].r_type.rt];
				}
			} else if (instrs[pc].r_type.funct == 16){  //mfhi
				regs[instrs[pc].r_type.rd] = hi;
			} else if (instrs[pc].r_type.funct == 18){  //mflo
				regs[instrs[pc].r_type.rd] = lo;
			} else if (instrs[pc].r_type.funct == 24){  //mult
				temp = regs[instrs[pc].r_type.rs] * regs[instrs[pc].r_type.rt];
				lo = 0x00000000ffffffff & temp;
				hi = temp >> 32;
			} else if (instrs[pc].r_type.funct == 37){  //or
				regs[instrs[pc].r_type.rd] = regs[instrs[pc].r_type.rs] | regs[instrs[pc].r_type.rt];
			} else if (instrs[pc].r_type.funct == 42){  //slt
				if (regs[instrs[pc].r_type.rs] < regs[instrs[pc].r_type.rt]){
					regs[instrs[pc].r_type.rd] = 1;
				} else{
					regs[instrs[pc].r_type.rd] = 0;
				}
			} else if (instrs[pc].r_type.funct == 35){  //subu
				regs[instrs[pc].r_type.rd] = regs[instrs[pc].r_type.rs] - regs[instrs[pc].r_type.rt];
			} else if (instrs[pc].r_type.funct == 12){  //syscall
				if (regs[2] == 1){
					printf("%d\n", regs[4]);
				} else if (regs[2] == 5){
					scanf("%d", &temp);
					regs[2] = temp;
				} else if (regs[2] == 10){
					break;
				}
			}
		} else if (instrs[pc].i_type.opcode == 9){  //addiu
			regs[instrs[pc].i_type.rt] = regs[instrs[pc].i_type.rs] + instrs[pc].i_type.imm;
		} else if (instrs[pc].i_type.opcode == 4){  //beq
			if (regs[instrs[pc].i_type.rs] == regs[instrs[pc].i_type.rt]){
				pc = pc + instrs[pc].i_type.imm - 1;
			}
		} else if (instrs[pc].i_type.opcode == 5){  //bne
			if (regs[instrs[pc].i_type.rs] != regs[instrs[pc].i_type.rt]){
                                pc = pc + instrs[pc].i_type.imm - 1;
                        }
		} else if (instrs[pc].i_type.opcode == 35){  //lw
			if (instrs[pc].i_type.imm >= datasize || instrs[pc].i_type.imm < 0){
				fprintf(stderr, "Error: Illegal Data Address\n");
				free(instrs);
				free(data);
				return 0;
			} else {
				regs[instrs[pc].i_type.rt] = data[instrs[pc].i_type.imm];
			}
		} else if (instrs[pc].i_type.opcode == 43){  //sw
			if (instrs[pc].i_type.imm >= datasize || instrs[pc].i_type.imm < 0){
                                fprintf(stderr, "Error: Illegal Data Address\n");
				free(instrs);
				free(data);
                                return 0;
                        } else {
				data[instrs[pc].i_type.imm] = regs[instrs[pc].i_type.rt];
			}
		} else if (instrs[pc].j_type.opcode == 2){  //j
			pc = instrs[pc].j_type.imm - 1;
		}
		fprintf(outputf, "regs:\n");
		for (i = 0; i < 32; i++){
			fprintf(outputf, "%8s =%6d     %3s =%6d     %3s =%6d     %3s =%6d\n", registers[i], regs[i], registers[i+1], regs[i+1], registers[i+2], regs[i+2], registers[i+3], regs[i+3]);
			i = i + 3;
		}
		fprintf(outputf, "%8s =%6d     %3s =%6d\n\n", "$lo", lo, "$hi", hi);
		fprintf(outputf, "data memory:\n");
		for (i = 0; i < datasize; i++){
			fprintf(outputf, "   data[%3d] =%6d\n\n\n", i, data[i]);
		}
		pc++;
	}
	fprintf(outputf, "regs:\n");  //prints regs and values
        for (i = 0; i < 32; i++){
       		fprintf(outputf, "%8s =%6d     %3s =%6d     %3s =%6d     %3s =%6d\n", registers[i], regs[i], registers[i+1], regs[i+1], registers[i+2], regs[i+2], registers[i+3], regs[i+3]);
                i = i + 3;
      	}
        fprintf(outputf, "%8s =%6d     %3s =%6d\n\n", "$lo", lo, "$hi", hi);
     	fprintf(outputf, "data memory:\n");
        for (i = 0; i < datasize; i++){  //prints data words
        	fprintf(outputf, "   data[%3d] =%6d\n", i, data[i]);
        }
	free(instrs);
	free(data);

    	return 0;
}

// ----------------------------------------------------------- 
// FUNCTION  printInst:                          
//     prints mips instruction to output file                            
// PARAMETER USAGE :                                           
//     in: instruction to print out
//     outputf: file to output to                   
// FUNCTION CALLED :                                           
//              
// -----------------------------------------------------------
int printInst(union instruction in, FILE* outputf){
	if (in.r_type.opcode == 0){  //r type instructions
                        if (in.r_type.funct == 33){  //addu
                                fprintf(outputf, "addu %s,%s,%s\n", registers[in.r_type.rd], registers[in.r_type.rs], registers[in.r_type.rt]);
                        } else if (in.r_type.funct == 36){  //and
                                fprintf(outputf, "and %s,%s,%s\n", registers[in.r_type.rd], registers[in.r_type.rs], registers[in.r_type.rt]);
                        } else if (in.r_type.funct == 26){  //div
                                fprintf(outputf, "div %s,%s\n", registers[in.r_type.rs], registers[in.r_type.rt]);
                        } else if (in.r_type.funct == 16){  //mfhi
                                fprintf(outputf, "mfhi %s\n", registers[in.r_type.rd]);
                        } else if (in.r_type.funct == 18){  //mflo
                                fprintf(outputf, "mflo %s\n", registers[in.r_type.rd]);
                        } else if (in.r_type.funct == 24){  //mult
				fprintf(outputf, "mult %s,%s\n", registers[in.r_type.rs], registers[in.r_type.rt]);
                        } else if (in.r_type.funct == 37){  //or
                                fprintf(outputf, "or %s,%s,%s\n", registers[in.r_type.rd], registers[in.r_type.rs], registers[in.r_type.rt]);
                        } else if (in.r_type.funct == 42){  //slt
                                fprintf(outputf, "slt %s,%s,%s\n", registers[in.r_type.rd], registers[in.r_type.rs], registers[in.r_type.rt]);
                        } else if (in.r_type.funct == 35){  //subu
                                fprintf(outputf, "subu %s,%s,%s\n", registers[in.r_type.rd], registers[in.r_type.rs], registers[in.r_type.rt]);
                        } else if (in.r_type.funct == 12){  //syscall
                                fprintf(outputf, "syscall\n");
                        } else {
				return -1;
			}
		} else if (in.i_type.opcode == 9){  //addiu
                        fprintf(outputf, "addiu %s,%s,%d\n", registers[in.i_type.rt], registers[in.i_type.rs], in.i_type.imm);
                } else if (in.i_type.opcode == 4){  //beq
                        fprintf(outputf, "beq %s,%s,%d\n", registers[in.i_type.rs], registers[in.i_type.rt], in.i_type.imm);
                } else if (in.i_type.opcode == 5){  //bne
                        fprintf(outputf, "bne %s,%s,%d\n", registers[in.i_type.rs], registers[in.i_type.rt], in.i_type.imm);
                } else if (in.i_type.opcode == 35){  //lw
                        fprintf(outputf, "lw %s,%d(%s)\n", registers[in.i_type.rt], in.i_type.imm, registers[in.i_type.rs]);
                } else if (in.i_type.opcode == 43){  //sw
                        fprintf(outputf, "sw %s,%d(%s)\n", registers[in.i_type.rt], in.i_type.imm, registers[in.i_type.rs]);
                } else if (in.j_type.opcode == 2){  //j
                        fprintf(outputf, "j %d\n", in.j_type.imm);
                } else {
			return -1;
		}
	return 1;
}
