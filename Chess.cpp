#include "Chess.h"

Chess::Chess(int w, int h, int fps, int nRows, int nCols, int cSize, int lMargin, int tMargin, Color c1, Color c2)
: selectedPieceOriginalPos(0, 0)
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
	hoveredColor = Color{ 255, 204, 0,140 };// {0,255, 0,140 };


	// init widow + grid + piece
	Init();
}

Chess::~Chess()
{
	delete grid;
	// player1,2 are unique_ptr
}

Chess::infoCase::infoCase()
{
	empty = true;
	idCell = -1;
	idPiece = -1;
	pieceName = "-";
	caseName = "-";
	playerSide = -1; // player 1 by default
	//bool caseHovered;
	// positions are initialised by default constructor
}

void Chess::infoCase::setToEmpty()
{
	empty = true;
	idPiece = -1;
	pieceName = "-";
	playerSide = -1;
}

void Chess::Init()
{
	// Game window
	InitWindow(windowWidth, windowHeigh, "Raouf's 2D Chess Game");
	setGameFPS(gameFps);	// frames per second

	//grid
	grid = new Grid(numRows, numCols, cellSize , color1, color2);
	grid->SetMargins(leftMargin, topMargin);


	// init m_Bord values
	// should be before init pieces
	InitBoardInfo();


	//pirngt m_bord positions


	//init Pieces
	player1 = InitPlayersPieces(true); // player 1
	player2 = InitPlayersPieces(false); // player 2

	PrintBoardQuickInfo("Names");
	PrintBoardQuickInfo("playerSide");

	selectdPieceID = -1; // no piece is selected
	m_selectedPiecePossiblePositions = {}; // empty vector

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

		//get  possible positions
		
		
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

		// update m_Board for each piece released

		UpdateBoardInfo(flag_isPlayer1Turn);

		PrintBoardQuickInfo("Names");

		selectdPieceID = -1; // no selected piece

		flag_isAnyPieceSelected = false;

		m_selectedPiecePossiblePositions = {}; // reset to empty vector

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

	// draw possible pos for selected piece 
	// (no need to specify side)
	DrawPossiblePositions(hoveredColor);
}

//----------------------
// additionel functions 
//----------------------

void Chess::setGameFPS(int fps)
{
	gameFps = fps;
	SetTargetFPS(gameFps);
}


void Chess::DrawPlayerPieces(std::vector<std::unique_ptr<Piece>> const &player)
{
	for (const auto& e : player)
		e->Draw();
}


/* m_board 
	A8  B8  C8  D8  E8  F8  G8  H8
	A7  B7  C7  D7  E7  F7  G7  H7
	A6  B6  C6  D6  E6  F6  G6  H6
	A5  B5  C5  D5  E5  F5  G5  H5
	A4  B4  C4  D4  E4  F4  G4  H4
	A3  B3  C3  D3  E3  F3  G3  H3
	A2  B2  C2  D2  E2  F2  G2  H2
	A1  B1  C1  D1  E1  F1  G1  H1
*/
std::vector<std::unique_ptr<Piece>>  Chess::InitPlayersPieces(bool player1Side)
{
	std::vector<std::unique_ptr<Piece>> vectOfPieces;

	// pions
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));  //pion left
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));
		vectOfPieces.push_back(std::make_unique<Pion>(player1Side));  // right pion
	//autres pieces
		vectOfPieces.push_back(std::make_unique<Fou>(player1Side));  // left fou
		vectOfPieces.push_back(std::make_unique<Fou>(player1Side));  // right fou
		vectOfPieces.push_back(std::make_unique<Cavalier>(player1Side)); // left cavalier
		vectOfPieces.push_back(std::make_unique<Cavalier>(player1Side)); // right cavalier
		vectOfPieces.push_back(std::make_unique<Tour>(player1Side));  // left tour
		vectOfPieces.push_back(std::make_unique<Tour>(player1Side));  // right tour
		vectOfPieces.push_back(std::make_unique<Roi>(player1Side));   // roi
		vectOfPieces.push_back(std::make_unique<Dame>(player1Side));  // dame
		
	// set pieceImage size to cellSize
	for (auto& p : vectOfPieces)
		p->SetImageSize(cellSize);

	// set Pieces positions according to player side
	if (player1Side)
	{
		// initial cases for player 1 (from left pion to dame)
		std::string player1InitialCases[] = { "A2","B2","C2","D2","E2","F2","G2","H2","C1","F1","B1","G1","A1","H1","E1","D1" };

		SetPlayerInitialPositions(vectOfPieces, player1InitialCases);

		// update Board info for player 1
		UpdateBoardInfo(player1Side);
		
	}
	else
	{
		// initial cases for player 1 (from left pion to dame)
		std::string player2InitialCases[] = { "A7","B7","C7","D7","E7","F7","G7","H7","C8","F8","B8","G8","A8","H8","E8","D8" };

		SetPlayerInitialPositions(vectOfPieces, player2InitialCases);

		// update Board info for player 2
		UpdateBoardInfo(player1Side);
	}
	
	return vectOfPieces;
	
}

