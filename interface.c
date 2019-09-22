#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include "structs.h"
#include "enums.h"
#include "prototypes.h"


// prints the main menu & gets the user's choice
int getMenuChoice(void){
	int choice;
	char input;

	do {
		system("cls");
	
		// print the main menu
		printf("Choose one of the following:\n");
		printf("\t1. Start a new game\n");
		printf("\t2. Load a saved game\n");
		printf("\t3. Quit\n");
		printf("--> Your choice: ");

		// get the user's choice
		fflush(stdin);
		scanf("%d", &choice);
		switch (choice){
			case 1:
				choice = PLAY;
				break;
			case 2:
				choice = LOAD;
				break;
			case 3:
				choice = QUIT;
				break;
			case 4:	// demo option--not shown in main menu
				// does the human want to examine each move?
				printf("\tExamine each move (y/n)? ");
				fflush(stdin);
				scanf("%c", &input);
				if (input == 'y' || input == 'Y')
					choice = STEPDEMO;
				else
					choice = DEMO;
				break;
			default:
				choice = 999;
		}
	} while (choice == 999);

	return choice;
}

// prints the board to the console
void printBoard(const Square board[8][8]){
	char line[8];
	int x, y, i;

	// clear the screen
	system("cls");

	puts(" +--+--+--+--+--+--+--+--+");
	for (y = 0; y < 8; y++){
		// NOTE: indicate whose pieces are whose

		printf("%d|", 8 - y);
		for (x = 0; x < 8; x++){
			if (board[x][y].owner != ' '){
				if (board[x][y].owner == HUMAN)
					printf("%c |", board[x][y].type);
				else
					printf(" %c|", board[x][y].type);
			}
			else
				printf("  |");
		}
		puts("\n +--+--+--+--+--+--+--+--+");
	}
	puts("  a  b  c  d  e  f  g  h  ");
}

// gets the human player's next move
Move getHumanMove(const Piece humanPieces[16], const Piece computerPieces[16], const Square board[8][8]){
	Move theMove;
	int moveStatus;
	int choice;
	char inputA[100], inputB[100];
	char fromX, toX;
	int fromY, toY = -1;
	
	// MOVE INFORMATION
	do {
		choice = 

		// player input
		printf("\nPlayer's move: ");
		fflush(stdin);
		scanf("%s", inputA);
		// check if the player requested the game menu
		if (!strcmp(inputA, "menu")){
			choice = getGameMenuChoice();
			switch (choice){
				case PLAY:
					printBoard(board);
					break;
				case SAVE:
					saveGame(humanPieces, computerPieces);
					theMove.owner = 999;
					return theMove;
				case QUIT:
					theMove.owner = 999;
					return theMove;
			}
		}
		// check if move is in "a1b2" format
		else if (strlen(inputA) == 4){
			//assign coordinates
			if (isalpha(inputA[0]) && isdigit(inputA[1]) && isalpha(inputA[2]) && isdigit(inputA[3])){
				sscanf(inputA, "%c%d%c%d", &fromX, &fromY, &toX, &toY);
			}
		}
		// check if move is in "a1 b2" format
		else if (strlen(inputA) == 2){
			scanf("%s", inputB);
			if (strlen(inputB) == 2){
				//assign coordinates
				if (isalpha(inputA[0]) && isdigit(inputA[1]) && isalpha(inputB[0]) && isdigit(inputB[1])){
					sscanf(inputA, "%c%d", &fromX, &fromY);
					sscanf(inputB, "%c%d", &toX, &toY);
				}
			}
		}
		if (toY == -1)
			puts("Incorrect player input.");
	} while (toY == -1);

	// interpet/adjust player input
	theMove.moveFrom[Y] = 8 - fromY;
	theMove.moveTo[Y] = 8 - toY;
	theMove.moveFrom[X] = AtoI(fromX);
	theMove.moveTo[X] = AtoI(toX);
	// add in other move information
	theMove.owner = HUMAN;
	theMove.type = board[theMove.moveFrom[X]][theMove.moveFrom[Y]].type;
	
	// CHECK LEGITIMACY
	moveStatus = checkLegitimacy(theMove, board, humanPieces, computerPieces, TRUE);
	if (moveStatus != LEGITIMATE)
		theMove.owner = moveStatus;

	return theMove;
}

