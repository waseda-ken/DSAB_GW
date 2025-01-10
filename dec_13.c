#include <stdio.h>
#include <stdlib.h>
#include "grpwk24.h"
#define ROUND_FACTOR 30

// Function to round a number to the nearest multiple of ROUND_FACTOR
int round_to_nearest(int count, int factor) {
    if (count % factor >= (factor / 2))
        return (count / factor + 1) * factor;
    else
        return (count / factor) * factor;
}

// Function to write 'copies' number of the base to the output file
void write_copies(FILE *dfp, char base, int copies) {
    unsigned char res;
    for(int i = 0; i < copies; i++) {
      switch(base){
    case BASE_A:
      res = 0;
      break;
    case BASE_C:
      res = 1;      
      break;
    case BASE_G:
      res = 2;      
      break;
    case BASE_T:
      res = 3;      
      break;
    default:
      res = 0;
      break;
    }
    fputc((res>>1)+'0', dfp);
    fputc((res&0x1)+'0', dfp);    
  }
}

int dec(){
    // Open the input file for reading
    FILE *sfp = fopen(SEQDATA, "r");
    if(sfp == NULL){
        fprintf(stderr, "Cannot open input file: %s\n", SEQDATA);
        return 1; // Return non-zero to indicate failure
    }

    // Open the output file for writing
    FILE *dfp = fopen(DECDATA, "w");
    if(dfp == NULL){
        fprintf(stderr, "Cannot open output file: %s\n", DECDATA);
        fclose(sfp); // Close the input file before exiting
        return 1;
    }

    int c; // Use int to properly handle EOF
    char current_base = '\0'; // To store the current nucleotide base
    int run_length = 0; // To count the number of consecutive repeats
    int line_number = 1; // To track line numbers for error reporting

    // Read the input file character by character
    while((c = getc(sfp)) != EOF){
        // Handle newline characters to support multi-line sequences
        if(c == '\n' || c == '\r') {
            // If there's an ongoing run, process it
            if(run_length > 0){
                int rounded = round_to_nearest(run_length, ROUND_FACTOR);
                int copies = rounded / ROUND_FACTOR;
                if(copies > 0){
                    write_copies(dfp, current_base, copies);
                }
                run_length = 0;
                current_base = '\0';
            }
            fprintf(dfp, "\n"); // Write newline to output
            line_number++;
            continue;
        }

        if(c == current_base){
            run_length++; // Increment run length for the same base
        }
        else{
            // If there's a previous run, process it
            if(run_length > 0){
                int rounded = round_to_nearest(run_length, ROUND_FACTOR);
                int copies = rounded / ROUND_FACTOR;
                if(copies > 0){
                    write_copies(dfp, current_base, copies);
                }
            }
            // Start a new run with the current base
            current_base = c;
            run_length = 1;
        }
    }

    // After the loop, handle any remaining run
    if(run_length > 0){
        int rounded = round_to_nearest(run_length, ROUND_FACTOR);
        int copies = rounded / ROUND_FACTOR;
        if(copies > 0){
            write_copies(dfp, current_base, copies);
        }
    }

    // Close both files
    fclose(sfp);
    fclose(dfp);
    return 0; // Return zero to indicate success
}

int main(){
    dec();
    return 0;
}