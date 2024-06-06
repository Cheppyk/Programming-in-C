#include "parser.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void add_command_to_parser(struct parser* p, char* name, char* description, char* pattern, size_t nmatch) {
    struct command* cmd = create_command(name, description, pattern, nmatch);
    if (cmd) {
        struct container* newContainer = malloc(sizeof(struct container));
        if (newContainer) {
            newContainer->type = COMMAND;
            newContainer->command = cmd;
            newContainer->next = p->commands;
            p->commands = newContainer;
        }
    }
}

struct parser* create_parser() {
    struct parser* p = malloc(sizeof(struct parser));
    if (!p) return NULL;

    p->history = NULL;
    p->commands = NULL;

    add_command_to_parser(p, "KONIEC", "Príkaz ukončí rozohratú hru. Nastaví príslušný stav hry.", "^(QUIT|EXIT|KONIEC)$", 1);
    add_command_to_parser(p, "SEVER", "Presun do miestnosti nachádzajúcej sa na sever od aktuálnej.", "(S|SEVER)", 1);
    add_command_to_parser(p, "JUH", "Presun do miestnosti nachádzajúcej sa na juh od aktuálnej.", "^(J|JUH)$", 1);
    add_command_to_parser(p, "VYCHOD", "Presun do miestnosti nachádzajúcej sa na východ od aktuálnej.", "^(V|VYCHOD)$", 1);
    add_command_to_parser(p, "ZAPAD", "Presun do miestnosti nachádzajúcej sa na západ od aktuálnej.", "^(Z|ZAPAD)$", 1);
    add_command_to_parser(p, "ROZHLIADNI SA", "Príkaz vypíše aktuálne informácie o miestnosti, v ktorej sa hráč práve nachádza.", "^(LOOK|ROZHLIADNI SA)$", 1);
    add_command_to_parser(p, "PRIKAZY", "Príkaz vypíše na obrazovku zoznam všetkých príkazov, ktoré hra poskytuje.", "^(HELP|POMOC|PRIKAZY)$", 1);
    add_command_to_parser(p, "VERZIA", "Príkaz zobrazí číslo verzie hry, ľubovoľný sprievodný text a meno a priezvisko autora.", "^(VERSION|VERZIA)$", 1);
    add_command_to_parser(p, "RESTART", "Znovu spustí hru od začiatku. Zmení stav hry na požadovaný.", "^RESTART$", 1);
    add_command_to_parser(p, "O HRE", "Príkaz zobrazí krátky text, ktorý poslúži ako úvod do príbehu.", "^(ABOUT|O HRE)$", 1);
    add_command_to_parser(p, "VEZMI", "Vloží predmet z miestnosti do batohu.", "^(TAKE|VEZMI)$", 1);
    add_command_to_parser(p, "POLOZ", "Položí predmet z batohu do miestnosti.", "^(DROP|POLOZ)$", 1);
    add_command_to_parser(p, "INVENTAR", "Zobrazí obsah hráčovho batohu.", "^(INVENTORY|I|INVENTAR)$", 2);
    add_command_to_parser(p, "POUZI", "Použije predmet z batohu alebo miestnosti.", "^(USE|POUZI)$", 1);
    add_command_to_parser(p, "PRESKUMAJ", "Vypíše opis predmetu, ktorý sa musí nachádzať v miestnosti alebo batohu.", "^(EXAMINE|PRESKUMAJ)$", 1);
    add_command_to_parser(p, "NAHRAJ", "Príkaz zabezpečí nahratie uloženej pozície hry z disku.", "^(LOAD|NAHRAJ)$", 1);
    add_command_to_parser(p, "ULOZ", "Príkaz uloží stav rozohratej hry na disk.", "^(SAVE|ULOZ)$", 1);

    return p;
}
void destro_command(struct command* cmd) {
    if (cmd) {
        free(cmd->name);
        free(cmd->description);
        regfree(&cmd->preg); // Free the compiled regular expression
        free(cmd);
    }
}


void destroy_container_list(struct container* list) {
    struct container* current = list;
    struct container* next;
    while (current != NULL) {
        next = current->next;
        destro_command(current->command);
        free(current); 
        current = next;
    }
}

struct parser* destroy_parser(struct parser* parser) {
    if (parser) {
        destroy_container_list(parser->commands); 
        destroy_container_list(parser->history);
        free(parser);
    }
    return NULL;
}

char* normalize_input(char* input) {
    char* output = malloc(strlen(input) + 1); 
    if (!output) return NULL; 
    char* dest = output; 
    int is_space = 0;  

    while (*input && isspace((unsigned char)*input)) input++;

    // Process the string
    while (*input) {
        if (isspace((unsigned char)*input)) {
            if (!is_space) {
                *dest++ = ' ';  // Add one space to the output
                is_space = 1;  // Set flag indicating last char was space
            }
        } else {
            *dest++ = *input;  
            is_space = 0;  
        }
        input++;
    }

    // Remove trailing space if present
    if (is_space && dest > output) dest--;

    *dest = '\0'; 
    return output;
}

struct command* parse_input(struct parser* parser, char* input) {
    if (!parser || !input) return NULL;

    char* normalized_input = normalize_input(input);
    if (!normalized_input) return NULL; 

    struct container* current = parser->commands;
    while (current != NULL) {
        if (regexec(&current->command->preg, normalized_input, 0, NULL, 0) == 0) {
            return current->command; 
        }
        current = current->next;
    }
    return NULL;
}
