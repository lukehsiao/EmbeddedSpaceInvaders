
/**
 *  WaveConverter takes in a WAV file and outputs a
 *  C Source file containing the data as an int array
 *  This means that we will need to extern the variables
 *  author: Luke Hsiao & Jeff Ravert
 *  25 October 2014
 */

#include <stdio.h>
#include "waveConverter.h"


int main(int argc, char *argv[]) {
    if (argc != 3) { /* We're always expecting two arguments */
        printf("Usage: waveConverter inputWAV nameOfSound");
    }
    else {
        // argv[1] should be filename to open
        FILE *input = fopen(argv[1], "r");

        // argv[2] is the name of the sound (e.g. alienSound1)
        char* soundName = argv[2];

        /* FOpen returns 0 on failer */
        if (input == 0) {
            printf("Could not open the input WAV file\n");
        }
        else {
            FILE *output = fopen("spaceInvadersSounds.c", "w");
            if (output == 0) {
                printf("Error opening output file");
            }
            else {
                // Parse the WAV file here
                fprintf(output, "Output sound's name: %s\n\r", soundName);
            }
        }
    }
} /* End of MAIN */