#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

char ch;
int shift=-1, i=0;

char Caesar(char ch, int shift){
    if (ch >= 'A' && ch <= 'Z')
        ch = ((ch - 'A') + shift) % 26 + 'A';
    else if (ch >= 'a' && ch <= 'z')
        ch = ((ch - 'a') + shift) % 26 + 'a';
    return ch;
}

int ask_for_shift(){
    printf("Input shift:");
	scanf("%d", &shift);
	return shift;
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

int main(int argc, char **argv){
    
	if (argc>3){
		fprintf(stderr, "usage: %s [k] [file]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else if (argc == 1){
			shift = ask_for_shift();
			getchar();
			message_encryption(shift);
	}
	else if (argc == 2){
			if (is_number(argv[1]))
			    shift = atoi(argv[1]);
			if (shift ==-1){
			    FILE* fm = NULL;
			    fm = fopen(argv[1],"r");
			    if (fm == NULL){
				    fprintf(stderr, "no such input file or k is not an integer\n");
				    perror("fopen()");
				    exit(EXIT_FAILURE);
			    }
			    FILE* fe = NULL;
			    fe = fopen("enc.txt", "w");
			    if (fe == NULL){
                    fprintf(stderr, "no write file\n");
                    perror("fopen()");
                    exit(EXIT_FAILURE);
                }
			    shift = ask_for_shift();
			    getchar();
			    file_encryption(fm, fe, shift);
			    fclose(fm);
			    fclose(fe);
			}
			else
			    message_encryption(shift);
		}
	else {
		    FILE* fm = NULL;
			if (is_number(argv[1])){
			    shift = atoi(argv[1]);
			    fm = fopen(argv[2],"r");
			}
			else if (is_number(argv[2])){
			    shift = atoi(argv[2]);
			    fm = fopen(argv[1],"r");
			}
			else {
			    fprintf(stderr, "no such input file or k is not an integer\n");
				perror("fopen()");
				exit(EXIT_FAILURE);
			}
			FILE* fe = NULL;
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
	return 0;
}
