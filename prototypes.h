#include <time.h>

//MAIN FUNCTIONS
int playGame(Piece humanPieces[16], Piece computerPieces[16], Square board[8][8], int gameType);
//INITIALIZATION
void createGame(Piece humanPieces[16], Piece computerPieces[16], Square board[8][8]);
void createBoard(Square board[8][8]);
void populate(Piece arr[], int owner, Square board[8][8]);
Piece spawn(char type, int owner, int x, int y);
//INTERFACE
int getMenuChoice(void);
void printBoard(const Square board[8][8]);
Move getHumanMove(const Piece humanPieces[16], const Piece computerPieces[16], const Square board[8][8]);
void makeMove(const Move theMove, Square board[8][8], Piece player[16], Piece opponent[16], int pieceOwner);
void updateLocation (const Move theMove, Piece *playerPtr, Square board[8][8]);
void deletePiece (Piece *playerPtr, Square board[8][8]);
void removePiece(Piece *playerPtr, Square board[8][8]);
void convertPawn(Piece *player, Square board[8][8], int pieceOwner);
int getGameMenuChoice(void);
int endGame(int victor, int gameStatus, Piece humanPieces[16], Piece computerPieces[16], Square board[8][8]);
void killKing(Piece winner[16], Piece loser[16], Square board[8][8]);
void sleep(clock_t wait);
//INTERPRETATION
void getError(int error, char errorMessage[32]);
int AtoI(char loc);
//EVALUATION
int getGameStatus(const Square board[8][8], const Piece player[16], const Piece opponent[16]);
int getAllMoves(Move allMoves[200], const Piece player[16], const Piece opponent[16], const Square board[8][8]);
int getThreats(Move threats[16], int x, int y, const Piece player[16], const Piece opponent[16], const Square board[8][8]);
//MOVE CHECKS
int checkLegitimacy(Move theMove, const Square board[8][8], const Piece player[16], const Piece opponent[16], int checkAll);
int isNotPiece(const Move theMove, const Square board[8][8]);
int isNotYours(const Move theMove, const Square board[8][8]);
int isSameSquare(const Move theMove);
int isOutOfBounds(const Move theMove);
int isOnPiece(const Move theMove, const Square board[8][8]);
int isLegal(const Move theMove, const Square board[8][8], const Piece player[16], const Piece opponent[16]);
int isEnPassant(const Move theMove, const Piece opponent[16], const Square board[8][8]);
int isCapturable(int x, int y, const Square board[8][8], const Piece player[16], const Piece opponent[16]);
int isStillCheck(const Move theMove, const Square board[8][8], const Piece player[16], const Piece opponent[16]);
int isBlocked(const Move theMove, const Square board[8][8]);
int blockedDiagonal(const Move theMove, const Square board[8][8]);
int blockedLinear(const Move theMove, const Square board[8][8]);
void generateTestBoard(Square testBoard[8][8], const Square board[8][8], const Move theMove);
//FILE IO
void loadGame(Piece humanPieces[16], Piece computerPieces[16], Square board[8][8]);
void saveGame(const Piece humanPieces[16], const Piece computerPieces[16]);
//AI... DAVID
void aiSwitch(Piece player[16], Piece opponent[16], Square board[8][8], struct pointVal);				//Switches between strategies and weights points for goals
void compMoveMid(Piece player[16], Piece opponent[16], Square board[8][8], struct pointVal, int movesAhead);	//returns which of legalmoves to use, for mid-game (general case)
Points getPoints(Piece player[16], Piece opponent[16], const Square board[8][8], struct pointVal, struct points selection, int fauxCount);	//Totals current points
Points chooseMove(struct points newPoints, struct points oldPoints, int fauxCount);
Points lookAhead(Piece move[16], Piece stat[16], Square board[8][8], int fauxCount, int movesAhead, struct pointVal values, struct points selection);