// makes the next move, updating player arrays and the board
void makeMove(const Move theMove, Square board[8][8], Piece player[16], Piece opponent[16], int pieceOwner){
	Move rookMove;
	int fromX = theMove.moveFrom[X],
		fromY = theMove.moveFrom[Y],
		toX = theMove.moveTo[X],
		toY = theMove.moveTo[Y];
	int oppX, oppY;
	int i, isCapture = FALSE;

	// CAPTURE CHECK
	// normal captures
	for (i = 0; i < 16; i++){
		if (toX == opponent[i].location[X] && toY == opponent[i].location[Y]){
			isCapture = TRUE;
			deletePiece(&opponent[i], board);
			break;
		}
		// delete ally pieces as well (ENDGAME ONLY)
		else if (toX == player[i].location[X] && toY == player[i].location[Y]){
			isCapture = TRUE;
			deletePiece(&player[i], board);
			break;
		}
	}
	// en passant captures
	if (isCapture == FALSE && theMove.type == 'P' && abs(fromX - toX) == 1){
		// find the pawn in the opponent's array to delete
		for (i = 0; i < 8; i++){
			oppX = opponent[i].location[X];
			oppY = opponent[i].location[Y];

			// if we've found the pawn in the opponent's array
			if (oppX == toX && oppY == fromY){
				deletePiece(&opponent[i], board);
				break;
			}
		}
	}

	// PIECE MOVE
	// find the piece to move in the piece array and move it
	for (i = 0; i < 16; i++){
		if (fromX == player[i].location[X] && fromY == player[i].location[Y]){
			updateLocation(theMove, &player[i], board);
			// check if a pawn has reached the end of the board, convert it into the player's choice
			if (player[i].type == 'P' && (player[i].location[Y] == 0 || player[i].location[Y] == 7))
				convertPawn(&player[i], board, pieceOwner);	
			break;
		}
	}

	// CASTLE CHECK
	// check if the king is castling
	if (theMove.type == 'K' && abs(fromX - toX) == 2){
		// castle short (to the right)
		if (fromX < toX){
			// player[9] is the rook on the right
			rookMove.owner = player[15].owner;
			rookMove.type = 'R';
			rookMove.moveFrom[X] = player[9].location[X];
			rookMove.moveFrom[Y] = player[9].location[Y];
			rookMove.moveTo[X] = player[9].location[X] - 2;
			rookMove.moveTo[Y] = player[9].location[Y];
			// make the rook move
			updateLocation(rookMove, &player[9], board);
		}
		// castle long (to the left)
		else {
			// player[8] is the rook on the left
			rookMove.owner = player[15].owner;
			rookMove.type = 'R';
			rookMove.moveFrom[X] = player[8].location[X];
			rookMove.moveFrom[Y] = player[8].location[Y];
			rookMove.moveTo[X] = player[8].location[X] + 3;
			rookMove.moveTo[Y] = player[8].location[Y];
			// make the rook move
			updateLocation(rookMove, &player[8], board);
		}
	}
}

// removes the piece from the old location & adds it to the new location
void updateLocation (const Move theMove, Piece *playerPtr, Square board[8][8]){
	// remove the piece from the old location
	removePiece(playerPtr, board);

	// update the piece's location indicator
	playerPtr->location[X] = theMove.moveTo[X];
	playerPtr->location[Y] = theMove.moveTo[Y];
	// update the piece's move count
	playerPtr->moveCount = (playerPtr->moveCount)++;


	// add the new location to the board
	board[playerPtr->location[X]][playerPtr->location[Y]].owner = theMove.owner;
	board[playerPtr->location[X]][playerPtr->location[Y]].type = playerPtr->type;
}

// deletes a piece and removes it from the board
void deletePiece (Piece *playerPtr, Square board[8][8]){
	// remove the piece from the board
	removePiece(playerPtr, board);

	// delete the piece from the player's piece array
	playerPtr->type = ' ';
	playerPtr->owner = NONE;
	playerPtr->location[X] = -1;
	playerPtr->location[Y] = -1;
}

