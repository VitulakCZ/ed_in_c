#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#define BUF_SIZE 256
#define ARGS_LIMIT 5
#define FILE_SIZE 1e7
#define ERROR printf("?\n")

size_t string_length(const char *s) {
    size_t length = 0;
    while (*s++ != '\0')
        length++;
    return length;
}

int main(int argc, char *argv[]) {
    bool append_mode = false;
    int c;
    FILE *file;
    char *filename;
    char **file_buffer = malloc(sizeof(char*)*FILE_SIZE);
    int pocet_charu = 0;
    int pocet_radku = 0;
    int pointer = 1;
    if (argc > 1) {
        if (argc >= 3) {
            printf("Too many arguments!\n");
            return 1;
        }
        filename = argv[1];
        if (access(filename, F_OK)) {
            printf("%s: No such file or directory\n", filename);
            goto begin;
        }
        file = fopen(filename, "r");
        while ((c = getc(file)) != EOF) {
            pocet_charu++;
            if (c == '\n') {
                pointer++;
            }
        }
        fclose(file);
        file = fopen(filename, "r");
        char line[BUF_SIZE];
        while (fgets(line, sizeof(line), file)) {
            file_buffer[pocet_radku] = malloc(strlen(line) + 1);
            strcpy(file_buffer[pocet_radku++], line);
        }
        fclose(file);
        printf("%d\n", pocet_charu);
    }
begin:
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
                if ((input[i] == ' ' && slovo)) {
                    strcpy(argumenty[pocet_args++], argument);
                    memset(argument, 0, BUF_SIZE);
                    pocet_pismen = 0;
                    slovo = false;
                }
                if (input[i] != ' ') {
                    slovo = true;
                    argument[pocet_pismen++] = input[i];
                }
                if ((i + 1 == strlen(input)) && input[i] != ' ') {
                    strcpy(argumenty[pocet_args++], argument);
                    memset(argument, 0, BUF_SIZE);
                    pocet_pismen = 0;
                    slovo = false;
                }
            }
            if (!strcmp(argumenty[0], "a"))
                append_mode = true;
            else if (!strcmp(argumenty[0], ",p"))
                for (int i = 0; i < pocet_radku; i++)
                    printf("%s", file_buffer[i]);
            else if (strcmp(argumenty[0], "w") == 0) {
                filename = argumenty[1];
                if (strcmp(argumenty[1], "") == 0 || strcmp(argumenty[2], "") != 0)  {
                    if (argc < 2) {
                        ERROR;
                        continue;
                    }
                    filename = argv[1];
                }
                file = fopen(filename, "w");
                for (int i = 0; i < pocet_radku; i++)
                    fprintf(file, "%s", file_buffer[i]);
                fclose(file);
                printf("%d\n", pocet_charu);
            }
            else if (strcmp(argumenty[0], "") == 0) {
                if (pointer < pocet_radku)
                    pointer++;
                else
                    ERROR;
            }
            else if (atoi(input)) {
                if (atoi(input) <= pocet_radku)
                    pointer = atoi(input);
                else
                    ERROR;
            }
            else if (strcmp(argumenty[0], "q") == 0)
                break;
            else ERROR;
        } else if (strcmp(input, ".") == 0) {
            append_mode = false;
        } else {
            strcat(input, "\n");
            pocet_charu += string_length(input);
            file_buffer[pocet_radku++] = malloc(BUF_SIZE);
            char **file_buffer_cpy = malloc(sizeof(char*)*FILE_SIZE);
            for (int i = 0; i < pocet_radku; i++) {
                file_buffer_cpy[i] = (char*)malloc(strlen(file_buffer[i]) + 1);
                strcpy(file_buffer_cpy[i], file_buffer[i]);
            }
            for (int i = pointer-1; i+1 < pocet_radku; i++) {
                strcpy(file_buffer[i+1], file_buffer_cpy[i]);
            }
            strcpy(file_buffer[pointer-1], input);
            pointer++;
            free(file_buffer_cpy);
        }
    }
    free(file_buffer);
    return 0;
}