void Chess::SetPlayerInitialPositions(std::vector<std::unique_ptr<Piece>>& vectOfPieces, std::string caseNames[])
{
	for (int i = 0; i < 16; i++)
	{
		//set piece to corresponding position
		vectOfPieces[i]->SetPosition(GetPostionFromBoardCaseName(caseNames[i]));

		//set initial position for current piece
		vectOfPieces[i]->SetInitialPosition(vectOfPieces[i]->GetPosition());

		// get corresponndging board row & col
		int row, col;
		GetBoardRowColFromCaseName(caseNames[i], row, col);

		// update m_board corresponding case
		UpdateCellInfo(*vectOfPieces[i], row, col);
	}
}

void Chess::DrawPossiblePositions(Color color)
{
	// draw rectangle on existing possilbe positions
	if(!m_selectedPiecePossiblePositions.empty())
		for (auto p : m_selectedPiecePossiblePositions)
		{
			//DrawRectangle(p.col, p.row, cellSize, cellSize, color);

			// draw corresponding circles coordinates
			int xCircle = (int)(p.col + ((float)(cellSize / 2)));
			int yCircle = (int)(p.row + ((float)(cellSize / 2)));

			int thickness = 5;
			for (int i = 0; i < thickness; i++)
			{
				DrawCircleLines(xCircle, yCircle, (float)(cellSize / 2) - i, color);  // CenterX, CenterY, Radius, Color
			}
		}
}

