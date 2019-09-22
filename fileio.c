#include <stdio.h>
#include <string.h>
#include "prototypes.h"
#include "structs.h"
#include "enums.h"

void saveGame(const Piece humanPieces[16], const Piece computerPieces[16]){
	FILE *txtOut;
	char fileName[50];
	int success = FALSE;
	int i;

	do {
		printf("\tSave name: ");
		scanf("%s", fileName);
		if (!strstr(fileName, ".txt"))
			strcat(fileName, ".txt");

		if ((txtOut = fopen(fileName, "w")) == NULL){
			puts("\tError creating file");
			continue;
		}

		fprintf(txtOut, "HUMAN\n");
		for (i = 0; i < 16; i++){
			fprintf(txtOut, "%c [%d,%d] %d\n", humanPieces[i].type, humanPieces[i].location[X], humanPieces[i].location[Y], humanPieces[i].moveCount);
		}
		fprintf(txtOut, "\nCOMPUTER\n");
		for (i = 0; i < 16; i++){
			fprintf(txtOut, "%c [%d,%d] %d\n", computerPieces[i].type, computerPieces[i].location[X], computerPieces[i].location[Y], computerPieces[i].moveCount);
		}

		success = TRUE;

	} while (success == FALSE);

	fclose(txtOut);
}

void loadGame(Piece humanPieces[16], Piece computerPieces[16], Square board[8][8]){
	FILE *txtIn;
	char fileName[50];
	char line[20];
	int owner;
	int i;

	printf("\tFile name: ");
	scanf("%s", fileName);
	if (!strstr(fileName, ".txt"))
		strcat(fileName, ".txt");

	if ((txtIn = fopen(fileName, "r")) == NULL){
		puts("Error opening file");
	}

	fgets(line, 20, txtIn);
	for (i = 0; i < 16; i++){
		fgets(line, 20, txtIn);
		sscanf(line, "%c [%d,%d] %d\n", &humanPieces[i].type, &humanPieces[i].location[X], &humanPieces[i].location[Y], &humanPieces[i].moveCount);
		if (humanPieces[i].type == ' ')
			humanPieces[i].owner = NONE;
		else
			humanPieces[i].owner = HUMAN;
	}
	fgets(line, 20, txtIn);
	fgets(line, 20, txtIn);
	for (i = 0; i < 16; i++){
		fgets(line, 20, txtIn);
		sscanf(line, "%c [%d,%d] %d\n", &computerPieces[i].type, &computerPieces[i].location[X], &computerPieces[i].location[Y], &computerPieces[i].moveCount);
		if (computerPieces[i].type == ' ')
			computerPieces[i].owner = NONE;
		else
			computerPieces[i].owner = COMPUTER;

	}
	fclose(txtIn);

	// initialize the board
	createBoard(board);

	for (i = 0; i < 16; i++){
		board[humanPieces[i].location[X]][humanPieces[i].location[Y]].owner = humanPieces[i].owner;
		board[humanPieces[i].location[X]][humanPieces[i].location[Y]].type = humanPieces[i].type;
		board[computerPieces[i].location[X]][computerPieces[i].location[Y]].owner = computerPieces[i].owner;
		board[computerPieces[i].location[X]][computerPieces[i].location[Y]].type = computerPieces[i].type;
	}
}
