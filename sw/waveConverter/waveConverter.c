
/**
 *  WaveConverter takes in a WAV file and outputs a
 *  C Source file containing the data as an int array
 *  This means that we will need to extern the variables
 *
 *  Note this only supports non-compressed wav files
 *  author: Luke Hsiao & Jeff Ravert
 *  25 October 2014
 */

#include <stdio.h>
//#include <libsndfile.h>
#include "waveConverter.h"

int main(int argc, char *argv[]) {
    u32 chunkID = 0;
    u32 isDataChunk = FALSE;
    u32 numSamples = 0;
    u32 sampleRate = 0;
    u32 tempData = 0;
    
    if (argc != 3) { /* We're always expecting two arguments */
        printf("Usage: waveConverter [inputWAV] [nameOfSound]\n\n");
    }
    else {
        // argv[1] should be filename to open
        FILE *input = fopen(argv[1], "rb");

        // argv[2] is the name of the sound (e.g. alienSound1)
        char* soundName = argv[2];

        /* FOpen returns 0 on failer */
        if (input == 0) {
            printf("Could not open %s\n", argv[1]);
        }
        else {
            FILE *output = fopen("spaceInvadersSounds.c", "a+");
            if (output == 0) {
                printf("Error opening output: Check spaceInvadersSounds.c");
            }
            else {
                // Parse the WAV file here
                while(!isDataChunk) {
                    fread(&(chunkID), sizeof(int), 1, input);  
                    printf("chunkID: %x\n", chunkID);
                    
                    switch (chunkID) {
                        // Extract the sample rate
                        case FORMAT:
                            //Advance reader past formatsize, format, channels
                            fread(&(sampleRate), sizeof(int)*2, 2, input);
                            fread(&(sampleRate), sizeof(int), 1, input);
                            printf("    Sample Rate: %d\n", sampleRate);
                            break;
                            
                        // Extract actual data
                        case DATA:
                            fprintf(output, "int %s_soundData[] = {", soundName);
                            //Advance past chunk size
                            fread(&(tempData), sizeof(int), 1, input);
                            
                            // Parse Data
                            
                            break
                        default:
                            // Do nothing
                            break;
                    }
                    
                    if (chunkID == DATA) {
                        isDataChunk = TRUE;
                    }
                }
            }
        }
        
        // Finish up string output
        
    }
} /* End of MAIN */
