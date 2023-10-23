#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

struct TokenStruct;


enum TokenType {
    TokenType_Unknown = 0,
    TokenType_Symbol,       //noms de variables/fonctions/etc définis par l'utilisateur
    TokenType_Keyword,      //if, else, begin, etc
    TokenType_VariableType, //Integer, Float, String, etc
    TokenType_Bool,         //True, False
    TokenType_Int,          // 42, 69, 10, etc
    TokenType_Float,        // 33.1, 2.7, etc
    TokenType_Character,    //'s', 'c' etc
    TokenType_String,       //"hello", "oui", etc
    TokenType_Operator,     //+, -, <-, >=, etc
    TokenType_Parentheses,  //(, )
    TokenType_Brackets,     //[, ]
    TokenType_Braces,       //{, }  (dans les définitions de tableaux)
    TokenType_Colon,        //:    (ex : "my_var : Integer")
    TokenType_Comma,        //,
    TokenType_EndOfLine,    //à la fin de chaque ligne
    TokenType_EndOfFile,    //à la fin du fichier (quand fgetc retourne -1)
    TokenType_Error,        //utilisé pour indiquer une erreur lors du lexing
};


//Liste de tous les mots clés possibles en pseudocode
enum PseudocodeKeyword {
    Keyword_Algorithm = 0,
    Keyword_Function,
    Keyword_Constants,
    Keyword_Variables,
    Keyword_Of,         // ex : array of int
    Keyword_Begin,
    Keyword_End,
    Keyword_If,
    Keyword_Then,       // ex : If <condition> Then
    Keyword_Else,
    Keyword_While,
    Keyword_For,
    Keyword_To,         // ex : For i <- 1 to 5
};


//Liste de tous les types de variables possibles en pseudocode
enum PseudocodeVariableType {
    VariableType_Bool,
    VariableType_Int,
    VariableType_Float,
    VariableType_Character,
    VariableType_String,
    VariableType_Array
};


//Liste de tous les opérateurs du pseudocode
enum PseudocodeOperator {
    Operator_Assignment,        // <-
    Operator_Plus,              // +
    Operator_Minus,             // -
    Operator_Multiplication,    // *
    Operator_Division,          // /
    Operator_Equal,             // =  (utilisé pour les comparaisons et les constantes)
    Operator_Inferior,          // <
    Operator_InferiorEqual,     // <=
    Operator_Superior,          // >
    Operator_SuperiorEqual,     // >=
    Operator_RightByteShift,    // >>
    Operator_LeftByteShift,     // <<
    Operator_Not,               // not
    Operator_And,               // and
    Operator_Or,                // or
};


union TokenValue {
    enum PseudocodeKeyword as_keyword;
    enum PseudocodeVariableType as_variable_type;
    bool as_bool;
    int as_int;
    float as_float;
    char as_char;       //utilisé pour TokenType_Character, TokenType_Parentheses, TokenType_Brackets et TokenType_Braces
    char *as_string;    //utilisé pour TokenType_String et TokenType_Symbol
    enum PseudocodeOperator as_operator;
};


typedef struct TokenStruct {
    enum TokenType type;
    union TokenValue value;
    bool str_on_heap;       //utilisé par destroy_token
} Token;

void print_token(Token *t);
//Allouer un token sur le heap et renvoyer son pointeur
Token *create_token(enum TokenType type, union TokenValue value);
//supprimer un toker
void destroy_token(Token *t);

//Pour créer des token plus facilement
Token *create_token_int(int value);
Token *create_token_float(float value);
Token *create_token_char(char value);
Token *create_token_error(int errorcode);

#endif