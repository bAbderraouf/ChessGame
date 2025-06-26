#include "Chess.h"

Chess::Chess(int w, int h, int fps, int nRows, int nCols, int cSize , Color c1 ,Color c2)
{
	// game window
	windowWidth = w;
	windowHeigh = h;
	gameFps = fps;

	//grid
	numRows = nRows;
	numCols = nCols;
	cellSize = cSize;
	color1 = c1;
	color2 = c2;

	// init widow + grid + piece
	Init();
}

Chess::~Chess()
{
	delete grid;
	// player1,2 are unique_ptr
}

void Chess::Init()
{
	// Game window
	InitWindow(windowWidth, windowHeigh, "Raouf's 2D Chess Game");
	setGameFPS(gameFps);	// frames per second

	//grid
	grid = new Grid(numRows, numCols, cellSize , color1, color2);
	grid->SetMargins(10, 10);

	//init Pieces
	player1 = InitPlayersPieces(true); // player 1
	player2 = InitPlayersPieces(false); // player 2
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
	DrawPlayerPieces(player1);
	DrawPlayerPieces(player2);
	
}

// additionel functions 

void Chess::setGameFPS(int fps)
{
	gameFps = fps;
	SetTargetFPS(gameFps);
}


void Chess::AddPieceToGrid(Piece& const piece)
{
}

void Chess::EreasePieceFromGrid(Piece& const piece)
{

}

void Chess::DrawPlayerPieces(std::vector<std::unique_ptr<Piece>> const &player)
{
	for (const auto& e : player)
		e->Draw();
}

std::vector<std::unique_ptr<Piece>>  Chess::InitPlayersPieces(bool player1Side)
{
	std::vector<std::unique_ptr<Piece>> vectOfPieces;

	// pions
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
	//autres pieces
		vectOfPieces.push_back(std::make_unique<Fou>(player1Side));
		vectOfPieces.push_back(std::make_unique<Fou>(player1Side));
		vectOfPieces.push_back(std::make_unique<Cavalier>(player1Side));
		vectOfPieces.push_back(std::make_unique<Cavalier>(player1Side));
		vectOfPieces.push_back(std::make_unique<Tour>(player1Side));
		vectOfPieces.push_back(std::make_unique<Tour>(player1Side));
		vectOfPieces.push_back(std::make_unique<Roi>(player1Side));
		vectOfPieces.push_back(std::make_unique<Dame>(player1Side));
		

	if (player1Side)
	{
		vectOfPieces[0]->SetCurrentPosition(Position(6 * cellSize, 0 * cellSize)); //pion left
		vectOfPieces[1]->SetCurrentPosition(Position(6 * cellSize, 1 * cellSize));
		vectOfPieces[2]->SetCurrentPosition(Position(6 * cellSize, 2 * cellSize));
		vectOfPieces[3]->SetCurrentPosition(Position(6 * cellSize, 3 * cellSize));
		vectOfPieces[4]->SetCurrentPosition(Position(6 * cellSize, 4 * cellSize));
		vectOfPieces[5]->SetCurrentPosition(Position(6 * cellSize, 5 * cellSize));
		vectOfPieces[6]->SetCurrentPosition(Position(6 * cellSize, 6 * cellSize));
		vectOfPieces[7]->SetCurrentPosition(Position(6 * cellSize, 7 * cellSize)); // pion right

		vectOfPieces[8]->SetCurrentPosition(Position(7 * cellSize, 2 * cellSize)); // left fou
		vectOfPieces[9]->SetCurrentPosition(Position(7 * cellSize, 5 * cellSize)); // right fou
		vectOfPieces[10]->SetCurrentPosition(Position(7 * cellSize, 1 * cellSize)); // left cavalier
		vectOfPieces[11]->SetCurrentPosition(Position(7 * cellSize, 6 * cellSize)); // right cavalier
		vectOfPieces[12]->SetCurrentPosition(Position(7 * cellSize, 0 * cellSize)); // left tour
		vectOfPieces[13]->SetCurrentPosition(Position(7 * cellSize, 7 * cellSize)); // right tour
		vectOfPieces[14]->SetCurrentPosition(Position(7 * cellSize, 4 * cellSize)); // roi
		vectOfPieces[15]->SetCurrentPosition(Position(7 * cellSize, 3 * cellSize)); // dame
	}
	else
	{
		vectOfPieces[0]->SetCurrentPosition(Position(1 * cellSize, 0 * cellSize)); //pion left
		vectOfPieces[1]->SetCurrentPosition(Position(1 * cellSize, 1 * cellSize));
		vectOfPieces[2]->SetCurrentPosition(Position(1 * cellSize, 2 * cellSize));
		vectOfPieces[3]->SetCurrentPosition(Position(1 * cellSize, 3 * cellSize));
		vectOfPieces[4]->SetCurrentPosition(Position(1 * cellSize, 4 * cellSize));
		vectOfPieces[5]->SetCurrentPosition(Position(1 * cellSize, 5 * cellSize));
		vectOfPieces[6]->SetCurrentPosition(Position(1 * cellSize, 6 * cellSize));
		vectOfPieces[7]->SetCurrentPosition(Position(1 * cellSize, 7 * cellSize)); // pion right

		vectOfPieces[8]->SetCurrentPosition(Position(0 * cellSize, 2 * cellSize)); // left fou
		vectOfPieces[9]->SetCurrentPosition(Position(0 * cellSize, 5 * cellSize)); // right fou
		vectOfPieces[10]->SetCurrentPosition(Position(0 * cellSize, 1 * cellSize)); // left cavalier
		vectOfPieces[11]->SetCurrentPosition(Position(0 * cellSize, 6 * cellSize)); // right cavalier
		vectOfPieces[12]->SetCurrentPosition(Position(0 * cellSize, 0 * cellSize)); // left tour
		vectOfPieces[13]->SetCurrentPosition(Position(0 * cellSize, 7 * cellSize)); // right tour
		vectOfPieces[14]->SetCurrentPosition(Position(0 * cellSize, 4 * cellSize)); // roi
		vectOfPieces[15]->SetCurrentPosition(Position(0 * cellSize, 3 * cellSize)); // dame
	}
	
	return vectOfPieces;
	
}

