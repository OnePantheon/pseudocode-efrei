#ifndef EFREIPSEUDOCODE_LEXER_H
#define EFREIPSEUDOCODE_LEXER_H

#include "Token.h"

#include <stdbool.h>
#include <stdio.h>

struct LexingStateStruct;


typedef struct LexingStateStruct {
    const char *string;     //se string qu'on est en train de lire
    size_t str_len;
    size_t index;           //endroit où on est actuellement dans le string
    bool done;              //true si on est arrivé à la fin du fichier
} LexingState;


Token *get_single_token(LexingState *ls);


#endif //EFREIPSEUDOCODE_LEXER_H
