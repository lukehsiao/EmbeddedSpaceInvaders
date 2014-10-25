/**
 *  WaveConverter takes in a WAV file and outputs a
 *  C Source file containing the data as an int array
 *  This means that we will need to extern the variables
 *  author: Luke Hsiao & Jeff Ravert
 *  25 October 2014
 */

#ifndef waveConverter_h
#define waveConverter_h

#define u32 unsigned int
#define u16 unsigned short
#define u8  unsigned char

#define TRUE 1
#define FALSE 0

// Hex values of chunks as little-endian
#define RIFFHEADER          0x46464952 // "FFIR"
#define WAVRIFF             0x45564157 // "EVAW"
#define FORMAT              0x20746D66 // "tmf "
#define DATA                0x61746164 // "atad"
    
#endif  /* waveConverter_h */
