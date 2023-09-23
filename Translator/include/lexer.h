#ifndef LIB_H
#define LIB_H 20230923L
#include <stdio.h>
/*all what starts with # are preprocessing derectives*/
extern int symbol;
extern int Pos;

int ReadSymbol();

int Digit();
int NonDigit();

int Identifier();
#endif /*LIB_H*/