#include <stdio.h>
#include <stdlib.h>
#include "grpwk24.h"
#define ROUND_FACTOR 30

int enc(){
  FILE *ofp;
  if((ofp = fopen(ORGDATA, "r")) ==NULL){
    fprintf(stderr, "cannot open %s\n", ORGDATA);
    exit(1);
  }

  FILE *efp;
  if((efp = fopen(ENCDATA, "w")) ==NULL){
    fprintf(stderr, "cannot open %s\n", ENCDATA);
    exit(1);
  }

  unsigned char c1, c2, res;
  for(int i=0; i<ORGDATA_LEN; i+=2){
    c1 = getc(ofp);
    c2 = getc(ofp);
    
    switch( ( (c1 & 0x1) << 7) >> 6 | ( c2 & 0x1) ){
    case 0:
      for(int i=0;i<ROUND_FACTOR;i++){
        res = BASE_A;
        fputc(res, efp);
      }
      break;
    case 1:
      for(int i=0;i<ROUND_FACTOR;i++){
        res = BASE_C;
        fputc(res, efp);
      }      
      break;
    case 2:
      for(int i=0;i<ROUND_FACTOR;i++){
        res = BASE_G;
        fputc(res, efp);
      }     
      break;
    case 3:
      for(int i=0;i<ROUND_FACTOR;i++){
        res = BASE_T;
        fputc(res, efp);
      }
      break;
    }
  }
  res = '\n';
  fputc(res, efp);
  
  
  fclose(ofp);
  fclose(efp);
  return(0);
}

int main(){
  enc();
  return(0);
}
