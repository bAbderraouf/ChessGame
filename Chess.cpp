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
	topMargin = tMargin;
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

	selectdPieceID = -1; // no piece is selected

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
	flag_isAnyPieceSelected = false;

}

void Chess::Input()
{
	// LEFT mouse mouse PRESSED
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		flag_leftMouseButtonPressed = true;
	}

	// RIGHT mouse mouse PRESSED
	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		flag_rightMouseButtonPressed = true;
	}

	// LEFT mouse mouse is DOWN
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		flag_leftMouseButtonDown = true;
	}

	// LEFT mouse mouse is RELEASED
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
	// piece released
	//----------------------
	if (flag_leftMouseButtonReleased)
	{
		flag_leftMouseButtonReleased = false;

		ReleasePiece();

		selectdPieceID = -1; // no selected piece

		flag_isAnyPieceSelected = false;

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

		vectOfPieces[0]->SetPosition(Position(6 * cellSize + topMargin, 0 * cellSize + leftMargin)); //pion left
		vectOfPieces[1]->SetPosition(Position(6 * cellSize + topMargin, 1 * cellSize + leftMargin));
		vectOfPieces[2]->SetPosition(Position(6 * cellSize + topMargin, 2 * cellSize + leftMargin));
		vectOfPieces[3]->SetPosition(Position(6 * cellSize + topMargin, 3 * cellSize + leftMargin));
		vectOfPieces[4]->SetPosition(Position(6 * cellSize + topMargin, 4 * cellSize + leftMargin));
		vectOfPieces[5]->SetPosition(Position(6 * cellSize + topMargin, 5 * cellSize + leftMargin));
		vectOfPieces[6]->SetPosition(Position(6 * cellSize + topMargin, 6 * cellSize + leftMargin));
		vectOfPieces[7]->SetPosition(Position(6 * cellSize + topMargin, 7 * cellSize + leftMargin)); // pion right

		vectOfPieces[8]->SetPosition(Position(7 * cellSize + topMargin, 2 * cellSize + leftMargin)); // left fou
		vectOfPieces[9]->SetPosition(Position(7 * cellSize + topMargin, 5 * cellSize + leftMargin)); // right fou
		vectOfPieces[10]->SetPosition(Position(7 * cellSize + topMargin, 1 * cellSize + leftMargin)); // left cavalier
		vectOfPieces[11]->SetPosition(Position(7 * cellSize + topMargin, 6 * cellSize + leftMargin)); // right cavalier
		vectOfPieces[12]->SetPosition(Position(7 * cellSize + topMargin, 0 * cellSize + leftMargin)); // left tour
		vectOfPieces[13]->SetPosition(Position(7 * cellSize + topMargin, 7 * cellSize + leftMargin)); // right tour
		vectOfPieces[14]->SetPosition(Position(7 * cellSize + topMargin, 4 * cellSize + leftMargin)); // roi
		vectOfPieces[15]->SetPosition(Position(7 * cellSize + topMargin, 3 * cellSize + leftMargin)); // dame
	}
	else
	{
		vectOfPieces[0]->SetPosition(Position(1 * cellSize + topMargin, 0 * cellSize + leftMargin)); //pion left
		vectOfPieces[1]->SetPosition(Position(1 * cellSize + topMargin, 1 * cellSize + leftMargin));
		vectOfPieces[2]->SetPosition(Position(1 * cellSize + topMargin, 2 * cellSize + leftMargin));
		vectOfPieces[3]->SetPosition(Position(1 * cellSize + topMargin, 3 * cellSize + leftMargin));
		vectOfPieces[4]->SetPosition(Position(1 * cellSize + topMargin, 4 * cellSize + leftMargin));
		vectOfPieces[5]->SetPosition(Position(1 * cellSize + topMargin, 5 * cellSize + leftMargin));
		vectOfPieces[6]->SetPosition(Position(1 * cellSize + topMargin, 6 * cellSize + leftMargin));
		vectOfPieces[7]->SetPosition(Position(1 * cellSize + topMargin, 7 * cellSize + leftMargin)); // pion right

		vectOfPieces[8]->SetPosition(Position(0 * cellSize + topMargin, 2 * cellSize + leftMargin)); // left fou
		vectOfPieces[9]->SetPosition(Position(0 * cellSize + topMargin, 5 * cellSize + leftMargin)); // right fou
		vectOfPieces[10]->SetPosition(Position(0 * cellSize + topMargin, 1 * cellSize + leftMargin)); // left cavalier
		vectOfPieces[11]->SetPosition(Position(0 * cellSize + topMargin, 6 * cellSize + leftMargin)); // right cavalier
		vectOfPieces[12]->SetPosition(Position(0 * cellSize + topMargin, 0 * cellSize + leftMargin)); // left tour
		vectOfPieces[13]->SetPosition(Position(0 * cellSize + topMargin, 7 * cellSize + leftMargin)); // right tour
		vectOfPieces[14]->SetPosition(Position(0 * cellSize + topMargin, 4 * cellSize + leftMargin)); // roi
		vectOfPieces[15]->SetPosition(Position(0 * cellSize + topMargin, 3 * cellSize + leftMargin)); // dame
	
	}
	
	return vectOfPieces;
	
}

