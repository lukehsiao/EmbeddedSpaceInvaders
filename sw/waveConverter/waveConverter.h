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
#define TRUE 1
#define FALSE 0

// Hex Values of Wave Chunks as little endian
#define RIFFHEADER              0x46464952
#define WAVRIFF                 0x54651475
#define FORMAT                  0x20746d66
#define LABELED_TEXT            0x478747C6
#define INSTRUMENTATION         0x478747C6
#define SAMPLE                  0x6C706D73
#define FACT                    0x47361666
#define DATA                    0x61746164



#endif  /* waveConverter_h */