// removes a piece from the board
void removePiece(Piece *playerPtr, Square board[8][8]){
	board[playerPtr->location[X]][playerPtr->location[Y]].owner = NONE;
	board[playerPtr->location[X]][playerPtr->location[Y]].type = ' ';
}

// converts a pawn to the player's choice
void convertPawn(Piece *player, Square board[8][8], int pieceOwner){
	char type;

	if (pieceOwner == HUMAN){
		printf("Convert pawn to: ");
		fflush(stdin);
		scanf("%c", &type);
		type = toupper(type);
	}
	// if it's the ai's piece, just convert to a queen
	else
		type = 'Q';

	// update the board
	board[player->location[X]][player->location[Y]].type = type;
	// update the piece entry
	player->type = type;
}

// prints the game menu & gets the user's choice
int getGameMenuChoice(void){
	int choice;

	// print the in-game menu
	printf("\t1. Continue playing\n");
	printf("\t2. Save game and quit to main menu\n");
	printf("\t3. Quit to main menu\n");
	printf("--> Your choice: ");

	// get the user's choice
	fflush(stdin);
	scanf("%d", &choice);
	switch (choice){
		case 1:
			choice = PLAY;
			break;
		case 2:
			choice = SAVE;
			break;
		case 3:
			choice = QUIT;
			break;
		default:
			choice = PLAY;
	}

	return choice;
}

// end game functions
void endGame(int victor, int gameStatus, Piece humanPieces[16], Piece computerPieces[16], Square board[8][8]){
	char endDialog[150], endLine[50];
	char word[20], endType[12];
	int i, timeout;

	// choose the correct string
	if (victor == COMPUTER){
		strcpy(endLine, "Your king has been slaughtered!");
		if (gameStatus == CHECKMATE)
			strcpy(endDialog, "HAHAHAAA!!! \n\t   I have checkmated you, foolish human! \n\t   My coding is obviously superior!!!\n");
		else
			strcpy(endDialog, "HAHAHAAA!!! \n\t   I have stalemated you, foolish human! \n\t   In this game that means I win, for some reason!\n");
	}
	else {
		strcpy(endLine, "You have slaughtered your opponent's king!");
		if (gameStatus == CHECKMATE)
			strcpy(endDialog, "You have checkmated me...\n\t   ...urgh...\n\t   My programmers must be inferior...\n");
		else
			strcpy(endDialog, "You have stalemated me...\n\t   ...urgh...\n\t   That means you win, for some insane reason.\n");
	}
		
	// print the victory/loss string
	printf("-COMPUTER: ");
	for (i = 0; i < strlen(endDialog); i++){
		printf("%c", endDialog[i]);

		timeout = 12 * (1 + (rand() % 10));
		sleep(timeout);
	}
	printf("----------------------------------------------------------\n");
	system("PAUSE");

	// do the ending graphics
	if (victor == COMPUTER)
		killKing(computerPieces, humanPieces, board);
	else
		killKing(humanPieces, computerPieces, board);

	// print the ending line
	printf("\n%s\n\n", endLine);
	printf("----------------------------------------------------------\n");
	system("PAUSE");
}

