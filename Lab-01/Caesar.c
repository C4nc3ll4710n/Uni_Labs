#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char **argv){
	if (argc>2){
		fprintf(stderr, "usage: %s [file]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else{
		if (argc==1){
		    char ch;
			int shift, i=0;
			printf("Input shift:");
			scanf("%d", &shift);
			printf("Input message:");
			getchar();
			while ((ch = getchar()) != '\n' && ch != EOF){
			    if (i==0){
			        printf("Encrypted message:");
			        i=1;
			    }
			    if (ch >= 'A' && ch <= 'Z') {
            		ch = ((ch - 'A') + shift) % 26 + 'A';
        		}
        		else if (ch >= 'a' && ch <= 'z') {
            		ch = ((ch - 'a') + shift) % 26 + 'a';
        		}
        		printf("%c", ch);
			}
			printf("\n");
		}
		else {
			FILE* fm = NULL;
			FILE* fe = NULL;
			fm = fopen(argv[1],"r");
			fe = fopen("enc.txt", "w");
			if (fm == NULL){
				fprintf(stderr, "no such file\n");
				perror("fopen()");
				exit(EXIT_FAILURE);
			}
			if (fe == NULL){
                fprintf(stderr, "no such file\n");
                perror("fopen()");
                exit(EXIT_FAILURE);
            }
			int shift;
			printf("Input shift:");
            scanf("%d", &shift);

			char ch, enc;
			while ((ch = fgetc(fm))!=EOF){
                if (ch >= 'A' && ch <= 'Z') {
                    ch = ((ch - 'A') + shift) % 26 + 'A';
                }
                else if (ch >= 'a' && ch <= 'z') {
                    ch = ((ch - 'a') + shift) % 26 + 'a';
                }
				fprintf(fe, "%c", ch);
			}
			fclose(fm);
			fclose(fe);
		}
	}
	return 0;
}
