#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv){
	//argc - the number of parameters
	if (argc!=2){
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else{
		FILE* f = NULL;
		f = fopen(argv[1], "rb");
		if (f == NULL){
			fprintf(stderr, "AAAAAAAAAA\n", argv[0]);
			perror("fopen()");
			exit(EXIT_FAILURE);
		}
		else {
			unsigned int c, crc=0, i, j;
			unsigned int crc_table[256];
			unsigned int size=0;
			for (i=0; i<256; i++){
                crc = i<<24;
                for (j = 0; j<8; j++)
                    crc = (crc & 0x80000000) ? (crc << 1)^ 0x04C11DB7 : crc << 1;

                crc_table[i] = crc;
            }
        	crc =0UL;
			while ((c = fgetc(f))!= EOF){
				crc = crc_table[(crc >>24) ^c] ^ (crc << 8);
				size+=1;
			}
			while (size){
				crc = crc_table[(crc>>24) ^ (size & 0xFF)] ^ (crc << 8);
				size >>=8;
			}
			printf("%lu\n", ~crc);
			fclose(f);
		}
    }	
	return 0;
}