// move the winning king on top of the losing king
void killKing(Piece winner[16], Piece loser[16], Square board[8][8]){
	Move kingMove;
	Square block;
	int loserX = loser[15].location[X],
		loserY = loser[15].location[Y];
	
	// set kingMove variables
	kingMove.owner = winner[15].owner;
	kingMove.type = 'K';
	// move in the x-dimension
	kingMove.moveFrom[Y] = winner[15].location[Y];
	kingMove.moveTo[Y] = winner[15].location[Y];
	if (winner[15].location[X] > loser[15].location[X]){
		while (winner[15].location[X] != loserX){
			kingMove.moveFrom[X] = winner[15].location[X];
			kingMove.moveTo[X] = winner[15].location[X] - 1;

			// initialize the block square
			block.owner = NONE;
			block.type = ' ';

			// check if the king is moving onto a piece
			if (isOnPiece(kingMove, board) != LEGITIMATE){
				// clone the blocking piece square
				block.owner = board[kingMove.moveTo[X]][kingMove.moveTo[Y]].owner;
				block.type = board[kingMove.moveTo[X]][kingMove.moveTo[Y]].type;
			}

			// make the king move (deleting any blocking pieces)
			makeMove(kingMove, board, winner, loser, COMPUTER);

			// place the block piece square where the king moved from
			board[kingMove.moveFrom[X]][kingMove.moveFrom[Y]].owner = block.owner;
			board[kingMove.moveFrom[X]][kingMove.moveFrom[Y]].type = block.type;

			printBoard(board);
			sleep(500);
		}
	}
	else if (winner[15].location[X] < loser[15].location[X]){
		while (winner[15].location[X] != loserX){
			kingMove.moveFrom[X] = winner[15].location[X];
			kingMove.moveTo[X] = winner[15].location[X] + 1;

			// initialize the block square
			block.owner = NONE;
			block.type = ' ';

			// check if the king is moving onto a piece
			if (isOnPiece(kingMove, board) != LEGITIMATE){
				//clone the blocking piece square
				block.owner = board[kingMove.moveTo[X]][kingMove.moveTo[Y]].owner;
				block.type = board[kingMove.moveTo[X]][kingMove.moveTo[Y]].type;
			}

			// make the king move (deleting any blocking pieces)
			makeMove(kingMove, board, winner, loser, COMPUTER);

			// place the block piece square where the king moved from
			board[kingMove.moveFrom[X]][kingMove.moveFrom[Y]].owner = block.owner;
			board[kingMove.moveFrom[X]][kingMove.moveFrom[Y]].type = block.type;

			printBoard(board);
			sleep(500);
		}
	}
	// move in the y-dimension
	kingMove.moveFrom[X] = winner[15].location[X];
	kingMove.moveTo[X] = winner[15].location[X];
	if (winner[15].location[Y] > loser[15].location[Y]){
		while (winner[15].location[Y] != loserY){
			kingMove.moveFrom[Y] = winner[15].location[Y];
			kingMove.moveTo[Y] = winner[15].location[Y] - 1;

			// initialize the block square
			block.owner = NONE;
			block.type = ' ';

			// check if the king is moving onto a piece
			if (isOnPiece(kingMove, board) != LEGITIMATE){
				//clone the blocking piece square
				block.owner = board[kingMove.moveTo[X]][kingMove.moveTo[Y]].owner;
				block.type = board[kingMove.moveTo[X]][kingMove.moveTo[Y]].type;
			}

			// make the king move (deleting any blocking pieces)
			makeMove(kingMove, board, winner, loser, COMPUTER);

			// place the block piece square where the king moved from
			board[kingMove.moveFrom[X]][kingMove.moveFrom[Y]].owner = block.owner;
			board[kingMove.moveFrom[X]][kingMove.moveFrom[Y]].type = block.type;

			printBoard(board);
			sleep(500);
		}
	}
	else if (winner[15].location[Y] < loser[15].location[Y]){
		while (winner[15].location[Y] != loserY){
			kingMove.moveFrom[Y] = winner[15].location[Y];
			kingMove.moveTo[Y] = winner[15].location[Y] + 1;

			// initialize the block square
			block.owner = NONE;
			block.type = ' ';

			// check if the king is moving onto a piece
			if (isOnPiece(kingMove, board) != LEGITIMATE){
				// clone the blocking piece square
				block.owner = board[kingMove.moveTo[X]][kingMove.moveTo[Y]].owner;
				block.type = board[kingMove.moveTo[X]][kingMove.moveTo[Y]].type;
			}

			// make the king move (deleting any blocking pieces)
			makeMove(kingMove, board, winner, loser, COMPUTER);

			// place the block piece square where the king moved from
			board[kingMove.moveFrom[X]][kingMove.moveFrom[Y]].owner = block.owner;
			board[kingMove.moveFrom[X]][kingMove.moveFrom[Y]].type = block.type;

			printBoard(board);
			sleep(500);
		}
	}
}

// timeout
void sleep(clock_t wait){
	clock_t goal;
	
	goal = wait + clock();
	while (clock() < goal);
}
