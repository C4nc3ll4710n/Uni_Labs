#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int c, count_chars = 0, count_lines = 0, count_words = 0, in_word = 0;
    FILE *file;

    if (argc <2 || argc>5) {
        fprintf(stderr, "Usage: [-l] [-w] [-c] %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int file_index = 1;
    for (int i=1; i<argc; i++){
    	if (strcmp(argv[i],"-c") && strcmp(argv[i],"-w") && strcmp(argv[i],"-l"))
    		file_index = i;
    }
    file = fopen(argv[file_index], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: cannot open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Count the characters, lines, and words in the file
    while ((c = fgetc(file)) != EOF) {
        count_chars++;

        if (c == '\n') {
            count_lines++;
        }

        if (isspace(c)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count_words++;
        }
    }

    fclose(file);

    if (argc == 2) {
        printf("%d %d %d %s\n", count_lines, count_words, count_chars, argv[1]);
    } else {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-c") == 0) {
                printf("%d ", count_chars);
            } else if (strcmp(argv[i], "-l") == 0) {
                printf("%d ", count_lines);
            } else if (strcmp(argv[i], "-w") == 0) {
                printf("%d ", count_words);
            }
        }
        printf("%s\n", argv[1]);
    }

    return 0;
}
