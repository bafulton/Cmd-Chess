#include "structs.h"
#include "enums.h"
#include "prototypes.h"

//determines if a king is in check or checkmate
int getGameStatus(const Square board[8][8], const Piece player[16], const Piece opponent[16]){
	int gameStatus = PLAY;
	Move allMoves[200];
	int x, y;

	//opponent's king is in danger (check)
	if (isCapturable(opponent[15].location[X], opponent[15].location[Y], board, opponent, player) == TRUE)
		gameStatus = CHECK;
	//opponent has no legal moves (checkmate/stalemate)
	if (getAllMoves(allMoves, opponent, player, board) == 0){
		//king is in danger && player has no legal moves (checkmate)
		if (gameStatus == CHECK)
			gameStatus = CHECKMATE;
		//player has no legal moves (stalemate)
		else
			gameStatus = STALEMATE;
	}

	return gameStatus;
}
//gets all the possible moves of the designated player (first player array). Edits allMoves, returns move count.
int getAllMoves(Move allMoves[], const Piece player[16], const Piece opponent[16], const Square board[8][8]){
	Move tryMove;
	int i, x, y, dX, dY;
	int	count = 0;

	//initalize the allMoves array to zero
	for (i = 0; i < 200; i++){
		allMoves[i].owner = NONE;
		allMoves[i].moveFrom[X] = -1;
		allMoves[i].moveFrom[Y] = -1;
		allMoves[i].moveTo[X] = -1;
		allMoves[i].moveTo[Y] = -1;
	}

	//loop through player's pieces
	for (i = 0; i < 16; i++){
		if (player[i].owner != NONE){
			tryMove.moveFrom[X] = player[i].location[X];
			tryMove.moveFrom[Y] = player[i].location[Y];
			//loop through column by column
			for (x = 0; x < 8; x++){
				tryMove.moveTo[X] = x;
				//loop through all the rows of a single column
				for (y = 0; y < 8; y++){
					tryMove.type = board[tryMove.moveFrom[X]][tryMove.moveFrom[Y]].type;
					tryMove.owner = player[15].owner;
					tryMove.moveTo[Y] = y;

					dX = abs(tryMove.moveFrom[X] - tryMove.moveTo[X]);
					dY = abs(tryMove.moveFrom[Y] - tryMove.moveTo[Y]);
					//reduce processing time by eliminating all obviously impossible moves
					if (dX == 0 || dY == 0 || dX == dY || (dX == 1 && dY == 2) || (dX == 2 && dY == 1)){
						//reduce processing time by eliminating all illegal moves
						if (isLegal(tryMove, board, player, opponent) == TRUE){
							//add any legitimate possible moves to the allMoves array, provided there's still room in the allMoves array
							if(checkLegitimacy(tryMove, board, player, opponent, FALSE) > 0 && count < 200){
								allMoves[count].owner = player[15].owner;
								allMoves[count].type = board[tryMove.moveFrom[X]][tryMove.moveFrom[Y]].type;
								allMoves[count].moveFrom[X] = player[i].location[X];
								allMoves[count].moveFrom[Y] = player[i].location[Y];
								allMoves[count].moveTo[X] = x;
								allMoves[count].moveTo[Y] = y;
								count++;
							}
						}
					}
				}
			}
		}
	}

	return count;
}
//returns an array of threat structs listing all the pieces that could move to [x,y]. Edits threats, returns threat count.
int getThreats(Move threats[16], int x, int y, const Piece player[16], const Piece opponent[16], const Square board[8][8]){
	Move tryMove;
	int otherPlayer;
	int i, count = 0;

	//initialize the threats array
	for (i = 0; i < 16; i++){
		threats[i].owner = NONE;
		threats[i].type = ' ';
		threats[i].moveFrom[X] = -1;
		threats[i].moveFrom[Y] = -1;
		threats[i].moveTo[X] = -1;
		threats[i].moveTo[Y] = -1;
	}

	//figure out who the opponent is
	if (player[15].owner == COMPUTER)
		otherPlayer = HUMAN;
	else
		otherPlayer = COMPUTER;

	for (i = 0; i < 16; i++){
		//create a potential opponent move to [x,y]
		tryMove.owner = otherPlayer;
		tryMove.moveFrom[X] = opponent[i].location[X];
		tryMove.moveFrom[Y] = opponent[i].location[Y];
		tryMove.moveTo[X] = x;
		tryMove.moveTo[Y] = y;

		//add legitimate threats to the threats array
		if (checkLegitimacy(tryMove, board, player, opponent, TRUE) > 0){
			threats[count].owner = tryMove.owner;
			threats[count].type = opponent[i].type;
			threats[count].moveFrom[X] = tryMove.moveFrom[X];
			threats[count].moveFrom[Y] = tryMove.moveFrom[Y];
			threats[count].moveTo[X] = tryMove.moveTo[X];
			threats[count].moveTo[Y] = tryMove.moveTo[Y];

			count++;
		}
	}

	return count;
}

