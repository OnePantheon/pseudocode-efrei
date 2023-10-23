#include "Utils.h"

#include <stdio.h>
#include <string.h>

bool is_letter(char c) {
    return ( c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_digit(char c) {
    return (c >= '0' && c <= '9');
}

char special_char_to_ascii(char c) {
    switch (c) {
        case 'n':return '\n';
        default: return c;
    }
}


void substrcpy(char *dest, size_t destsize, const char *src, size_t cpystart, size_t cpyend) {
    size_t src_len = strlen(src);

    size_t i;
    //on vérifie si :
    // on a pas dépassé la taille de la destination-1 (-1 pour placer le 0)
    // on a pas atteind la fin de src
    // on a terminé de copier le str
    for(i = cpystart; i-cpystart < destsize-1 && i < src_len && i < cpyend; i++) {
        dest[i-cpystart] = src[i];
    }
    dest[i-cpystart] = 0;   //null terminator
}


void to_lowercase(char *str) {
    size_t str_len = strlen(str);
    for(size_t i = 0; i < str_len; i++) {
        if(str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
    }
}


int find_str_in_list(const char *str, const char *arr[], size_t arr_size) {
    for(int i = 0; i < arr_size; i++) {
        if(strcmp(str, arr[i]) == 0) return i;
    }
    return -1;
}

int find_char_in_list(char c, const char arr[], size_t arr_size) {
    for(int i = 0; i < arr_size; i++) {
        if(c == arr[i]) return i;
    }
    return -1;
}