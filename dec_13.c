#include <stdio.h>
#include <stdlib.h>
#include "grpwk24.h"
#define ROUND_FACTOR 30

// 四捨五入用の関数
int round_to_nearest(int count, int factor) {
    if (count % factor >= (factor / 2))
        return (count / factor + 1) * factor;
    else
        return (count / factor) * factor;
}

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
    FILE *sfp = fopen(SEQDATA, "r");
    if(sfp == NULL){
        fprintf(stderr, "Cannot open input file: %s\n", SEQDATA);
        return 1; 
    }

    FILE *dfp = fopen(DECDATA, "w");
    if(dfp == NULL){
        fprintf(stderr, "Cannot open output file: %s\n", DECDATA);
        fclose(sfp);
        return 1;
    }

    int c; 
    char current_base = '\0';
    int run_length = 0;
    int line_number = 1;


    while((c = getc(sfp)) != EOF){

        if(c == '\n' || c == '\r') {

            if(run_length > 0){
                int rounded = round_to_nearest(run_length, ROUND_FACTOR);
                int copies = rounded / ROUND_FACTOR;
                if(copies > 0){
                    write_copies(dfp, current_base, copies);
                }
                run_length = 0;
                current_base = '\0';
            }
            fprintf(dfp, "\n"); 
            line_number++;
            continue;
        }

        if(c == current_base){
            run_length++; 
        }
        else{
        
            if(run_length > 0){
                int rounded = round_to_nearest(run_length, ROUND_FACTOR);
                int copies = rounded / ROUND_FACTOR;
                if(copies > 0){
                    write_copies(dfp, current_base, copies);
                }
            }
        
            current_base = c;
            run_length = 1;
        }
    }

    if(run_length > 0){
        int rounded = round_to_nearest(run_length, ROUND_FACTOR);
        int copies = rounded / ROUND_FACTOR;
        if(copies > 0){
            write_copies(dfp, current_base, copies);
        }
    }

    fclose(sfp);
    fclose(dfp);
    return 0; 
}

int main(){
    dec();
    return 0;
}