std::vector<Position> Chess::GetPossiblePositionsOnBoard(Piece const & piece) const
{
	std::vector<Position> possiblePos;
	Position pos = piece.GetPosition();
	Position initialPos = piece.GetInitialPosition();

	int rowCell =0, colCell = 0;
	int step = flag_isPlayer1Turn ? -1 : 1;

	int caseInfo = 0;
	int idPiece = piece.getId();

	//get corresponding row and col on the board
	GetBoardRowColFromPiecePosition(pos, rowCell, colCell);

	switch (idPiece)
	{
	case 1:  //pion

		//one case forward
		if(m_Board[rowCell + step][colCell].empty)
			AddPossiblePossition(possiblePos, piece,  rowCell + step , colCell );

		// two cases (only initial pos)
		if (m_Board[rowCell + 2 * step][colCell].empty)
			if( piece.IsNeverMoved())
				AddPossiblePossition(possiblePos, piece, rowCell + 2*step, colCell);

		// capturable movements
		if(IsCapturableObstacle(rowCell + step, colCell + step))
			AddPossiblePossition(possiblePos, piece, rowCell + step , colCell + step);
		if (IsCapturableObstacle(rowCell + step, colCell - step))
			AddPossiblePossition(possiblePos, piece, rowCell + step , colCell - step);

		break;
	case 2: // fou
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell + i * step, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}
			
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell + i * step, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}
			
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell - i * step, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}
			
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell - i * step, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}
		break;
	case 3 : // cava

		AddPossiblePossition(possiblePos, piece, rowCell +  step, colCell + 2 * step);
		AddPossiblePossition(possiblePos, piece, rowCell +  step, colCell - 2 * step);
		AddPossiblePossition(possiblePos, piece, rowCell + 2 * step, colCell + step);
		AddPossiblePossition(possiblePos, piece, rowCell + 2 * step, colCell - step);

		AddPossiblePossition(possiblePos, piece, rowCell -  step, colCell - 2 * step);
		AddPossiblePossition(possiblePos, piece, rowCell -  step, colCell + 2 * step);
		AddPossiblePossition(possiblePos, piece, rowCell - 2 * step, colCell -  step);
		AddPossiblePossition(possiblePos, piece, rowCell - 2 * step, colCell +  step);

		break;
	case 4: // tour
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell + i * step, colCell);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}
			
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell - i * step, colCell);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}
			
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}
			
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}

		break;
	case 5: // dame
			
		//fou type
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell + i * step, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell + i * step, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell - i * step, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell - i * step, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}
		//tour type
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell + i * step, colCell);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell - i * step, colCell);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(possiblePos, piece, rowCell, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2)
				break;
		}
		break;
	case 6: // roi
		//+ type
		AddPossiblePossition(possiblePos, piece, rowCell +  step, colCell +  step);
		AddPossiblePossition(possiblePos, piece, rowCell +  step, colCell -  step);
		AddPossiblePossition(possiblePos, piece, rowCell -  step, colCell +  step);
		AddPossiblePossition(possiblePos, piece, rowCell -  step, colCell -  step);
		//x type
		AddPossiblePossition(possiblePos, piece, rowCell + step, colCell);
		AddPossiblePossition(possiblePos, piece, rowCell - step, colCell);
		AddPossiblePossition(possiblePos, piece, rowCell, colCell + step);
		AddPossiblePossition(possiblePos, piece, rowCell, colCell - step);
		break;

	}


	return possiblePos;
}


int Chess::AddPossiblePossition(std::vector<Position>& positions, Piece const& piece, const int& row, int const& col) const
{
	int caseInfo = 0;
	// 0 : obstacle present (team side) : cant move to this case
	// 1 : no obstacle : a possible empty position can move to it
	// 2 : obstacle present (enemy side) : can capture this piece and move to its case (the piece is not Roi).

	// not out of board
	// if not check or check mat
	if (row >= 0 && row < 8 && col >= 0 && col < 8)
	{
		// case is empty
		if (m_Board[row][col].empty)
		{
			positions.push_back(m_Board[row][col].cellPosition);
			caseInfo = 1;
		}
		// enemy side & not king & not pion (pion capture otherwise)
		else if (IsCapturableObstacle(row, col) )
		{
			positions.push_back(m_Board[row][col].cellPosition);
			caseInfo = 2;
		}
		// obstacle present (cant move)
		else
			caseInfo = 0;
	}

	return caseInfo;

}


bool Chess::IsCapturableObstacle(int const& row, int const& col) const
{
	if(! m_Board[row][col].empty)
		if (m_Board[row][col].playerSide != flag_isPlayer1Turn)
			if (m_Board[row][col].idPiece != 6) // not king (pas le roi) 
				return true;
	return false;
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
			// set selectedPieceOriginalPos
			if (!flag_isAnyPieceSelected)
				selectedPieceOriginalPos = player1[selectdPieceID]->GetPosition();

			//reset flag
			flag_isAnyPieceSelected = true;

			// get possible postions
			m_selectedPiecePossiblePositions = GetPossiblePositionsOnBoard(*player1[selectdPieceID]);

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
			// set selectedPieceOriginalPos
			if ( !flag_isAnyPieceSelected )
				selectedPieceOriginalPos = player2[selectdPieceID]->GetPosition();

			//reset flag
			flag_isAnyPieceSelected = true;

			// get possible postions
			m_selectedPiecePossiblePositions = GetPossiblePositionsOnBoard(*player2[selectdPieceID]);

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
		// correct draged Piece Position

		if (flag_isPlayer1Turn)
			CorrectPiecePosition(player1);
		else
			CorrectPiecePosition(player2);
		
	}	
}

