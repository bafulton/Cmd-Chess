enum status {
	STEPDEMO = -4,
	DEMO = -3,
	QUIT = -2,
	SAVE = -1,
	LOAD = 0,
	PLAY = 1,
	CHECK = 2,
	STALEMATE = 3,
	CHECKMATE = 4
};

enum player {
	NONE,
	COMPUTER,
	HUMAN
};

enum legitimacy {
	LEGITIMATE = 1,
	NOTPIECE = -1,
	NOTYOURS = -2,
	SAMESQUARE = -3,
	OUTOFBOUNDS = -4,
	ONPIECE = -5,
	ILLEGAL = -6,
	KINGINTOCHECK = -7,
	KINGINCHECK = -8,
	BLOCKED = -9
};

enum column {A, B, C, D, E, F, G, H};

enum coordinates {X, Y};

enum boolean {FALSE, TRUE};
