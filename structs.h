struct piece {
	char type;
	int owner;
	int location[2];
	int moveCount;
};
struct square {
	int owner;
	char type;
};
struct move {
	int owner;
	char type;
	int moveFrom[2];
	int moveTo[2];
};
struct points {				//DAVID
	int cp; //Computer points
	int pp; //Player points
	int spread;  //computer's current advantage
	int moveInit;
};

struct pointVal {			//array of weighted point values... DAVID
	int pieceScale;
	int formation;
	int advancing;
};

typedef struct piece Piece;
typedef struct square Square;
typedef struct move Move;
typedef struct points Points;		//DAVID