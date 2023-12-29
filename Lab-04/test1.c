#include <stdio.h>
#include <stdlib.h>

FILE* f = NULL; 

int get_len(FILE* f){
    int ans=0, k=16777216;
    for (int i=0; i<4; i++){
        ans+=fgetc(f)*k;
        k/=16*16;
    }
    return ans;
}

unsigned long rev(unsigned long len){
    return ((len & 0xFF) << 24) | (((len >> 8) & 0xFF) << 16) | (((len >> 16) & 0xFF) << 8) | ((len >> 24) & 0xFF);
}

int main(){
    unsigned short s, sign=0xccee;
    /*int shift = 0, i = 0;
    unsigned long len =0, l = 0xFFFFFFFF;
    while (i<1){
	    i++;
	    fread(&s, 2, 1, f);
	    printf("%x\n",s);
	    if (s==0xeecc)
	        printf("YES\n"); 
	}
	shift = fgetc(f);
	fread(&len, 4, 1, f);
	/*len = get_len(f);*/
	/*printf("%x\n",len);
	const char lol[10]="Hellohello";*/
	
	FILE* fe = NULL;
	fe = fopen("SOME.txt","rb");
	fread(&sign, 2, 1, fe);
	printf("%x", sign);
	fclose(fe);
}
