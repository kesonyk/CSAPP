#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start,int len){
    int i;
    for(i=0;i!=len;i++)
        printf("%.2x",start[i]);
    printf("\n");
}

void show_int(int x){
    show_bytes((byte_pointer)&x,sizeof(int));
}

void show_float(float x){
    show_bytes((byte_pointer)&x,sizeof(float));
}

void show_pointer(void *x){
    show_bytes((byte_pointer) &x,sizeof(void *));
}

//unsigned integers encoding
int b2u(const char *binpointer,int len){
	int idx=0;
	int ret=0;
	if(len<1){
		printf("Error:b2u len is to small.\n");
		exit(EXIT_FAILURE);
	}
	for(idx=0;idx!=len;idx++)
		ret+=(binpointer[idx]-'0')*pow(2,len-1-idx);
	return ret;
}

//signed integers encoding :two's complement
int b2t(const char *binpointer,int len){
	int idx=0;
	int ret=0;
	if(len<2){
		printf("Error:b2t len is too small.\n");
		exit(EXIT_FAILURE);
	}
	ret-=(binpointer[0]-'0')*pow(2,len-1);
	for(idx=1;idx!=len;idx++)
		ret+=(binpointer[idx]-'0')*pow(2,len-1-idx);
	return ret;
}

//logical right shift
int logical_right_shift(int x,int n){
	// return (int)((unsigned int)x >> n);
	int size = sizeof(int) * 8; // usually sizeof(int) is 4 bytes (32 bits)
    return (x >> n) & ~(((0x1 << size) >> n) << 1);
}

//convert signed intergers to binary
//binSize=sizeof(int)*
//负数本身在计算机中以补码的形式存储，而且前面都为1,即sign extension
void int2bin(int a,char *bin,int binSize){
	/*
	bin+=binSize-1;
	int idx=0;
	for(;idx!=binSize;++idx){
		*bin--=(a&1)+'0';
		a>>=1;
	}

	*/
	unsigned int mask=0x80000000;
	while(mask){
		*bin++=!!(mask&a)+'0';
		mask>>=1;
	}
}

//hex to binary
//arguments:hex begin with 0x,so skip the first two characters
void hex2bin(char *bits,const char* hex,int len){
	char* quads[]={"0000", "0001", "0010", "0011", "0100", "0101",
                     "0110", "0111", "1000", "1001", "1010", "1011",
                     "1100", "1101", "1110", "1111"};
   	int i=2;
   	for(;i!=len;i++){
   		if(hex[i]>='0'&&hex[i]<='9')
   			strcat(bits,quads[hex[i]-'0']);
   		if(hex[i]>='A'&&hex[i]<='F')
   			strcat(bits,quads[10+hex[i]-'A']);
   		if(hex[i]>='a'&&hex[i]<='f')
   			strcat(bits,quads[10+hex[i]-'a']);
   	}
}

//binary to hex，len is the size of bits
//e.g bin2hex(hex,"0010",4) hex=0x2
void bin2hex(char *hex,const char *bits,int len){
	unsigned int hexnum;
	char four[4];
	int i,j;
	hex[0]='0';
	hex[1]='x';
	if(len%4){
		//若输入的二进制数不是4的倍数，则在前面补0
		//e.g "011100"->"00011100"
		int bitSize=4*(len/4+1);
		char bits2[bitSize];
		int k=0;
		for(;k!=4-len%4;k++)
			bits2[k]='0';
		strncpy(bits2+(bitSize-len),bits,len);
		for(i=0;i!=bitSize/4;++i){
			hexnum=0;
			strncpy(four,bits2+(i*4),4);
			for(j=0;j!=4;++j)
				hexnum+=(four[j]-'0')<<(3-j);
			sprintf(hex+2+i,"%X",hexnum);
		}
	}else{
		for(i=0;i!=len/4;++i){
			hexnum=0;
			strncpy(four,bits+(i*4),4);
			for(j=0;j!=4;++j)
				hexnum+=(four[j]-'0')<<(3-j);
			sprintf(hex+2+i,"%X",hexnum);
		}
	}
}

/*Determine whether arguments can be added without overflow*/
int uadd_ok(unsigned x,unsigned y){
	unsigned sum=x+y;
	return sum>=x;
}

int tadd_ok(int x,int y){
	int sum=x+y;
	int neg_over=x<0&&y<0&&sum>=0;
	int pos_over=x>0&&y>0&&sum<0;
	return !neg_over&&!pos_over;
}

int fun1(unsigned word){
	return (int) ((word<<24)>>24);
}

int fun2(unsigned word){
	return ((int)word<<24)>>24;
}

int bitCount(int x) {
  unsigned int mask=0x80000000;
  int ret=0;
  while(mask){
    if(mask&x) ++ret;
    mask>>=1;
  }
  return ret;
}

int bang(int x) {
  unsigned int mask=0x80000000;
  while(mask){
    if(mask&x)
      return 0;
    mask>>=1;
  }
  return 1;
}

int fitsBits(int x, int n) {
  unsigned int mask=0x80000000;
  int i=0;
  int ret;
  if(!(mask&x)){
  	//printf("pos\n");
    while(i<=32&&((x>>i++)&0x1))
      ret=i+1;
  }else{
  	//printf("neg\n");
    while(i<=32&&(~x>>i++)&0x1)
      ret=i+1;
  }
  return ret+1<=n;

}




int divpwr2(int x, int n) {
    return x>>n;
}
int divpwr(int x, int n){
    return (x + ((x >> 31) & ((1 << n) + ~0))) >> n;
}

int main(){
	int val=fitsBits(5,3);
	printf("%d\n",val);
	printf("%d\n",(1<<4)+~0);
	printf("%d\n",(-33>>31)&((1<<4)+~0));
	int val2=divpwr2(-17,4);
	printf("%d\n",val2);

}


