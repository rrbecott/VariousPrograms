#include "bitsy.h" //This header includes prototypes for the proposed bit abstractions
#include <stdio.h>
/*Include any additional headers you require*/

/*You may use any global variables/structures that you like*/

unsigned char buf[8] = {0};
int items = 0;

void updateBuffer(unsigned char byte){
        if (items < 8){
                items++;
        }
        for (int i = 0; i < items - 1; i++){
                buf[i + 1] = buf[i];
        }
        buf[0] = byte;
}

/* main - dzy de-compression implementation
 * This program decompresses a compressed stream directed at its standard input 
 * and writes decompressed data to its standard output.
 *
 * To compile dzy: make dzy
 * To execute: ./dzy < somefile.encoded > somefile_decoded.txt
 */
int main(int argc, char *argv[]){
	//The implementation of your decoder should go here.

	//It is recommeded that you implement the bit abstractions in bitsy.c and
	//utilize them to implement your decoder.
	//If so, do NOT call read/write here. Instead rely exclusively on 
	//readBit, readByte, writeBit, writeByte, and flushWriteBuffer.

	unsigned char repeat;
	unsigned short bit;
	unsigned short byte;
	unsigned short newByte = 0;

	while (1){
		bit = readBit();
		if (bit >= 256){
			break;
		} else if (bit == 1){  //infrequent
			byte = readByte();
			if (byte >= 256){
				break;
			}
			writeByte((unsigned char) byte);
			updateBuffer((unsigned char) byte);
		} else if (bit == 0){  //recent
			bit = readBit();
			if (bit >= 256){
				break;
			} else if (bit == 0){  //nonrepeating
				newByte = readBit();
				newByte = (newByte << 1) | readBit();
				newByte = (newByte << 1) | readBit();
				writeByte(buf[newByte]);
				updateBuffer(buf[newByte]);
			} else if (bit == 1){  //repeating
				newByte = readBit();
                                newByte = (newByte << 1) | readBit();
                                newByte = (newByte << 1) | readBit();
				repeat = buf[newByte];
				writeByte(repeat);
                                updateBuffer(repeat);
				newByte = 0;
				newByte = readBit();
                                newByte = (newByte << 1) | readBit();
                                newByte = (newByte << 1) | readBit();
				for (int i = 0; i <= newByte; i++){
					writeByte(repeat);
                                	updateBuffer(repeat);
				}
			}
		} else {
			return -1;
		}
		newByte = 0;
	}


	return 0; //exit status. success=0, error=-1
}
