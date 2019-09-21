#include "structs.h"
#include "enums.h"
#include "prototypes.h"


// creates a new board and spawns the pieces
void createGame(Piece humanPieces[16], Piece computerPieces[16], Square board[8][8]){
	//create the chessboard
	createBoard(board);
	//populate the board
	populate(computerPieces, COMPUTER, board);
	populate(humanPieces, HUMAN, board);
}

// create the initial unpopulated board
void createBoard(Square board[8][8]){
	int x, y;
	
	for (y = 0; y < 8; y++){
		for (x = 0; x < 8; x++){
			board[x][y].owner = NONE;
			board[x][y].type = ' ';
		}
	}
}

// create the pieces & place them on the board
void populate(Piece arr[], int owner, Square board[8][8]){
	int i;
	int line;

	if (owner == COMPUTER)
		line = 0;
	else
		line = 7;

	// SPAWN--add piece structs to the player array
	// spawn the pawns
	for (i = 0; i < 8; i++){
		if (line == 0)
			arr[i] = spawn('P', owner, i, line + 1);
		else
			arr[i] = spawn('P', owner, i, line - 1);
	}
	// spawn the rooks
	arr[8] = spawn('R', owner, 0, line);
	arr[9] = spawn('R', owner, 7, line);
	// spawn the knights
	arr[10] = spawn('H', owner, 1, line);
	arr[11] = spawn('H', owner, 6, line);
	// spawn the bishops
	arr[12] = spawn('B', owner, 2, line);
	arr[13] = spawn('B', owner, 5, line);
	// spawn the king and queen
	arr[14] = spawn('Q', owner, 3, line);
	arr[15] = spawn('K', owner, 4, line);

	// PLACE--place the player array on the board
	for (i = 0; i < 16; i++){
		board[arr[i].location[X]][arr[i].location[Y]].owner = owner;
		board[arr[i].location[X]][arr[i].location[Y]].type = arr[i].type;
	}
}

// spawn a single piece
Piece spawn(char type, int owner, int x, int y){
	Piece item;

	// ASSIGNMENT--initialize a single chess piece
	// set the piece type
	item.type = type;
	// set the piece's owner
	item.owner = owner;
	// set the move count to zero
	item.moveCount = 0;
	// set the starting piece location
	item.location[X] = x;
	item.location[Y] = y;

	return item;
}
