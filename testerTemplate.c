#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/random.h>
#include <sys/syscall.h>
#include "grpwk24.h"

#define TEST_NUMBER 1000
int failCount;
unsigned char buf[ORGDATA_LEN];


//gen
int gen(){
FILE *fp;
if((fp = fopen(ORGDATA, "w")) ==NULL){
    fprintf(stderr, "cannot open %s\n", ORGDATA);
    exit(1);
}

size_t buf_size = sizeof(buf);
if(-1 == (int)syscall(SYS_getrandom, buf, buf_size, GRND_NONBLOCK)) {
    perror ("getrandom");
    exit(1);
}  

char c;
for(int i=0; i<ORGDATA_LEN; i++){
    switch(buf[i]&0x1){
    case 0:
    c = '0';
    break;
    case 1:
    c = '1';
    break;
    }
    fputc(c, fp);
}
c = '\n';
fputc(c, fp);

fclose(fp);    
return(0);
}


//enc
int enc(){
    return(0);  
}

//syn
int syn(){
FILE *efp;
if((efp = fopen(ENCDATA, "r")) ==NULL){
    fprintf(stderr, "cannot open %s\n", ENCDATA);
    exit(1);
}

FILE *dfp;
if((dfp = fopen(SYNDNA, "w")) ==NULL){
    fprintf(stderr, "cannot open %s\n", SYNDNA);
    exit(1);
}

int cnt=0;
unsigned char c, res;
while((c = getc(efp)) != '\n' && cnt < MAX_SYN_SIZE){
    switch(c){
    case BASE_A:
    res = BASE_A;
    break;
    case BASE_C:
    res = BASE_C;      
    break;
    case BASE_G:
    res = BASE_G;      
    break;
    case BASE_T:
    res = BASE_T;      
    break;
    default:
    res = BASE_A;
    break;
    }      
    fputc(res, dfp);
    cnt++;
}
res = '\n';
fputc(res, dfp);
    
fclose(efp);
fclose(dfp);
return(0);
}


//seq
int cost;

int getrandom(){
size_t buf_size = sizeof(buf);
if(-1 == (int)syscall(SYS_getrandom, buf, buf_size, GRND_NONBLOCK)) {
    perror ("getrandom");
    exit(1);
}
return(buf_size);
}

int np(int start, int len){
FILE *dfp;
if((dfp = fopen(SYNDNA, "r")) ==NULL){
    fprintf(stderr, "cannot open %s\n", SYNDNA);
    exit(1);
}
fseek(dfp, start, SEEK_SET);

FILE *sfp;
if((sfp = fopen(SEQDATA, "a")) ==NULL){
    fprintf(stderr, "cannot open %s\n", SEQDATA);
    exit(1);
}

if(len == 0)
    len = MAX_SYN_SIZE;

int ins=0, del=0, rcnt = -1, cnt;
unsigned char c1, c2 = BASE_N, r;
for(cnt = 0; cnt<len && (c1 = getc(dfp)) != '\n'; cnt++){
    if(rcnt<1)
    rcnt = ((int)getrandom())-1;

    r = buf[rcnt--];
    if(c1 == c2){     
    if(r<8){
    putc(c1, sfp);
    putc(c1, sfp);
    ins++;
    }else if(r>=16){
    putc(c1, sfp);
    }else{
    del++;
    }
    }else{
    if(r==0){
    r = buf[rcnt--];
    if(r%2 == 0){
    putc(c1, sfp);
    putc(c1, sfp);	
    ins++;
    }else{
    del++;
    }
    }else{
    putc(c1, sfp);
    }
    }
    c2 = c1;
}
putc('\n', sfp);
//  fprintf(stderr,"ins=%d, del=%d\n",ins, del);
fclose(sfp);
fclose(dfp);

if(cnt%COST_UNIT==0){
    cost += (cnt/COST_UNIT)*NP_RUN_COST_PER_UNIT;
}else{
    cost += (1 + cnt/COST_UNIT)*NP_RUN_COST_PER_UNIT;
}  
return(0);
}

