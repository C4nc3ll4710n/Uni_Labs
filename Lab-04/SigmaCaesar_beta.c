#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char ch;
int shift=-1, i=0;
char msg[256];
unsigned short sign=0; 
FILE* fe = NULL;
FILE* fm = NULL;

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

void file_encryption_LE(FILE* fm, FILE* fe){
    if (shift!=-1){
        ask_for_shift();
		getchar();
    }
    unsigned char chc;
    while ((chc = fgetc(fm))!=EOF)
		fputc( Caesar(chc, shift), fe);
    fclose(fm);
    fclose(fe);
}

void file_encryption_BE(FILE* fm, char *f, FILE* fe){
    char ch;
    unsigned char data[4294967295];
    while ((ch = fgetc(fm))!=EOF){
		data[i]=(unsigned char)ch;
		i++;
	}
	for (unsigned long j=0; j<i; i+=4){
	    for (int q=3; q>=0; q--)
	        fputc(Caesar(data[j+q], shift), fe);
	}
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

void decrypt_file_with_shift(FILE* fm, FILE* fe, int shift){
	while ((ch = fgetc(fm))!=EOF)
		fprintf(fe,"%c", Caesar(ch, 26-shift));
    fprintf(fe, "\n");
}

void troubleshooting_files(){
    if (fm == NULL){
	    fprintf(stderr, "no input file\n");
	    perror("fopen()");
	    exit(EXIT_FAILURE);
    }
    if (fe == NULL){
        fprintf(stderr, "no write file\n");
        perror("fopen()");
        exit(EXIT_FAILURE);
    }
}

void troubleshooting_input(){
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
    unsigned short s;
    fread(&s, 2, 1, fm);
    if (s==0xeecc || s==0xccee)
	    return 1;
	return 0;
}

unsigned long rev(unsigned long len){
    return ((len & 0xFF) << 24) | (((len >> 8) & 0xFF) << 16) | (((len >> 16) & 0xFF) << 8) | ((len >> 24) & 0xFF);
}

unsigned long crc32(unsigned char *data, unsigned long data_len){
    unsigned int crc=0, i, j, size=0;
    unsigned char c;
	unsigned int crc_table[256];
	
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
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
void print_ans_to_strange_file_from_input_LE(){
    fe = fopen("lil_end_enc.txt","wb");
    unsigned short s=0xeecc;
    unsigned long data_len, l=0xFFFFFFFF, i=0;
    unsigned char data[4294967295];
    fwrite(&s, 2, 1, fe);
    ask_for_shift();
    fputc(shift, fe);
    fwrite(&l, 4, 1, fe);
    printf("Input message:");
	while ((ch = getchar()) != '\n' && ch != EOF){
		fputc(Caesar(ch, shift),fe);
		data[i]=ch;
		i++;
	}
	unsigned long crc_value = crc32(data, i);
    fwrite(&crc_value, 4, 1, fe);
    fclose(fe);
}
/*???????????????????????????????????????????????????????????*/
void print_ans_from_norm_file_to_file_strange_LE(FILE* fm, char *f) {
    unsigned short s=0xeecc;
    fe = fopen("lil_end_enc.txt" , "wb") ;
    fwrite(&s, 2,1, fe);
    if (shift==-1) 
        ask_for_shift();
    unsigned long len_dat=0, k=0, crc_value=0;
    char ch;
    while((ch=fgetc(fm))!=EOF) 
        len_dat++;
    fclose(fm) ;
    fwrite(&len_dat, 4,1, fe) ;
    fm=fopen(f,"rb");
    unsigned char chc, data[len_dat] ;
    while (k<len_dat) {
        k++;
        chc=fgetc(fm);
        fputc(Caesar(chc, shift), fe) ;
    }
    crc_value=crc32(data, len_dat);
    fwrite(&crc_value, 4,1,fe);
    fclose(fm) ;
    fclose(fe);
}

void print_ans_from_norm_file_to_file_strange_BE(FILE* fm, char *f) {
    unsigned short s=0xccee;
    fe = fopen("big_end_enc.txt" , "wb") ;
    fwrite(&s, 2,1, fe);
    if (shift==-1) 
        ask_for_shift();
    unsigned long len_dat=0, k=0, crc_value=0;
    char ch;
    while((ch=fgetc(fm))!=EOF) 
        len_dat++;
    fclose(fm) ;
    unsigned long revlen =rev(len_dat) ;
    fwrite(&revlen, 4,1, fe) ;
    fm=fopen(f,"rb");
    unsigned char chc, data[len_dat] ;
    while (k<len_dat) {
        ch = fgetc(fm);
        data[k]=ch;
	    	k++;
    }
	for (unsigned long j=0; j<k; k+=4){
	    for (int q=3; q>=0; q--)
	        fputc(Caesar(data[j+q], shift), fe);
	}
	
    crc_value=crc32(data, len_dat);
    fwrite(&crc_value, 4,1,fe);
    fclose(fm) ;
    fclose(fe);
}

void print_ans_to_strange_file_from_strange_file_LE(FILE* fm){
    unsigned short s;
    int fshift=0;
    unsigned long data_len;
    fe = fopen("lil_end_enc.txt", "wb");
    fread(&s, 2, 1, fm); /*0xccee || 0xeecc        2 bytes*/
    fshift=fgetc(fm);   /*shift                    1 byte*/
    if (fshift!=shift && shift!=-1){
        printf("Different shifts\n");
		exit(EXIT_FAILURE);
    }
    fread(&data_len, 4, 1, fm); /*data len          4 bytes*/
	data_len = rev(data_len);
    unsigned char data[data_len];
    fread(data, data_len, 1, fm);
    
    fwrite(&s, 2, 1, fe);
	fputc(fshift, fe);
	fwrite(&data_len, 4, 1, fe);
	for (unsigned long i=0; i<data_len; i++){
        fputc(Caesar(data[i], fshift), fe);
    }
    unsigned long crc_value = crc32(data, data_len);
    fwrite(&crc_value, 4, 1, fe);
    fclose(fm);
    fclose(fe);
    
}


void print_ans_to_strange_file_from_input_BE(){
    
    fe = fopen("big_end_enc.txt","wb");
    unsigned short s=0xccee;/*changed*/
    unsigned long data_len, l=0xFFFFFFFF, i=0;
    unsigned char data[4294967295];
    fwrite(&s, 2, 1, fe);
    ask_for_shift();
    getchar();
    fputc(shift, fe);
    fwrite(&l, 4, 1, fe);
    printf("Input message:");
	while ((ch = getchar()) != '\n' && ch != EOF){
		data[i]=ch;
		i++;
	}
	for (unsigned long j=0; j<i; j+=4){
	    if (j+3<data_len){
	        for (int q=3; q>=0; q--)
	            fputc(Caesar(data[j+q], shift), fe);
	    }
	    else {
	         if (data_len%4==3){
	            fputc(Caesar(data[j+(data_len%4)], shift), fe);
	            fputc(Caesar(data[j+(data_len%4-1)], shift), fe);
	            fputc(Caesar(data[j+(data_len%4-2)], shift), fe);
	        }
	        else if (data_len%4==2){
	            fputc(Caesar(data[j+(data_len%4)], shift), fe);
	            fputc(Caesar(data[j+(data_len%4-1)], shift), fe);
	        }
	        else if (data_len%4==1)
	            fputc(Caesar(data[j+(data_len%4)], shift), fe);

	    }
	    
	}
	unsigned long crc_value = rev(crc32(data, i));
    fwrite(&crc_value, 4, 1, fe);
    fclose(fe);
}

void print_ans_to_strange_file_from_strange_file_BE(FILE* fm){
    /*в таком случае сдвиг считывается из файла и занимает 2 байта чтобы не сдохнуть(*/
    fe = fopen("big_end_enc.txt", "wb");
    unsigned short s;
    int fshift=0;
    unsigned long data_len;
    fread(&fshift, 2, 1, fm);   /*shift            2 bytes*/
    shift=rev(fshift);
    fread(&s, 2, 1, fm); /*0xccee || 0xeecc        2 bytes*/
    
    if (fshift!=shift && shift!=-1){
        printf("Error: Different shifts\n");
		exit(EXIT_FAILURE);
    }
    fread(&data_len, 4, 1, fm); /*data len          4 bytes*/
    
    unsigned char data[data_len];
    fread(data, data_len, 1, fm);
    
    fwrite(&s, 2, 1, fe);
	fputc(shift, fe);
	for (unsigned long i=0; i<data_len; i++){
        fputc(Caesar(data[i], shift), fe);
    }
    for (unsigned long j=0; j<data_len; j+=4){
	    for (int q=3; q>=0; q--)
	        fputc(Caesar(data[j+q], shift), fe);
	}
    unsigned long crc_value = crc32(data, data_len);
    crc_value=rev(crc_value);
    fwrite(&crc_value, 4, 1, fe);
    fclose(fm);
    fclose(fe);
    
}
/*00000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
void print_ans_to_norm_file_from_strange_file_LE(FILE* fm){
    unsigned short s;
    int fshift=0;
    unsigned long data_len;
    fe = fopen("enc.txt", "wb");
    fread(&s, 2, 1, fm); /*0xccee || 0xeecc        2 bytes*/
    fshift=fgetc(fm);   /*shift                    1 byte*/
    if (fshift!=shift && shift!=-1){
        printf("Different shifts\n");
		exit(EXIT_FAILURE);
    }
    fread(&data_len, 4, 1, fm); /*data len          4 bytes*/
	data_len = rev(data_len);
    unsigned char data[data_len];
    fread(data, data_len, 1, fm);
    
	for (unsigned long i=0; i<data_len; i++){
        fputc(Caesar(data[i], fshift), fe);
    }
    fclose(fm);
    fclose(fe);
    
}

void print_ans_to_strange_file_from_strange_file_BE(FILE* fm){
    /*в таком случае сдвиг считывается из файла и занимает 2 байта чтобы не сдохнуть(*/
    fe = fopen("enc.txt", "wb");
    unsigned short s;
    int fshift=0;
    unsigned long data_len;
    fread(&fshift, 2, 1, fm);   /*shift            2 bytes*/
    shift=rev(fshift);
    fread(&s, 2, 1, fm); /*0xccee || 0xeecc        2 bytes*/
    
    if (fshift!=shift && shift!=-1){
        printf("Error: Different shifts\n");
		exit(EXIT_FAILURE);
    }
    fread(&data_len, 4, 1, fm); /*data len          4 bytes*/
    
    unsigned char data[data_len];
    fread(data, data_len, 1, fm);
    
	for (unsigned long i=0; i<data_len; i++)
        fputc(Caesar(data[i], shift), fe);
    
    
    fclose(fm);
    fclose(fe);
    
}

/*00000000000000000000000000000000000000000000000000000000000000000000000000000000000*/

/*ДЕКТРИПТЫ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

void dec_print_ans_to_strange_file_from_input_LE(){
    
    fe = fopen("lil_end_enc.txt","wb");
    unsigned short s=0xeecc;
    unsigned long data_len, l=0xFFFFFFFF, i=0;
    unsigned char data[4294967295];
    fwrite(&s, 2, 1, fe);
    if (shift==-1){
        ask_for_shift();
		getchar();
    }
    fputc(shift, fe);
    fwrite(&l, 4, 1, fe);
    printf("Input message:");
	while ((ch = getchar()) != '\n' && ch != EOF){
		fputc(Caesar(ch, 26-shift), fe);
		data[i]=ch;
		i++;
	}
	unsigned long crc_value = crc32(data, i);
    fwrite(&crc_value, 4, 1, fe);
    fclose(fe);
}

void dec_print_ans_to_strange_file_from_strange_file_LE(FILE* fm){
    unsigned short s;
    int fshift=0;
    unsigned long data_len;
    fe = fopen("lil_end_enc.txt", "wb");
    fread(&s, 2, 1, fm); /*0xccee || 0xeecc        2 bytes*/
    fshift=fgetc(fm);   /*shift                    1 byte*/
    if (fshift!=shift && shift!=-1){
        printf("Different shifts\n");
		exit(EXIT_FAILURE);
    }
    fread(&data_len, 4, 1, fm); /*data len          4 bytes*/
	data_len = rev(data_len);
    unsigned char data[data_len];
    fread(data, data_len, 1, fm);
    
    fwrite(&s, 2, 1, fe);
	fputc(shift, fe);
	for (unsigned long i=0; i<data_len; i++){
        fputc(Caesar(data[i], 26-fshift), fe);
    }
    unsigned long crc_value = crc32(data, data_len);
    fwrite(&crc_value, 4, 1, fe);
    fclose(fm);
    fclose(fe);
    
}

