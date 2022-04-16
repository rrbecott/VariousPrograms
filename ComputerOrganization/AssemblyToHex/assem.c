/*
 *  CS3421 Assignment 2
 *  Name: RYAN BECOTTE
 *        rrbecott
 *
 *  This program reads assembly language instructions as 
 *  input and outputs the hex values of the instructions
 */

#include <stdio.h>
#include <string.h>

#define MAXLINE 80
#define MAXNAME 10
#define MAXREG 5
#define MAXINSTRUCTION 32768

unsigned int rType();
unsigned int iType();
unsigned int loadStore();
unsigned int multDiv();
unsigned int loHi();
unsigned int jumpTo();
int regValue();
char* skiplabel();

char registers[32][5] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9","k0", "k1", "gp", "sp", "fp", "ra"};

int main() {

	char line[MAXLINE], oper[MAXNAME], rs[MAXREG], rd[MAXREG], rt[MAXREG], *s, immediate[MAXNAME];

	char instructions[MAXINSTRUCTION][MAXLINE];    /*holds all instructions*/
	char label[MAXINSTRUCTION][MAXNAME];    /*holds jump/branch labels*/
	int address[MAXINSTRUCTION];    /*holds program addresses of labels*/

	int labelCounter = 0;    /*index of label array*/
	int counter = 0;    /*program counter*/
	int dataWords = 0;    /*counts number of data words*/
	
	int j = 0;
	while (fgets(line, MAXLINE, stdin)){    /*reads through file*/
		strcpy(instructions[j], line);
		j++;
		if (strstr(line, ".text") == NULL && strstr(line, ".data") == NULL){    /*if the read input is not .data or .text*/
			if (strstr(line, ".word") != NULL || strstr(line, ".space") != NULL){    /*increments number of datawords if it reads .word or .space*/
				dataWords++;
			}
			int i = 0;	
			for (i = 0; i < MAXLINE; i++){    /*for each letter in the line*/
				if (line[i] == ':'){    /*if the line contains a colon (for a label)*/
					if (strstr(line, ".word") != NULL){
						int nuts = sscanf(line, "%10s %10s %10s", oper, rs, immediate);
						strncpy(label[labelCounter], line, i);
                                                address[labelCounter] = atoi(immediate);
                                                labelCounter++;
					} else {
						strncpy(label[labelCounter], line, i);    /*update label and address with found label*/
						address[labelCounter] = counter;
						labelCounter++;
					}
				}
			}
		counter++;
		}
	}

	printf("%d %d\n", counter, dataWords);    /*prints number of instructions and data words*/

	int pc = 0;    /*program counter*/
	int i = 0;
	for (i = 0; i < j; i++) {    /*reads through every line again*/
		strcpy(line, instructions[i]);
		s = skiplabel(line);    /*if s has a label, skips over it*/
		if (strstr(line, ".data") != NULL || strstr(line, ".text") != NULL) {    /*skips over .data and .text*/
			pc--;
		} else if (sscanf(s, "%10s $%5[^,],$%5[^,],$%5s", oper, rd, rs, rt) == 4) {    /*3 argument r type*/
			unsigned int rtype = rType(oper, rd, rs, rt);
			printf("%08x\n", rtype);
		} else if (sscanf(s, "%10s $%5[^,],$%5[^,],%5s", oper, rt, rs, immediate) == 4){    /*3 argument i type*/
			unsigned int itype = iType(oper, rt, rs, immediate, label, address, pc);
			printf("%08x\n", itype);
		} else if (sscanf(s, "%10s $%5[^,],%5[^(]($%5[^)]s)", oper, rt, immediate, rs) == 4){    /*lw and sw*/
			unsigned int lwsw = loadStore(oper, rt, immediate, rs, label, address);
			printf("%08x\n", lwsw);
		} else if (sscanf(s, "%10s $%5[^,],$%5s", oper, rs, rt) == 3){    /*mult and div*/
			unsigned int multdiv = multDiv(oper, rs, rt);
			printf("%08x\n", multdiv);
		} else if (sscanf(s, "%10s $%5s", oper, rd) == 2){    /*mflo and mfhi*/
			unsigned int lohi = loHi(oper, rd);
			printf("%08x\n", lohi);
		} else if (strstr(line, "syscall") != NULL){    /*syscall*/
                        printf("%08x\n", 12);
                } else if (sscanf(s, "%10s %5s", oper, immediate) == 2){    /*.space, .word, and j*/
			if (strcmp(oper, ".space") == 0 || strcmp(oper, ".word") == 0){    /*if oper is .space or .word*/
				unsigned int data = atoi(immediate);
				printf("%08x\n", data);
			} else {    /*else it is j*/
				unsigned int jump = jumpTo(oper, immediate, label, address);
				printf("%08x\n", jump);
			}
		}
	pc++;    /*increment program counter*/
	}
}

/*skips label in input line and returns the new line without the label*/
char* skiplabel(char* inLine){
	
	char *result;
	char colon = ':';
	if (strchr(inLine, colon) != NULL){
		result = strchr(inLine, colon);
		result++;
		return result;
	}
	return inLine;

}

