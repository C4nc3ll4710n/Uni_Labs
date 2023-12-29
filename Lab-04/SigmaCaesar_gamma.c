#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char ch;
int shift=-1, i=0;
char msg[256];
unsigned short signa;
unsigned char data[268435455], denc[268435455], enc[268435455];
FILE *fe = NULL, *fm = NULL;

char Caesar(char ch, int shift){
    if (ch >= 'A' && ch <= 'Z')
        ch = ((ch - 'A') + shift) % 26 + 'A';
    else if (ch >= 'a' && ch <= 'z')
        ch = ((ch - 'a') + shift) % 26 + 'a';
    return ch;
}

void ask_for_shift(){
    printf("Input shift:");
	scanf("%d", &shift);
	getchar();
}

unsigned long crc32(unsigned char *data, unsigned long data_len){
    unsigned int crc=0, i, j, size=0, crc_table[256];
    unsigned char c;
	for (i=0; i<256; i++){
        crc = i<<24;
        for (j = 0; j<8; j++)
            crc = (crc & 0x80000000) ? (crc << 1)^ 0x04C11DB7 : crc << 1;
        crc_table[i] = crc;
    }
    crc =0UL;
    for (unsigned long i=0; i<data_len; i++){
	    crc = crc_table[(crc >>24) ^data[i]] ^ (crc << 8);
		size+=1;
	}
	while (size){
	    crc = crc_table[(crc>>24) ^ (size & 0xFF)] ^ (crc << 8);
		size >>=8;
	}
	return (unsigned long)(~crc);
}

void message_encryption(int shift){
    printf("Input message:");
	while ((ch = getchar()) != '\n' && ch != EOF){
		if (i==0){
		    printf("Encrypted message:");
			i=1;
	    }
		printf("%c", Caesar(ch, shift));
	}
	printf("\n");
}

void message_encryption_to_str_file(int shift){
    unsigned long i=0, crc_val=0;
    fe = fopen("benc.txt", "wb");
	while ((ch = getchar()) != '\n'){
		data[i] = ch;
		i++;
	}
	fwrite(data, i, 1, fe);
	crc_val = crc32(data, i);
	fwrite(&crc_val, 4, 1, fe);
}

void enc_strange_file_to_output(FILE* fm){
    int fshift=0;
    unsigned long data_len=0;
    unsigned char dat[data_len], enc[data_len];
    fe = fopen("lil_end_enc.txt", "wb");
    fread(&signa, 2, 1, fm); /*0xccee || 0xeecc        2 bytes*/
    fshift=fgetc(fm);   /*shift                    1 byte*/
    if (fshift!=shift && shift!=-1){
        printf("Different shifts\n");
		exit(EXIT_FAILURE);
    }
    shift = fshift;
    fread(&data_len, 4, 1, fm); /*data len          4 bytes*/
    fread(dat, data_len, 1, fm);
	for (unsigned long p=0; p<data_len; p++)
        printf("%c", Caesar(dat[p], shift));
    printf("\n");
    fclose(fm);
    fclose(fe);
    
}

void file_encryption(FILE* fm, FILE* fe, int shift){
    while ((ch = fgetc(fm))!=EOF)
		fprintf(fe, "%c", Caesar(ch, shift));
}

void file_encryption_to_str_file(FILE* fm, int shift, int ind_b){
    unsigned long i=0, crc_val=0;
    fe = fopen("str_enc.txt", "wb");
    while ((ch = fgetc(fm))!=EOF){
		data[i] = ch;
		i++;
	}
	if (shift==-1)
	    shift = 5;
	if (ind_b==-1)
	    signa = 0xccee;
	else 
	    signa = 0xeecc;
	fwrite(&signa, 2, 1, fe);
	for (int j =0; j<i; j++)
	    enc[j] = Caesar(data[j], shift);
	fwrite(&shift, 1, 1, fe);
	fwrite(&i, 4, 1, fe);
	fwrite(enc, i, 1, fe);
	crc_val = crc32(data, i);
	fwrite(&crc_val, 4, 1, fe);
}

