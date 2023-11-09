#include "Token.h"

#include "Dictionary.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

void print_token(Token *t) {
    putc('{', stdout);
    switch(t->type) {

        case TokenType_Unknown:
            printf("Unknown");
            break;
        case TokenType_Symbol:
            printf("Symbol : %s ", t->value.as_string);
            break;
        case TokenType_Keyword:
            printf("Keyword : %s ", c_keywords_strings_en[t->value.as_keyword]);
            break;
        case TokenType_VariableType:
            printf("VarTypeName : %s ", c_vartypes_strings_en[t->value.as_variable_type]);
            break;
        case TokenType_Bool:
            printf("Boolean : %s ", (t->value.as_bool) ? "True" : "False");
            break;
        case TokenType_Int:
            printf("Integer : %i ", t->value.as_int);
            break;
        case TokenType_Float:
            printf("Float : %f ", t->value.as_float);
            break;
        case TokenType_Character:
            printf("Character : '%c' ", t->value.as_char);
            break;
        case TokenType_String:
            printf("String : \"%s\" ", t->value.as_string);
            break;
        case TokenType_Operator:
            printf("Operator : %s ", c_operators_strings_en[t->value.as_operator]);
            break;
        case TokenType_Parentheses:
            printf("Parentheses : %c ", t->value.as_char);
            break;
        case TokenType_Brackets:
            printf("Brackets : %c ", t->value.as_char);
            break;
        case TokenType_Braces:
            printf("Braces : %c ", t->value.as_char);
            break;
        case TokenType_Colon:
            printf("Colon");
            break;
        case TokenType_Comma:
            printf("Comma");
            break;
        case TokenType_EndOfLine:
            printf("EndOfLine");
            break;
        case TokenType_EndOfFile:
            printf("EndOfFile");
            break;
        case TokenType_Error:
            printf("Error");
            break;
    }
    putc('}', stdout);
}


Token *create_token(enum TokenType type, union TokenValue value) {
    Token *r = (Token*) malloc(sizeof(Token));
    r->type = type;
    r->value = value;
    r->str_on_heap = false;
    return r;
}


Token *copy_token(Token *token) {
    Token *r = (Token*) malloc(sizeof(Token));
    r->type = token->type;
    r->str_on_heap = token->str_on_heap;

    if(r->type == TokenType_String) {
        r->str_on_heap = true;
        r->value.as_string = malloc(sizeof(char) * (strlen(token->value.as_string)+1)); //+1 pour null
        strcpy(r->value.as_string, token->value.as_string);
    }
    else {
        r->value = token->value;
    }

    return r;
}


void destroy_token(Token *t) {
    if(t == NULL) return;
    if(t->str_on_heap && t->value.as_string != NULL) free(t->value.as_string);
    free(t);
}


Token *create_token_int(int value) {
    union TokenValue v;
    v.as_int = value;
    return create_token(TokenType_Int, v);
}

Token *create_token_float(float value) {
    union TokenValue v;
    v.as_float = value;
    return create_token(TokenType_Float, v);
}

Token *create_token_char(char value) {
    union TokenValue v;
    v.as_char = value;
    return create_token(TokenType_Character, v);
}

Token *create_token_error(int errorcode) {
    union TokenValue val;
    val.as_int = errorcode;
    return create_token(TokenType_Error, val);
}
