#include "command.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct command* create_command(char* name, char* description, char* pattern, size_t nmatch) {
    if(name == NULL || description == NULL || name[0] == '\0' || description[0] == '\0'){
        return NULL;
    }

    struct command* newCommand = malloc(sizeof(struct command));
    if(newCommand == NULL){
        return NULL;
    }

    newCommand->name = malloc(strlen(name)+1);
    strcpy(newCommand->name, name);
    newCommand->description = malloc(strlen(description)+1);
    strcpy(newCommand->description, description);


    int ret = regcomp(&newCommand->preg, pattern, REG_EXTENDED | REG_ICASE);
    if (ret != 0) {
        printf("Error in regex\n");
        free(newCommand->name);
        free(newCommand->description);
        free(newCommand);
        return NULL;
    }
    return newCommand;
}

struct command* destroy_command(struct command* command){
    if (command == NULL) return NULL;
    free(command->name);
    free(command->description);
    regfree(&command->preg); // Free the compiled regular expression
    for (size_t i = 0; i < command->nmatch; i++) {
        free(command->groups[i]);
    }
    free(command->groups);
    free(command);
    return NULL;
}