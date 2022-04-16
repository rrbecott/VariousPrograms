#include "bitsy.h" //This header includes prototypes for the proposed bit abstractions
#include <stdio.h>
/*Include any additional headers you require*/

unsigned char buf[8] = {0};
int items = 0;
/*You may use any global variables/structures that you like*/

void updateBuffer(unsigned char byte){
	if (items < 8){
		items++;
	}
	for (int i = 1; i <= items - 1; i++){
		buf[items - i - 1] = buf[items - i];
	}
	buf[0] = byte;
}

/* main - czy compression implementation
 * Develop a program called czy which compresses the data stream directed 
 * at its standard input and writes the compressed stream to its standard 
 * output.
 *
 * The compression algorithm reads the input one symbol (i.e., byte) at a 
 * time and compares it with each of the 8 bytes previously seen.  It also 
 * checks to see if the following n characters are the same as the current 
 * symbol.  If the byte has been previously seen, it outputs the position of 
 * the previous byte relative to the current position as well as the number 
 * of times it occurs using the format described below. Otherwise, the symbol 
 * is output as is by prefixing it with a binary one.
 *
 * To compile czy: make czy
 * To execute: ./czy < somefile.txt > somefile.encoded
 */
int main(int argc, char *argv[]){
	//The implementation of your encoder should go here.
	
	//It is recommeded that you implement the bit abstractions in bitsy.c and
	//utilize them to implement your encoder. 
	//If so, do NOT call read/write here. Instead rely exclusively on 
	//readBit, readByte, writeBit, writeByte, and flushWriteBuffer.

	int inx = 0;
	int repeating = -1;
	int pos = -1;
	unsigned char curByte;
	unsigned char nextByte;
	unsigned short read;
	unsigned short nextRead;

	read = readByte();
	curByte = (unsigned char) read;
	while (read < 256){  //while not EOF
		for (inx = 0; inx < items; inx++){  //check recent array
			if (buf[inx] == curByte){  //found in recent array
				pos = inx;
				nextRead = readByte();
				nextByte = (unsigned char) nextRead;
				updateBuffer(curByte);
				while ((nextRead == read) && (repeating < 7)){
					repeating++;
					updateBuffer(nextByte);
					nextRead = readByte();
                                	nextByte = (unsigned char) nextRead;
				}
				break;
			}
		}
		if (pos == -1){  //infrequent
			writeBit(1);
			writeByte(curByte);
			updateBuffer(curByte);
			read = readByte();
        		curByte = (unsigned char) read;
		} else if (pos >= 0 && repeating == -1){  //recent nonrepeating
			writeBit(0);
			writeBit(0);
			writeBit(((unsigned short)pos >> 2) & 0x1);
                        writeBit(((unsigned short)pos >> 1) & 0x1);
                        writeBit((unsigned short)pos & 1);
			read = nextRead;
			curByte = nextByte;
		} else if (pos >= 0 && repeating >= 0){  //recent repeating
			writeBit(0);
                        writeBit(1);
                        writeBit(((unsigned short)pos >> 2) & 0x1);
                        writeBit(((unsigned short)pos >> 1) & 0x1);
                        writeBit((unsigned short)pos & 1);
			writeBit(((unsigned short)repeating >> 2) & 0x1);
                        writeBit(((unsigned short)repeating >> 1) & 0x1);
                        writeBit((unsigned short)repeating & 0x1);
			read = nextRead;
			curByte = nextByte;
		} else {
			return -1;
		}
		pos = -1;
		repeating = -1;
		inx = 0;
	}

	flushWriteBuffer();

	return 0; //exit status. success=0, error=-1
}
