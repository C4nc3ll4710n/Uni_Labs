#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char ch;
int shift=-1, i=0;
char msg[256];
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

void file_encryption(FILE* fm, FILE* fe, int shift){
    while ((ch = fgetc(fm))!=EOF){
		fprintf(fe, "%c", Caesar(ch, shift));
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
	while ((ch = fgetc(fm))!=EOF){
		fprintf(fe,"%c", Caesar(ch, 26-shift));
	}
    fprintf(fe, "\n");
}

int main(int argc, char **argv){
    
	if (argc>4){
		fprintf(stderr, "usage: %s [c/d] [k] [file]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else if (argc == 1 || (argc == 2 && *argv[1]=='c')){ /* ./a.out      ./a.out c */
			ask_for_shift();
			getchar();
			message_encryption(shift);
	}
	else if (argc == 2 || (argc==3 && (*argv[1]=='c' || *argv[2]=='c'))){  /*./a.out d/file/k [с]*/
	        if (argc == 2 && *argv[1]=='d') /*./a.out d */
	            decrypt_input();
	        else if (argc == 3 && (*argv[1]=='d' || *argv[2]=='d')) { /*./a.out [d] file/k [c]*/
	            fprintf(stderr, "usage: %s [c/d] [k] [file]\n", argv[0]);
	            exit(EXIT_FAILURE);
	        }
	        else {/*./a.out file/k [с]*/
	            int the_ind=1;
	            if (*argv[1]=='c')
	                the_ind=2;
	            /*------------------------------------------------------------*/
			    if (is_number(argv[the_ind]))
			        shift = atoi(argv[the_ind]);
			    if (shift ==-1){
			            fm = fopen(argv[the_ind],"r");
			            if (fm == NULL){
				            fprintf(stderr, "no such input file or k is not an integer\n");
				            perror("fopen()");
				            exit(EXIT_FAILURE);
			            }
			            fe = fopen("enc.txt", "w");
			            if (fe == NULL){
                            fprintf(stderr, "no write file\n");
                            perror("fopen()");
                            exit(EXIT_FAILURE);
                        }
			            ask_for_shift();
			            getchar();
			            file_encryption(fm, fe, shift);
			            fclose(fm);
			            fclose(fe);
			    }
			    else
			        message_encryption(shift);
			}
    }
	else if (argc == 3 || (argc==4 && (*argv[1]=='c' || *argv[2]=='c' ||*argv[3]=='c'))) { /*./a.out d/file/k d/file/k [с]*/
	    int ind1=1, ind2=2, the_ind=1;
	    if (argc ==3 && (*argv[1]=='d' || *argv[2]=='d')){
	        if (*argv[1]=='d')
	            the_ind=2;
	        if (is_number(argv[the_ind])){
			    shift = atoi(argv[the_ind]);
			    decrypt_input_with_shift(shift);
			}
			else{
			    fm = fopen(argv[the_ind],"r");
			    fe = fopen("enc.txt", "w");
			    decrypt_file(fm, fe);
			    fclose(fm);
		        fclose(fe);
			} 
	    }
	    else{
	        if(argc == 4 && (*argv[1]=='d' || *argv[2]=='d' ||*argv[3]=='d')){ /*./a.out [d] d/file/k [с]*/ 
	            fprintf(stderr, "usage: %s [c/d] [k] [file]\n", argv[0]);
	            exit(EXIT_FAILURE);
	        }
	        else if (argc == 4 && *argv[1]=='c')
	            ind1=3;
	        else if (argc == 4 && *argv[2]=='c')
	            ind2=3;
	        /*./a.out d/file/k d/file/k [с]*/
		    
		    if (is_number(argv[ind1])){
		        shift = atoi(argv[ind1]);
			    fm = fopen(argv[ind2],"r");
		    }
		    else if (is_number(argv[ind2])){
			    shift = atoi(argv[ind2]);
			    fm = fopen(argv[ind1],"r");
		    }
		    else {
			    fprintf(stderr, "no such input file or k is not an integer\n");
		        perror("fopen()");
			    exit(EXIT_FAILURE);
		    }
		    fe = fopen("enc.txt", "w");
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
		    file_encryption(fm, fe, shift);
	        fclose(fm);
		    fclose(fe);
	    }
	}
	else{
	    int ind1=1, ind2=2;
	    if(argc == 4 && (*argv[1]=='c' || *argv[2]=='c' ||*argv[3]=='c')){
	        fprintf(stderr, "usage: %s [c/d] [k] [file]\n", argv[0]);
	        exit(EXIT_FAILURE);
	    }
	    else if (argc == 4 && *argv[1]=='d')
	        ind1=3;
	    else if (argc == 4 && *argv[2]=='d')
	        ind2=3;
		if (is_number(argv[ind1])){
		    shift = atoi(argv[ind1]);
		    fm = fopen(argv[ind2],"r");
		}
		else if (is_number(argv[ind2])){
		    shift = atoi(argv[ind2]);			
		    fm = fopen(argv[ind1],"r");
		}
		else {
		    fprintf(stderr, "no such input file or k is not an integer\n");
			perror("fopen()");	
			exit(EXIT_FAILURE);
		}
		fe = fopen("enc.txt", "w");
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
        decrypt_file_with_shift(fm, fe, shift);
        fclose(fm);
		fclose(fe);
	}
	return 0;
}