void dec_print_ans_to_strange_file_from_input_BE(){
    
    fe = fopen("big_end_enc.txt","wb");
    unsigned short s=0xccee;/*changed*/
    unsigned long data_len, l=0xFFFFFFFF, i=0;
    unsigned char data[4294967295], dec[4294967295];
    fwrite(&s, 2, 1, fe);
    if (shift==-1){
        ask_for_shift();
		getchar();
    }
    fputc(shift, fe);
    fwrite(&l, 4, 1, fe);
    printf("Input message:");
	while ((ch = getchar()) != '\n' && ch != EOF){
		data[i]=ch;
		i++;
	}
	for (unsigned long j=0; j<i; i+=4){
	    for (int q=3; q>=0; q--){
	        fputc(Caesar(data[j+q], 26-shift), fe);
	        dec[j+q]=data[j+q];
	    }
	}
	unsigned long crc_value = rev(crc32(data, i));
    fwrite(&crc_value, 4, 1, fe);
    fclose(fe);
}

void dec_print_ans_to_strange_file_from_strange_file_BE(FILE* fm){
    /*в таком случае сдвиг считывается из файла и занимает 2 байта чтобы не сдохнуть(*/
    fe = fopen("big_end_enc.txt", "wb");
    unsigned short s;
    int fshift=0;
    unsigned long data_len;
    fread(&fshift, 2, 1, fm);   /*shift            2 bytes*/
    shift=rev(fshift);
    fread(&s, 2, 1, fm); /*0xccee || 0xeecc        2 bytes*/
    
    if (fshift!=shift && shift!=-1){
        printf("Different shifts\n");
		exit(EXIT_FAILURE);
    }
    fread(&data_len, 4, 1, fm); /*data len          4 bytes*/
    
    unsigned char data[data_len];
    fread(data, data_len, 1, fm);
    
    fwrite(&s, 2, 1, fe);
	fputc(shift, fe);
	for (unsigned long i=0; i<data_len; i++){
        fputc(Caesar(data[i], 26-shift), fe);
    }
    unsigned long crc_value = crc32(data, data_len);
    crc_value=rev(crc_value);
    fwrite(&crc_value, 4, 1, fe);
    fclose(fm);
    fclose(fe);
    
}

