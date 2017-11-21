#ifndef myStringLib_H_
#define myStringLib_H_

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
Syfte: Delar upp en sträng i mindre strängar separerad av en "token"
Parametrar:	*string,	strängen som ska delas upp
			token,		tecknet som ska separera strängarna
Returvärden:	Ett strängfält som innehåller de strängarna som har
				separerats.
Beskrivning: Funktionen loopar först igenom den inskickade strängen
och räknar antalet tokens. Minne för ett strängfält allokeras och
strängen loopas igenom igen för att finna positionerna för sina
"tokens". De separerade strängarna lagras sedan i strängfältet
med hjälp av strcpy.
Kommentarer: Strängfältet termineras av en NULL-pekare.
*/
char** stringTokenizer(char* string, char token);

/*
Syfte: Ger längden på ett null-pointer terminerat strängfält.
Parametrar:	**stringVector,	strängfältet vars längd ska räknas ut
Returvärden: Strängfältets längd
*/
void stringVectorFreeFunc(char** stringVector);

/*
Syfte: Frigöra minnet i ett strängfält.
Parametrar:	**stringVector,	strängfältet som ska frigöras
*/
int stringVectorLength(char** stringVector);

/*
Syfte: Beräknar längden på en sträng
Parametrar:	*string,	strängen vars längd ska beräknas
Returvärden: Strängens längd.
*/
int stringLength(char* string);

/*
Syfte: Gör om en sträng till ett nummer
Parametrar: *string,	strängen vars innehåll är en siffra.
Kommentarer: Funktionen undersöker inte om strängen faktiskt är
en siffra, använd strIsNum för att avgöra detta innan str2num
används.
*/
int str2num(char* string);

/*
Syfte: Avgör om en sträng innehåller en siffra
Parametrar:	*string,	strängen som ska undersökas
Returvärden:	true, om strängen är en siffra
				false, om strängen inte är en siffra
*/
bool strIsNum(char* string);

#endif