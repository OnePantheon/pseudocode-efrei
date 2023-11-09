#include "Lexer.h"

#include "../Utils/Utils.h"
#include "Dictionary.h"

#include <math.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>


//forward declarations
static void ignore_whitespace(LexingState *ls);
static Token *get_keyword(LexingState *ls);
static Token *get_operator_keyword(LexingState *ls);
static Token *get_vartype(LexingState *ls);
static Token *get_symbol(LexingState *ls);
static Token *get_character(LexingState *ls);
static Token *get_string(LexingState *ls);
static Token *get_number(LexingState *ls);
static Token *get_operator(LexingState *ls);


//////////////////////////////////////////////////////
//// Fonction principale pour le lexing

//TODO : trouver un meilleur moyen de coder ça, car actuellement c'est horrible
Token *get_single_token(LexingState *ls) {
    if(ls->index >= ls->str_len) ls->done = true;
    if(ls->done) return NULL;

    ignore_whitespace(ls);
    char current_char = ls->string[ls->index];

    Token *r = NULL;

    if(is_letter(current_char)) {
        r = get_keyword(ls);
        if(r == NULL) r = get_operator_keyword(ls);
        if(r == NULL) r = get_vartype(ls);
        if(r == NULL) r = get_symbol(ls);
    }
    else if(current_char >= '0' && current_char <= '9') {
        r = get_number(ls);
    }
    else if(current_char == '\'') {
        r = get_character(ls);
    }
    else if(current_char == '"') {
        r = get_string(ls);
    }
    else if(current_char == '\n') {
        union TokenValue val;
        val.as_char = current_char;
        r = create_token(TokenType_EndOfLine, val);
        ls->index++;
    }
    else if(current_char == ',') {
        union TokenValue val;
        val.as_char = current_char;
        r = create_token(TokenType_Comma, val);
        ls->index++;
    }
    else if(current_char == ':') {
        union TokenValue val;
        val.as_char = current_char;
        r = create_token(TokenType_Colon, val);
        ls->index++;
    }
    else if(current_char == '(' || current_char == ')') {
        union TokenValue val;
        val.as_char = current_char;
        r = create_token(TokenType_Parentheses, val);
        ls->index++;
    }
    else if(current_char == '[' || current_char == ']') {
        union TokenValue val;
        val.as_char = current_char;
        r = create_token(TokenType_Brackets, val);
        ls->index++;
    }
    else if(current_char == '{' || current_char == '}') {
        union TokenValue val;
        val.as_char = current_char;
        r = create_token(TokenType_Braces, val);
        ls->index++;
    }
    else {
        r = get_operator(ls);

        if(r == NULL) {
            union TokenValue val;
            val.as_char = current_char;
            r = create_token(TokenType_Unknown, val);
        }
    }

    if(r == NULL || r->type == TokenType_Error || r->type == TokenType_Unknown) ls->done = true;
    return r;
}


//////////////////////////////////////////////////////
//// Fonctions "utilitaires" pour le lexing (pas utilisable depuis l'exterieur de ce fichier)

/**
 * ignorer tous les espaces et aller au prochain "vrai" charactère
 * @param ls
 */
static void ignore_whitespace(LexingState *ls) {
    const char *str = ls->string;
    //TODO : ignorer les commentaires ici ?
    while(str[ls->index] == ' ' || str[ls->index] == 9) ls->index++;       //espace ou tab
}


/**
 * Obtenir le mot clé sur lequel est le curseur (si le curseur n'est pas sur un mot clé renourne NULL)
 * @param ls
 * @return Un Token avec le keyword dedans, ou NULL
 */
static Token *get_keyword(LexingState *ls) {
    size_t start_index = ls->index;     //inclus
    size_t end_index = ls->index;       //exclus

    //trouver la fin du mot
    const char *str = ls->string;
    size_t str_len = strlen(str);
    //while(end_index < str_len && is_letter(str[end_index])) end_index++;
    while(end_index < str_len && (is_letter(str[end_index]) || is_digit(str[end_index]) || str[end_index] == '_'))
        end_index++;

    //aucun mot clé est supérieur à 12 charactères (15 au cas où on adapte pour le français)
    if(end_index-start_index > 15) return NULL;

    char buffer[16];
    substrcpy(buffer, 16, str, start_index, end_index);
    to_lowercase(buffer);

    enum PseudocodeKeyword keyword = find_str_in_list(buffer, c_keywords_strings_en, c_keywords_strings_en_size);
    if(keyword == -1) return NULL;

    ls->index = end_index;  //Avancer dans le lexing

    union TokenValue val;
    val.as_keyword = keyword;
    return create_token(TokenType_Keyword, val);
}