void dec_print_ans_from_norm_file_to_file_strange_LE(FILE* fm, char *f) {
    unsigned short s=0xeecc;
    fe = fopen("lil_end_enc.txt" , "wb") ;
    fwrite(&s, 2,1, fe);
    if (shift==-1){ 
        ask_for_shift();
		getchar();
    }
    unsigned long len_dat=0, k=0, crc_value=0;
    char ch;
    while((ch=fgetc(fm))!=EOF) 
        len_dat++;
    fclose(fm) ;
    fwrite(&len_dat, 4,1, fe) ;
    fm=fopen(f,"rb");
    unsigned char chc, data[len_dat] ;
    while (k<len_dat) {
        chc=fgetc(fm);
        fputc(Caesar(chc, 26-shift), fe) ;
        data[k]=Caesar(chc, 26-shift);
        k++;
    }
    crc_value=crc32(data, len_dat);
    fwrite(&crc_value, 4,1,fe);
    fclose(fm) ;
    fclose(fe);
}

void dec_print_ans_from_norm_file_to_file_strange_BE(FILE* fm, char *f){
    unsigned short s=0xccee;
    fe = fopen("big_end_enc.txt" , "wb") ;
    fwrite(&s, 2,1, fe);
    if (shift==-1) {
        ask_for_shift();
        getchar();
    }
    unsigned long len_dat=0, k=0, crc_value=0;
    char ch;
    while((ch=fgetc(fm))!=EOF) 
        len_dat++;
    fclose(fm) ;
    unsigned long revlen =rev(len_dat) ;
    fwrite(&revlen, 4,1, fe) ;
    fm=fopen(f,"rb");
    unsigned char chc, data[len_dat], encdat[len_dat] ;
    while (k<len_dat) {
        ch = fgetc(fm);
        data[k]=ch;
	    	k++;
    }
	for (unsigned long j=0; j<k; k+=4){
	    for (int q=3; q>=0; q--){
	        fputc(Caesar(data[j+q], 26-shift), fe);
	        encdat[j+q]=Caesar(data[j+q], 26-shift);
	    }
	        
	}
	
    crc_value=crc32(data, len_dat);
    fwrite(&crc_value, 4,1,fe);
    fclose(fm) ;
    fclose(fe);
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/



int main(int argc, char **argv){
    int ind_f=-1, ind_d=-1,ind_c=-1, ind_k=-1, ind_b=-1, ind_t=-1, ind_sign=-1, ex=0;
    for (int i=1; i<argc; i++){
        if (*argv[i]== 'c'){
            ind_c = i;
            ex++;
        }
        if (*argv[i]== 'd'){
            ind_d = i;
            ex++;
        }
        if (*argv[i]== 'b'){
            ind_b = i;
            ex++;
        }
        if (is_number(argv[i])){
            ind_k = i;
            ex++;
            shift = atoi(argv[ind_k]);
        }
        if (*argv[i]== '-'){
            ind_t = i;
            ex++;
        }
        fm = NULL;
        fm = fopen(argv[i],"r");
        if (fm!=NULL){
            if (check_for_sign(fm)){
                printf("YES\n");
                ind_sign = i;
                ex++;
                fclose(fm);
            }
            else {
                ind_f=i;
                ex++;
                fclose(fm);
            }
        }
    }
    printf("%d %d %d %d %d %d %d\n", ind_f, ind_d,ind_c, ind_k, ind_b, ind_t, ind_sign);
    
	if (argc>6 || (ind_d!=-1 && ind_c!=-1)){
		fprintf(stderr, "usage: %s [c/d] [k] [file] [b] [-]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else if (ex==0 || (ex==1 && ind_c!=-1))/*argc == 1 || (argc == 2 && *argv[1]=='c')){  ./a.out      ./a.out c */
		print_ans_to_strange_file_from_input_LE();
	else if (ex==1 || (ex==2 && ind_c!=-1)){/*argc == 2 || (argc==3 && (*argv[1]=='c' || *argv[2]=='c'))){  ./a.out d/file/k [с]*/
	        if (ind_d!=-1) /*./a.out d */
	            dec_print_ans_to_strange_file_from_input_LE();
	        else if (ind_f!=-1){
	            fm = fopen(argv[ind_f], "r");
	            print_ans_from_norm_file_to_file_strange_LE(fm, argv[ind_f]);
	        }
	        else if (ind_k!=-1)
			    print_ans_to_strange_file_from_input_LE();
			else if (ind_b!=-1)/*АХТУНГ БИГ ЕНДИАН!!!! */
			    print_ans_to_strange_file_from_input_BE();
			else if (ind_sign!=-1){
			    fm = fopen(argv[ind_sign], "rb");
			    print_ans_to_strange_file_from_strange_file_LE(fm);
			}    
			else { /*ind_t!=-1*/
			    ask_for_shift();
		        getchar();
		        message_encryption(shift);   
			}
    }
	else if (ex==2 || (ex==3 && ind_c!=-1)){/*argc == 3 || (argc==4 && (*argv[1]=='c' || *argv[2]=='c' ||*argv[3]=='c'))) { /*./a.out d/file/k d/file/k [с]*/
	    if (ind_d!=-1){
	        if (ind_k!=-1)
			    dec_print_ans_to_strange_file_from_input_LE();
			else if (ind_f!=-1){
			    fm = fopen(argv[ind_f],"r");
			    dec_print_ans_from_norm_file_to_file_strange_LE(fm, argv[ind_f]);
			} 
			else if (ind_b!=-1)
			    dec_print_ans_to_strange_file_from_input_BE();
			else /*ind_t*/
			    decrypt_input();
	    }
	    else if (ind_k!=-1){
	        if (ind_f!=-1){
			    fm = fopen(argv[ind_f],"r");
			    print_ans_from_norm_file_to_file_strange_LE(fm, argv[ind_f]);
			} 
			else if (ind_b!=-1)
			    print_ans_to_strange_file_from_input_BE();
			else if (ind_sign!=-1){
			    fm = fopen(argv[ind_sign],"r");
			    print_ans_to_strange_file_from_strange_file_LE(fm);
			}   
			else /*ind_t!=-1*/
			    message_encryption(atoi(argv[ind_k]));
	    }
	    else if (ind_f!=-1){
	        if (ind_b!=-1){
	            fm = fopen(argv[ind_f], "r");
	            print_ans_from_norm_file_to_file_strange_BE(fm, argv[ind_f]);
	        }
	        else if (ind_sign!=-1){
			    printf("Error: something with file");
			    exit(EXIT_FAILURE);
			} 
	        else { /*ind_t!=-1*/
	            fm = fopen(argv[ind_f],"r");
			    fe = fopen("enc.txt", "w");
			    file_encryption_LE(fm, fe);
	        }
	    }
	    else if (ind_b!=-1){
	        if (ind_sign!=-1){
	            fm = fopen(argv[ind_sign], "rb");
			    print_ans_to_strange_file_from_strange_file_BE(fm);
			} 
	        else { /*ind_t!=-1*/ 
	            ask_for_shift();
		        getchar();
		        message_encryption(shift); 
	        }
	    }
	}
	else if (ex==3 || (ex==4 && ind_c!=-1)){
	    /*ind_f, ind_d,ind_c, ind_k, ind_b, ind_t, ind_sign*/
	       
	    if (ind_d!=-1){ 
	        if (ind_f!=-1){
	            if (ind_k!=-1){
	                fm = fopen(argv[ind_f], "r");
	                dec_print_ans_from_norm_file_to_file_strange_LE(fm, argv[ind_f]);
	            }
	            else if (ind_b!=-1){
	                fm = fopen(argv[ind_f], "r");
	                dec_print_ans_from_norm_file_to_file_strange_BE(fm, argv[ind_f]);
	            }
	            else{ /*ind_t!=-1*/
	                fm = fopen(argv[ind_f], "r");
	                fe = fopen("dec.txt", "w");
	                decrypt_file(fm, fe);
	            }
	        }
	        else if (ind_k!=-1){
	            if (ind_b!=-1){
	                dec_print_ans_to_strange_file_from_input_BE();
	            }
	            else if (ind_t!=-1)
	                decrypt_input_with_shift(shift);
	            else { /*ind_sign!=-1*/
	                fm = fopen(argv[ind_sign], "rb");
	                dec_print_ans_to_strange_file_from_strange_file_LE(fm);
	            }
	        }
	        else {/*if (ind_b!=-1){*/
	            if (ind_t!=-1){
	                decrypt_input();
	            }
	            else { /*ind_sign!=-1*/
	                dec_print_ans_to_strange_file_from_input_BE();
	            }
	        }/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	    }
	    else if (ind_f!=-1){
	        if (ind_k!=-1){
	            if (ind_b!=-1){
	                fm = fopen(argv[ind_f], "r");
	                print_ans_from_norm_file_to_file_strange_BE(fm, argv[ind_f]);
	            }
	            else {/*if (ind_t!=-1){*/
	                fm = fopen(argv[ind_f], "rb");
	                fe = fopen("enc.txt", "wb");
	               file_encryption_LE(fm, fe);
	            }
	        }
	        else { /*if (ind_b!=-1 && ind_t!=-1){*/
	            
	        }
	    
	    }
	    else if (ind_sign!=-1){
	        if (ind_k!=-1){
	            if (ind_t!=-1){
	                
	            }
	        }
	    }
	    
	}
	else{
	}
	return 0;
	
}

