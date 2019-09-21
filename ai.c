#include "structs.h"
#include "enums.h"
#include "prototypes.h"

void aiSwitch(Piece human[16], Piece comp[16], Square board[8][8], struct pointVal values){  //this is a relic of intintionality...
	//it can be ultimatly used to make a decision on if the game is in early stages, late stages, if the computer is up or down piecewise, etc
	//it was never properly utilized in this code

	struct points currentPoints ={0};
	int movesAhead = 3;
	currentPoints = getPoints(human, comp, board, values, currentPoints, 0);
	//printf("Comp has %d points\n", currentPoints.cp);
	compMoveMid(human, comp, board, values, movesAhead);
}

void compMoveMid(Piece human[16], Piece comp[16], Square board[8][8], struct pointVal values, int movesAhead) {
	int select=0;

	int fauxCount;
	Move allMoves[200];
	int count;
	int fauxMoves = 2;
	struct points selection= {0};
	fauxCount=0;
	count = getAllMoves(allMoves, comp, human, board);
	selection = getPoints(human, comp, board, values, selection, fauxCount);
	selection.moveInit = 0;
	selection.spread = -10000; //make sure a move is chosen

	selection = lookAhead(comp, human, board, -1, movesAhead, values, selection);

	/*
	//Next Player Move
	for (i=0; i<count; i++){
		fauxCount=0;					//even indicates computers hypothetical move, assuming this only initiates on computer's turn
		generateTestBoard(fauxBoard[fauxCount], board, allMoves[fauxCount][i]);
		for (j=0; j< 16; j++){
			fauxHuman[fauxCount][j].location[X] = human[j].location[X];
			fauxHuman[fauxCount][j].location[Y] = human[j].location[Y];
			fauxHuman[fauxCount][j].owner = human[j].owner;
			fauxHuman[fauxCount][j].type = human[j].type;
			fauxComp[fauxCount][j].location[X]=comp[j].location[X];
			fauxComp[fauxCount][j].location[Y]=comp[j].location[Y];
			fauxComp[fauxCount][j].owner=comp[j].owner;
			fauxComp[fauxCount][j].type=comp[j].type;
		}
		makeMove(allMoves[fauxCount][i], fauxBoard[fauxCount], fauxComp[fauxCount], fauxHuman[fauxCount]);
		moveValue = getPoints(fauxHuman[fauxCount], fauxComp[fauxCount], fauxBoard[fauxCount], values);
		select = chooseMove(moveValue, futureValues, fauxCount, i, select);
	}
	*/
	//select = rand() % count;

	makeMove(allMoves[selection.moveInit], board, comp, human, COMPUTER);
}

Points getPoints(Piece human[16], Piece comp[16], const Square board[8][8], struct pointVal values, struct points selection, int fauxCount){
	int i;
	char type;
	struct points currentPoints;
	struct piece bin[16];
	currentPoints.cp=0;
	currentPoints.pp=0;
	currentPoints.spread=0;
	currentPoints.moveInit = selection.moveInit;

	if (fauxCount % 2 ==1)
		for (i=0; i<16; i++){
			bin[i]=comp[i];
			comp[i]=human[i];
			human[i]=bin[i];
		}

	for (i=0; i<16; i++){
		type=human[i].type;
		switch (type){
			case 'P':
				currentPoints.pp += 1 * values.pieceScale;
				currentPoints.pp += values.advancing*(6-(human[i].location)[1]);
			break;
			case 'R':
				currentPoints.pp += 5 * values.pieceScale;
			break;
			case 'H':
				currentPoints.pp += 4 * values.pieceScale;
			break;
			case 'B':
				currentPoints.pp += 3 * values.pieceScale;
			break;
			case 'Q':
				currentPoints.pp += 12 * values.pieceScale;
			break;
			case 'K':
				currentPoints.pp += 0 * values.pieceScale;
			break;
		}
	}
	
	for(i=0;i<16;i++){
		type=comp[i].type;
		switch (type){
		case 'P':
			currentPoints.cp += 1 * values.pieceScale;
			currentPoints.cp += values.advancing*((comp[i].location)[1]-1);
			break;
		case 'R':
			currentPoints.cp += 5 * values.pieceScale;
			break;
		case 'H':
			currentPoints.cp += 4 * values.pieceScale;
			break;
		case 'B':
			currentPoints.cp += 3 * values.pieceScale;
			break;
		case 'Q':
			currentPoints.cp += 12 * values.pieceScale;
			break;
		case 'K':
			currentPoints.cp += 0 * values.pieceScale;
			break;
		}
	}
	currentPoints.spread = currentPoints.cp - currentPoints.pp;

	if (fauxCount % 2 ==1)
		for (i=0; i<16; i++){
			bin[i]=comp[i];
			comp[i]=human[i];
			human[i]=bin[i];
		}
	
	return currentPoints;
}

Points chooseMove(struct points newChoice, struct points oldChoice, int fauxCount) {
	if(fauxCount % 2 == 0){
		if (newChoice.spread > oldChoice.spread){
			return newChoice;
		}
		else
			return oldChoice;
	}
	else{
		if (newChoice.spread < oldChoice.spread){
			return newChoice;
		}
		else
			return oldChoice;
	}
}

Points lookAhead(Piece move[16], Piece stat[16], Square board[8][8], int fauxCount, int movesAhead, struct pointVal values, struct points selection){
	Piece nextMove[16];
	Piece nextStat[16];
	Move allMoves[200];
	Square nextBoard[8][8];
	struct points moveValue={0};
	struct points selectionbin;
	int count;
	int i, j;
	fauxCount++;
	count = getAllMoves(allMoves, move, stat, board);

	if (fauxCount %2 == 0)
		selection.spread = -10000;
	else
		selection.spread = 10000;

	for (i=0; i<count; i++){					//even indicates computers hypothetical move, assuming this only initiates on computer's turn
		generateTestBoard(nextBoard, board, allMoves[i]);

		if (fauxCount == 0){
			moveValue.spread = -10000; //insure move is chosen
			moveValue.moveInit = i;
		}
		else{
			//moveValue.spread = 10000;
			moveValue.moveInit = selection.moveInit;
		}

		for (j=0; j< 16; j++){
			nextMove[j].location[X] = stat[j].location[X];
			nextMove[j].location[Y] = stat[j].location[Y];
			nextMove[j].owner = stat[j].owner;
			nextMove[j].type = stat[j].type;
			nextStat[j].location[X]=move[j].location[X];
			nextStat[j].location[Y]=move[j].location[Y];
			nextStat[j].owner=move[j].owner;
			nextStat[j].type=move[j].type;
		}
		makeMove(allMoves[i], nextBoard, nextStat, nextMove, COMPUTER);

		if (fauxCount==0) //make 1 mve checkmate always
			if (getGameStatus(nextBoard, nextStat, nextMove)==CHECKMATE)
				return moveValue;


		if (fauxCount < movesAhead){
			selectionbin = selection;
			
			selectionbin.moveInit = moveValue.moveInit;

			moveValue = lookAhead(nextMove, nextStat, nextBoard, fauxCount, movesAhead, values, selectionbin);
			//selection = chooseMove(moveValue, selection, fauxCount);
			//moveValue = getPoints(nextMove, nextStat, nextBoard, values);
			//selection = chooseMove(moveValue, futureValues, fauxCount, selection);
		}

		if (fauxCount == movesAhead)
			moveValue = getPoints(nextMove, nextStat, nextBoard, values, moveValue, fauxCount);

		selection = chooseMove(moveValue, selection, fauxCount);
	}
	return selection;
}
