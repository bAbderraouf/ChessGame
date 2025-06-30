#include "Chess.h"

Chess::Chess(int w, int h, int fps, int nRows, int nCols, int cSize, int lMargin, int tMargin, Color c1, Color c2)
{
	// game window
	windowWidth = w;
	windowHeigh = h;
	gameFps = fps;

	//grid
	numRows = nRows;
	numCols = nCols;
	cellSize = cSize;
	leftMargin = lMargin;
	topMargin = topMargin;
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
	grid->SetMargins(leftMargin, topMargin);

	//init Pieces
	player1 = InitPlayersPieces(true); // player 1
	player2 = InitPlayersPieces(false); // player 2


	//case info
	/*
	strCaseInfo.empty = false;
	strCaseInfo.caseHovered = false;
	strCaseInfo.caseName =
	strCaseInfo.
	strCaseInfo.
	strCaseInfo.
	*/


	
	//falgs
	flag_isPlayer1Turn = true;
	flag_leftMouseButtonPressed = false;
	flag_rightMouseButtonPressed = false;
	flag_leftMouseButtonDown = false;
	flag_leftMouseButtonReleased = false;

}

void Chess::Input()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		flag_leftMouseButtonPressed = true;
	}

	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		flag_rightMouseButtonPressed = true;
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		flag_leftMouseButtonDown = true;
	}
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		flag_leftMouseButtonReleased = true;
	}
}
;


void Chess::Update()
{
	//----------------------
	// Piece selected 
	//----------------------
	if (flag_leftMouseButtonPressed )
	{
		flag_leftMouseButtonPressed = false;

		// if 
		Position step(-1 * cellSize, 0);

		//MovePiece(*player1[3], step);
	}

	//----------------------
	// Drag piece
	//----------------------
	if (flag_leftMouseButtonDown)
	{
		flag_leftMouseButtonDown = false;
		
		DragPiece();
	}
	//----------------------
	// change player side  (after each finished move)
	//----------------------
	if (flag_rightMouseButtonPressed)
	{		
		flag_rightMouseButtonPressed = false;

		// change player side
		flag_isPlayer1Turn = !flag_isPlayer1Turn;
	}
}

void Chess::Draw()
{
	// grid
	grid->Draw();
	// players (all pieces)
	DrawPlayerPieces(player1);
	DrawPlayerPieces(player2);
	
}

