#include "structs.h"
#include "enums.h"
#include "prototypes.h"

//checks to ensure the proposed move is legitimate
int checkLegitimacy(Move theMove, const Square board[8][8], const Piece player[16], const Piece opponent[16], int checkAll){
	int moveStatus = LEGITIMATE;
	Square testBoard[8][8];

	//generate the test board with the player's move
	generateTestBoard(testBoard, board, theMove);

	//is there a piece there? Only check if you need to (using checkAll)
	if (isNotPiece(theMove, board) != LEGITIMATE && checkAll == TRUE)
		moveStatus = NOTPIECE;
	//is it your piece? Only check if you need to (using checkAll)
	else if (isNotYours(theMove, board) != LEGITIMATE && checkAll == TRUE)
		moveStatus = NOTYOURS;
	//is the piece not moving anywhere?
	else if (isSameSquare(theMove) != LEGITIMATE)
		moveStatus = SAMESQUARE;
	//is the piece moving out of bounds?
	else if (isOutOfBounds(theMove) != LEGITIMATE)
		moveStatus = OUTOFBOUNDS;
	//is it moving onto another one of the player's pieces?
	else if (isOnPiece(theMove, board) != LEGITIMATE)
		moveStatus = ONPIECE;
	//is it a legal move for the piece? Only check if you need to (using checkAll)
	else if (isLegal(theMove, board, player, opponent) != LEGITIMATE && checkAll == TRUE)
		moveStatus = ILLEGAL;
	//is the player's king capturable if it moves there?
	else if (theMove.type == 'K'){
		if (isCapturable(theMove.moveTo[X], theMove.moveTo[Y], testBoard, player, opponent) == TRUE)
			moveStatus = KINGINTOCHECK;
	}
	//is the king in check and the move won't save him?
	else if (isStillCheck(theMove, testBoard, player, opponent) != LEGITIMATE)
		moveStatus = KINGINCHECK;
	//is the move blocked by another piece?
	if (moveStatus > 0 && isBlocked(theMove, board) != LEGITIMATE)
		moveStatus = BLOCKED;

	return moveStatus;
}
void generateTestBoard(Square testBoard[8][8], const Square board[8][8], const Move theMove){
	int x, y;
	
	//CLONE REAL BOARD
	//copy the positions from the real board
	for (x = 0; x < 8; x++){
		for (y = 0; y < 8; y++){
			testBoard[x][y].owner = board[x][y].owner;
			testBoard[x][y].type = board[x][y].type;
		}
	}

	//UPDATE TEST BOARD
	//remove piece from old location
	testBoard[theMove.moveFrom[X]][theMove.moveFrom[Y]].owner = NONE;
	testBoard[theMove.moveFrom[X]][theMove.moveFrom[Y]].type = ' ';
	//add piece to new location
	testBoard[theMove.moveTo[X]][theMove.moveTo[Y]].owner = theMove.owner;
	testBoard[theMove.moveTo[X]][theMove.moveTo[Y]].type = theMove.type;
}