/**
 * Obtenir un opérateur écrit en lettre ("or", "and", not")
 * @param ls
 * @return L'opérateur dans un token, ou NULL
 */
static Token *get_operator_keyword(LexingState *ls) {
    size_t start_index = ls->index;     //inclus
    size_t end_index = ls->index;       //exclus

    //trouver la fin du mot
    const char *str = ls->string;
    size_t str_len = strlen(str);
    while(end_index < str_len && (is_letter(str[end_index]) || is_digit(str[end_index]) || str[end_index] == '_'))
        end_index++;

    //aucun opérateur est supérieur à 3 charactères (5 pour être sûr qu'il y a rien après)
    if(end_index-start_index > 5) return NULL;

    char buffer[6];
    substrcpy(buffer, 6, str, start_index, end_index);
    to_lowercase(buffer);

    enum PseudocodeOperator operator = find_str_in_list(buffer, c_operators_strings_en, c_operators_strings_en_size);
    if(operator == -1) return NULL;

    ls->index = end_index;  //Avancer dans le lexing

    union TokenValue val;
    val.as_operator = operator;
    return create_token(TokenType_Operator, val);
}


/**
 * Obtenir le type de variable sur lequel le curseur est
 * @param ls
 * @return
 */
static Token *get_vartype(LexingState *ls) {
    size_t start_index = ls->index;     //inclus
    size_t end_index = ls->index;       //exclus

    //trouver la fin du mot
    const char *str = ls->string;
    size_t str_len = strlen(str);
    while(end_index < str_len && (is_letter(str[end_index]) || is_digit(str[end_index]) || str[end_index] == '_'))
        end_index++;

    //aucun nom de type de variable clé est supérieur à 12 charactères (15 au cas où on adapte pour le français)
    if(end_index-start_index > 15) return NULL;

    char buffer[16];
    substrcpy(buffer, 16, str, start_index, end_index);
    to_lowercase(buffer);

    enum PseudocodeVariableType keyword = find_str_in_list(buffer, c_vartypes_strings_en, c_vartypes_strings_en_size);
    if(keyword == -1) return NULL;

    ls->index = end_index;  //Avancer dans le lexing

    union TokenValue val;
    val.as_variable_type = keyword;
    return create_token(TokenType_VariableType, val);
}

/**
 * Obtenir le symbole (nom de fonction, variable) sur lequel le curseur est
 * @param ls
 * @return Un Token avec le symbole dedans
 */
static Token *get_symbol(LexingState *ls) {
    size_t start_index = ls->index;     //inclus
    size_t end_index = ls->index;       //exclus

    //trouver la fin du symbole
    const char *str = ls->string;
    size_t str_len = strlen(str);
    while(end_index < str_len && (is_letter(str[end_index]) || is_digit(str[end_index]) || str[end_index] == '_'))
        end_index++;

    //taille max de noms de variables
    if(end_index-start_index > 255) return NULL;

    char buffer[256];
    substrcpy(buffer, 256, str, start_index, end_index);
    size_t buffer_len = strlen(buffer)+1;   //+1 pour inclure NULL

    //allouer le string dans le heap
    char* var_name = malloc(sizeof(char) * buffer_len);
    substrcpy(var_name, buffer_len, buffer, 0, buffer_len-1);

    ls->index = end_index;  //Avancer dans le lexing

    //créer le token
    union TokenValue val;
    val.as_string = var_name;
    Token *r = create_token(TokenType_Symbol, val);
    r->str_on_heap = true;
    return r;
}


/**
 * obtenir le charactère 'c' sur le curseur (le curseur doit être sur le premier ')
 * @param ls
 * @return Un token avec le charactère dedans
 */
static Token *get_character(LexingState *ls) {
    const char* str = ls->string;
    size_t index = ls->index;

    if(index + 2 >= ls->str_len) {
        return create_token_error(0);
    }

    //charactère spécial
    if(str[index+1] == '\\') {
        if (index + 3 >= ls->str_len || str[index+3] != '\'') return create_token_error(0);
        ls->index += 4;     //avancer dans le lexing
        return create_token_char(special_char_to_ascii(str[index+2]));
    }
    else {  //charactère normal
        if(str[index+2] != '\'') return create_token_error(0);
        ls->index += 3;     //avancer dans le lexing
        return create_token_char(str[index+1]);
    }
}


/**
 * obtenir le string "string" sur le curseur (le curseur doit être sur le premier ")
 * @param ls
 * @return
 */