void Chess::CorrectPiecePosition(std::vector<std::unique_ptr<Piece>>& player)
{
	// get center pos
	Position centerPos = player[selectdPieceID]->GetCenterOfPiecePosition();

	// get current pos
	Position currentPos = player[selectdPieceID]->GetPosition();

	// get corresponding board case position
	Position correspondingCase = GetCorrespondingBoardCase(centerPos);

	// calculate step
	Position step(0, 0);
	step.row = correspondingCase.row - currentPos.row;
	step.col = correspondingCase.col - currentPos.col;

	// move piece to the corresponding case
	MovePiece(*player[selectdPieceID], step);

	// check if piece is moved 
	bool colCondition = selectedPieceOriginalPos.col != player[selectdPieceID]->GetPosition().col;
	bool rowCondition = selectedPieceOriginalPos.row != player[selectdPieceID]->GetPosition().row;

	if (colCondition || rowCondition)
	{
		player[selectdPieceID]->SetNeverMoved(false);
		player[selectdPieceID]->SetLastPosition(selectedPieceOriginalPos);
	}
}

/*******************************************************************************
* @brief This function gets the selected piece to drag using the cursor.
* @param piece : a vector of pieces.
* @return : The index of the selected piece from the correspondig vector of pieces, 
* returns -1 if no piece is selected.
**********************************************************************************/
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

/*******************************************************************************
* @brief This function sets the center of the piece on the same positon as the cursor.
* @param piece : the selected piece to drag
* @param cursoPos : the current position of the cursor.
********************************************************************************/
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
	Position result = selectedPieceOriginalPos;
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
				// ceck if this new position is from Possible possitions 

				for(auto const &p : m_selectedPiecePossiblePositions)
					if (p.col == x && p.row == y)
					{
						result.row = y;
						result.col = x;
						
					}
			}
		}
	}

	return result;
}


//------------------------------
// m_Board methods
//------------------------------

Position Chess::GetCellPosition(int i, int j)
{
	// row = i * cellSize + topMargin
	// col = j * cellSize + leftMargin

	return Position( (i * cellSize) + topMargin ,  (j * cellSize) + leftMargin);
}

Position Chess::GetCellCentralPosition(int i, int j)
{
	Position cellPos = GetCellPosition( i, j);
	
	float halfCellSize = (float)(cellSize / 2);

	return Position(cellPos.row + halfCellSize , cellPos.col + halfCellSize );
}

Position Chess::GetPostionFromBoardCaseName(std::string const &caseName) const
{
	for( int i = 0 ; i < 8 ; i ++)
		for (int j = 0 ; j<8 ; j++)
			if(m_Board[i][j].caseName == caseName)
				return Position(m_Board[i][j].cellPosition.row  , m_Board[i][j].cellPosition.col);
	return Position(0,0);
}


void Chess::InitBoardInfo()
{
	int cpt = 1; // id cell on board

	for (int i = 0; i < 8; i++) // 8 7 6 5 4 3 2 1 
	{
		for (int j = 0; j < 8; j++) // A B C D E F G H
		{
			m_Board[i][j].caseName = GetCaseName(i, j);
			m_Board[i][j].empty = true;
			m_Board[i][j].idCell = cpt++;
			m_Board[i][j].idPiece = -1;
			m_Board[i][j].pieceName = "-"; // no piece
			m_Board[i][j].playerSide = -1;
			m_Board[i][j].cellPosition = GetCellPosition(i, j);
			m_Board[i][j].cellCentralPosition = GetCellCentralPosition(i, j);

			std::cout << m_Board[i][j].caseName << "  ";
		}

		std::cout << std::endl;
	}
}

