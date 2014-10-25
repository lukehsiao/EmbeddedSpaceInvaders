
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
    u8 tempData = 0;
    u16 bitsPerSample = 0;
    
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
                            fread(&(sampleRate), sizeof(int), 1, input);
                            fread(&(sampleRate), sizeof(int), 1, input);
                            fread(&(sampleRate), sizeof(int), 1, input);
                            printf("    Sample Rate: %d\n", sampleRate);
                            
                            // Advance to bits per sample
                            fread(&(bitsPerSample), sizeof(u16), 3, input);
                            fread(&(bitsPerSample), sizeof(u16), 1, input);
                            printf("    Bits per Sample: %d\n", bitsPerSample);
                            break;                            
                        // Extract actual data
                        case DATA:
                            fprintf(output, "int %s_soundData[] = {\n    ", soundName);
                            // Store number of samples
                            fread(&(numSamples), sizeof(int), 1, input);
                            
                            // Parse Data
                            int i;
                            for (i = numSamples; i > 1; i--) {
                                if (i % 10 == 0) {
                                   fprintf(output, "\n    ", tempData); 
                                }
                                fread(&(tempData), sizeof(u8), 1, input);
                                fprintf(output, "%d,", tempData);
                            }
                            fread(&(tempData), sizeof(u8), 1, input); 
                            fprintf(output, "%d};\n", tempData);

                   
                            if (chunkID == DATA) {
                                isDataChunk = TRUE;
                            }
                            
                            break;
                        default:
                            // Do nothing
                            break;
                    }
                }
                
                fprintf(output, "int %s_numberOfSamples = %d\n",soundName,numSamples);
                fprintf(output, "int %s_sampleRate = %d\n",soundName,sampleRate);
                
            }
        }
        
        
    }
} /* End of MAIN */
