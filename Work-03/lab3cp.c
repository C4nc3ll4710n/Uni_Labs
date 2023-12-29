#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv){
	if (argc<3||argc>4){
		fprintf(stderr, "usage: %s <file> %s [option] %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else{
		FILE* rf = NULL;
		FILE* wf = NULL; 
		rf = fopen(argv[1], "r");
		int overwrite = 0;
		for (int i = 1; i < argc; i++) {
        	if (strcmp(argv[i], "-i") == 0) {
            	overwrite = 1;
            	printf("Do you want to overwrite %s? (y/n) ", argv[2+overwrite]);
            	char c = getchar();
            	if (c != 'y' && c != 'Y') {
                	exit(EXIT_SUCCESS);
            		break;
        		}
        	}
    	}
        wf = fopen(argv[2+overwrite], "w");

		if (rf == NULL || wf == NULL){
			if (rf == NULL)
				fprintf(stderr, "no such read file\n", argv[0]);
			if (wf == NULL)
				fprintf(stderr, "no such write file\n", argv[0]);
			perror("fopen()");
			exit(EXIT_FAILURE);
		}
		else{
			char ch;
			while ((ch = fgetc(rf)) != EOF) {
        		fputc(ch, wf);
    		}
			fclose(rf);
			fclose(wf);


		}
	}
	return 0;
}