std::string Chess::GetCaseName(int i, int j)
{

	if ((i < 0) || (i > 7) || (j > 7) || (j < 0))
		return "";

	std::string rowName = "", colName = "";

	// 8 7 6 5 4 3 2 1 
	rowName = (i == 0) ? "8" : 
		(i==1) ? "7" : 
		(i==2) ? "6" : 
		(i==3) ? "5" : 
		(i==4) ? "4" : 
		(i==5) ? "3" :
		(i==6) ? "2" : "1";

	// A B C D E F G H
	colName = (j == 0) ? "A" :
		(j == 1) ? "B" :
		(j == 2) ? "C" :
		(j == 3) ? "D" :
		(j == 4) ? "E" :
		(j == 5) ? "F" :
		(j == 6) ? "G" : "H";
	
	return colName + rowName;
}

void Chess::UpdateCellInfo(Piece const& piece  , int row , int col)
{
	// put cell info according to piece details 
	if (m_Board[row][col].cellPosition.row == piece.GetPosition().row && 
		m_Board[row][col].cellPosition.col == piece.GetPosition().col)
	{
		m_Board[row][col].empty = false;
		m_Board[row][col].idPiece = piece.getId();
		m_Board[row][col].pieceName = piece.GetName();
		m_Board[row][col].playerSide = piece.GetPlayerSide();
	}
		
}

void Chess::UpdateBoardInfo( bool player1Turn )
{
	// update only for the current playerX turn.

	// set empty board info (from the correct side)
	SetEmptyBoardInfo(player1Turn);

	// set new values to board
	SetNewValuesToBoardInfo(player1Turn);
}


void Chess::SetEmptyBoardInfo(bool player1Turn)
{
	// set board values to empty
	for (auto& const piece : (player1Turn ? player1 : player2))
	{
		for (int row = 0; row < 8; row++)
		{
			for (int col = 0; col < 8; col++)
			{
				if (m_Board[row][col].playerSide == player1Turn)
				{
					m_Board[row][col].setToEmpty();
				}
			}
		}
	}
}

void Chess::SetNewValuesToBoardInfo(bool player1Turn)
{
	for (auto& const piece : (player1Turn ? player1 : player2))
	{
		for (int row = 0; row < 8; row++)
		{
			for (int col = 0; col < 8; col++)
			{
				UpdateCellInfo(*piece, row, col);
			}
		}
	}
}

void Chess::GetBoardRowColFromCaseName(std::string caseName , int& row, int& col)
{
	for (int i = 0; i < 8; i++) 
	{
		for (int j = 0; j < 8; j++) 
		{
			if (m_Board[i][j].caseName == caseName)
			{
				row = i;
				col = j;
				break;
			}
		}
	}
}

void Chess::GetBoardRowColFromPiecePosition(Position const& pos, int& row, int& col) const
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_Board[i][j].cellPosition.row == selectedPieceOriginalPos.row &&
				m_Board[i][j].cellPosition.col == selectedPieceOriginalPos.col) // original pos (not current pos)
			{
				row = i;
				col = j;
				break;
			}
		}
	}
}

void Chess::PrintBoardQuickInfo(std::string infoType) const
{
	std::cout << std::endl;

	if (infoType == "Position")
	{
			std::cout << "Position" << std::endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << "(" << m_Board[i][j].cellPosition.row << "," << m_Board[i][j].cellPosition.col << ") ";
			}
			std::cout << std::endl;
		}
	}

	else if (infoType == "Names")
	{
		std::cout << "Names" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << std::setw(4) << m_Board[i][j].pieceName << " ";
			}
			std::cout << std::endl;
		}
	}

	else if (infoType == "empty")
	{
		std::cout << "empty" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << std::setw(4) << m_Board[i][j].empty << " ";
			}
			std::cout << std::endl;
		}
	}

	else if (infoType == "playerSide")
	{
		std::cout << "playerSide" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << std::setw(3) << m_Board[i][j].playerSide << " ";
			}
			std::cout << std::endl;
		}
	}

	std::cout << std::endl;
}