/*returns decimal number of encoded 3 argument r type instruction*/
unsigned int rType(char* oper, char* rd, char* rs, char* rt){

	unsigned int op2 = 0;
	unsigned int rd2 = 0;
	unsigned int rs2 = 0;
	unsigned int rt2 = 0;
	unsigned int funct = 0;

	if (strcmp(oper, "addu") == 0){
		funct = 33;
	} else if (strcmp(oper, "and") == 0){
		funct = 36;
	} else if (strcmp(oper, "or") == 0){
		funct = 37;
	} else if (strcmp(oper, "slt") == 0){
		funct = 42;
	} else if (strcmp(oper, "subu") == 0){
		funct = 35;
	} else {
		return -1;
	}
	
	rd2 = regValue(rd);
	rs2 = regValue(rs);
	rt2 = regValue(rt);

	op2 = op2 << 5;
	op2 = op2 | rs2;
	op2 = op2 << 5;
	op2 = op2 | rt2;
	op2 = op2 << 5;
	op2 = op2 | rd2;
	op2 = op2 << 11;
	op2 = op2 | funct;

	return op2;
	
}

/*returns decimal number of encoded 3 argument i type instructions*/
unsigned int iType(char* oper, char* rt, char* rs, char* immediate, char labels[MAXINSTRUCTION][MAXNAME], int address[], int pc){
	
	unsigned int temp = 0;
	unsigned int op2 = 0;
        unsigned int rs2 = 0;
        unsigned int rt2 = 0;
	signed int immediate2 = 0;

        rs2 = regValue(rs);
        rt2 = regValue(rt);

	int i = 0;	
	if (strcmp(oper, "addiu") == 0){
		op2 = 9;
		immediate2 = atoi(immediate);
	} else if (strcmp(oper, "beq") == 0){
		temp = rs2;
		rs2 = rt2;
		rt2 = temp;
		op2 = 4;
		for (i = 0; i < MAXINSTRUCTION; i++){
			if (strcmp(immediate, labels[i]) == 0){
				immediate2 = address[i] - pc;
				break;
			}
		}
	} else if (strcmp(oper, "bne") == 0){
		temp = rs2;
                rs2 = rt2;
                rt2 = temp;
                op2 = 5;
                for (i = 0; i < MAXINSTRUCTION; i++){
                        if (strcmp(immediate, labels[i]) == 0){
                                immediate2 = address[i] - pc;
                                break;
                        }
                }
	} else {
		return -1;
	}

	op2 = op2 << 5;
	op2 = op2 | rs2;
	op2 = op2 << 5;
	op2 = op2 | rt2;
	op2 = op2 << 16;
	op2 = op2 | immediate2;

	return op2;

}

/*returns decimal number of encoded lw and sw instructions*/
unsigned int loadStore(char* oper, char* rt, char* immediate, char* rs, char labels[MAXINSTRUCTION][MAXNAME], int address[]){

	unsigned int op2 = 0;
        unsigned int rs2 = 0;
        unsigned int rt2 = 0;
        signed int immediate2 = 0;

	int i = 0;
	for (i = 0; i < MAXINSTRUCTION; i++){
		if (strcmp(immediate, labels[i]) == 0){
			immediate2 = address[i];
		}
	}

	rs2 = regValue(rs);
	rt2 = regValue(rt);

	if (strcmp(oper, "lw") == 0){
		op2 = 35;
	} else if (strcmp(oper, "sw") == 0){
		op2 = 43;
	} else {
		return -1;
	}

	op2 = op2 << 5;
	op2 = op2 | rs2;
	op2 = op2 << 5;
	op2 = op2 | rt2;
	op2 = op2 << 16;
	op2 = op2 | immediate2;
	
	return op2;

}

/*returns decimal number of encoded mult and div instructions*/
unsigned int multDiv(char* oper, char* rs, char* rt){

	unsigned int op2 = 0;
	unsigned int rs2 = 0;
	unsigned int rt2 = 0;
	unsigned int funct = 0;

	rs2 = regValue(rs);
	rt2 = regValue(rt);

	if (strcmp(oper, "div") == 0){
		funct = 26;
	} else if (strcmp(oper, "mult") == 0){
		funct = 24;
	} else {
		return -1;
	}

	op2 = op2 << 5;
	op2 = op2 | rs2;
	op2 = op2 << 5;
	op2 = op2 | rt2;
	op2 = op2 << 16;
	op2 = op2 | funct;

	return op2;

}

/*returns decimal number of encoded mflo and mfhi instructions*/
unsigned int loHi(char* oper, char* rd){
	
	unsigned int op2 = 0;
	unsigned int rd2 = 0;
	unsigned int funct = 0;

	rd2 = regValue(rd);

	if (strcmp(oper, "mfhi") == 0){
		funct = 16;
	} else if (strcmp(oper, "mflo") == 0){
		funct = 18;
	} else {
		return -1;
	}

	op2 = op2 << 15;
	op2 = op2 | rd2;
	op2 = op2 << 11;
	op2 = op2 | funct;

	return op2;

}

/*returns decimal number of encoded jump instructions*/
unsigned int jumpTo(char* oper, char* immediate, char label[MAXINSTRUCTION][MAXNAME], int address[]){
	
	unsigned int op2 = 0;
	unsigned int dest = 0;

	if (strcmp(oper, "j") == 0){
		op2 = 2;
	} else {
		return -1;
	}

	int i = 0;
	for (i = 0; i < MAXINSTRUCTION; i++){
		if (strcmp(immediate, label[i]) == 0){
			dest = address[i];
			break;
		}
	}

	op2 = op2 << 26;
	op2 = op2 | dest;

	return op2;

}

/*returns integer value of a register*/
int regValue(char* inReg){

	int i = 0;
	for (i = 0; i < 32; i++){
		if (strcmp(inReg, registers[i]) == 0){
			return i;
		}
	}
	return -1;
}
