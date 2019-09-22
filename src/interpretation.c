#include <ctype.h>
#include <string.h>
#include "enums.h"

//converts the error code to an intelligible string
void getError(int error, char errorMessage[32]){
	switch (error) {
		case NOTPIECE:
			strcpy(errorMessage, "You do not have a piece there.");
			break;
		case NOTYOURS:
			strcpy(errorMessage, "That is not your piece.");
			break;
		case SAMESQUARE:
			strcpy(errorMessage, "You must move the piece somewhere.");
			break;
		case OUTOFBOUNDS:
			strcpy(errorMessage, "That is off the board.");
			break;
		case ONPIECE:
			strcpy(errorMessage, "You cannot move onto your own pieces.");
			break;
		case ILLEGAL:
			strcpy(errorMessage, "That is not a legal move.");
			break;
		case KINGINTOCHECK:
			strcpy(errorMessage, "You cannot move your king into check.");
			break;
		case KINGINCHECK:
			strcpy(errorMessage, "Your king is in check.");
			break;
		case BLOCKED:
			strcpy(errorMessage, "That move is blocked.");
			break;
	}
}
//converts the chessboard letters to integers
int AtoI(char loc){
	int locInt;

	loc = tolower(loc);

	switch (loc){
		case 'a':
			locInt = A;
			break;
		case 'b':
			locInt = B;
			break;
		case 'c':
			locInt = C;
			break;
		case 'd':
			locInt = D;
			break;
		case 'e':
			locInt = E;
			break;
		case 'f':
			locInt = F;
			break;
		case 'g':
			locInt = G;
			break;
		case 'h':
			locInt = H;
			break;
		default:
			locInt = -1;
	}

	return locInt;
}