void enc_strange_file_to_strange_file(FILE* fm){
    int fshift=0;
    unsigned long data_len=0;
    fe = fopen("lil_end_enc.txt", "wb");
    fread(&signa, 2, 1, fm); /*0xccee || 0xeecc        2 bytes*/
    fshift=fgetc(fm);   /*shift                    1 byte*/
    if (fshift!=shift && shift!=-1){
        printf("Different shifts\n");
		exit(EXIT_FAILURE);
    }
    shift = fshift;
    fread(&data_len, 4, 1, fm); /*data len          4 bytes*/
    unsigned char dat[data_len], enc[data_len];
    fread(dat, data_len, 1, fm);
    fwrite(&signa, 2, 1, fe);
	fputc(fshift, fe);
	fwrite(&data_len, 4, 1, fe);
	for (unsigned long p=0; p<data_len; p++)
        enc[p] = Caesar(dat[p], shift);
    fwrite(enc, data_len, 1, fe);
    unsigned long crc_value = crc32(dat, data_len);
    fwrite(&crc_value, 4, 1, fe);
    fclose(fm);
    fclose(fe);
    
}

int is_number(char* str){
    for (int i=0; *(str+i)!='\0' && *(str+i)!='\n'; i++){
        if (*(str+i) < '0' || *(str+i)>'9')
            return 0;
    }
    return 1;
}

void decrypt_input(){
    printf("Input message:");
	int i = 0;
	while ((ch = getchar()) != '\n' && ch != EOF){
	    msg[i]=ch;
	    i++;
	}
    for (int j =1; j<26; j++){
	    for (int k=0; k<i; k++){
		    if (k==0)
		        printf("If shift=%d:", j);
		    printf("%c", Caesar(msg[k],j));
		}
		printf("\n");
	}
}

void decrypt_input_to_str_file(int shift, int ind_b){
    
    unsigned long i=0, crc_val=0;
    fe = fopen("strdec.txt", "wb");
    printf("Input message:");
	while ((ch = getchar()) != '\n'){
	    data[i] = ch;
	    i++;
	}
	if (shift==-1)
	    shift = 5;
	if (ind_b==-1)
	    signa = 0xccee;
	else 
	    signa = 0xeecc;
	fwrite(&signa, 2, 1, fe);
    for (int j =0; j<i; j++)
	    denc[j] = Caesar(data[j], 26-shift);
	fwrite(&shift, 1, 1, fe);
	fwrite(&i, 4, 1, fe);
	fwrite(denc, i, 1, fe);
	crc_val = crc32(data, i);
	fwrite(&crc_val, 4, 1, fe);
}

void decrypt_input_with_shift(int shift){
    printf("Input message:");
	while ((ch = getchar()) != '\n' && ch != EOF){
	    if (i==0){
	        printf("Decoded message:");
	        i=1;
	    }
		printf("%c", Caesar(ch, 26-shift));
	}
	printf("\n");
}

void decrypt_file(FILE* fm, FILE* fe){
    int i = 0;
	while ((ch = fgetc(fm))!=EOF){
		msg[i]=ch;
	    i++;
	}
    for (int j =1; j<26; j++){
	    for (int k=0; k<i; k++){
		    if (k==0)
		        fprintf(fe,"If shift=%d:", j);
		    fprintf(fe,"%c", Caesar(msg[k],j));
		}
		fprintf(fe, "\n");
	}
}

void decrypt_file_with_shift(FILE* fm, int shift){
    fe = fopen("dec.txt", "w");
	while ((ch = fgetc(fm))!=EOF)
		fprintf(fe,"%c", Caesar(ch, 26-shift));
    fprintf(fe, "\n");
}

void decrypt_file_to_str_file(int shift, int ind_b){
    unsigned long i=0, crc_val=0;
    fe = fopen("strdec.txt", "wb");
	if (shift==-1)
	    shift = 5;
	if (ind_b==-1)
	    signa = 0xccee;
	else 
	    signa = 0xeecc;
	fwrite(&signa, 2, 1, fe);
	while ((ch = fgetc(fm))!=EOF ){
		data[i] =  ch;
		i++;
	}
    for (int j =0; j<i; j++)
	    denc[j] = Caesar(data[j], 26-shift);
	fwrite(&shift, 1, 1, fe);
	fwrite(&i, 4, 1, fe);
	fwrite(denc, i, 1, fe);
	crc_val = crc32(data, i);
	fwrite(&crc_val, 4, 1, fe);
}