int isNotPiece(const Move theMove, const Square board[8][8]){
	int moveStatus = NOTPIECE;
	int fromX = theMove.moveFrom[X],
		fromY = theMove.moveFrom[Y];
	
	if (fromX >= 0 && fromY >= 0)
		if (board[fromX][fromY].owner != NONE)
			moveStatus = LEGITIMATE;

	return moveStatus;
}
int isNotYours(const Move theMove, const Square board[8][8]){
	int moveStatus = NOTYOURS;

	if (board[theMove.moveFrom[X]][theMove.moveFrom[Y]].owner == theMove.owner)
		moveStatus = LEGITIMATE;

	return moveStatus;
}
int isSameSquare(const Move theMove){
	int moveStatus = LEGITIMATE;
	
	if (theMove.moveFrom[X] == theMove.moveTo[X] && theMove.moveFrom[Y] == theMove.moveTo[Y])
		moveStatus = SAMESQUARE;

	return moveStatus;
}
int isOutOfBounds(const Move theMove){
	int moveStatus = LEGITIMATE;
	int i;

	for (i = 0; i < 2; i++){
		if (theMove.moveTo[i] > 7 || theMove.moveTo[i] < 0)
			moveStatus = OUTOFBOUNDS;
	}
	
	return moveStatus;
}
int isOnPiece(const Move theMove, const Square board[8][8]){
	int moveStatus = LEGITIMATE;

	if (board[theMove.moveTo[X]][theMove.moveTo[Y]].owner == theMove.owner)
		moveStatus = ONPIECE;
	
	return moveStatus;
}
int isLegal(const Move theMove, const Square board[8][8], const Piece player[16], const Piece opponent[16]){
	int moveStatus = LEGITIMATE;
	int fromX = theMove.moveFrom[X],
		fromY = theMove.moveFrom[Y],
		toX = theMove.moveTo[X],
		toY = theMove.moveTo[Y];
	int startRow;	//for pawn first moves
	int i;

	switch (theMove.type){
		case 'P':
			moveStatus = ILLEGAL;
			if (theMove.owner == COMPUTER)
				startRow = 1;
			else
				startRow = 6;

			//if the pawn is not moving diagonally, then allow for normal movement
			if (fromX == toX){
				//allow for 1-square forward movement
				if (theMove.owner == COMPUTER){
					//the second check blocks the pawn from non-diagonal captures
					if (toY - fromY == 1  && board[toX][toY].owner != HUMAN)
						moveStatus = LEGITIMATE;
				}
				else {
					//the second check blocks the pawn from non-diagonal captures
					if (fromY - toY == 1 && board[toX][toY].owner != COMPUTER)
						moveStatus = LEGITIMATE;
				}
				//allow for 2-square first moves
				if (fromY == startRow){
					if (theMove.owner == COMPUTER){
						//the second check blocks the pawn from non-diagonal captures
						if (toY - fromY == 2 && board[toX][toY].owner != HUMAN)
							moveStatus = LEGITIMATE;
					}
					else {
						//the second check blocks the pawn from non-diagonal captures
						if (fromY - toY == 2 && board[toX][toY].owner != COMPUTER)
							moveStatus = LEGITIMATE;
					}
				}
			}
			//allow for diagonal forward capture movement between adjacent columns
			else if (abs(fromX - toX) == 1){
				if (theMove.owner == COMPUTER){
					if (toY - fromY == 1){
						//normal capture
						if (board[toX][toY].owner == HUMAN)
							moveStatus = LEGITIMATE;
						//en passant (call a separate function because the loop won't function here)
						else if (isEnPassant(theMove, opponent, board) == TRUE)
							moveStatus = LEGITIMATE;
					}
				}
				else {
					if (fromY - toY == 1){
						//normal capture
						if (board[toX][toY].owner == COMPUTER)
							moveStatus = LEGITIMATE;
						//en passant (call a separate function because the loop won't function here)
						else if (isEnPassant(theMove, opponent, board) == TRUE)
							moveStatus = LEGITIMATE;
					}
				}
			}
			break;
		case 'R':
			//check if same row or same column
			if (fromX != toX && fromY != toY)
				moveStatus = ILLEGAL;
			break;
		case 'B':
			//check if (absolute) change in x and y are equal to each other
			if (abs(fromX - toX) != abs(fromY - toY))
				moveStatus = ILLEGAL;
			break;
		case 'H':
			//look for change in location of 2 in one dimension and 1 in the other
			moveStatus = ILLEGAL;
			if (abs(fromX - toX) == 2 && abs(fromY - toY) == 1)
				moveStatus = LEGITIMATE;			
			else if (abs(fromX - toX) == 1 && abs(fromY - toY) == 2)
				moveStatus = LEGITIMATE;
			break;
		case 'Q':
			//check if same row or column (x or y axis move)
			if (fromX != toX && fromY != toY){
				//check if diagonal move
				if (abs(fromX - toX) != abs(fromY - toY))
					moveStatus = ILLEGAL;
			}
			break;
		case 'K':
			//check if the king is only moving one space
			if (abs(fromX - toX) > 1 || abs(fromY - toY) > 1)
				moveStatus = ILLEGAL;
			//check if the king is trying to castle
			if (fromY == toY && abs(fromX - toX) == 2){
				//if the king has never moved and isn't in check
				if (player[15].moveCount == 0 && isCapturable(theMove.moveFrom[X], theMove.moveFrom[Y], board, player, opponent) == FALSE){
					//castle short (to the right) -- player[9] is the rook on the right
					if (theMove.moveFrom[X] < theMove.moveTo[X] && player[9].moveCount == 0)
						moveStatus = LEGITIMATE;
					//castle long (to the left) -- player[8] is the rook on the left
					else if (theMove.moveFrom[X] > theMove.moveTo[X] && player[8].moveCount == 0)
						moveStatus = LEGITIMATE;
				}
			}
	}

	return moveStatus;
}
int isEnPassant(const Move theMove, const Piece opponent[16], const Square board[8][8]){
	int status = FALSE;
	int fromX = theMove.moveFrom[X],
		fromY = theMove.moveFrom[Y],
		toX = theMove.moveTo[X],
		toY = theMove.moveTo[Y];
	int oppX, oppY;
	int i, otherPlayer;

	if (theMove.owner == COMPUTER)
		otherPlayer = HUMAN;
	else
		otherPlayer = COMPUTER;

	//if player's pawn is cutting off an opponent's pawn && player's pawn is on row 4 or 5
	if (board[toX][fromY].owner == otherPlayer && board[toX][fromY].type == 'P' && (fromY == 3 || fromY == 4)){
		for (i = 0; i < 8; i++){
			oppX = opponent[i].location[X];
			oppY = opponent[i].location[Y];

			//if we've found the pawn in the opponent's array
			if (oppX == toX && oppY == fromY){
				status = TRUE;
				break;
			}
		}
	}

	return status;
}
int isCapturable(int x, int y, const Square testBoard[8][8], const Piece player[16], const Piece opponent[16]){
	int status = FALSE;
	Move tryMove;
	int otherPlayer;
	int i;

	//determine who the other player is
	if (testBoard[x][y].owner == COMPUTER)
		otherPlayer = HUMAN;
	else
		otherPlayer = COMPUTER;

	//generate an opponent move to the new location
	for (i = 0; i < 16; i++){
		tryMove.owner = otherPlayer;
		tryMove.type = opponent[i].type;
		tryMove.moveFrom[X] = opponent[i].location[X];
		tryMove.moveFrom[Y] = opponent[i].location[Y];
		tryMove.moveTo[X] = x;
		tryMove.moveTo[Y] = y;

		//if the move is legitimate, then the square is capturable (reverse opponent and player)
		if (checkLegitimacy(tryMove, testBoard, opponent, player, TRUE) > 0){
			status = TRUE;
			break;
		}
	}

	return status;
}
int isStillCheck(const Move theMove, const Square testBoard[8][8], const Piece player[16], const Piece opponent[16]){
	int moveStatus = LEGITIMATE;

	//determine if the player's king is still in check after the move
	if (isCapturable(player[15].location[X], player[15].location[Y], testBoard, player, opponent) == TRUE)
		moveStatus = KINGINCHECK;
	
	return moveStatus;
}
int isBlocked(const Move theMove, const Square board[8][8]){
	int moveStatus = LEGITIMATE;
	
	switch (theMove.type){
		case 'P':
			if (abs(theMove.moveFrom[Y] - theMove.moveTo[Y]) == 2){
				if (theMove.owner == COMPUTER){
					if (board[theMove.moveFrom[X]][theMove.moveFrom[Y] + 1].owner != NONE)
						moveStatus = BLOCKED;
				}
				else {
					if (board[theMove.moveFrom[X]][theMove.moveFrom[Y] - 1].owner != NONE)
						moveStatus = BLOCKED;
				}
			}
			break;
		case 'R':
			if (blockedLinear(theMove, board) != LEGITIMATE)
				moveStatus = BLOCKED;
			break;
		case 'H':
			break;
		case 'B':
			if (blockedDiagonal(theMove, board) != LEGITIMATE)
				moveStatus = BLOCKED;
			break;
		case 'Q':
			if (blockedLinear(theMove, board) != LEGITIMATE || blockedDiagonal(theMove, board) != LEGITIMATE)
				moveStatus = BLOCKED;
			break;
		case 'K':
			break;
	}

	return moveStatus;
}
int blockedDiagonal(const Move theMove, const Square board[8][8]){
	int moveStatus = LEGITIMATE;
	int fromX = theMove.moveFrom[X],
		fromY = theMove.moveFrom[Y],
		toX = theMove.moveTo[X],
		toY = theMove.moveTo[Y];
	int i;
	
	if (abs(fromX - toX) == abs(fromY - toY)){
		for (i = 1; i <= abs(fromX - toX) - 1; i++){
			if (fromX > toX && fromY > toY){
				if (board[fromX - i][fromY - i].owner != NONE){
					moveStatus = BLOCKED;
					break;
				}
				}
			if (fromX > toX && fromY < toY){
				if (board[fromX - i][fromY + i].owner != NONE){
					moveStatus = BLOCKED;
					break;
				}
			}
			if (fromX < toX && fromY > toY){
				if (board[fromX + i][fromY - i].owner != NONE){
					moveStatus = BLOCKED;
					break;
				}
			}
			if (fromX < toX && fromY < toY){
				if (board[fromX + i][fromY + i].owner != NONE){
					moveStatus = BLOCKED;
					break;
				}
			}
		}
	}

	return moveStatus;
}
int blockedLinear(const Move theMove, const Square board[8][8]){
	int moveStatus = LEGITIMATE;
	int fromX = theMove.moveFrom[X],
		fromY = theMove.moveFrom[Y],
		toX = theMove.moveTo[X],
		toY = theMove.moveTo[Y];
	int i;

	if (fromX == toX){
		for (i = 1; i <= abs(fromY - toY) - 1; i++){
			if (fromY > toY){
				if (board[fromX][fromY - i].owner != NONE){
					moveStatus = BLOCKED;
					break;
				}
			}
			else {
				if (board[fromX][fromY + i].owner != NONE){
					moveStatus = BLOCKED;
					break;
				}
			}
		}
	}
	else if (fromY == toY){
		for (i = 1; i <= abs(fromX - toX) - 1; i++){
			if (fromX > toX){
				if (board[fromX - i][fromY].owner != NONE){
					moveStatus = BLOCKED;
					break;
				}
			}
			else {
				if (board[fromX + i][fromY].owner != NONE){
					moveStatus = BLOCKED;
					break;
				}
			}
		}
	}

	return moveStatus;
}