//----------------------
// additionel functions 
//----------------------

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
		
	// set pieceImage size to cellSize
	for (auto& p : vectOfPieces)
		p->SetImageSize(cellSize);

	// set Pieces positions according to player side
	if (player1Side)
	{
		// position(row,col) where :
		// row: (cellIdx * cellSize) + topMargin , coll:( cellIdx * cellSize) + leftMargin 

		vectOfPieces[0]->SetCurrentPosition(Position(6 * cellSize + topMargin, 0 * cellSize + leftMargin)); //pion left
		vectOfPieces[1]->SetCurrentPosition(Position(6 * cellSize + topMargin, 1 * cellSize + leftMargin));
		vectOfPieces[2]->SetCurrentPosition(Position(6 * cellSize + topMargin, 2 * cellSize + leftMargin));
		vectOfPieces[3]->SetCurrentPosition(Position(6 * cellSize + topMargin, 3 * cellSize + leftMargin));
		vectOfPieces[4]->SetCurrentPosition(Position(6 * cellSize + topMargin, 4 * cellSize + leftMargin));
		vectOfPieces[5]->SetCurrentPosition(Position(6 * cellSize + topMargin, 5 * cellSize + leftMargin));
		vectOfPieces[6]->SetCurrentPosition(Position(6 * cellSize + topMargin, 6 * cellSize + leftMargin));
		vectOfPieces[7]->SetCurrentPosition(Position(6 * cellSize + topMargin, 7 * cellSize + leftMargin)); // pion right

		vectOfPieces[8]->SetCurrentPosition(Position(7 * cellSize + topMargin, 2 * cellSize + leftMargin)); // left fou
		vectOfPieces[9]->SetCurrentPosition(Position(7 * cellSize + topMargin, 5 * cellSize + leftMargin)); // right fou
		vectOfPieces[10]->SetCurrentPosition(Position(7 * cellSize + topMargin, 1 * cellSize + leftMargin)); // left cavalier
		vectOfPieces[11]->SetCurrentPosition(Position(7 * cellSize + topMargin, 6 * cellSize + leftMargin)); // right cavalier
		vectOfPieces[12]->SetCurrentPosition(Position(7 * cellSize + topMargin, 0 * cellSize + leftMargin)); // left tour
		vectOfPieces[13]->SetCurrentPosition(Position(7 * cellSize + topMargin, 7 * cellSize + leftMargin)); // right tour
		vectOfPieces[14]->SetCurrentPosition(Position(7 * cellSize + topMargin, 4 * cellSize + leftMargin)); // roi
		vectOfPieces[15]->SetCurrentPosition(Position(7 * cellSize + topMargin, 3 * cellSize + leftMargin)); // dame
	}
	else
	{
		vectOfPieces[0]->SetCurrentPosition(Position(1 * cellSize + topMargin, 0 * cellSize + leftMargin)); //pion left
		vectOfPieces[1]->SetCurrentPosition(Position(1 * cellSize + topMargin, 1 * cellSize + leftMargin));
		vectOfPieces[2]->SetCurrentPosition(Position(1 * cellSize + topMargin, 2 * cellSize + leftMargin));
		vectOfPieces[3]->SetCurrentPosition(Position(1 * cellSize + topMargin, 3 * cellSize + leftMargin));
		vectOfPieces[4]->SetCurrentPosition(Position(1 * cellSize + topMargin, 4 * cellSize + leftMargin));
		vectOfPieces[5]->SetCurrentPosition(Position(1 * cellSize + topMargin, 5 * cellSize + leftMargin));
		vectOfPieces[6]->SetCurrentPosition(Position(1 * cellSize + topMargin, 6 * cellSize + leftMargin));
		vectOfPieces[7]->SetCurrentPosition(Position(1 * cellSize + topMargin, 7 * cellSize + leftMargin)); // pion right

		vectOfPieces[8]->SetCurrentPosition(Position(0 * cellSize + topMargin, 2 * cellSize + leftMargin)); // left fou
		vectOfPieces[9]->SetCurrentPosition(Position(0 * cellSize + topMargin, 5 * cellSize + leftMargin)); // right fou
		vectOfPieces[10]->SetCurrentPosition(Position(0 * cellSize + topMargin, 1 * cellSize + leftMargin)); // left cavalier
		vectOfPieces[11]->SetCurrentPosition(Position(0 * cellSize + topMargin, 6 * cellSize + leftMargin)); // right cavalier
		vectOfPieces[12]->SetCurrentPosition(Position(0 * cellSize + topMargin, 0 * cellSize + leftMargin)); // left tour
		vectOfPieces[13]->SetCurrentPosition(Position(0 * cellSize + topMargin, 7 * cellSize + leftMargin)); // right tour
		vectOfPieces[14]->SetCurrentPosition(Position(0 * cellSize + topMargin, 4 * cellSize + leftMargin)); // roi
		vectOfPieces[15]->SetCurrentPosition(Position(0 * cellSize + topMargin, 3 * cellSize + leftMargin)); // dame
	
	}
	
	return vectOfPieces;
	
}

void Chess::MovePiece(Piece & piece, Position step)
{
	//current position
	Position currentPos = piece.GetCurrentPosition();

	// next position = current + step
	Position nextPos(0, 0);
	nextPos.row = currentPos.row + step.row;
	nextPos.col = currentPos.col + step.col;

	// move piece to the next position
	piece.SetCurrentPosition(nextPos);
}

void Chess::DragPiece()
{
	//----------------------
	// get mouse position
	//----------------------
	Vector2 mouseStep = GetMouseDelta();
	Position step(0, 0);
	step.row = mouseStep.y;
	step.col = mouseStep.x;

	//----------------------
	//get selected piece
	//----------------------
	int index = -1;
	if (flag_isPlayer1Turn)
	{
		index = GetSelectedPiece(player1);
		if (index != -1)
			MovePiece(*player1[index], step);
	}
	else
	{
		index = GetSelectedPiece(player2);
		if (index != -1)
			MovePiece(*player2[index], step);
	}
}

int Chess::GetSelectedPiece(std::vector<std::unique_ptr<Piece>> const &player)
{
	for (int idx = 0 ; idx < player.size(); idx++)
	{
		// check if mouse position is inside piece image
		if (CheckCollisionPointRec(GetMousePosition(), player[idx]->GetImageBounds()))
			return idx;
	}
	return -1; // no piece is selected
}

