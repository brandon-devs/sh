#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void execute(char** args) {

    if (strcmp(args[0], "cd") == 0) {

        chdir(args[1]);
        printf("Changing directories to: %s\n", args[1]);

    } else if ((strcmp(args[0], "quit") == 0)) {
        
        printf("Quitting\n");
        exit(0);

    } else {

    if (fork() == 0) {

        int status_code = execvp(args[0], args);

        if (status_code == -1) {
            printf("Error: Your command terminated incorrectly\n");
            exit(1);
        }

    }

    }

}

char** parse(char* input) {

    int all_tokens_buffer = 100;
    char** all_tokens = malloc(all_tokens_buffer * sizeof(char*));
    int all_tokens_counter = 0;

    int current_token_buffer = 1024;
    char* current_token = malloc(current_token_buffer * sizeof(char*));
    int current_token_counter = 0;

    for (int i = 0; i < strlen(input); i++) {

        char character = input[i];

        if (character == ' ') {

            if (all_tokens_counter + 1 > all_tokens_buffer) {
                all_tokens_buffer += 100;
                all_tokens = realloc(all_tokens, all_tokens_buffer * sizeof(char*));
            }
            
            if (current_token_counter + 1 >= current_token_buffer) {
                current_token_buffer += 1024;
                current_token = realloc(current_token, current_token_buffer);
            }

            all_tokens[all_tokens_counter] = current_token;
            all_tokens_counter++;

            current_token = malloc(current_token_buffer * sizeof(char*));
            current_token_counter = 0;

        } else if (character == '\n') {
            
            if (all_tokens_counter + 2 > all_tokens_buffer) {
                all_tokens_buffer += 100;
                all_tokens = realloc(all_tokens, all_tokens_buffer * sizeof(char*));
            }

            if (current_token_counter + 1 > current_token_buffer) {
                current_token_buffer += 1024;
                current_token = realloc(current_token, current_token_buffer);
            }
            
            all_tokens[all_tokens_counter] = current_token;
            all_tokens_counter++;

            all_tokens[all_tokens_counter] = NULL;
            break;

        } else {

            if (current_token_counter + 1 > current_token_buffer) {
                current_token_buffer += 1024;
                current_token = realloc(current_token, current_token_buffer);
            }

            current_token[current_token_counter] = character;
            current_token_counter++;

        }

    }

    return all_tokens;
}

char* read_line() {
    // Thanks to https://bit.ly/2EQE7A6 for the tutorial on reading from stdin with getline()

    char* buffer;
    size_t bufsize = 100;
    size_t line;

    buffer = (char*) malloc(bufsize * sizeof(char));

    if (buffer == NULL) {
        printf("Error: Unable to allocate buffer");
        exit(1);
    }

    line = getline(&buffer, &bufsize, stdin);

    return buffer;
}

int main() {
    printf("Welcome to sh!\n");
    printf("Made by BrandonDoesDev\n");
    printf("\n");

    while (true) {        
        char* input = read_line();

        char** parsed = parse(input);

        execute(parsed);

        free(input);
        free(parsed);
    }

    return 0;
}