static Token *get_string(LexingState *ls) {
    size_t start_index = ls->index+1;   //inclus (+1 pour ne pas prendre le premier '"')
    size_t end_index = ls->index+1;       //exclus

    //trouver la fin du string
    const char *str = ls->string;
    size_t str_len = ls->str_len;
    while(end_index < str_len && str[end_index] != '\n' &&
            (str[end_index] != '"' || (str[end_index] == '"' && str[end_index-1] == '\\' && str[end_index-2] != '\\'))      //posez pas de question ça marche à peu prêt (dans l'idéal il faudrait checks s'il n'y a pas un nombre impaire de '\' avant le '"')
    ) end_index++;

    //vérifier si on est sur des guillemets doubles (fin du string)
    if(end_index >= str_len || str[end_index] != '"') {
        union TokenValue val;
        val.as_int = 0;
        return create_token(TokenType_Error, val);
    }

    //allouer le string dans le heap
    size_t max_str_len = end_index - start_index + 1;   //taille max du string (sans compté les charactères précédé de \ .
    char *str_val = malloc(sizeof(char)*max_str_len);

    size_t val_i = 0;   //progression dans str_val
    size_t i;           //progression dans str
    for(i = start_index; i < end_index; i++) {
        if(str[i] == '\\') {
            str_val[val_i] = special_char_to_ascii(str[i+1]);
            i++;    //avancer d'un rank en plus
        }
        else {
            str_val[val_i] = str[i];
        }

        val_i++;
    }

    str_val[val_i] = 0; //fin de string

    ls->index = end_index+1;    //avancer dans le lexing

    //créer le token
    union TokenValue val;
    val.as_string = str_val;
    Token *r = create_token(TokenType_String, val);
    r->str_on_heap = true;
    return r;
}


/**
 * Obtenir le nombre (float ou int) sur lequel le curseur est actuellement
 * @param ls
 * @return le int ou float dans un token
 */
static Token *get_number(LexingState *ls) {
    //tous ces charactères sont concidérés comme "fin de nombre"
    const char end_of_number[] = {' ', ',', ')', ']', '}', '+', '-', '/', '*', '=', '>', '<', '\n'};

    size_t start_index = ls->index;     //inclus
    size_t end_index = ls->index;       //exclus

    const char *str = ls->string;
    size_t str_len = ls->str_len;

    bool is_float = false;
    bool in_number = true;
    while(in_number) {
        char current_char = str[end_index];

        if(current_char == '.') {
            if(is_float) return create_token_error(0);  //il ne peut pas y avoir deux '.' dans un nombre
            else is_float = true;
        }

        if((current_char >= '0' && current_char <= '9') || current_char == '.') end_index++;    //chiffre ou point
        else if(end_index >= str_len || find_char_in_list(current_char, end_of_number, sizeof(end_of_number)) != -1) in_number = false;    //end of number
        else return create_token_error(0);
    }

    size_t number_len = end_index-start_index+1;
    char* number_str = malloc(sizeof(char) * number_len);
    substrcpy(number_str, number_len, str, start_index, end_index);

    ls->index = end_index;      //avancer dans le lexing

    Token *r = NULL;
    if(is_float) r = create_token_float(atof(number_str));
    else r =  create_token_int(atoi(number_str));

    free(number_str);
    return r;
}


/**
 * Obtenir l'opérateur (+, -, >=, <-, etc) sur lequel est le curseur
 * @param ls
 * @return Le token avec l'opérateur dedans, ou NULL
 */
static Token *get_operator(LexingState *ls) {
    const char operator_end[] = {' ',  '\'', '"', '\n', ','};
    size_t start_index = ls->index;     //inclus
    size_t end_index = ls->index;       //exclus

    //trouver la fin de l'opérateur
    const char *str = ls->string;
    size_t str_len = strlen(str);
    while(end_index < str_len && !is_letter(str[end_index]) && !is_digit(str[end_index])
    && find_char_in_list(str[end_index], operator_end, sizeof(operator_end))==-1)
        end_index++;

    //aucun opérateur est supérieur à 3 charactères (5 pour être sûr qu'il y a rien après)
    if(end_index-start_index > 5) return NULL;

    char buffer[6];
    substrcpy(buffer, 6, str, start_index, end_index);
    to_lowercase(buffer);

    enum PseudocodeOperator operator = find_str_in_list(buffer, c_operators_strings_en, c_operators_strings_en_size);
    if(operator == -1) return NULL;

    ls->index = end_index;  //Avancer dans le lexing

    union TokenValue val;
    val.as_operator = operator;
    return create_token(TokenType_Operator, val);
}
