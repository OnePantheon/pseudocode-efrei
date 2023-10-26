#include <stdio.h>

#include "Lexer/Lexer.h"

#include <malloc.h>
#include <string.h>

int main_repl_lexer();
int main_interpreter_lexer(int argc, const char *argv[]);


int main(int argc, const char *argv[]) {
    if(argc == 1) return main_repl_lexer();
    else return main_interpreter_lexer(argc, argv);
}


/**
 * Point d'entrée pour le repl (affiche uniquement la sortie du lexer)
 * @return code de retour
 */
int main_repl_lexer() {
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

        //TODO : mettre tous les token dans un tableau
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


/**
 * Point d'entrée pour l'interpréteur (affiche uniquement la sortie du lexer)
 * @param argc même que main
 * @param argv même que main
 * @return code de retour
 */
//TODO : mettre tous les token dans un tableau
int main_interpreter_lexer(int argc, const char *argv[]) {
    FILE *input_file = fopen(argv[1], "r");
    if(input_file == NULL) {
        printf("Err : le fichier %s n'existe pas :(\n", argv[1]);
        return 1;
    }


    while(true) {
        size_t begin_pos = ftell(input_file);

        //obtenir la taille de la ligne
        int c = ' ';    //int car fgetc retourne un int
        while(c != EOF && c != '\n') c = fgetc(input_file);
        size_t end_pos = ftell(input_file);
        size_t size_of_line = end_pos+1 - begin_pos;  // +1 car fgets va ajouter un \0 à la fin du string

        //copier la ligne en mémoire
        fseek(input_file, begin_pos, SEEK_SET); //retourner au début de la ligne
        char* buffer = malloc(sizeof(char) * size_of_line);     //allouer le buffer
        fgets(buffer, size_of_line, input_file);    //après cette ligne, le curseur devrait avoir ateint la fin de la ligne du fichier


        LexingState ls = {
                buffer,
                strlen(buffer),
                0
        };
        while (true) {
            Token *t = get_single_token(&ls);
            if (t == NULL) break;   //ici il y a techniquement la possibilité de détecter une erreur de lexing avec t->type
            print_token(t);
            putc(' ', stdout);
            destroy_token(t);       //les token sont libéré ici car on ne fait rien avec. Dans l'interpréteur final il faudra sûrement les stocker dans un tableau, puis les envoyer au parser
        }
        printf("\n");

        if(c == EOF) break;
    }

    fclose(input_file);
}