void dec_strange_file_to_strange_file(FILE* fm){
    int fshift=0;
    unsigned long data_len=0;
    fe = fopen("lil_end_dec.txt", "wb");
    fread(&signa, 2, 1, fm); /*0xccee || 0xeecc        2 bytes*/
    fshift=fgetc(fm);   /*shift                    1 byte*/
    if (fshift!=shift && shift!=-1){
        printf("Different shifts\n");
		exit(EXIT_FAILURE);
    }
    shift = fshift;
    fread(&data_len, 4, 1, fm); /*data len          4 bytes*/
    unsigned char dat[data_len], dec[data_len];
    fread(dat, data_len, 1, fm);
    fwrite(&signa, 2, 1, fe);
	fputc(fshift, fe);
	fwrite(&data_len, 4, 1, fe);
	for (unsigned long p=0; p<data_len; p++){
        dec[p] = Caesar(dat[p], 26-shift);
    }
    fwrite(dec, data_len, 1, fe);
    unsigned long crc_value = crc32(dat, data_len);
    fwrite(&crc_value, 4, 1, fe);
    fclose(fm);
    fclose(fe);
}

void dec_strange_file_to_file(FILE* fm){
    int fshift=0;
    unsigned long data_len=0;
    fe = fopen("dec.txt", "wb");
    fread(&signa, 2, 1, fm); /*0xccee || 0xeecc        2 bytes*/
    fshift=fgetc(fm);   /*shift                    1 byte*/
    if (fshift!=shift && shift!=-1){
        printf("Different shifts\n");
		exit(EXIT_FAILURE);
    }
    shift = fshift;
    fread(&data_len, 4, 1, fm); /*data len          4 bytes*/
    unsigned char dat[data_len], dec[data_len];
    fread(dat, data_len, 1, fm);
	for (unsigned long p=0; p<data_len; p++)
        fprintf(fe, "%c", Caesar(dat[p], 26-shift));
    fprintf(fe, "\n");
    fclose(fm);
    fclose(fe);
    
}

void troubleshooting_input(FILE* fm, FILE* fe){
    if (fm == NULL){
		fprintf(stderr, "no such input file or k is not an integer\n");
        perror("fopen()");
		exit(EXIT_FAILURE);
    }
    if (fe == NULL){
        fprintf(stderr, "no write file\n");
        perror("fopen()");
        exit(EXIT_FAILURE);
    }
}

int check_for_sign(FILE* fm){
    fread(&signa, 2, 1, fm);
    if (signa==0xeecc || signa==0xccee)
	    return 1;
	return 0;
}

