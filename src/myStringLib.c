#include "myStringLib.h"

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
char** stringTokenizer(char* string, char token) {
	int* positions;
	int count = 0;
	int pos = 0;

	while((string[pos] != '\n') && (string[pos] != '\0')){

		if(string[pos] == token)
			count++;

		pos++;
	}

	count++;
	positions = calloc(count + 1,sizeof(int));
	positions[0] = -1;
	int i = 1;
	pos = 0;

	while((string[pos] != '\n') && (string[pos] != '\0')){

		if(string[pos] == token) {
			positions[i] = pos;
			i++;
		}

		pos++;
	}

	positions[i] = pos;
	char** stringVector = calloc(count+1, sizeof(char*));

	for (i = 0; i < count; i++) {
		int length = positions[i+1] - positions[i] - 1;
		if (length > 0) {
			stringVector[i] = calloc(length + 1, sizeof(char));
			memcpy(stringVector[i], string + positions[i] + 1, length);
		} else {
			stringVector[i] = calloc(1, sizeof(char));
			char* nullchar = stringVector[i];
			*nullchar = '\0';
		}
	}

	stringVector[i] = NULL;
	free(positions);
	
	return stringVector;
}

/*
Syfte: Ger längden på ett null-pointer terminerat strängfält.
Parametrar:	**stringVector,	strängfältet vars längd ska räknas ut
Returvärden: Strängfältets längd
*/
int stringVectorLength(char** stringVector) {
	int length;

	for(length = 0; stringVector[length] != NULL; length++);

	return length;
}

/*
Syfte: Frigöra minnet i ett strängfält.
Parametrar:	**stringVector,	strängfältet som ska frigöras
*/
void stringVectorFreeFunc(char** stringVector) {
	int length = stringVectorLength(stringVector);

	for(int i = 0; i < length; i++) {
		free(stringVector[i]);
	}
	free(stringVector);
}

/*
Syfte: Beräknar längden på en sträng
Parametrar:	*string,	strängen vars längd ska beräknas
Returvärden: Strängens längd.
*/
int stringLength(char* string) {
	int length = 0;

	while(string[length] != '\0')
		length++;

	return length;
}

/*
Syfte: Gör om en sträng till ett nummer
Parametrar: *string,	strängen vars innehåll är en siffra.
Kommentarer: Funktionen undersöker inte om strängen faktiskt är
en siffra, använd strIsNum för att avgöra detta innan str2num
används.
*/
int str2num(char* string) {
	int length = stringLength(string);
	int number = 0;
	int pow10 = 1;

	for(int i = 0; i < length; i++) {
		number += pow10 * (string[length - 1 - i] - '0');
		pow10 *= 10;
	}

	return number;
}

/*
Syfte: Avgör om en sträng innehåller en siffra
Parametrar:	*string,	strängen som ska undersökas
Returvärden:	true, om strängen är en siffra
				false, om strängen inte är en siffra
*/
bool strIsNum(char* string) {
	int length = stringLength(string);

	for(int i = 0; i < length; i++) {
		if ((string[i] < '0') || (string[i] > '9'))
			return false;
	}

	return true;
}