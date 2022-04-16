#include "bitsy.h"
#include <unistd.h>
#include <stdio.h>
/* Add any global variables or structures you need.*/
unsigned char readChar[1] = {0};
unsigned char readBuf[8] = {0};
unsigned char writeChar[1] = {0};
int writeInx = 0;
int readInx = 0;
int readNum;

/* readByte
 * Abstraction to read a byte.
 * Relys on readBit.
 */
unsigned short readByte(){ 
	/* This function should not call read() directly.
	 * If we are buffering data in readBit, we dont want to skip over
	 * that data by calling read again. Instead, call readBit and 
	 * construct a byte one bit at a type. This also allows us to read
	 * 8 bits that are not necessarily byte alligned. */

	//Do NOT call read here. Instead rely on readBit.

	for (int i = 0; i < 8; i++){
		unsigned short bit = readBit();
		readBuf[i] = bit;
		if (bit > 256){
			return 999;
		}
	}

	unsigned short byte = 0;
	for (int i = 0; i < 8; i++){
		byte = byte | readBuf[i];
		if ( i < 7){
			byte = byte << 1;
		}
        }

	//I suggest returning a unique value greater than the max byte value
	//to communicate end of file. We need this since 0 is a valid byte value
	//so we need another way to communicate eof. These functions are typed
	//as short to allow us to return a value greater than the max byte value.
	return byte;
}

/* readBit
 * Abstraction to read a bit.
 * */
unsigned short readBit(){
	/* This function is responsible for reading the next bit on the
	 * input stream from stdin (fd = 0). To accomplish this, keep a 
	 * byte sized buffer. Each time read bit is called, use bitwise
	 * operations to extract the next bit and return it to the caller.
	 * Once the entire buffered byte has been read the next byte from 
	 * the file. Once eof is reached, return a unique value > 255
	 *
	 */

	if (readInx == 0){
		readNum = read(0, readChar, sizeof(readChar));
	}
	if (readNum == 0){
		return 999;
	}

	unsigned short bit = readChar[0] & 0x80;
	readChar[0] = readChar[0] << 1;
	readInx++;

	if (readInx == 8){
		readInx = 0;
	}

	//I suggest returning a unique value greater than the max byte value
	//to communicate end of file. We need this since 0 is a valid byte value
	//so we need another way to communicate eof. These functions are typed
	//as short to allow us to return a value greater than the max byte value.
	return bit >> 7;
}

/* writeByte
 * Abstraction to write a byte.
 */
void writeByte(unsigned char byt){
	/* Use writeBit to write each bit of byt one at a time. Using writeBit
	 * abstracts away byte boundaries in the output.*/

	char byte = byt;
	unsigned char bit = 0;
	for (int i = 0; i < 8; i++){
		bit = (byte & 0x80) >> 7;
		writeBit(bit);
		byte = byte << 1;
	}

}

/* writeBit
 * Abstraction to write a single bit.
 */
void writeBit(unsigned char bit){
	/* Keep a byte sized buffer. Each time this function is called, insert the 
	 * new bit into the buffer. Once 8 bits are buffered, write the full byte
	 * to stdout (fd=1).
	 */

	writeChar[0] = (writeChar[0] << 1) | bit;
	writeInx++;

	if (writeInx == 8){
		write(1, writeChar, sizeof(writeChar));
		writeInx = 0;
		writeChar[0] = 0;
	}

}

/* flushWriteBuffer
 * Helper to write out remaining contents of a buffer after padding empty bits
 * with 1s.
 */
void flushWriteBuffer(){
	/* This will be utilized when finishing your encoding. It may be that some bits
	 * are still buffered and have not been written to stdout. Call this function 
	 * which should do the following: Determine if any buffered bits have yet to be 
	 * written. Pad remaining bits in the byte with 1s. Write byte to stdout
	 */
	int flag = 0;
	while (writeInx < 8 && writeInx > 0){
		writeChar[0] = (writeChar[0] << 1) | 1;
		writeInx++;
		flag = 1;
	}
	if (flag == 1){
		write(1, writeChar, sizeof(writeChar));
	}
	writeInx = 0;
}