int main(int argc, char **argv){
    int ind_f=-1, ind_d=-1,ind_c=-1, ind_k=-1, ind_b=-1, ind_t=-1, ind_sign=-1, ex=0, argccc=0;
    for (int i = 1; i < argc; i++) {
        switch (*argv[i]) {
            case 'c':
                ind_c = i;
                ex++;
                argccc++;
                break;
            case 'd':
                ind_d = i;
                ex++;
                argccc++;
                break;
            case 'b':
                ind_b = i;
                argccc++;
                break;
            case '-':
                ind_t = i;
                argccc++;
                break;
            default:
                if (is_number(argv[i])) {
                    ind_k = i;
                    ex++;
                    shift = atoi(argv[ind_k]);
                    argccc++;
                } else {
                    fm = NULL;
                    fm = fopen(argv[i], "r");
                    if (fm != NULL) {
                        if (check_for_sign(fm)) {
                            printf("YES\n");
                            ind_sign = i;
                            ex++;
                            argccc++;
                            fclose(fm);
                        } else {
                            ind_f = i;
                            ex++;
                            argccc++;
                            fclose(fm);
                        }
                    }
                }
        }
    }
    printf("%d %d %d %d %d %d %d\n", ind_f, ind_d,ind_c, ind_k, ind_b, ind_t, ind_sign);
	if (argc>5 || (ind_d!=-1 && ind_c!=-1)){
		fprintf(stderr, "usage: %s [c/d] [k] [file] [-] [b]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else if (argc!=argccc+1){
	    fprintf(stderr, "no such file or key\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else if (ind_f!=-1 && ind_sign!=-1){
	    fprintf(stderr, "something wrong with file\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else if (ind_t!=-1){
	    if (ex==0 || (ex==1 && ind_c!=-1)){
			    ask_for_shift();
			    message_encryption(shift);
	    }
	    else if (ex==1 || (ex==2 && ind_c!=-1)){
	            if (ind_d!=-1)
	                decrypt_input();
	            else if (ind_sign!=-1){
	                fm = fopen(argv[ind_sign], "rb");
	                enc_strange_file_to_output(fm);
	            }
	            else if (ind_f!=-1){
	                fm = fopen(argv[ind_f],"r");
	                fe = fopen("enc.txt", "w");
	                troubleshooting_input(fm, fe);
	                ask_for_shift();
			        file_encryption(fm, fe, shift);
			        fclose(fm);
			        fclose(fe);
	            }
	            else if (ind_k!=-1)
			        message_encryption(shift); 
	    }
	    else if (ex == 2 || (ex == 3 && ind_c!=-1)) { /*ind_d/c, ind_f/sign, ind_k*/
	        if (ind_d!=-1){
	            if (ind_k!=-1)
		    	    decrypt_input_with_shift(shift);
		    	else if (ind_f!=-1){
		    	    fm = fopen(argv[ind_f],"r");
		    	    fe = fopen("dec.txt", "w");
		            decrypt_file(fm, fe);
		            fclose(fm);
	                fclose(fe);
		    	} 
		    	else if (ind_sign!=-1){
		    	    fm = fopen(argv[ind_sign], "rb");
		    	    dec_strange_file_to_file(fm);
		        }
	        }
	        else if (ind_k!=-1){
	            if (ind_f!=-1){
	                fm = fopen(argv[ind_f],"r");
	                fe = fopen("enc.txt", "w");
	                troubleshooting_input(fm, fe);
		    	    file_encryption(fm, fe, shift);
		    	    fclose(fm);
		    	    fclose(fe);
	            }
	            else if (ind_sign!=-1){
	                fm = fopen(argv[ind_sign], "rb");
	                enc_strange_file_to_output(fm);
	            }
	        }
	    }
	    else if (ex == 3 ){
	        if (ind_d!=-1){
	            if (ind_f==-1){
	                fm = fopen(argv[ind_f],"r");
	                decrypt_file_with_shift(fm, shift);
	            }
	            else if (ind_sign!=-1){
	                fm = fopen(argv[ind_sign],"r");
	                dec_strange_file_to_file(fm);
	            }
	        }   
	    }
	}
	else if (ind_t==-1){
	    if (ex==0 || (ex==1 && ind_c!=-1))
	        message_encryption_to_str_file(shift);
	    else if (ex==1 || (ex==2 && ind_c!=-1)){
	            if (ind_d!=-1)
	                decrypt_input_to_str_file(shift, ind_b);
	            else if (ind_sign!=-1){
	                fm = fopen(argv[ind_sign], "rb");
	                enc_strange_file_to_strange_file(fm);
	            }
	            else if (ind_f!=-1){
	                fm = fopen(argv[ind_f], "rb");
	                file_encryption_to_str_file(fm, shift, ind_b);
	            }
	            else if (ind_k!=-1)
	                message_encryption_to_str_file(shift);
	    }
	    else if (ex == 2 || (ex == 3 && ind_c!=-1)) { /*ind_d/c, ind_f/sign, ind_k*/
	        if (ind_d!=-1){
	            if (ind_k!=-1)
	                decrypt_input_to_str_file(shift, ind_b);
		    	else if (ind_f!=-1){
		    	    fm = fopen(argv[ind_f], "rb");
		    	    decrypt_file_to_str_file(shift, ind_b);
		    	} 
		    	else if (ind_sign!=-1){
		    	    fm = fopen(argv[ind_sign], "rb");
		            dec_strange_file_to_strange_file(fm);
		        }
	        }
	        else if (ind_k!=-1){
	            if (ind_f!=-1){
	                fm = fopen(argv[ind_f], "r");
	                file_encryption_to_str_file(fm, shift, ind_b);
	            }
	            else if (ind_sign!=-1){
	                fm = fopen(argv[ind_sign], "rb");
	                enc_strange_file_to_strange_file(fm);
	            }
	        }
	    }
	    else if (ex == 3 ){
	        if (ind_d!=-1){
	            if (ind_f!=-1){
	                fm = fopen(argv[ind_f], "r");
	                decrypt_file_to_str_file(shift, ind_b);
	            }
	            else if (ind_sign!=-1){
	                fm = fopen(argv[ind_sign], "rb");
	                dec_strange_file_to_strange_file(fm);
	            }
	        }   
	    }
	}
	return 0;
}