int bs(int start, int ilen){
FILE *dfp;
if((dfp = fopen(SYNDNA, "r")) ==NULL){
    fprintf(stderr, "cannot open %s\n", SYNDNA);
    exit(1);
}
    
int tail, head, len, nor;
fseek(dfp, 0L, SEEK_END);
tail = ftell(dfp);
fseek(dfp, start, SEEK_SET);
head = ftell(dfp);
len = tail-head-1;

if(ilen < len && ilen != 0)
    len = ilen;

nor = len/SR_SIZE;
if(len%SR_SIZE!=0)
    nor++;
len = nor*SR_SIZE;
char *dat = (char*)malloc(sizeof(char)*(len));

int i = 0, rcnt = -1, mut = 0;
char c;
unsigned int r;

for(int cnt = 0; cnt<len && (c = getc(dfp)) != '\n'; cnt++){    
    if(rcnt < 1)
    rcnt = getrandom() - 2;
    
    r = buf[rcnt--];
    if(r%MUTATION_RATE == 0){
    r = buf[rcnt--];
    switch(r%4){
    case 0:
    c = BASE_A;
    break;	
    case 1:
    c = BASE_C;
    break;
    case 2:
    c = BASE_G;
    break;
    case 3:
    c = BASE_T;
    break;
    }
    mut++;
    }
    dat[i++]=c;
}

for(;i<len; i++){
    if(rcnt < 0)
    rcnt = getrandom() - 1;
    r = buf[rcnt--];
    switch(r%4){
    case 0:
    c = BASE_A;
    break;	
    case 1:
    c = BASE_C;
    break;
    case 2:
    c = BASE_G;
    break;
    case 3:
    c = BASE_T;
    break;
    }
    dat[i]=c;
}
//  fprintf(stderr, "%u\n", mut);

unsigned int p1, p2;
rcnt = -1;
char tmp;
for(i=0; i<SHUFFLE_RATIO*nor; i++){
    if(rcnt < 8)
    rcnt = getrandom() - 8;    
    r = buf[rcnt]<<24 | buf[rcnt+1]<<16 | buf[rcnt+2]<<8 | buf[rcnt+3];
    p1 = (r%nor)*SR_SIZE;
    rcnt -= 4;
    r = buf[rcnt]<<24 | buf[rcnt+1]<<16 | buf[rcnt+2]<<8 | buf[rcnt+3];
    p2 = (r%nor)*SR_SIZE;    
    rcnt -= 4;
    //    fprintf(stderr, "%u, %u\n", p1, p2);
    
    for(int j=0 ; j<SR_SIZE; j++){
    tmp = dat[p1];
    dat[p1] = dat[p2];
    dat[p2] = tmp;
    p1++; p2++;
    }    
}

FILE *sfp;
if((sfp = fopen(SEQDATA, "a")) ==NULL){
    fprintf(stderr, "cannot open %s\n", SEQDATA);
    exit(1);
}
for(i=0; i<len; i++){
    putc(dat[i], sfp);
}
putc('\n', sfp);

free(dat);
fclose(dfp);
fclose(sfp);

if(len%COST_UNIT==0){
    cost += (len/COST_UNIT)*BS_RUN_COST_PER_UNIT;
}else{
    cost += (1 + len/COST_UNIT)*BS_RUN_COST_PER_UNIT;
}
return(0);  
}

//dec

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
    return(0);
}

//eval

int eval(){
FILE *ofp;
if((ofp = fopen(ORGDATA, "r")) ==NULL){
    fprintf(stderr, "cannot open %s\n", ORGDATA);
    exit(1);
}

FILE *dfp;
if((dfp = fopen(DECDATA, "r")) ==NULL){
    fprintf(stderr, "cannot open %s\n", DECDATA);
    exit(1);
}

unsigned char c1 = 0, c2 = 0;
int hd=0;
while(1){
    if(c1 != '\n')
    c1 = getc(ofp);
    if(c2 != '\n')
    c2 = getc(dfp);
    if(c1 != c2) hd++;
    if(c1 == '\n' && c2 == '\n')
    break;
}
if(hd!=0) failCount++;
printf(" hd=%d\n",hd);
fclose(ofp);
fclose(dfp);
return(0);
}


int main(){
    for(int i=1;i<=TEST_NUMBER;i++){
        printf("Running test: %d,", i);
        gen();
        enc();
        syn();
        //seq
        int nbs = 0, nnp = 1;
        int sbs = 0, snp = 0;
        int lbs = 0, lnp = 0;  

        FILE *sfp;
        if((sfp = fopen(SEQDATA, "w")) ==NULL){
            fprintf(stderr, "cannot open %s\n", SEQDATA);
            exit(1);
        }
        fclose(sfp);

        cost = 0;
        for(int i=0; i<nbs;i++){
            bs(sbs, lbs);
        }
        
        for(int i=0; i<nnp;i++){
            np(snp, lnp);
        }
        dec();
        eval();
    }
    printf("The number of failure is: %d\n", failCount);

    return(0);
}
