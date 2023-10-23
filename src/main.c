#include <stdio.h>

#include "Lexer/Lexer.h"

#include <string.h>

// NOTE : le but de ce code est de pouvoir tester le lexer facilement.
//        l'interpréteur final aura surement son propre main, et ce fichier sera déplacé autre part.
int main() {
    while(true) {
        char statement[512] = { 0 };
        putc('>', stdout);
        fgets(statement, 512, stdin);

        if(strcmp(statement, "exit\n") == 0) break;

        //pour tester le fonctionnement sans le \n à la fin
        //strcpy(statement, "+ - <-");

        LexingState ls = {
                statement,
                strlen(statement),
                0
        };

        while (true) {
            Token *t = get_single_token(&ls);
            if (t == NULL) break;   //ici il y a techniquement la possibilité de détecter une erreur de lexing avec t->type
            print_token(t);
            putc(' ', stdout);
            destroy_token(t);       //les token sont libéré ici car on ne fait rien avec. Dans l'interpréteur final il faudra sûrement les stocker dans un tableau, puis les envoyer au parser
        }
        putc('\n', stdout);
    }

    return 0;
}
