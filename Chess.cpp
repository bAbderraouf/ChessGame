#include "Chess.h"

Chess::Chess(int w, int h, int fps, int nRows, int nCols, int cSize)
{
	// game window
	windowWidth = w;
	windowHeigh = h;
	gameFps = fps;

	//grid
	numRows = nRows;
	numCols = nCols;
	cellSize = cSize;

	// init widow + grid + piece
	Init();
}

Chess::~Chess()
{
	delete piece;
	delete grid;
}

void Chess::Init()
{
	// Game window
	InitWindow(windowWidth, windowHeigh, "Raouf's Tetris");
	setGameFPS(gameFps);	// frames per second

	//grid
	grid = new Grid(numRows, numCols, cellSize);
	grid->SetMargins(10, 10);
}

void Chess::Input()
{
}

void Chess::Update()
{
}

void Chess::Draw()
{
	grid->Draw();
}

// additionel functions 

void Chess::setGameFPS(int fps)
{
	gameFps = fps;
	SetTargetFPS(gameFps);
}


void Chess::AddPieceToGrid(Piece& const piece)
{
	Position pos = piece.getPosition();
}

void Chess::EreasePieceFromGrid(Piece& const piece)
{
	Position pos = piece.getPosition();

}
