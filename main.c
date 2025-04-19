#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define BUF_SIZE 256
#define FILE_SIZE 1e6

int main(int argc, char *argv[]) {
    bool append_mode = false;
    int pocet_radku = 0;
    FILE *file;
    char *filename;
    char *file_buffer = malloc(sizeof(char)*FILE_SIZE);
    if (argc > 1) {
        if (argc >= 3) {
            printf("Too many arguments!\n");
            return 1;
        }
        filename = argv[1];
        file = fopen(filename, "r");
        while (getc(file) != EOF) {
            file_buffer[pocet_radku++] = getc(file);
        }
        fclose(file);
        printf("%d\n", pocet_radku);
    }
    char input[BUF_SIZE];
    while (strcmp(input, "q")) {
        fgets(input, BUF_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;
        if (!append_mode) {
            if (!strcmp(input, "a"))
                append_mode = true;
            else if (!strcmp(input, "p"))
                printf(file_buffer);
            else if (input[0] == *"w") {
                printf("Write comming soon...\n");
            }
        } else if (!strcmp(input, ".")) {
            append_mode = false;
        } else {
            for (size_t i = 0; i < strlen(input); i++)
                file_buffer[pocet_radku++] = input[i];
            file_buffer[pocet_radku++] = *"\n";
        }
    }
    free(file_buffer);
    return 0;
}