void Chess::MovePiece(Piece & piece, Position step)
{
	//current position
	Position currentPos = piece.GetPosition();

	// next position = current + step
	Position nextPos(0, 0);
	nextPos.row = currentPos.row + step.row;
	nextPos.col = currentPos.col + step.col;

	// move piece to the next position
	piece.SetPosition(nextPos);
}

void Chess::DragPiece()
{
	//----------------------
	// get mouse delta position
	//----------------------
	Vector2 mouseStep = GetMouseDelta();
	Position step(0, 0);
	step.row = mouseStep.y;
	step.col = mouseStep.x;

	//----------------------
	// get cursor position
	//----------------------
	Vector2 cursor = GetMousePosition();
	Position cursorPos(0, 0);
	cursorPos.row = cursor.y;
	cursorPos.col = cursor.x;
	
	
	//----------------------
	//get selected piece
	//----------------------
	if (flag_isPlayer1Turn)
	{
		if (!flag_isAnyPieceSelected )
		{
			selectdPieceID = GetSelectedPiece(player1);
		}
			

		if (selectdPieceID != -1)
		{
			flag_isAnyPieceSelected = true;

			// set centerPiece = mousePos
			SetCenterPieceToCursorPosition(*player1[selectdPieceID], cursorPos);

			// drag piece to cursor position
			MovePiece(*player1[selectdPieceID], step);
		}
	}
	else
	{
		if (!flag_isAnyPieceSelected )
		{
			selectdPieceID = GetSelectedPiece(player2);
		}

		if (selectdPieceID != -1)
		{
			flag_isAnyPieceSelected = true;

			// set centerPiece = mousePos
			SetCenterPieceToCursorPosition(*player2[selectdPieceID], cursorPos);

			// drag piece to cursor position
			MovePiece(*player2[selectdPieceID], step);
		}
	}
}

void Chess::ReleasePiece()
{
	if(selectdPieceID != -1  && flag_isAnyPieceSelected)
	{
		if (flag_isPlayer1Turn)
		{
			// get center pos
			Position centerPos = player1[selectdPieceID]->GetCenterOfPiecePosition();

			// get current pos
			Position currentPos = player1[selectdPieceID]->GetPosition();

			// get corresponding board case position
			Position correspondingCase = GetCorrespondingBoardCase(centerPos);
				
			// calculate step
			Position step(0, 0);
			step.row = correspondingCase.row - currentPos.row;
			step.col = correspondingCase.col - currentPos.col;
			
			// move piece to the corresponding case
			MovePiece(*player1[selectdPieceID], step);
		}
		else
		{
			// get center pos
			Position centerPos = player2[selectdPieceID]->GetCenterOfPiecePosition();

			// get current pos
			Position currentPos = player2[selectdPieceID]->GetPosition();

			// get corresponding board case position
			Position correspondingCase = GetCorrespondingBoardCase(centerPos);

			// calculate step
			Position step(0, 0);
			step.row = correspondingCase.row - currentPos.row;
			step.col = correspondingCase.col - currentPos.col;

			// move piece to the corresponding case
			MovePiece(*player2[selectdPieceID], step);
		}
	}
	
}

/*******************************************
* @brief This function gets the selected piece to drag using the cursor.
* @param piece : a vector of pieces.
* @return : The index of the selected piece from the correspondig vector of pieces, 
* returns -1 if no piece is selected.
*/
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

/*******************************************
* @brief This function sets the center of the piece on the same positon as the cursor.
* @param piece : the selected piece to drag
* @param cursoPos : the current position of the cursor.
*/
void Chess::SetCenterPieceToCursorPosition(Piece& piece, Position const &cursorPos)
{
	
	Position delta_center_cursor(0, 0),
		centerPiece(0, 0);

	// get center of piece position
	centerPiece = piece.GetCenterOfPiecePosition();

	//calculate delta_center_cursor
	delta_center_cursor.row = cursorPos.row - centerPiece.row;
	delta_center_cursor.col = cursorPos.col - centerPiece.col;

	MovePiece(piece, delta_center_cursor);
}

Position Chess::GetCorrespondingBoardCase(Position centerPos)
{
	Position result(0,0);
	int x = 0, y = 0;

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			// position of each cell
			x = col * cellSize;
			y = row *  cellSize;

			// add margins
			x = x + leftMargin;
			y = y + topMargin;

			// position of the center of the piece (vector2)
			Vector2 centerCordinantes;
			centerCordinantes.x = centerPos.col;
			centerCordinantes.y = centerPos.row;

			if (CheckCollisionPointRec(centerCordinantes, Rectangle{ (float)(x), (float)(y), (float)cellSize, (float)cellSize }))
			{
				result.row = y ;
				result.col = x ;
			}
		}
	}

	return result;
}

