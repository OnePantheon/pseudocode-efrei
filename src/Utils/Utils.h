#ifndef EFREIPSEUDOCODE_UTILS_H
#define EFREIPSEUDOCODE_UTILS_H

#include <stdbool.h>
#include <stddef.h>

/**
 * Permet de savoir si un charactère est une lettre
 * @param c le charactère
 * @return true si oui, sinon false
 */
bool is_letter(char c);


/**
 * Permet de svoir si un charactère est un chiffre
 * @param c le charactère
 * @return true si oui, sinon false
 */
bool is_digit(char c);


/**
 *
 * @param c
 * @return
 */
char special_char_to_ascii(char c);


/**
 * Permet de copier une partie d'un string dans un buffer
 * @param dest buffer de destination
 * @param destsize taille max de la destination
 * @param src source de la copie
 * @param cpystart index du charactère du début de la copie (inclus)
 * @param cpyend index du charactère de la fin de la copie (exclus
 */
void substrcpy(char *dest, size_t destsize, const char *src, size_t cpystart, size_t cpyend);


/**
 * permet de mettre un string en lowercase
 * @param str le string que l'on souhaite mettre en lowercase (il est directement modifié)
 */
void to_lowercase(char *str);


/**
 * trouver l'index d'un string est dans un tableau de string (retourne -1 si le string n'est pas dans la liste)
 * @param str le string
 * @param arr tableau de string
 * @param arr_size nombre de strings dans le tableau
 * @return position dans le tableau (-1 si pas présent)
 */
int find_str_in_list(const char *str, const char *arr[], size_t arr_size);

/**
 * Pareil que ci-dessus, mais avec un charactère et un tableau de charactère
 * @param c le charactère
 * @param arr le tableau de charactère
 * @param arr_size nombre de charactères dans le tableau
 * @return position dans le tableau (-1 si pas présent)
 */
int find_char_in_list(char c, const char arr[], size_t arr_size);

#endif //EFREIPSEUDOCODE_UTILS_H
