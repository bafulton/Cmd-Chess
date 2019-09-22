#include <stdio.h>
#include <stdlib.h>
#include "prototypes.h"
#include "structs.h"
#include "enums.h"


int main(void) {
	Piece computerPieces[16], humanPieces[16];
	Square board[8][8];
	int choice;

	do {
		choice = getMenuChoice();
		switch (choice){
			case LOAD:
				loadGame(humanPieces, computerPieces, board);
				playGame(humanPieces, computerPieces, board, PLAY);
				break;
			case PLAY:
				createGame(humanPieces, computerPieces, board);
				playGame(humanPieces, computerPieces, board, PLAY);
				break;
			case DEMO: case STEPDEMO:
				createGame(humanPieces, computerPieces, board);
				playGame(humanPieces, computerPieces, board, choice);
		}
	} while (choice != QUIT);
}

int playGame(Piece humanPieces[16], Piece computerPieces[16], Square board[8][8], int gameType){
	Move playerMove;
	Move allMoves[200];
	int gameStatus = PLAY, victor = NONE;
	int attempt = 0;		//tracks the number of move attempts the player has made this turn
	char errorMessage[50], endType[12], input;
	int count, i;
	Move previous;
	Move sortedMoves[16][21];	//an array of all possible moves by piece
	int piece, move;
	//Initialize Points for AI
	struct pointVal values;
		values.pieceScale = 10;
		values.formation = 1;
		values.advancing = 1;
	

	srand(time(NULL));

	printBoard(board);
	//play until one of the players is in stalemate or checkmate
	while (gameStatus < 3){
		//if the demo was requested, have the computer generate the human move
		if (gameType == DEMO || gameType == STEPDEMO){
			//COMPUTER-GENERATED HUMAN MOVE
			//get all the possible moves
			count = getAllMoves(allMoves, humanPieces, computerPieces, board);

			//pick a random move
			move = rand() % count;

			//make the move
			makeMove(allMoves[move], board, humanPieces, computerPieces, COMPUTER);

			/*
			//NOTE: the following code should generate a random piece to move, but is still very buggy
			//get all the possible pieces to move
			previous = allMoves[0];
			piece = 0;
			move = -1;
			for (i = 0; i < count; i++){
				move++;

				//move to the next piece entry in the sortedMoves array if we're on a new piece
				if (allMoves[i].moveFrom[X] != previous.moveFrom[X] || allMoves[i].moveFrom[Y] != previous.moveFrom[Y]){
					piece++;
					move = 0;
				}

				//increment the previous move
				previous = allMoves[i];

				//add the next allMoves entry to the proper sortedMoves entry
				sortedMoves[piece][move] = allMoves[i];
			}

			//pick a random piece
			piece = rand() % (piece + 1);
			//count the number of possible moves for that piece
			move = 0;
			while (sortedMoves[piece][move].owner == HUMAN || sortedMoves[piece][move].owner == COMPUTER || sortedMoves[piece][move].owner == 0){
				move++;								
			}
			//pick a random move for that piece
			move = rand() % move;

			//make the move
			makeMove(sortedMoves[piece][move], board, humanPieces, computerPieces, COMPUTER);
			*/
		}
		//otherwise play a normal human vs. computer game
		else {
			//HUMAN MOVE
			playerMove = getHumanMove(humanPieces, computerPieces, board);
			//stop the game if the player wants to quit
			if (playerMove.owner == 999)
				return QUIT;
			//if playerMove.owner is 0 or 1, it is the player owner
			//if playerMove.owner is < 0, it is an error listed in enum legitimacy
			if (playerMove.owner < 0){
				getError(playerMove.owner, errorMessage);
				printf("%s\n", errorMessage);
				attempt++;
				continue;
			}
			else {
				attempt = 0;
				makeMove(playerMove, board, humanPieces, computerPieces, HUMAN);
			}
		}
		//see if the human's king is in stalemate or checkmate
		if ((gameStatus = getGameStatus(board, humanPieces, computerPieces)) > 2){
			victor = HUMAN;
			break;
		}
		//print the new board
		printBoard(board);
		//if the player wanted to step through the demo, allow him to examine the human move
		if (gameType == STEPDEMO){
			printf("Press any key to continue...\n");
    		getchar();
		}

		//COMPUTER MOVE
		aiSwitch(humanPieces, computerPieces, board, values);

		//see if the human's king is in stalemate or checkmate
		if ((gameStatus = getGameStatus(board, computerPieces, humanPieces)) > 2){
			victor = COMPUTER;
			break;
		}
		//print the new board
		printBoard(board);
		//if the player wanted to step through the demo, allow him to examine the computer move
		if (gameType == STEPDEMO){
			printf("Press any key to continue...\n");
    		getchar();
		}
	}

	//END OF GAME
	printBoard(board);
	endGame(victor, gameStatus, humanPieces, computerPieces, board);

	return gameStatus;
}
