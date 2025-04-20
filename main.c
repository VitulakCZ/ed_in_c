#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define BUF_SIZE 256
#define ARGS_LIMIT 5
#define FILE_SIZE 1e7
#define ERROR printf("?\n")

int main(int argc, char *argv[]) {
    bool append_mode = false;
    int c;
    FILE *file;
    char *filename;
    char *file_buffer = malloc(sizeof(char)*FILE_SIZE);
    int pocet_radku = 0;
    if (argc > 1) {
        if (argc >= 3) {
            printf("Too many arguments!\n");
            return 1;
        }
        filename = argv[1];
        file = fopen(filename, "r");
        while ((c = getc(file)) != EOF) {
            file_buffer[pocet_radku++] = c;
        }
        fclose(file);
        printf("%d\n", pocet_radku);
    }
    char input[BUF_SIZE];
    while (true) {
        fgets(input, BUF_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;
        
        if (!append_mode) {
            bool slovo = false;
            char argument[BUF_SIZE];
            memset(argument, 0, BUF_SIZE);
            char argumenty[ARGS_LIMIT][BUF_SIZE];
            memset(argumenty, 0, ARGS_LIMIT*BUF_SIZE);
            int pocet_pismen = 0;
            int pocet_args = 0;
            for (int i = 0; i < strlen(input); i++) {
                if ((input[i] == *" " && slovo)) {
                    strcpy(argumenty[pocet_args++], argument);
                    memset(argument, 0, BUF_SIZE);
                    pocet_pismen = 0;
                    slovo = false;
                }
                if (input[i] != *" ") {
                    slovo = true;
                    argument[pocet_pismen++] = input[i];
                }
                if ((i + 1 == strlen(input)) && input[i] != *" ") {
                    strcpy(argumenty[pocet_args++], argument);
                    memset(argument, 0, BUF_SIZE);
                    pocet_pismen = 0;
                    slovo = false;
                }
            }
            if (!strcmp(argumenty[0], "a"))
                append_mode = true;
            else if (!strcmp(argumenty[0], "p"))
                printf(file_buffer);
            else if (!strcmp(argumenty[0], "w")) {
                filename = argumenty[1];
                if (!strcmp(argumenty[1], "") || strcmp(argumenty[2], ""))  {
                    ERROR;
                    continue;
                }
                file = fopen(filename, "w");
                fprintf(file, "%s", file_buffer);
                fclose(file);
                printf("%d\n", pocet_radku);
            }
            else if (!strcmp(argumenty[0], "q"))
                break;
            else ERROR;
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
