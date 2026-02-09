#include "Chess.h"

Chess::Chess(int w, int h, int fps, int nRows, int nCols, int cSize, int lMargin, int tMargin)
	: selectedPieceOriginalPos(0, 0), selectedPieceCurrentPos(0, 0), m_promotionPosition(0, 0)
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

	// colors :
	// color1,color2,bground, hovered
	//-------------------------------
	m_defaultTheme = GetDefaultTheme();
	SetColorsFromTheme(m_defaultTheme);

	// init widow + grid + piece
	Init();
}

Chess::~Chess()
{
	delete grid;
	UnloadImage(m_screenshot);
	//UnloadSound(); all sounds
	CloseAudioDevice();
	// player1,2 are unique_ptr
}

Chess::infoCase::infoCase()
{
	empty = true;
	idCell = -1;
	idPiece = -1;
	pieceTeamIndex = -1;
	pieceName = "-";
	pieceEnName = "-";
	piecePGNName = "-";
	caseName = "-";
	playerSide = -1; // player 1 by default
	//bool caseHovered;
	// positions are initialised by default constructor
}

void Chess::infoCase::setToEmpty()
{
	empty = true;
	idPiece = -1;
	pieceTeamIndex = -1;
	pieceName = "-";
	pieceEnName = "-";
	piecePGNName = "-";
	playerSide = -1;
}

void Chess::Init()
{
	// Game window
	InitWindow(windowWidth, windowHeigh, "Raouf's 2D Chess Game");
	setGameFPS(gameFps);	// frames per second

	//grid
	grid = new Grid(numRows, numCols, cellSize, m_color1, m_color2);
	grid->SetMargins(leftMargin, topMargin);


	// text
	m_xText = leftMargin + 8 * cellSize + cellSize / 3;
	m_sizeText = 20;
	m_fontText = LoadFontEx("assets/fonts/courrierNew.ttf", m_sizeText, nullptr, 0);
	m_spacingText = 1;

	//output files
	m_gameHistoryFileName = "outputs/Last_game_history.txt";

	//sound
	InitAudioDevice();
	m_normalMoveSound = LoadSound("assets/sounds/movePiece2.wav");
	m_checkSound = LoadSound("assets/sounds/checkKing.wav");
	m_checkmateSound = LoadSound("assets/sounds/victory.wav");
	m_restartGameSound = LoadSound("assets/sounds/restartGame.wav");
	m_timeOverSound = LoadSound("assets/sounds/timeOver.wav");
	m_capturepieceSound = LoadSound("assets/sounds/pieceCapture2.wav");

	//time	
	m_tMaxPlayer1 = 600;// default seconds 
	m_tMaxPlayer2 = 600;


	//drawable 
	m_posNewGame = { (float)(8 * cellSize + leftMargin + 1.4f * cellSize), (float)(2.3f * cellSize + topMargin) };
	m_posChgeTheme = { (float)(8 * cellSize + leftMargin + 0.6f * cellSize), (float)(2.3f * cellSize + topMargin) };
	m_imgNewGame = std::make_unique<ImageObject>(m_posNewGame, "assets/images/buttons/restart8.png");
	m_imgChngeTheme = std::make_unique<ImageObject>(m_posChgeTheme, "assets/images/buttons/changeTheme1.png");


	//list of colors used
			//  theme 2
	Color	darkBrown = { 128, 43, 0 ,255 },
		lightBrown = { 225, 135, 64, 255 },
		beige = { 255, 204, 156,255 },

		// theme 3
		darkMagenta = { 153, 51, 51 , 255 },
		lightMagenta = { 255, 80, 80 ,255 },
		magenta = { 255, 204, 204 , 255 };

	//theme list
	m_themeList.push_back(GetDefaultTheme());
	m_themeList.push_back({ lightBrown , beige , m_hoveredColor , darkBrown }); // theme2
	m_themeList.push_back({ lightMagenta, magenta, m_hoveredColor, darkMagenta }); //theme3


	// reset game parameters
	StartNewGame();
}

void Chess::StartNewGame()
{
	// init m_Bord values
	// should be before init pieces
	InitBoardInfo();

	// window type
	m_currentWindowType = enWindow::SETTINGS;

	//settings
	m_settingsWindow = std::make_unique<Settings>();

	//clear all pieces
	player1.clear();
	player2.clear();

	//init Pieces (m_board is created)
	player1 = InitPlayersPieces(true); // player 1
	player2 = InitPlayersPieces(false); // player 2	

	// set winner
	m_winner = Winner::None;

	m_Initial_Board = m_board; // keep it in its initial state
	m_lastBoard = m_board;

	//player1.push_back(std::make_unique<Dame>(true));
	//player1[16]->SetPosition(m_board.at(4).at(4).cellPosition);
	//player1[16]->SetTeamIndex(16);

	PrintBoardQuickInfo("Names", m_board);
	PrintBoardQuickInfo("ENG Small Names", m_board);


	selectdPieceID = -1; // no piece is selected

	m_possibleMouvement = {}; //empty

	// moves history
	m_WhiteMove = "";
	m_BlackMove = "";
	m_turnMove = "";
	m_allMoves = {};
	m_moveIdx = 1;
	m_currentMove = {};

	// erase vectors
	std::vector<PossibleMouvement>().swap(m_possibleMouvement);

	//falgs
	flag_isPlayer1Turn = true;
	flag_leftMouseButtonPressed = false;
	flag_rightMouseButtonPressed = false;
	flag_leftMouseButtonDown = false;
	flag_leftMouseButtonReleased = false;
	flag_isAnyPieceSelected = false;
	flag_player1InCheck = false;
	flag_player2InCheck = false;
	flag_checkMate = false;
	flag_isPat = false;
	flag_timeOverPlayer1 = false;
	flag_timeOverPlayer2 = false;
	flag_GameOver = false;
	flag_isMovementAllowed = false;
	flag_isAnyPieceCaptured = false;
	flag_possibleMouvemntsAreCalculated = false;
	flag_CPU_Movement_ON = true; // automatic movement CPU
	flag_isCPU_turn = false;
	flag_isRoundFinished = false;
	flag_isNewGame = true;
	flag_isScreenDrawed = false;
	flag_isPromotion = false;
	flag_isPromotionDone = false;
	flag_isSoundON = true;

	// reset current move flags
	m_currentMoveFlags.ResetFlags();
}

void Chess::Input()
{
	if (m_currentWindowType == enWindow::GAME)
	{
		InputGameWindow();

	}
	else if (m_currentWindowType == enWindow::SETTINGS)
	{
		m_settingsWindow->Input();
	}

	else if (m_currentWindowType == enWindow::GAME_OVER)
	{
		m_gameOverWindow->Input();
	}

	else if (m_currentWindowType == enWindow::PROMOTION)
	{
		m_promotion->Input();
	}

}

void Chess::InputGameWindow()
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
	// set background color according to current window type (Game, Settings)
	UpdateBackgroundColor(m_currentWindowType);

	switch (m_currentWindowType) {
	case enWindow::GAME:
		UpdateGameWindow();
		break;
	case enWindow::SETTINGS:
		UpdateSettingsWindow();
		break;
	case enWindow::GAME_OVER:
		UpdateGameOverWindow();
		break;
	case enWindow::PROMOTION:
		UpdatePromotionWindow();
		break;

	}

	flag_isScreenDrawed = false;
}

void Chess::UpdateGameWindow()
{

	//----------------------------------
	// update tempo
	//----------------------------------
	UpdateTempo();

	//----------------------------------
	// update buttons
	//----------------------------------
	UpdateButtons();


	//----------------------------------
	// check if time is over
	//----------------------------------
	if (IsTimeOver())
	{
		PlaySound(m_timeOverSound);
		flag_GameOver = true;
	}


	//----------------------
	// Piece selected 
	//----------------------
	if (flag_leftMouseButtonPressed)
	{
		flag_leftMouseButtonPressed = false;
	}

	//----------------------
	// Drag piece
	//----------------------
	if (flag_leftMouseButtonDown || IsCPUTurn())
	{
		flag_leftMouseButtonDown = false;

		if (flag_isPromotion == false)
			DragPiece();
	}

	//----------------------
	// piece released
	//----------------------
	if (flag_leftMouseButtonReleased || IsCPUTurn())
	{
		flag_leftMouseButtonReleased = false;

		if (selectdPieceID != -1 && flag_isAnyPieceSelected)
		{
			ReleasePiece();

			// chek movement type (if captured)

			if (flag_isPlayer1Turn)
				flag_isAnyPieceCaptured = IsAnyPieceCaptured(*player1[selectdPieceID]);  //<<*******ToDo better without paramaetre (check player side inside the fct)
			else
				flag_isAnyPieceCaptured = IsAnyPieceCaptured(*player2[selectdPieceID]);

			if (flag_isSoundON)
				if (flag_isAnyPieceCaptured)
					PlaySound(m_capturepieceSound);


		}

		//----------------------------------
		// update m_board for each piece released (keep last state in m_lastBoard)
		//----------------------------------
		UpdateBoardInfo(flag_isPlayer1Turn);

		//----------------------------------
		// update PionPromotion
		//----------------------------------
		PionPromotion();


		// moves history
		//-----------------
		if (flag_isMovementAllowed)
		{
			UpdateMovesHistory(m_lastBoard);

		}

		//----------------------------------
		// save infos
		//----------------------------------
		SaveGameSteps();


		//----------------------------------
		// Change player side if turn is finished
		//----------------------------------
		if (IsTurnFinished())
		{
			ChangeTurn();
			flag_isNewGame = false;
		}


		//----------------------------------
		// is checkMate state ?
		//----------------------------------
		if (IsCheckmat())
		{
			flag_checkMate = true;
			flag_GameOver = true;

			if (flag_isSoundON)
			{
				PlaySound(m_checkmateSound);
				std::cout << RED_COUT << "CHECKMATE" << RESET_COUT << std::endl;
			}
		}

		//----------------------------------
		// is pat state ?
		//----------------------------------
		if (IsPat())
		{
			flag_isPat = true;
			flag_GameOver = true;
			std::cout << RED_COUT << "PAT !" << RESET_COUT << std::endl;
		}

		//----------------------------------
		// play check sound
		//----------------------------------
		if (IsCheck() && !flag_checkMate)
			if (flag_isSoundON)
				PlaySound(m_checkSound);


		// print / save infos
		//----------------------------------
		PrintBoardQuickInfo("ENG Small Names", m_board);
		PrintBoardQuickInfo("Names", m_board);
		PrintBoardQuickInfo("indexes", m_board);


		// reset values
		//----------------------------------

		if (flag_isPromotion == false)
		{
			selectdPieceID = -1; // no selected piece

			flag_isAnyPieceSelected = false;

			// reset current move flags
			m_currentMoveFlags.ResetFlags();
		}

		// erase vectors
		m_possibleMouvement = {}; // reset to empty vector	
		std::vector<PossibleMouvement>().swap(m_possibleMouvement);
	}

	if (flag_GameOver) 
	{
		OpenGameOverScreen();
	}

	//----------------------
	// change player side  (after each finished move)
	//----------------------
	if (flag_rightMouseButtonPressed)
	{
		flag_rightMouseButtonPressed = false;

		// change player side

		ChangeTurn();  //<<****ToDo to be remove at the end (only for tests)

	}

}

void Chess::UpdateSettingsWindow()
{

	m_settingsWindow->Update();

	// check if selection is done 
	//------------------------------
	if (m_settingsWindow->IsSelectionDone())
	{
		// load settings from selection
		//-----------------------------
		LoadSettings();

		//play sound
		if(flag_isSoundON)
			PlaySound(m_restartGameSound);

		// back to game window
		//---------------------
		m_currentWindowType = enWindow::GAME;

		m_t1 = time(NULL); // start time computing //curent time
	}
}

void Chess::UpdateGameOverWindow()
{
	m_gameOverWindow->Update();

	// check if selection is done 
	//------------------------------
	if (m_gameOverWindow->IsRestartGameRequested())
	{
		// startNewGame
		//-----------------------------
		StartNewGame();

	}
}

void Chess::UpdatePromotionWindow()
{

	m_promotion->Update();

	if (m_promotion->IsPromotionDone())
	{
		// get selection type from rect (get id piece)
		PieceName promotionPiece = m_promotion->GetSelectedPiece();

		//--------Replace pawn by the selection ------
		std::unique_ptr<Piece> p;

		if (promotionPiece == PieceName::Queen)
			p = std::make_unique<Dame>(flag_isPlayer1Turn);

		else if (promotionPiece == PieceName::Rook)
			p = std::make_unique<Tour>(flag_isPlayer1Turn);

		else if (promotionPiece == PieceName::Bishop)
			p = std::make_unique<Fou>(flag_isPlayer1Turn);

		else if (promotionPiece == PieceName::Knight)
			p = std::make_unique<Cavalier>(flag_isPlayer1Turn);
		//---------------------------------------------

		if (flag_isPlayer1Turn)
		{
			player1[selectdPieceID] = std::move(p);

			player1[selectdPieceID]->SetPosition(m_promotionPosition);
			player1[selectdPieceID]->SetImageSize(cellSize);
			player1[selectdPieceID]->SetTeamIndex(selectdPieceID);

			ChessCase cellPos = PosToCase(m_promotionPosition);
			UpdateCellInfo(*player1[selectdPieceID], cellPos.row, cellPos.col, m_board);
		}
		else
		{
			player2[selectdPieceID] = std::move(p);

			player2[selectdPieceID]->SetPosition(m_promotionPosition);
			player2[selectdPieceID]->SetImageSize(cellSize);
			player2[selectdPieceID]->SetTeamIndex(selectdPieceID);

			ChessCase cellPos = PosToCase(m_promotionPosition);
			UpdateCellInfo(*player2[selectdPieceID], cellPos.row, cellPos.col, m_board);
		}

		m_currentWindowType = enWindow::GAME;


		m_promotionPieceName = GetPGNNameFromPieceName(promotionPiece);

		flag_isPromotionDone = true;

		flag_isPromotion = false;

		flag_isMovementAllowed = true;

		selectdPieceID = -1;
	}
}

void Chess::Draw()
{
	switch (m_currentWindowType)
	{
	case enWindow::GAME:
		DrawGameWindow();
		break;
	case enWindow::SETTINGS:
		m_settingsWindow->Draw();
		break;
	case enWindow::GAME_OVER:
		m_gameOverWindow->Draw();
		break;
	case enWindow::PROMOTION:
		DrawGameWindow();
		m_promotion->Draw();
		break;
	}

	flag_isScreenDrawed = true;
}

void Chess::DrawGameWindow()
{
	// grid
	grid->Draw();

	// draw possible pos for selected piece 
	DrawPossiblePositions(m_hoveredColor);

	// players (all pieces)
	DrawPlayerPieces(player1);
	DrawPlayerPieces(player2);

	//draw text
	DrawLateralTexts();
}

//----------------------
// additionel functions 
//----------------------

bool Chess::IsScreenDrawed()
{
	return flag_isScreenDrawed;
}

void Chess::setGameFPS(int fps)
{
	gameFps = fps;
	SetTargetFPS(gameFps);
}


void Chess::DrawPlayerPieces(std::vector<std::unique_ptr<Piece>> const& player)
{
	for (const auto& e : player)
		e->Draw();
}

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

	// set pieceImage size to cellSize && teamIndex
	int idx = 0;
	for (auto& p : vectOfPieces)
	{
		// set pieceImage size to cellSize
		p->SetImageSize(cellSize);

		// set teamIndex
		p->SetTeamIndex(idx);
		idx++;
	}


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

		//set last position
		vectOfPieces[i]->SetLastPosition(vectOfPieces[i]->GetPosition());

		//set never moved
		vectOfPieces[i]->SetNeverMoved(true);

		// get corresponndging board row & col
		int row, col;
		GetBoardRowColFromCaseName(caseNames[i], row, col);

		// update m_board corresponding case
		UpdateCellInfo(*vectOfPieces[i], row, col, m_board);
	}
}

void Chess::DrawPossiblePositions(Color color)
{

	// draw it once (when we click to select the piece)
		// draw rectangle on existing possilbe positions
	if (!m_possibleMouvement.empty())
	{
		for (auto p : m_possibleMouvement)
		{
			if (p.movementType == 2)
			{
				DrawRectangle(p.possiblePosition.col, p.possiblePosition.row, cellSize, cellSize, color);
			}
			if (p.movementType == -1) // check posi
			{
				Color lightRed = { 205, 0 , 0, 150 };
				DrawRectangle(p.possiblePosition.col, p.possiblePosition.row, cellSize, cellSize, lightRed);
			}
			else
			{
				if (p.canMove == true) // dont draw possible pos for team obstacle cases
				{
					// draw corresponding circles coordinates
					int xCircle = (int)(p.possiblePosition.col + ((float)(cellSize / 2)));
					int yCircle = (int)(p.possiblePosition.row + ((float)(cellSize / 2)));

					int thickness = 25;
					for (int i = 0; i < thickness; i += 2)
					{
						DrawCircleLines(xCircle, yCircle, (float)(cellSize / 4) - i, color);  // CenterX, CenterY, Radius, Color
					}
				}
			}

		}

	}

}

bool Chess::GetPossiblePositionsOnBoard(Piece const& piece, std::vector<PossibleMouvement>& PossibleMvt, enActionType const& ActionType)
{

	Position pos = piece.GetPosition();
	Position initialPos = piece.GetInitialPosition();

	int rowCell = 0, colCell = 0;

	int step = 0;

	if (ActionType == enActionType::MOUVEMENT)
		step = flag_isPlayer1Turn ? -1 : 1;
	else
		step = piece.GetPlayerSide() ? -1 : 1;

	int caseInfo = 0;
	int idPiece = piece.getId();

	//get corresponding row and col on the board
	if (ActionType == enActionType::MOUVEMENT)
		GetBoardRowColFromPiecePosition(pos, rowCell, colCell, true); // use orignal positions for each piece (because of the drag)
	else
		GetBoardRowColFromPiecePosition(pos, rowCell, colCell, false);

	switch (idPiece)
	{
	case 1:  //pion

		//one case forward
		if (m_board.at(rowCell + step).at(colCell).empty)
			AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + step, colCell);

		// two cases (only initial pos)
		if (m_board.at(rowCell + 2 * step).at(colCell).empty)
			if (piece.IsNeverMoved())
				AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + 2 * step, colCell);

		// capturable movements
		if (IsEnemyObstacle(rowCell + step, colCell + step))
			AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + step, colCell + step);
		if (IsEnemyObstacle(rowCell + step, colCell - step))
			AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + step, colCell - step);

		break;
	case 2: // fou
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + i * step, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + i * step, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - i * step, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - i * step, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}
		break;
	case 3: // cava

		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + step, colCell + 2 * step);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + step, colCell - 2 * step);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + 2 * step, colCell + step);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + 2 * step, colCell - step);

		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - step, colCell - 2 * step);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - step, colCell + 2 * step);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - 2 * step, colCell - step);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - 2 * step, colCell + step);

		break;
	case 4: // tour
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + i * step, colCell);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - i * step, colCell);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		break;
	case 5: // dame

		//fou type
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + i * step, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + i * step, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - i * step, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - i * step, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}
		//tour type
		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + i * step, colCell);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - i * step, colCell);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell, colCell + i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			caseInfo = AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell, colCell - i * step);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}
		break;
	case 6: // roi
		//+ type
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + step, colCell + step);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + step, colCell - step);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - step, colCell + step);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - step, colCell - step);
		//x type
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell + step, colCell);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell - step, colCell);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell, colCell + step);
		AddPossiblePossition(ActionType, piece, PossibleMvt, rowCell, colCell - step);


		break;

	}

	return true; 
}

bool Chess::GenerateLegalMoves(ChessCase const& cellCoords,
	Board const& board,
	std::vector<PossibleMouvement>& PossibleMvt,
	enActionType const& ActionType)
{

	// get piece info from cell
	//---------------------
	int rowCell = cellCoords.row, colCell = cellCoords.col; // piece coordinates

	Position pos = board.at(rowCell).at(colCell).cellPosition;
	int idPiece = board.at(rowCell).at(colCell).idPiece;
	bool side = board.at(rowCell).at(colCell).playerSide;  // 1 : player1, 0: player2
	int pieceTeamID = board.at(rowCell).at(colCell).pieceTeamIndex;
	ChessCase attacker = ChessCase(rowCell, colCell);

	int step = side ? -1 : 1,
		caseInfo = 0; // to check return values


	// to use as a paramettre (to check idx also)
	int r = 0, c = 0;


	switch (idPiece)
	{
	case 1:  //pion

		//one case forward
		r = rowCell + step;
		c = colCell;
		if (IsValidIdx(r, c))
			if (board.at(rowCell + step).at(colCell).empty)
				AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		// two cases (only initial pos)
		r = rowCell + 2 * step;
		c = colCell;
		if (IsValidIdx(r, c) && IsValidIdx(r - step, c))
			if ((board.at(r).at(c).empty) && (board.at(r - step).at(c).empty))
				if (Compare2Cells(board.at(rowCell).at(colCell), m_Initial_Board.at(rowCell).at(colCell))) // piece NeverMoved : compare current cell with initial one
					AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		// capturable movements
		r = rowCell + step;
		c = colCell + step;
		if (IsValidIdx(r, c))
			if (IsEnemyObstacle_2(r, c, attacker, board))
				AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell + step;
		c = colCell - step;
		if (IsValidIdx(r, c))
			if (IsEnemyObstacle_2(r, c, attacker, board))
				AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		break;
	case 2: // fou
		for (int i = 1; i < 8; i++)
		{
			r = rowCell + i * step;
			c = colCell + i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell + i * step;
			c = colCell - i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell - i * step;
			c = colCell + i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell - i * step;
			c = colCell - i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}
		break;
	case 3: // cava

		r = rowCell + step;
		c = colCell + 2 * step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell + step;
		c = colCell - 2 * step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell + 2 * step;
		c = colCell + step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell + 2 * step;
		c = colCell - step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell - step;
		c = colCell - 2 * step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell - step;
		c = colCell + 2 * step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell - 2 * step;
		c = colCell - step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell - 2 * step;
		c = colCell + step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		break;
	case 4: // tour
		for (int i = 1; i < 8; i++)
		{
			r = rowCell + i * step;
			c = colCell;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell - i * step;
			c = colCell;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, colCell, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell;
			c = colCell + i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell;
			c = colCell - i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		break;
	case 5: // dame

		//fou type
		for (int i = 1; i < 8; i++)
		{
			r = rowCell + i * step;
			c = colCell + i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell + i * step;
			c = colCell - i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell - i * step;
			c = colCell + i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell - i * step;
			c = colCell - i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}
		//tour type
		for (int i = 1; i < 8; i++)
		{
			r = rowCell + i * step;
			c = colCell;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell - i * step;
			c = colCell;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell;
			c = colCell + i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			r = rowCell;
			c = colCell - i * step;
			if (IsValidIdx(r, c))
				caseInfo = AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			if (caseInfo == 0 || caseInfo == 2 || caseInfo == -1)
				break;
		}
		break;
	case 6: // roi
		//+ type
		r = rowCell + step;
		c = colCell + step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell + step;
		c = colCell - step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell - step;
		c = colCell + step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell - step;
		c = colCell - step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		//x type
		r = rowCell + step;
		c = colCell;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell - step;
		c = colCell;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell;
		c = colCell + step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		r = rowCell;
		c = colCell - step;
		if (IsValidIdx(r, c))
			AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);

		// king castle    
		if (ActionType == enActionType::MOUVEMENT)
		{
			if (IsKingCastelingAllowed(cellCoords, board))
			{
				r = rowCell;
				c = colCell + 2;
				if (IsValidIdx(r, c))
					AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			}

			// queen castle     
			if (IsQueenCastelingAllowed(cellCoords, board))
			{
				r = rowCell;
				c = colCell - 2;
				if (IsValidIdx(r, c))
					AddPossiblePossitionFromBoard(ActionType, board, PossibleMvt, r, c, attacker);
			}
		}


		break;

	}

	return true; 
}

int Chess::GetPossibleMovementsID(std::vector<PossibleMouvement> const& possibleMoves, ChessCase const& toCell)
{
	int result = -1; // invalid value

	Position pos = CaseToPos(toCell);

	if (possibleMoves.empty() == false)
	{
		for (int i = 0; i < possibleMoves.size(); i++)		
			if (possibleMoves[i].possiblePosition.col == pos.col && possibleMoves[i].possiblePosition.row == pos.row)
				return i; // index found
	}

	return result;
}

bool Chess::IsKingCastelingAllowed(ChessCase const& cellCoords, Board const& board)
{

	/* === king castle ===
	* 2 king never moved && tour 13 never moved
	* 3 empty cells between king & tour && no attackers on this cases
	*/

	// get piece info from cell
	//---------------------
	int rowCell = cellCoords.row, colCell = cellCoords.col; // piece coordinates

	Position pos = board.at(rowCell).at(colCell).cellPosition;
	int idPiece = board.at(rowCell).at(colCell).idPiece;
	bool side = board.at(rowCell).at(colCell).playerSide;  // 1 : player1, 0: player2
	int pieceTeamID = board.at(rowCell).at(colCell).pieceTeamIndex;
	ChessCase attacker = ChessCase(rowCell, colCell);


	// castle needed flags
	bool	isKingSelected = (idPiece == 6);
	bool	isKingNeverMoved = false,
			isRookKingSideNeverMoved = false;
	bool	isEmptyCellsKingSide = false;
	bool	noAttackersInEmptyCases = false;
	bool	isRookKingCaptured = false;


	if (isKingSelected)
	{
		if (side)  // player1  ( isRookKingSideNeverMoved && isEmptyCellsKingSide && noAttackersInEmptyCases);
		{
			isKingNeverMoved = player1[pieceTeamID]->IsNeverMoved();
			isRookKingCaptured = player1[13]->IsCaptured();

			if (isKingNeverMoved && !isRookKingCaptured)
			{
				// small rook (king side)
				isEmptyCellsKingSide = (board.at(7).at(5).empty == true && board.at(7).at(6).empty == true); // Bf1 & Ng1 are empty
				isRookKingSideNeverMoved = player1[13]->IsNeverMoved();

				// is emmpty cases attacked ?
				if (isRookKingSideNeverMoved && isEmptyCellsKingSide)
				{
					//noAttackersInEmptyCases = IsNoAttackersOnEmptyCastle(CastleSide::KingSide, enPlayerNum::PLAYER1);
					//if(noAttackersInEmptyCases)
					return true;
				}
			}
		}
		else  //player2
		{
			isKingNeverMoved = player2[pieceTeamID]->IsNeverMoved();
			isRookKingCaptured = player2[13]->IsCaptured();

			if (isKingNeverMoved && !isRookKingCaptured)
			{
				// small rook (king side)
				isEmptyCellsKingSide = (board.at(0).at(5).empty == true && board.at(0).at(6).empty == true); // Bf8 & Ng8 are empty
				isRookKingSideNeverMoved = player2[13]->IsNeverMoved();

				if (isRookKingSideNeverMoved && isEmptyCellsKingSide)
				{
					// is emmpty cases attacked ?
					//noAttackersInEmptyCases = IsNoAttackersOnEmptyCastle(CastleSide::KingSide, enPlayerNum::PLAYER2);
					//if (noAttackersInEmptyCases)
					return true;
				}
			}

		}
	}

	return false;

	// true if : (isKingSelected && isKingNeverMoved && isRookKingSideNeverMoved && isEmptyCellsKingSide && noAttackersInEmptyCases);
}

bool Chess::IsQueenCastelingAllowed(ChessCase const& cellCoords, Board const& board)
{
	/* ==== queen castle ====
	* 2 king never moved && Rook never moved
	* 3 empty cells between king & tour && no attackers on this cases 	*/

	// get piece info from cell
	//---------------------
	int rowCell = cellCoords.row, colCell = cellCoords.col; // piece coordinates

	Position pos = board.at(rowCell).at(colCell).cellPosition;
	int idPiece = board.at(rowCell).at(colCell).idPiece;
	bool side = board.at(rowCell).at(colCell).playerSide;  // 1 : player1, 0: player2
	int pieceTeamID = board.at(rowCell).at(colCell).pieceTeamIndex;
	ChessCase attacker = ChessCase(rowCell, colCell);

	// castle needed flags
	bool	isKingSelected = (idPiece == 6);
	bool	isKingNeverMoved = false,
		isRookQueenSideNeverMoved = false;
	bool	isEmptyCellsQueenSide = false;
	bool	noAttackersInEmptyCases = false;
	bool	isRookQueenCaptured = false;

	if (isKingSelected)
	{

		if (side)  // player1
		{
			isKingNeverMoved = player1[pieceTeamID]->IsNeverMoved();
			isRookQueenCaptured = player1[12]->IsCaptured();

			if (isKingNeverMoved && !isRookQueenCaptured)
			{
				// big rook (queen side)
				isEmptyCellsQueenSide = (board.at(7).at(1).empty == true && board.at(7).at(2).empty == true && board.at(7).at(3).empty == true); // Nb1 & Bc1 & Qd1 are empty
				isRookQueenSideNeverMoved = player1[12]->IsNeverMoved();

				if (isEmptyCellsQueenSide && isRookQueenSideNeverMoved)
				{
					// is emmpty cases attacked ?
					//noAttackersInEmptyCases = IsNoAttackersOnEmptyCastle(CastleSide::QueenSide, enPlayerNum::PLAYER1);
					//if(noAttackersInEmptyCases)
					return true;
				}
			}
		}
		else   // player2
		{
			isKingNeverMoved = player2[pieceTeamID]->IsNeverMoved();
			isRookQueenCaptured = player2[12]->IsCaptured();

			if (isKingNeverMoved && !isRookQueenCaptured)
			{
				// big rook (queen side)
				isEmptyCellsQueenSide = (board.at(0).at(1).empty == true && board.at(0).at(2).empty == true && board.at(0).at(3).empty == true); // Nb8 & Bc8 & Qd8 are empty
				isRookQueenSideNeverMoved = player2[12]->IsNeverMoved();

				// is emmpty cases attacked ?
				if (isEmptyCellsQueenSide && isRookQueenSideNeverMoved)
				{
					//noAttackersInEmptyCases = IsNoAttackersOnEmptyCastle(CastleSide::QueenSide, enPlayerNum::PLAYER2);

					//if (noAttackersInEmptyCases)
					return true;
				}
			}
		}
	}

	return false;

	// true if : (isKingSelected && isKingNeverMoved && isRookQueenSideNeverMoved && isEmptyCellsQueenSide && noAttackersInEmptyCases);
}

bool Chess::IsNoAttackersOnEmptyCastle(CastleSide castleSide, enPlayerNum const& playerNum)
{
	ChessCase	KnightKing,
		BishopKing,
		KnightQueen,
		BishopQueen,
		Queen;

	// define cell positions values
	//-----------------------------

	if (playerNum == enPlayerNum::PLAYER1)
	{
		KnightKing = GetCellCoordsFromCaseName("G1");
		BishopKing = GetCellCoordsFromCaseName("F1");
		KnightQueen = GetCellCoordsFromCaseName("B1");
		BishopQueen = GetCellCoordsFromCaseName("C1");
		Queen = GetCellCoordsFromCaseName("D1");

	}
	else
	{
		KnightKing = GetCellCoordsFromCaseName("G8");
		BishopKing = GetCellCoordsFromCaseName("F8");
		KnightQueen = GetCellCoordsFromCaseName("B8");
		BishopQueen = GetCellCoordsFromCaseName("C8");
		Queen = GetCellCoordsFromCaseName("D8");
	}


	// check if empty cells are attacked
	//----------------------------------

	if (castleSide == CastleSide::KingSide)
	{
		if (IsCellAttacked(KnightKing, playerNum))
			return false;
		if (IsCellAttacked(BishopKing, playerNum))
			return false;
	}
	else
	{
		if (IsCellAttacked(KnightQueen, playerNum))
			return false;
		if (IsCellAttacked(BishopQueen, playerNum))
			return false;
		if (IsCellAttacked(Queen, playerNum))
			return false;
	}

	return true;
}

bool Chess::IsCellAttacked(ChessCase const& cellCoords, enPlayerNum const& playerNum) 
{
	// get all possible positions for the opposite player pieces
	// if any possible possision == cellCoords  return true

	Board board = m_board;

	// local variable to get possible mouvement for each piece.
	std::vector<PossibleMouvement> possMvt = {};

	// player 1 turn = > we should check player2 pieces if they can attack this case
	bool oppositeSide = (playerNum == enPlayerNum::PLAYER1) ? false : true;

	for (auto const& row : board)
	{
		for (auto const& cell : row)
		{
			if (cell.playerSide == oppositeSide) // opposite player 
			{
				possMvt = {};

				ChessCase coords = cell.cellCoordinates;

				if (IsValidIdx(coords.row, coords.col))
				{
					GenerateLegalMoves(coords, board, possMvt, enActionType::MOUVEMENT);

					for (auto const& mvt : possMvt)
						if (IsSameCoordinates(mvt.possibleCase, cellCoords)) // <*****todo check & test results
						{
							return true;
						}
				}
				else
				{
					std::cerr << "Index invalide : " << coords.row << ", " << coords.col << '\n';
					std::abort();
				}
			}
		}
	}
	return false;

}

bool Chess::IsAnyPieceCaptured(Piece const& piece) //<<**********toDo to review the algo (after changing to struct)
{
	// compare piece position (after release) with possible position
	// and get index of that possible pos => then get movement type
	// if mvt type is captured move the captured piece outside of bord
	// return false : if no possible pos is selected or no piece is captured
	bool captured = false;

	float static	capturedPColForPlayer1 = leftMargin + 8 * cellSize + cellSize / 3,//+ cellSize/3, 
		capturedPColForPlayer2 = leftMargin + 8 * cellSize + cellSize / 3,//+ cellSize/3,
		capturedPRowForPlayer1 = windowHeigh - (1.5 * cellSize),
		capturedPRowForPlayer2 = windowHeigh - (3 * cellSize);
	int static cpt1 = 0, cpt2 = 0;


	// reset static values for new game cases
	if (IsNewGame())
	{
		cpt1 = 0;
		cpt2 = 0;

		capturedPColForPlayer1 = leftMargin + 8 * cellSize + cellSize / 3,//+ cellSize/3, 
			capturedPColForPlayer2 = leftMargin + 8 * cellSize + cellSize / 3,//+ cellSize/3,
			capturedPRowForPlayer1 = windowHeigh - (1.5 * cellSize),
			capturedPRowForPlayer2 = windowHeigh - (3 * cellSize);
	}

	for (auto const& p : m_possibleMouvement)
	{
		if ((piece.GetPosition().col == p.possiblePosition.col) && (piece.GetPosition().row == p.possiblePosition.row))
		{
			// move captured piece if any

			int row = 0, col = 0;

			GetBoardRowColFromPiecePosition(piece.GetPosition(), row, col, false);

			if (p.movementType == 2) // captured mvt
				// get corresponding piece info form cell case

				if (flag_isPlayer1Turn)
				{

					if (m_board.at(row).at(col).playerSide == !flag_isPlayer1Turn)
					{
						// capturePiece <<********ToDo  : creat a sepearte function for cpature
						player2[m_board.at(row).at(col).pieceTeamIndex]->SetImageSize(35);
						player2[m_board.at(row).at(col).pieceTeamIndex]->SetPosition(Position(capturedPRowForPlayer1, capturedPColForPlayer1));

						// set piece to captured from player2 vector
						player2[m_board.at(row).at(col).pieceTeamIndex]->SetCaptured(true);
						std::cout << "piece captured !" << std::endl;

						capturedPColForPlayer1 += 20;
						cpt1++;

						if (cpt1 == 6)
						{
							cpt1 = 0;
							capturedPColForPlayer1 = leftMargin + 8 * cellSize + cellSize / 3;
							capturedPRowForPlayer1 += 30;
						}

						//<<******To Do : add a break after captured movement (or just return true)
						captured = true;
						break;
					}
				}
				else  // player2 turn
				{
					if (m_board.at(row).at(col).playerSide == !flag_isPlayer1Turn)
					{
						// capturePiece <<********ToDo  : creat a sepearte function for cpature
						player1[m_board.at(row).at(col).pieceTeamIndex]->SetImageSize(35);
						player1[m_board.at(row).at(col).pieceTeamIndex]->SetPosition(Position(capturedPRowForPlayer2, capturedPColForPlayer2));


						// set piece to captured 
						player1[m_board.at(row).at(col).pieceTeamIndex]->SetCaptured(true);
						std::cout << "piece captured !" << std::endl;

						capturedPColForPlayer2 += 20;
						cpt2++;

						if (cpt2 == 6)
						{
							cpt2 = 0;
							capturedPRowForPlayer2 += 30;
							capturedPColForPlayer2 = leftMargin + 8 * cellSize + cellSize / 3;
						}

						//<<******To Do : add a break after captured movement (or just return true)
						captured = true;
						break;
					}
				}
		}

	}

	return captured;
}


bool Chess::IsPlayerInCheckPosition(Board const& board, enPlayerNum const& currentPlayerNum)
{


	// local variable to get possible mouvement for each piece.
	std::vector<PossibleMouvement> possMvt = {};

	if (currentPlayerNum == enPlayerNum::PLAYER1)
	{
		// player 1 turn = > we should check player2 pieces if they present a check position for player1
		//for (auto const& piece : player2) // for each piece
		for (auto const& row : board)
		{
			for (auto const& cell : row)
			{
				//if (!piece->IsCaptured() && piece->getId() != 6) // piece not captured	&& not king (king cant do check)
				if (cell.playerSide == 0) // player 2
				{
					//std::cout << "current piece : " << piece->GetName() << " id: " << piece->GetTeamIndex() << std::endl;
					//if (piece->GetTeamIndex() == 5)
						//std::cout << GREEN << "piece name : " << piece->GetName() << ",captured ? :" << piece->IsCaptured() << RESET << std::endl;
					// empty the vector for each piece.
					possMvt = {};

					ChessCase coords = cell.cellCoordinates;

					//GetPossiblePositionsOnBoard(*piece, possMvt, enActionType::CHECK_CASES);
					if (IsValidIdx(coords.row, coords.col))
					{
						GenerateLegalMoves(coords, board, possMvt, enActionType::CHECK_CASES);

						if (IsAnyCheckPossitionExists(possMvt))
						{
							//print
							//ChessCase cell = PosToCase(piece->GetInitialPosition());
							stPiece infoPiece = GetPieceFromBoardCell(cell);

							//print info

							std::cout << "(==========================)" << std::endl;
							std::cout << BLUE_COUT << "	==> check : " << RESET_COUT << player2[infoPiece.pieceTeamIdx]->GetName() << " id: " << player2[infoPiece.pieceTeamIdx]->GetTeamIndex() << "  pos: " << coords.col << "," << coords.row << std::endl;
							//print
							std::cout << "	CHECK CASE : present for player 1 " << std::endl;
							std::cout << "(==========================)" << RESET_COUT << std::endl;


							//flag_player1InCheck = true;
							return true;
						}
					}
					else
					{
						std::cerr << "Index invalide : " << coords.row << ", " << coords.col << '\n';
						std::abort();
					}
				}
			}
		}
		return false;
	}
	else
	{
		// player 2 turn = > we should check player1 pieces if they present a check position for player2
		//for (auto const& piece : player1) // for each piece
		for (auto const& row : board)
		{
			for (auto const& cell : row)
			{

				//if (!piece->IsCaptured() && piece->getId() != 6) // piece not captured	&& not king (king cant do check)
				if (cell.playerSide == 1) //&& cell.idPiece != 6) // player1
				{
					//std::cout << "current piece : " << piece->GetName() << " id: " << piece->GetTeamIndex() << std::endl;

					// empty the vector for each piece.
					possMvt = {};

					ChessCase coords = cell.cellCoordinates;

					//GetPossiblePositionsOnBoard(*piece, possMvt, enActionType::CHECK_CASES);
					if (IsValidIdx(coords.row, coords.col))
					{
						GenerateLegalMoves(coords, board, possMvt, enActionType::CHECK_CASES);

						if (IsAnyCheckPossitionExists(possMvt))
						{
							//print
							//ChessCase cell = PosToCase(piece->GetInitialPosition());

							stPiece infoPiece = GetPieceFromBoardCell(cell);


							//print infos
							/*
							std::cout << "(==========================)" << std::endl;
							std::cout << BLUE_COUT << "	==> check : " << RESET << player1[infoPiece.pieceTeamIdx]->GetName() << " id: " << player1[infoPiece.pieceTeamIdx]->GetTeamIndex() << "  pos: " << coords.col << "," << coords.row << std::endl;
							//print
							std::cout << "	CHECK CASE : present for player 2 " << std::endl;
							std::cout << "(==========================)" << std::endl;
							*/

							//flag_player2InCheck = true;
							return true;
						}
					}
					else
					{
						std::cerr << "Index invalide : " << coords.row << ", " << coords.col << '\n';
						std::abort();
					}
				}
			}

		}
		return false;

	}

}


bool Chess::IsTurnFinished()
{
	if ((flag_isMovementAllowed || flag_isAnyPieceCaptured) && (flag_isPromotion == false))
		return true;
	return false;
}

Color Chess::GetBackgroundColor()
{
	return m_backgroundColor;
}

void Chess::UpdateBackgroundColor(enWindow const& window)
{
	if (window == enWindow::GAME)
	{
		switch (m_currentTheme)
		{
		case Theme::Blue:
			m_backgroundColor = m_themeList[0].backgroundColor;
			break;
		case Theme::Brown:
			m_backgroundColor = m_themeList[1].backgroundColor;
			break;
		case Theme::Magenta:
			m_backgroundColor = m_themeList[2].backgroundColor;
			break;
		}
	}
	else if (window == enWindow::SETTINGS)
	{
		m_backgroundColor = m_themeList[0].backgroundColor;
	}
}


ChessCase Chess::PosToCase(Position pos)
{
	// row = i * cellSize + topMargin
	// col = j * cellSize + leftMargin

	int i, j;
	i = int((pos.row - topMargin) / cellSize);
	j = int((pos.col - leftMargin) / cellSize);
	ChessCase CCase(i, j);
	return CCase;
}

Position Chess::CaseToPos(ChessCase CCase)
{
	// row = i * cellSize + topMargin
	// col = j * cellSize + leftMargin

	int row, col;
	row = CCase.row * cellSize + topMargin;
	col = CCase.col * cellSize + leftMargin;
	Position pos(row, col);
	return pos;
}



int Chess::AddPossiblePossition(enActionType const& ActionType,
	Piece const& piece,
	std::vector<PossibleMouvement>& PossibleMvt,
	int const& row,
	int const& col)
{
	int movementType = 0;
	//-1 : obstacle present (enemy side but Roi) : its a check possition : cant move to this case
	// 0 : obstacle present (team side) : cant move to this case
	// 1 : no obstacle : a possible empty position can move to it
	// 2 : obstacle present (enemy side) : can capture this piece and move to its case (the piece is not Roi).


	// not out of board
	// if not check or check mat
	if (row >= 0 && row < 8 && col >= 0 && col < 8)
	{

		Position pos = m_board.at(row).at(col).cellPosition;
		ChessCase cell = PosToCase(pos);

		if (piece.GetTeamIndex() == 15 && ActionType == enActionType::CHECK_CASES)
			if (row == 7 && col == 4 || row == 4 && col == 7)
				std::cout << "=====reine======" << std::endl;


		// case is empty
		if (m_board.at(row).at(col).empty)
		{

			movementType = 1;
			if (ActionType == enActionType::MOUVEMENT)
				PossibleMvt.push_back({ pos , cell, movementType , true });  // can move to it

		}
		// enemy side & not king & not pion (pion capture otherwise)
		else if (IsCapturableObstacle(row, col))
		{

			movementType = 2;
			if (ActionType == enActionType::MOUVEMENT)
				PossibleMvt.push_back({ pos , cell, movementType ,true });  // can move to it (capture)
		}
		// enemy side ( king ) check position : cant move
		else if (IsCheckPosition(row, col, piece))
		{
			movementType = -1; // check  position shouldnt move
			PossibleMvt.push_back({ pos , cell, movementType , false });  // cant move to it

			//std::cout<< "==> check case from add pos 9dima : " << piece.GetName() << " id : " << piece.GetTeamIndex() << " cell pos : " << cell.col << ", " << cell.row << std::endl;

		}
		// obstacle present (cant move)
		else
		{
			movementType = 0;
			if (ActionType == enActionType::MOUVEMENT)
				PossibleMvt.push_back({ pos , cell, movementType , false });  // cant move to it
		}
	}

	return movementType;

}


int Chess::AddPossiblePossitionFromBoard(enActionType const& ActionType,
	Board const& board,
	std::vector<PossibleMouvement>& PossibleMvt,
	int const& row,
	int const& col,
	ChessCase const& attackerCoords)
{
	int movementType = 0;
	//-1 : obstacle present (enemy side but Roi) : its a check possition : cant move to this case
	// 0 : obstacle present (team side) : cant move to this case
	// 1 : no obstacle : a possible empty position can move to it
	// 2 : obstacle present (enemy side) : can capture this piece and move to its case (the piece is not Roi).
	// 3 : king castle 
	// 4 : Queen castle


	// not out of board
	// if not check or check mat
	if (row >= 0 && row < 8 && col >= 0 && col < 8)
	{

		Position pos = board.at(row).at(col).cellPosition;  // attacked position
		ChessCase cell = PosToCase(pos);					// attacked cell

		// case is empty
		if (board.at(row).at(col).empty)
		{
			//check if castle case
			if ((board.at(attackerCoords.row).at(attackerCoords.col).idPiece == 6) && (attackerCoords.col - col == 2))
			{
				// queen castel
				movementType = 4;
			}
			else if((board.at(attackerCoords.row).at(attackerCoords.col).idPiece == 6) && (attackerCoords.col - col == -2)) 
			{
				// king castel
				movementType = 3;
			}
			else
			{
				movementType = 1;	// normal move
				
			}

			if (ActionType == enActionType::MOUVEMENT)
				PossibleMvt.push_back({ pos , cell, movementType , true });  // can move to it

		}
		// enemy side & not king & not pion (pion capture otherwise)
		else if (IsCapturableObstacle_2(row, col, board))
		{
			movementType = 2;
			if (ActionType == enActionType::MOUVEMENT)
				PossibleMvt.push_back({ pos , cell, movementType ,true });  // can move to it (capture)
		}
		// enemy side ( king ) check position : cant move
		else if (IsCheckPosition_2(row, col, attackerCoords, board))
		{
			movementType = -1; // check  position shouldnt move
			PossibleMvt.push_back({ pos , cell, movementType , false });  // cant move to it

			//system("cls");
			//print info
			std::cout << RED_COUT << "==> check case from add pos => \n\tattacked piece name :" << board.at(row).at(col).pieceName << " ,\n\tid : " << board.at(row).at(col).pieceTeamIndex
				<< "\n\tattacker cell pos : " << attackerCoords.col << ", " << attackerCoords.row
				<< "\n\tattacker teamId : " << board.at(attackerCoords.row).at(attackerCoords.col).pieceTeamIndex
				<< "\n\tattacker name : " << board.at(attackerCoords.row).at(attackerCoords.col).pieceName << RESET_COUT << std::endl;

		}
		// obstacle present (cant move)
		else
		{
			movementType = 0;
			if (ActionType == enActionType::MOUVEMENT)
				PossibleMvt.push_back({ pos , cell, movementType , false });  // cant move to it
		}
	}

	return movementType;

}



bool Chess::IsCheckPosition(int const& row, int const& col, Piece const& piece) const
{
	if (!m_board.at(row).at(col).empty)
		//if (m_board.at(row).at(col).playerSide != flag_isPlayer1Turn)
		if (m_board.at(row).at(col).idPiece == 6 && m_board.at(row).at(col).playerSide != piece.GetPlayerSide()) //  king ( roi) 
			return true;
	return false;
}

bool Chess::IsCheckPosition_2(int const& row, int const& col, ChessCase const& attackerCoords, Board const& board) const
{
	// get infos
	bool AttackerPieceSide = board.at(attackerCoords.row).at(attackerCoords.col).playerSide;
	bool AttackedKingSide = board.at(row).at(col).playerSide;

	if (AttackerPieceSide == AttackedKingSide) // we cant make echec to same team
		return false;
	// attacked king position
	ChessCase AttackedKingCoordinates = GetAttackedKingCoordsOnTheBoard(AttackedKingSide, board);

	// decide
	if (AttackedKingCoordinates.col == -1 || AttackedKingCoordinates.row == -1) // king is not found on the board
		throw "king is not pressent on the board";
	if (AttackedKingCoordinates.col == col && AttackedKingCoordinates.row == row)
		return true;
	else
		return false;
}

bool Chess::IsAnyCheckPossitionExists(std::vector<PossibleMouvement>const& PossibleMvt)
{
	for (auto const& item : PossibleMvt)
	{
		if (item.movementType == -1)
			return true;
	}
	return false;
}

ChessCase Chess::GetAttackedKingCoordsOnTheBoard(bool const& attackedSide, Board const& board) const
{
	// id king = 6
	for (int i = 0; i < 8; i++) // row
	{
		for (int j = 0; j < 8; j++) // col
		{
			if (board.at(i).at(j).playerSide == attackedSide && board.at(i).at(j).idPiece == 6)
				return ChessCase(i, j);
		}
	}
	return ChessCase(-1, -1);
}

bool Chess::IsSelectedMoveLegal(Piece const& piece, ChessCase const& nextPosition, Board const& board, enPlayerNum const& playerNum)
{

	ChessCase currentPosition = PosToCase(selectedPieceOriginalPos);

	if (IsSameCoordinates(currentPosition, nextPosition)) // should move to a new postion to finish the turn
		return false;
	else
	{
		// make a copy from board
		Board temp_board = board;

		// get coordinates of last position
		ChessCase lastPosition = PosToCase(selectedPieceOriginalPos);

		//set starting cell to empty
		temp_board.at(lastPosition.row).at(lastPosition.col).setToEmpty();

		//set next cell info
		SetCellInfo(piece, nextPosition.row, nextPosition.col, temp_board);

		//print
		/*
		std::cout << BLUE_COUT << "=====temp_board====" << std::endl;
		PrintBoardQuickInfo("Names", temp_board);
		PrintBoardQuickInfo("indexes", temp_board);
		std::cout << "=====temp_board====" << RESET_COUT << std::endl;
		*/

		//board is updated with next move info 
		// now we should check if check possitions exists

		bool isAnyCheckPosition = IsPlayerInCheckPosition(temp_board, playerNum); // still in check or not

		return (isAnyCheckPosition ? false : true);
	}


}

bool Chess::IsLegalMove(stMove const& move, MoveFlags const& mvFlgs,  Board const& board)
{
	bool isLegalMove = false;

	// make a copy from board
	//-------------------------
	Board temp_board = board;

	//set starting cell to empty
	//---------------------------
	temp_board.at(move.fromCell.row).at(move.fromCell.col).setToEmpty();

	//set next cell info
	//--------------------
	if (move.pieceTeamSide == enPlayerNum::PLAYER1) // player 1
		SetCellInfo(*player1[move.pieceTeamIndex], move.toCell.row, move.toCell.col, temp_board);
	else
		SetCellInfo(*player2[move.pieceTeamIndex], move.toCell.row, move.toCell.col, temp_board);

	//print infos
	//------------------
	/*
	std::cout << BLUE_COUT << "=====temp_board====" << std::endl;
	PrintBoardQuickInfo("Names", temp_board);
	PrintBoardQuickInfo("indexes", temp_board);
	std::cout << "=====temp_board====" << RESET_COUT << std::endl;
	*/


	//board is updated with next move info 
	// now we should check if check possitions exists
	//------------------------------------------------
	bool isAnyCheckPosition = IsPlayerInCheckPosition(temp_board, move.pieceTeamSide); // still in check or not

	isLegalMove = (isAnyCheckPosition ? false : true);

	// validate castle case
	//----------------------
	bool isLegalCastleMove = true;

	if(IsCastleMove(mvFlgs))
	{
		isLegalCastleMove = IsLegalCastleMove(mvFlgs);
	}

	return isLegalMove && isLegalCastleMove;

}

MoveFlags Chess::SetMoveFlags(stMove const& move, MoveFlags& mvFlgs, Board const& board, std::vector<PossibleMouvement> const& possibleMvt)
{

	/* ( Quick infos about return type )
		Normal = 1,			/// move to empty cell
		Capture = 2,		/// move to not empty cell (containing enemy piece
		Promotion = 3,		/// pawn arived to board limit (could be capture)
		Check = 4,			/// if we move to this cell the other player will be in check position
		NotLegal = 5,		/// cant move :  not legal move (current team will still in check position)
		SameCell = 6,		/// cant move :  attempt to move to the same cell
		TeamCell = 7,		/// cant move :  cell occupied by team side

	*/

	// set falgs  
	//-----------

	if (IsSameCellMove(move, board))	// cant move
		mvFlgs.isSameCell = true;

	if (IsTeamCellMove(move, board)) // cant move
		mvFlgs.isTeamCell = true;

	if (IsNormalMove(move, board)) // could be promotion
		mvFlgs.isNormal = true;

	if (IsPromotionMove(move, board)) // could be capture
		mvFlgs.isPromotion = true;

	if (IsCaptureMove(move, board))
		mvFlgs.isCapture = true;

	if (IsCheckMove(move, board))
		mvFlgs.isCheck = true;


	// get moveement type from possible movement vector
	//--------------------------------------------------
	int idx = GetPossibleMovementsID(possibleMvt, move.toCell);

	// update castle flags 
	//---------------------
	if (idx != -1)
	{
		if (possibleMvt[idx].movementType == 3)
			mvFlgs.isKingCastle = true;

		if (possibleMvt[idx].movementType == 4)
			mvFlgs.isQueenCastle = true;
	}

	// call order is important
	if (IsLegalMove(move, mvFlgs , board)) //  move allowed
		mvFlgs.isLegal = true;



	return mvFlgs;

}

bool Chess::IsNormalMove(stMove const& move, Board const& board)
{
	/// MoveType::NormalMove  : move to empty cell

	int toRow = move.toCell.row,
		toColl = move.toCell.col;

	return  board.at(toRow).at(toColl).empty;
}

bool Chess::IsCaptureMove(stMove const& move, Board const& board)
{
	// MoveType::Capture : move to not empty cell (containing enemy piece)

	int fromRow = move.fromCell.row,
		fromColl = move.fromCell.col,
		toRow = move.toCell.row,
		toColl = move.toCell.col;

	bool toTeamSide = board.at(toRow).at(toColl).playerSide;
	bool fromTeamSide = board.at(fromRow).at(fromColl).playerSide;

	bool isToCellEmpty = board.at(toRow).at(toColl).empty;

	bool differentTeams = isToCellEmpty ? false : (toTeamSide != fromTeamSide);

	bool NoKingInToCell = board.at(toRow).at(toColl).piecePGNName != "K"; // security check not needed

	return (differentTeams && NoKingInToCell);
}

bool Chess::IsPromotionMove(stMove const& move, Board const& board)
{

	// MoveType::Promotion	: pawn arived to board limit (could be capture  and/or check)

	int fromRow = move.fromCell.row,
		fromColl = move.fromCell.col,
		toRow = move.toCell.row,
		toColl = move.toCell.col;

	bool isPawn = (board.at(fromRow).at(fromColl).piecePGNName == "P");

	bool teamSide = board.at(fromRow).at(fromColl).playerSide;

	bool isArrived = teamSide ? (toRow == 0) : (toRow == 7);

	return (isPawn && isArrived);
}

bool Chess::IsCheckMove(stMove const& move, Board const& board)
{
	// MoveType::Check = 4,	 : if we move to this cell the OTHER player will be in check position


	int fromRow = move.fromCell.row,
		fromColl = move.fromCell.col,
		toRow = move.toCell.row,
		toColl = move.toCell.col;


	// make a copy from board
	//-------------------------
	Board temp_board = board;

	//set starting cell to empty
	//---------------------------
	temp_board.at(fromRow).at(fromColl).setToEmpty();

	//set next cell info
	//--------------------
	if (move.pieceTeamSide == enPlayerNum::PLAYER1) // player 1
		SetCellInfo(*player1[move.pieceTeamIndex], toRow, toColl, temp_board);
	else
		SetCellInfo(*player2[move.pieceTeamIndex], toRow, toColl, temp_board);


	// check current player side
	enPlayerNum currentPlayerNum = move.pieceTeamSide;
	enPlayerNum oppositePlayerNum = (currentPlayerNum == enPlayerNum::PLAYER1) ? enPlayerNum::PLAYER2 : enPlayerNum::PLAYER1;


	//board is updated with next move info 
	// now we should check if check possitions exists for the opposite player
	//------------------------------------------------

	return  IsPlayerInCheckPosition(temp_board, oppositePlayerNum);

}

bool Chess::IsSameCellMove(stMove const& move, Board const& board)
{
	// MoveType::SameCell = 6,   cant move :  attempt to move to the same cell

	return IsSameCoordinates(move.fromCell, move.toCell);
}

bool Chess::IsTeamCellMove(stMove const& move, Board const& board)
{
	// MoveType::TeamCell = 7,	 cant move :  cell occupied by team side  

	int fromRow = move.fromCell.row,
		fromColl = move.fromCell.col,
		toRow = move.toCell.row,
		toColl = move.toCell.col;

	bool toTeamSide = board.at(toRow).at(toColl).playerSide;
	bool fromTeamSide = board.at(fromRow).at(fromColl).playerSide;

	bool isToCellEmpty = board.at(toRow).at(toColl).empty;

	bool SameTeam = isToCellEmpty ? false : (toTeamSide == fromTeamSide);

	return SameTeam;
}


Chess::stMove Chess::SetMoveInfo(ChessCase const& fromCell, ChessCase const& toCell, int pieceTeamIndex, enPlayerNum playerSide)
{
	stMove move{ fromCell , toCell , pieceTeamIndex , playerSide };
	return move;
}

std::string Chess::GetCurrentMoveText(stMove const& move, Board const& lastBoardState)
{
	// chess notation : [Piece][File][Rank]x[Destination] [Check/Checkmate]
	// Piece : nom piece (Roi : K , Dame : Q, tour : R,  Fou : B,  Cavalier : N, pion : "")	
	// file : row letter,   
	// rank : col number
	// destination : to (file + rank )
	// check : +  checkmate : #
	// promotion : e8=Q  : pawn => Queen

	PrintBoardQuickInfo("Names", lastBoardState);

	std::string piece = GetEnglishPieceName(lastBoardState.at(move.fromCell.row).at(move.fromCell.col).pieceName);
	std::string from = ToLowerCase(lastBoardState.at(move.fromCell.row).at(move.fromCell.col).caseName);
	std::string mouvementType = ""; //= lastBoardState.at(move.toCell.row).at(move.toCell.col).empty ? "" : "x"; // if capture => "x" else ""
	std::string to = ToLowerCase(lastBoardState.at(move.toCell.row).at(move.toCell.col).caseName);
	std::string check_checkMate = "";



	if (flag_checkMate)
	{
		check_checkMate = "#";
	}
	else if (m_currentMoveFlags.isCheck)
	{
		check_checkMate = "+";
	}

	if (m_currentMoveFlags.isCapture)
	{
		mouvementType = "x";
	}
	else if (m_currentMoveFlags.isNormal)
	{
		mouvementType = "";
	}

	// promotion case : (piece == pawn) && (ToPos = 0 or 7)
	if (flag_isPromotionDone)
	{
		std::string str = to + "=" + m_promotionPieceName + check_checkMate;

		// reset values
		m_promotionPieceName = "";
		flag_isPromotionDone = false;

		return str;

	}
	if (m_currentMoveFlags.isKingCastle)
	{
		return "O-O";
	}
	else if (m_currentMoveFlags.isQueenCastle)
	{
		return "O-O-O";
	}
	return piece + from + mouvementType + to + check_checkMate;
}

std::string Chess::GetEnglishPieceName(std::string const& pieceName)
{
	if (pieceName == "Pion")
		return "";
	if (pieceName == "Tour")
		return "R";
	if (pieceName == "Cava")
		return "N";
	if (pieceName == "Fou")
		return "B";
	if (pieceName == "Dame")
		return "Q";
	if (pieceName == "Roi")
		return "K";
}


void Chess::ValidateCurrentMove(ChessCase& selectedMoveCell)
{

	if (flag_isPlayer1Turn) // case player 1
	{
		// start a new round
		flag_isRoundFinished = false;

		// set move info
		stMove move{	PosToCase(selectedPieceOriginalPos), 
						selectedMoveCell, 
						player1[selectdPieceID]->GetTeamIndex(), 
						enPlayerNum::PLAYER1						};

		m_currentMove = move;

		SetMoveFlags(m_currentMove, m_currentMoveFlags , m_board , m_possibleMouvement);

		flag_player1InCheck = m_currentMoveFlags.isCheck;

		if (m_currentMoveFlags.isSameCell)
		{
			// back to orignal position
			selectedMoveCell = PosToCase(selectedPieceOriginalPos); // return the original position (before the drag)
			flag_isMovementAllowed = false;
		}
		else if (m_currentMoveFlags.isLegal) // no self check position
		{
			// allow the move
			//flag_player1InCheck = false;
			flag_isMovementAllowed = true;

			selectedPieceCurrentPos = CaseToPos(selectedMoveCell);

			// for move history
			m_lastBoard = m_board; // n-1 state of m_bord


			//play sound
			if (flag_isSoundON)
				PlaySound(m_normalMoveSound);

		
		}
		else
		{
			// back to orignal position
			selectedMoveCell = PosToCase(selectedPieceOriginalPos); // return the original position (before the drag)
			//flag_player1InCheck = true;
			flag_isMovementAllowed = false;

		}
	}

	else // case player 2
	{
		// set move info
		stMove move{	PosToCase(selectedPieceOriginalPos), 
						selectedMoveCell, 
						player2[selectdPieceID]->GetTeamIndex(), 
						enPlayerNum::PLAYER2						};

		m_currentMove = move;

		SetMoveFlags(m_currentMove, m_currentMoveFlags, m_board, m_possibleMouvement);

		flag_player2InCheck = m_currentMoveFlags.isCheck;

		if (m_currentMoveFlags.isSameCell)
		{
			// back to orignal position
			selectedMoveCell = PosToCase(selectedPieceOriginalPos); // return the original position (before the drag)
			flag_isMovementAllowed = false;
		}
		else if (m_currentMoveFlags.isLegal)
		{
			// allow the move
			//flag_player2InCheck = false;
			flag_isMovementAllowed = true;
			selectedPieceCurrentPos = CaseToPos(selectedMoveCell);

			// move history
			m_lastBoard = m_board;


			// both players played
			flag_isRoundFinished = true;

			//play sound
			if (flag_isSoundON)
				PlaySound(m_normalMoveSound);
		}

		else
		{
			// back to orignal position
			selectedMoveCell = PosToCase(selectedPieceOriginalPos);  // return the original position (before the drag)
			//flag_player2InCheck = true;
			flag_isMovementAllowed = false;
		}
	}
}

bool Chess::IsLegalCastleMove(MoveFlags const& mvFlgs)
{

		// validate castle case
		//----------------------
	bool noAttackersInEmptyCases = true;

	if (flag_isPlayer1Turn)
	{
		if (mvFlgs.isKingCastle)
			noAttackersInEmptyCases = IsNoAttackersOnEmptyCastle(CastleSide::KingSide, enPlayerNum::PLAYER1);
		else if (mvFlgs.isQueenCastle)
			noAttackersInEmptyCases = IsNoAttackersOnEmptyCastle(CastleSide::QueenSide, enPlayerNum::PLAYER1);
	}
	else
	{
		if (mvFlgs.isKingCastle)
			noAttackersInEmptyCases = IsNoAttackersOnEmptyCastle(CastleSide::KingSide, enPlayerNum::PLAYER2);
		else if (mvFlgs.isQueenCastle)
			noAttackersInEmptyCases = IsNoAttackersOnEmptyCastle(CastleSide::QueenSide, enPlayerNum::PLAYER2);
	}

	return noAttackersInEmptyCases;
}

bool Chess::IsCastleMove(MoveFlags const& mvFlgs)
{
	return (mvFlgs.isKingCastle || mvFlgs.isQueenCastle);
}

Chess::stPiece Chess::GetPieceFromBoardCell(infoCase const& cell)
{
	// get info
	bool side = cell.playerSide;
	int pieceTeamIndex = cell.pieceTeamIndex;

	for (auto const& p : (side ? player1 : player2))
	{
		if (p->GetTeamIndex() == pieceTeamIndex)
			return { side , pieceTeamIndex };
	}
	return { 0,-1 };
}

void Chess::PionPromotion()
{


	if (selectdPieceID != -1)
	{

		if (flag_isPlayer1Turn)
		{
			// get postion
			Position promotionPos = player1[selectdPieceID]->GetPosition();

			if (player1[selectdPieceID]->getId() == 1 && PosToCase(promotionPos).row == 0)
			{
				//get position
				m_promotionPosition = promotionPos;

				if (flag_isPromotion == false)
					m_promotion = std::make_unique<Promotion>(m_promotionPosition, cellSize, topMargin, leftMargin);

				// lock the game during the promotoin + allow promotion to be draw
				m_currentWindowType = enWindow::PROMOTION;

				flag_isPromotion = true;
			}
		}
		else
		{
			// get postion
			Position promotionPos = player2[selectdPieceID]->GetPosition();

			if (player2[selectdPieceID]->getId() == 1 && PosToCase(promotionPos).row == 7)
			{

				//get position
				m_promotionPosition = promotionPos;

				if (flag_isPromotion == false)
					m_promotion = std::make_unique<Promotion>(m_promotionPosition, cellSize, topMargin, leftMargin);


				// lock the game during the promotoin + allow promotion to be draw
				m_currentWindowType = enWindow::PROMOTION;
				flag_isPromotion = true;
			}
		}
	}

}


bool Chess::IsCheck()
{
	return (flag_isPlayer1Turn && flag_player1InCheck) || (!flag_isPlayer1Turn && flag_player2InCheck);
}

bool Chess::IsCheckmat()
{
	// local variable to get possible mouvement for each piece.
	std::vector<PossibleMouvement> possMvt = {};


	if (flag_isPlayer1Turn && flag_player1InCheck) // player1 turn + player1 in check
	{
		//------------------------------------------------------
		// check if all possible positions of king 1 are invalid
		//------------------------------------------------------
		ChessCase kingCellPos = PosToCase(player1[14]->GetPosition());

		GenerateLegalMoves(kingCellPos, m_board, possMvt, enActionType::MOUVEMENT);

		for (auto const& mvt : possMvt)
		{
			if (mvt.movementType != 0) // piece from team side is present
			{
				stMove move{ kingCellPos , mvt.possibleCase, player1[14]->GetTeamIndex() ,enPlayerNum::PLAYER1 };
				MoveFlags moveFlgs;
				moveFlgs.ResetFlags();
				
				SetMoveFlags(move, moveFlgs, m_board, possMvt);

				if (moveFlgs.isLegal)
					return false;
			}
		}

		//------------------------------------------------------
		// check if all possiblePositions of player1 team are invalid
		//------------------------------------------------------

		for (auto const& piece : player1)
		{
			possMvt = {}; // reset vector

			if (piece->GetTeamIndex() != 14 && !piece->IsCaptured())
			{
				ChessCase currentPieceCell = PosToCase(piece->GetPosition());
				GenerateLegalMoves(currentPieceCell, m_board, possMvt, enActionType::MOUVEMENT);

				for (auto const& mvt : possMvt)
				{
					if (mvt.movementType != 0) // piece from team side is present
					{
						stMove move{ currentPieceCell , mvt.possibleCase, piece->GetTeamIndex() ,enPlayerNum::PLAYER1 };
						
						MoveFlags moveFlgs;
						moveFlgs.ResetFlags();

						SetMoveFlags(move, moveFlgs, m_board, possMvt);

						if (moveFlgs.isLegal)
							return false;
					}
				}
			}
		}
		return true;
	}
	else if (!flag_isPlayer1Turn && flag_player2InCheck) // player2 turn + player2 in check
	{
		//------------------------------------------------------
		// check if all possible positions of king 2 are invalid
		//------------------------------------------------------
		ChessCase kingCellPos = PosToCase(player2[14]->GetPosition());

		GenerateLegalMoves(kingCellPos, m_board, possMvt, enActionType::MOUVEMENT);

		for (auto const& mvt : possMvt)
		{
			if (mvt.movementType != 0) // piece from team side is present
			{
				stMove move{ kingCellPos , mvt.possibleCase, player2[14]->GetTeamIndex() ,enPlayerNum::PLAYER2 };

				MoveFlags moveFlgs;
				moveFlgs.ResetFlags();

				SetMoveFlags(move, moveFlgs, m_board, possMvt);

				if (moveFlgs.isLegal)
					return false;
			}
		}

		//------------------------------------------------------
		// check if all possiblePositions of player2 team are invalid
		//------------------------------------------------------

		for (auto const& piece : player2)
		{
			possMvt = {}; // reset vector

			if (piece->GetTeamIndex() != 14 && !piece->IsCaptured())
			{
				ChessCase currentPieceCell = PosToCase(piece->GetPosition());
				GenerateLegalMoves(currentPieceCell, m_board, possMvt, enActionType::MOUVEMENT);

				for (auto const& mvt : possMvt)
				{
					if (mvt.movementType != 0) // piece from team side is present
					{
						stMove move{ currentPieceCell , mvt.possibleCase, piece->GetTeamIndex() ,enPlayerNum::PLAYER2 };

						MoveFlags moveFlgs;
						moveFlgs.ResetFlags();

						SetMoveFlags(move, moveFlgs, m_board, possMvt);

						if (moveFlgs.isLegal)
							return false;
					}
				}
			}
		}
		return true;
	}
	else
		return false;
}

bool Chess::IsPat()   //<<********ToDo to be finished
{

	// 3 times same pos
	//-------------------
	bool threeTimesSamePos = Is3TimesSameMove();

	// no check but no possible movement
	// ----------------------------------
	bool noPossibleMovesForCurrentPlayer = IsNoLegalMovesForCurrentPlayer();

	// insuficient material
	//----------------------
	bool insuficientMaterial = IsInsuficientMaterial();


	return threeTimesSamePos || insuficientMaterial || noPossibleMovesForCurrentPlayer;
}

std::vector<int> Chess::GetRemainingPiecesIds(enPlayerNum const& side)
{
	std::vector<int> remainingPieces;

	for (auto const& p : side == enPlayerNum::PLAYER1 ? player1 : player2)
	{
		if (p->IsCaptured() == false)
			remainingPieces.push_back(p->getId());
	}

	return remainingPieces;
}

bool Chess::Is3TimesSameMove()
{
	bool threeTimesSamePos = false;

	int numMvoes = m_allMoves.size();

	if (numMvoes > 4)
	{
		int startIdx = m_allMoves[numMvoes - 1].find('.');
		auto move = m_allMoves[numMvoes - 1].substr(startIdx); // last game  1. e2e4 - e7e5

		for (int i = numMvoes - 1 - 2; i >= numMvoes - 1 - 4; i -= 2)
		{

			int idx = m_allMoves[i].find('.');
			auto i_move = m_allMoves[i].substr(idx); // last game  1. e2e4 - e7e5

			if (move == i_move)
			{
				threeTimesSamePos = true;
				break;
			}
			else
			{
				threeTimesSamePos = false;
				break;
			}
		}

	}
	return threeTimesSamePos;
}

bool Chess::IsInsuficientMaterial()
{
	bool isInsuficient = false;

	int idBishop = 2, idKnight = 3, idKing = 6;

	auto materialP1 = GetRemainingPiecesIds(enPlayerNum::PLAYER1);
	auto materialP2 = GetRemainingPiecesIds(enPlayerNum::PLAYER2);

	if (materialP1.size() > 2 || materialP2.size() > 2)
		return false;

	else if (materialP1.size() <= 2 && materialP2.size() <= 2)
	{
		// roi + fou		vs roi + fou
		if (IsExisting(materialP1, idBishop) && IsExisting(materialP2, idBishop) &&
			IsExisting(materialP1, idKing)   && IsExisting(materialP2, idKing)   )
			return true;
		
			// roi vs roi + fou
		if ((IsExisting(materialP2, idBishop) && materialP1.size() == 1 || IsExisting(materialP1, idBishop) && materialP2.size() == 1) &&
			(IsExisting(materialP1, idKing) && IsExisting(materialP2, idKing)))
			return true;

			// roi vs roi + cavalier
		if ((IsExisting(materialP2, idKnight) && materialP1.size() == 1 || IsExisting(materialP1, idKnight) && materialP2.size() == 1) &&
			(IsExisting(materialP1, idKing) && IsExisting(materialP2, idKing)))
			return true;

			// roi vs roi
		if ((materialP1.size() == 1 || materialP2.size() == 1) && (IsExisting(materialP1, idKing) && IsExisting(materialP2, idKing)))
			return true;
	}
	
	return false;
}

bool Chess::IsNoLegalMovesForCurrentPlayer()
{
	// local variable to get possible mouvement for each piece.
	std::vector<PossibleMouvement> possMvt = {};


	if (flag_isPlayer1Turn && flag_player1InCheck == false) // player1 turn + no check
	{
		//------------------------------------------------------
		// check if all possible positions of king 1 are invalid
		//------------------------------------------------------
		ChessCase kingCellPos = PosToCase(player1[14]->GetPosition());

		GenerateLegalMoves(kingCellPos, m_board, possMvt, enActionType::MOUVEMENT);

		for (auto const& mvt : possMvt)
		{
			if (mvt.movementType != 0) // piece from team side is present
			{
				stMove move{ kingCellPos , mvt.possibleCase, player1[14]->GetTeamIndex() ,enPlayerNum::PLAYER1 };
				MoveFlags moveFlgs;
				moveFlgs.ResetFlags();

				SetMoveFlags(move, moveFlgs, m_board, possMvt);

				if (moveFlgs.isLegal)
					return false;
			}
		}

		//------------------------------------------------------
		// check if all possiblePositions of player1 team are invalid
		//------------------------------------------------------

		for (auto const& piece : player1)
		{
			possMvt = {}; // reset vector

			if (piece->GetTeamIndex() != 14 && !piece->IsCaptured())
			{
				ChessCase currentPieceCell = PosToCase(piece->GetPosition());
				GenerateLegalMoves(currentPieceCell, m_board, possMvt, enActionType::MOUVEMENT);

				for (auto const& mvt : possMvt)
				{
					if (mvt.movementType != 0) // piece from team side is present
					{
						stMove move{ currentPieceCell , mvt.possibleCase, piece->GetTeamIndex() ,enPlayerNum::PLAYER1 };

						MoveFlags moveFlgs;
						moveFlgs.ResetFlags();

						SetMoveFlags(move, moveFlgs, m_board, possMvt);

						if (moveFlgs.isLegal)
							return false;
					}
				}
			}
		}
		return true;
	}
	else if (flag_isPlayer1Turn == false && flag_player2InCheck == false) // player2 turn + no check
	{
		//------------------------------------------------------
		// check if all possible positions of king 2 are invalid
		//------------------------------------------------------
		ChessCase kingCellPos = PosToCase(player2[14]->GetPosition());

		GenerateLegalMoves(kingCellPos, m_board, possMvt, enActionType::MOUVEMENT);

		for (auto const& mvt : possMvt)
		{
			if (mvt.movementType != 0) // piece from team side is present
			{
				stMove move{ kingCellPos , mvt.possibleCase, player2[14]->GetTeamIndex() ,enPlayerNum::PLAYER2 };

				MoveFlags moveFlgs;
				moveFlgs.ResetFlags();

				SetMoveFlags(move, moveFlgs, m_board, possMvt);

				if (moveFlgs.isLegal)
					return false;
			}
		}

		//------------------------------------------------------
		// check if all possiblePositions of player2 team are invalid
		//------------------------------------------------------

		for (auto const& piece : player2)
		{
			possMvt = {}; // reset vector

			if (piece->GetTeamIndex() != 14 && !piece->IsCaptured())
			{
				ChessCase currentPieceCell = PosToCase(piece->GetPosition());
				GenerateLegalMoves(currentPieceCell, m_board, possMvt, enActionType::MOUVEMENT);

				for (auto const& mvt : possMvt)
				{
					if (mvt.movementType != 0) // piece from team side is present
					{
						stMove move{ currentPieceCell , mvt.possibleCase, piece->GetTeamIndex() ,enPlayerNum::PLAYER2 };

						MoveFlags moveFlgs;
						moveFlgs.ResetFlags();

						SetMoveFlags(move, moveFlgs, m_board, possMvt);

						if (moveFlgs.isLegal)
							return false;
					}
				}
			}
		}
		return true;
	}
	else
		return false;
}

bool Chess::IsExisting(std::vector<int> remainingPieces , int const & idPiece)
{
	for (auto const& id : remainingPieces)
		if (id == idPiece)
			return true;

	return false;
}

bool Chess::IsValidIdx(int const& row, int const& col) const
{
	return  (row >= 0 && row < 8 && col >= 0 && col < 8);  // idx should be inside the board
}

void Chess::ErrorIndex(int row, int col)
{
	if (row < 0 || row >= 8 || col < 0 || col >= 8)
	{
		std::cerr << "Index invalide : " << row << ", " << col << '\n';
		std::abort();
	}
}

void Chess::UpdateMovesHistory(Board const& lastBoardState)
{
	// player 1
	if (flag_isPlayer1Turn)
		m_WhiteMove = GetCurrentMoveText(m_currentMove, lastBoardState);
	else	// both players are played
	{
		m_BlackMove = GetCurrentMoveText(m_currentMove, lastBoardState);

		// update vector
		m_turnMove = std::to_string(m_moveIdx) + "." + m_WhiteMove + "-" + m_BlackMove;
		m_allMoves.push_back(m_turnMove);

		// print
		std::cout << "Movement: " << YELLOW_COUT << m_allMoves[m_moveIdx - 1] << RESET_COUT << std::endl;

		m_moveIdx++;

	}
}

std::string Chess::ToLowerCase(std::string const& str)
{
	std::string result = "";

	for (char const& e : str)
	{
		result += tolower(e);
	}
	return result;
}

void Chess::InitT1()
{
	m_t2 = time(nullptr); //std::chrono::steady_clock::now(); // t1 initialized when turn is started (for each player)
}

void Chess::UpdateTempo()
{
	// init m_t2 
	//-----------

	m_t2 = time(nullptr);// std::chrono::steady_clock::now();

	auto delta = m_t2 - m_t1;//std::chrono::duration_cast<std::chrono::seconds>(m_t2 - m_t1).count();


	// update m_tMax for each player
	//-----------------------------

	if (!flag_timeOverPlayer1 && !flag_timeOverPlayer2)
		if (delta >= 1)
			if (flag_isPlayer1Turn)
			{
				m_tMaxPlayer1 = m_tMaxPlayer1 - delta;
				m_t1 = m_t2;
			}
			else
			{
				m_tMaxPlayer2 = m_tMaxPlayer2 - delta;
				m_t1 = m_t2;
			}
}

void Chess::UpdateButtons()
{
	// change theme btn
	m_imgChngeTheme->Update(flag_leftMouseButtonPressed);
	if (m_imgChngeTheme->IsClicked())
	{
		SwitchThemes();
		m_imgChngeTheme->SetClicked(false);
	}

	// new game btn
	m_imgNewGame->Update(flag_leftMouseButtonPressed);
	if (m_imgNewGame->IsClicked())
	{
		StartNewGame();
		m_imgNewGame->SetClicked(false);
	}
}

void Chess::DrawPlayerTurn()
{
	float xText = m_xText, yText = topMargin;
	Color lightBlue = { 204, 204, 255, 255 };

	std::string title = "Turn:";
	std::string strText = flag_isPlayer1Turn ? "      White" : "      Black";

	DrawTextEx(m_fontText, title.c_str(), { xText, yText }, m_sizeText, m_spacingText, lightBlue);
	DrawTextEx(m_fontText, strText.c_str(), { xText , yText }, m_sizeText, m_spacingText, YELLOW);

	//DrawTextEx( font, "txt", { x, y }, size, spacing, BLACK );
}



void Chess::DrawState()
{
	// init position
	float xText = m_xText, yText = 0.5 * cellSize + topMargin;

	std::string StateText = "";


	// check state case
	if (flag_checkMate)
		StateText = "CHECK MATE";
	else if (IsCheck())
		StateText = "CHECK !";
	else if (flag_timeOverPlayer1 || flag_timeOverPlayer2)
		StateText = "TIME OUT !";
	else if (flag_isPat)
		StateText = "DRAW !";

	// draw tesxt
	DrawTextEx(m_fontText, StateText.c_str(), { xText, yText }, m_sizeText, m_spacingText, RED);

}

void Chess::DrawTime()
{
	float xText = m_xText, yText = 1. * cellSize + topMargin;
	Color lightBlue = { 204, 204, 255, 255 };

	std::string title = "Time";
	std::string player1time = "White: " + GetDurationToString(SecondsToDuration(m_tMaxPlayer1));
	std::string player2time = "Black: " + GetDurationToString(SecondsToDuration(m_tMaxPlayer2));

	DrawTextEx(m_fontText, title.c_str(), { xText, yText }, m_sizeText, m_spacingText, lightBlue);
	DrawTextEx(m_fontText, player1time.c_str(), { xText, (yText + 1.5f * m_sizeText) }, m_sizeText, m_spacingText, flag_isPlayer1Turn ? YELLOW : GRAY);
	DrawTextEx(m_fontText, player2time.c_str(), { xText, (yText + 3.f * m_sizeText) }, m_sizeText, m_spacingText, flag_isPlayer1Turn ? GRAY : YELLOW);


	// current time hh:min:sec
	//-------------------------
	std::string currentTime = GetDateTimeToString(GetSystemDateTime(), ":");
	DrawTextEx(m_fontText, currentTime.c_str(), { 8.7f * cellSize + leftMargin, 8.f * cellSize + topMargin }, m_sizeText, m_spacingText, GRAY);


}

void Chess::DrawLastMoves()
{
	float xText = m_xText, yText = 3 * cellSize + topMargin;
	Color lightBlue = { 204, 204, 255, 255 };

	std::string title = "Move history";


	DrawTextEx(m_fontText, title.c_str(), { xText, yText }, m_sizeText, m_spacingText, lightBlue);

	int len = m_allMoves.size(), cpt = 1;

	for (int i = len - 1; ; i--)
	{
		if (i < 0 || cpt == 5)
			break;

		if (i == len - 1)
			DrawTextEx(m_fontText, m_allMoves[i].c_str(), { xText, (yText + 1.5f * cpt * m_sizeText) }, 0.9 * m_sizeText, 0.5 * m_spacingText, YELLOW);
		else
			DrawTextEx(m_fontText, m_allMoves[i].c_str(), { xText, (yText + 1.5f * cpt * m_sizeText) }, 0.9 * m_sizeText, 0.5 * m_spacingText, GRAY);

		cpt++;
	}


}

void Chess::DrawLetters()
{
	float xText = 0,
		yText = 0.5f * cellSize + topMargin;
	int num = 8;

	std::string strNum = "";

	for (int i = 0; i < 8; i++)
	{
		strNum = std::to_string(num);
		DrawTextEx(m_fontText, strNum.c_str(), { xText, yText + i * cellSize }, m_sizeText, m_spacingText, GRAY);
		num--;
	}

	yText = topMargin + 8 * cellSize;
	xText = 0.5f * cellSize + leftMargin;
	char lettre = 'a';


	for (int i = 0; i < 8; i++)
	{
		lettre = 'a' + i; // from a to f
		strNum = lettre;
		DrawTextEx(m_fontText, strNum.c_str(), { xText + i * cellSize, yText }, m_sizeText, m_spacingText, GRAY);

	}

}

void Chess::DrawButtons()
{
	// button change theme
	m_imgChngeTheme->Draw();

	// button start a new game
	m_imgNewGame->Draw();
}

void Chess::DrawLateralTexts()
{
	DrawPlayerTurn(); // current turn

	DrawTime(); // player clocks + system time

	DrawState(); // checkmate / check / time over

	DrawLastMoves(); // moves history

	DrawLetters();  // case names

	DrawButtons();

}

bool Chess::IsTimeOver()
{
	if (m_tMaxPlayer1 <= 0)
	{
		flag_timeOverPlayer1 = true;
		return true;
	}
	if (m_tMaxPlayer2 <= 0)
	{
		flag_timeOverPlayer2 = true;
		return true;
	}

	return false;
}

bool Chess::GenerateCPUMove()
{
	bool moveFound = false;

	do
	{
		// randmom selection :
		int randomIdx = std::rand() % 16;  // reminder = 0 to 15

		//random cpu think time
		time_t time1 = time(NULL);
		int sleepTime = std::rand() % 2 + 1; // 1 or 2 

		if (player2[randomIdx]->IsCaptured() == false)
		{
			// erase vectors
			std::vector<PossibleMouvement> possMvt = {};
			std::vector<PossibleMouvement>().swap(possMvt);


			// generate possible pos
			selectdPieceID = randomIdx;
			selectedPieceOriginalPos = player2[selectdPieceID]->GetPosition();
			flag_isAnyPieceSelected = true;

			Position fromPos = selectedPieceOriginalPos;
			ChessCase fromCoords = PosToCase(fromPos);

			GenerateLegalMoves(fromCoords, m_board, possMvt, enActionType::MOUVEMENT);


			for (auto const& pos : possMvt)
			{
				if (pos.canMove == true)
				{
					ChessCase toCoords = pos.possibleCase;

					stMove move{ fromCoords, toCoords, selectdPieceID, enPlayerNum::PLAYER2 };

					if (IsLegalMove(move, m_currentMoveFlags, m_board))
					{
						moveFound = true;

						// set possible movement vector
						m_possibleMouvement = possMvt;

						time_t delta = 0;
						do {
							time_t time2 = time(nullptr);
							delta = time2 - time1;
						} while (delta < 2);


						MovePieceToNewCase(*player2[selectdPieceID], toCoords);
						return moveFound;
					}
				}
			}
		}

	} while (moveFound == false && flag_GameOver == false);

	return false;
}

bool Chess::IsCPUTurn()
{
	if (flag_CPU_Movement_ON)
	{
		if (flag_isPlayer1Turn == false)
			flag_isCPU_turn = true;
		else
			flag_isCPU_turn = false;

		return (flag_isCPU_turn && !flag_isPlayer1Turn);
	}
	else
		return false;

}

void Chess::LoadSettings()
{
	// Selected Theme 
	//-----------------------
	ChangeTheme(m_settingsWindow->GetTheme());

	// player vs CPU ?
	//-----------------------
	flag_CPU_Movement_ON = m_settingsWindow->GetIsCpuON();

	// Sound ON ? 
	//-----------------------
	flag_isSoundON = m_settingsWindow->GetIsSoundOn();

	// cpu time (game mode)
	//-----------------------
	GameMode mode = m_settingsWindow->GetGameMode();
	if (mode == GameMode::Bulet)
	{
		// 1min per player
		m_tMaxPlayer1 = 60;
		m_tMaxPlayer2 = 60;
	}
	else if (mode == GameMode::Blitz)
	{
		// 5min per player
		m_tMaxPlayer1 = 5 * 60;
		m_tMaxPlayer2 = 5 * 60;
	}
	else if (mode == GameMode::Rapid)
	{
		// 20min per player
		m_tMaxPlayer1 = 20 * 60;
		m_tMaxPlayer2 = 20 * 60;
	}
}

void Chess::SaveGameSteps()
{
	std::ofstream file(m_gameHistoryFileName, std::ios::app);  //append mode


	if (!file.is_open())
	{
		std::cerr << "error : Cannot oppen output text file!";
		return;
	}


	if (IsNewGame() && !flag_isAnyPieceSelected)
	{
		file << "\n\n\t==========================\n";
		file << "\t===  New Game Started  ===\n";
		file << "\t==========================\n";
		file << "\n\tDate :" << GetDateTimeToString(GetSystemDateTime()) << "\n\n";
	}
	else	// only after 1st move
	{
		if (flag_isMovementAllowed)
			file << PrintBoardQuickInfo("ENG Small Names", m_board);
	}


	if (IsRoundFinished())
	{
		file << m_allMoves[m_moveIdx - 2];
		file << "________________________________" << std::endl;
	}

	file.close();

}

void Chess::OpenGameOverScreen()
{
	if (IsScreenDrawed())
	{

		//capture last move
		m_screenshot = LoadImageFromScreen();
		ExportImage(m_screenshot, "assets/images/capture/capture.png");


		Vector2 windowSize;
		windowSize.x = (float)(windowWidth);
		windowSize.y = (float)(windowHeigh);

		//set the winner
		m_winner = GetWinner();

		if (m_winner == Winner::None)
		{
			std::cerr << "error : there no Winner!";
			return;
		}

		//new game over window
		m_gameOverWindow = std::make_unique<GameOver>(windowSize, m_winner);

		//switch to game over window
		m_currentWindowType = enWindow::GAME_OVER;

	}
}

Winner Chess::GetWinner()
{
	if (flag_GameOver)
	{
		if (IsCheckmat())
			if (flag_isPlayer1Turn)
				return Winner::Black;
			else
				return Winner::White;
		if (IsTimeOver())
			if (flag_timeOverPlayer1)
				return Winner::Black;
			else
				return Winner::White;
		if (IsPat())
			return Winner::Draw;
		else
			return Winner::None;
	}
	else
		return Winner::None;

}


bool Chess::IsCapturableObstacle(int const& row, int const& col) const
{
	if (!m_board.at(row).at(col).empty)
		if (m_board.at(row).at(col).playerSide != flag_isPlayer1Turn)
			if (m_board.at(row).at(col).idPiece != 6) // not king (pas le roi) 
				return true;
	return false;
}

bool Chess::IsCapturableObstacle_2(int const& row, int const& col, Board const& board) const  //<<********To do : avoir si rajouter attacker pour decider du side different
{
	if (!board.at(row).at(col).empty)
		if (board.at(row).at(col).playerSide != flag_isPlayer1Turn)
			if (board.at(row).at(col).idPiece != 6) // not king (pas le roi) 
				return true;
	return false;
}

bool Chess::IsEnemyObstacle(int const& row, int const& col) const
{
	if (row >= 0 && row < 8 && col >= 0 && col < 8) // security check
		if (!m_board.at(row).at(col).empty)
			if (m_board.at(row).at(col).playerSide != flag_isPlayer1Turn)
				return true;
	return false;
}

bool Chess::IsEnemyObstacle_2(int const& row, int const& col, ChessCase const& attackerCoords, Board const& board) const
{
	if (row >= 0 && row < 8 && col >= 0 && col < 8) // security check
		if (!board.at(row).at(col).empty)
			if (board.at(row).at(col).playerSide != board.at(attackerCoords.row).at(attackerCoords.col).playerSide)
				return true;
	return false;
}

void Chess::MovePieceByStep(Piece& piece, Position step)
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

void Chess::MovePieceToNewPos(Piece& piece, Position newPos)
{
	// move piece to the next position
	piece.SetPosition(newPos);
}

void Chess::MovePieceToNewCase(Piece& piece, ChessCase CCase)
{
	// get the corresponding position on the window
	Position newPos = CaseToPos(CCase);

	// move it to the new pos
	MovePieceToNewPos(piece, newPos);

}

bool Chess::IsNewGame()
{
	return flag_isNewGame;
}

void Chess::ChangeTurn()
{

	//system("cls");

	flag_isPlayer1Turn = !flag_isPlayer1Turn;

	// reset movement flags
	flag_player1InCheck = false;
	flag_player2InCheck = false;
	flag_isMovementAllowed = false;
	flag_isAnyPieceCaptured = false;


	// check if current player is in check position
	if (flag_isPlayer1Turn)				//<*******Todo to be changed by m_currentMoveFlags values
		flag_player1InCheck = m_currentMoveFlags.isCheck; 
	else
		flag_player2InCheck = m_currentMoveFlags.isCheck;



	// init t1 for tempo calculation
	InitT1();
}

bool Chess::IsRoundFinished()
{
	return flag_isRoundFinished;
}

void Chess::ChangeTheme(Theme const& theme)
{
	/*
		theme1 : m_themeList[0] (default)
		theme2 : m_themeList[1]
		theme3 : m_themeList[2]
	*/

	if (theme == Theme::Brown)	// set theme 2
	{
		SetColorsFromTheme(m_themeList[1]);
		m_currentTheme = Theme::Brown;
		grid->SetColors(m_color1, m_color2);

	}
	else if (theme == Theme::Blue)	// set theme 1 (default)
	{
		SetColorsFromTheme(m_themeList[0]);
		m_currentTheme = Theme::Blue;
		grid->SetColors(m_color1, m_color2);

	}
	else if (theme == Theme::Magenta)		// set theme 3
	{
		SetColorsFromTheme(m_themeList[2]);
		m_currentTheme = Theme::Magenta;
		grid->SetColors(m_color1, m_color2);

	}

}

void Chess::SwitchThemes()
{
	switch (m_currentTheme)
	{
	case  Theme::Blue:
		ChangeTheme(Theme::Brown);
		break;

	case  Theme::Brown:
		ChangeTheme(Theme::Magenta);
		break;

	case  Theme::Magenta:
		ChangeTheme(Theme::Blue);
		break;
	}
}

Chess::stTheme Chess::GetDefaultTheme()
{
	///	 theme 1
	Color 	color1 = { 204, 204, 255, 255 }, // color1
		color2 = BLUE,					// background
		hoveredColor = { 255, 204, 0,100 },
		backgroundColor = { 0, 0, 102 , 255 };  // dark blue

	stTheme defaultTheme = { color1 , color2 , hoveredColor , backgroundColor };
	return defaultTheme;
}

void Chess::SetColorsFromTheme(stTheme const& theme)
{

	m_color1 = theme.color1;
	m_color2 = theme.color2;
	m_hoveredColor = theme.heveredColor;
	m_backgroundColor = theme.backgroundColor;

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

		if (!flag_isAnyPieceSelected)
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

			//----------------------------------------------------------------
			// get possible postions

			if (!flag_possibleMouvemntsAreCalculated) //<<*******ToDo no need to that
			{
				UpdateBoardInfo(flag_isPlayer1Turn);
				Position pos = player1[selectdPieceID]->GetPosition();
				ChessCase coords = PosToCase(pos);
				GenerateLegalMoves(coords, m_board, m_possibleMouvement, enActionType::MOUVEMENT);

				flag_possibleMouvemntsAreCalculated = true;

			}

			//----------------------------------------------------------------

			// set centerPiece = mousePos
			SetCenterPieceToCursorPosition(*player1[selectdPieceID], cursorPos);

			// drag piece to cursor position (center of piece folows cursor)
			MovePieceByStep(*player1[selectdPieceID], step);

			//reset flag
			flag_isAnyPieceSelected = true;

		}
	}
	else // player 2 turn
	{

		if (flag_CPU_Movement_ON) // computer = player 2
		{
			GenerateCPUMove();
		}
		else
		{
			if (!flag_isAnyPieceSelected)
			{
				selectdPieceID = GetSelectedPiece(player2);
			}

			if (selectdPieceID != -1)
			{
				// set selectedPieceOriginalPos
				if (!flag_isAnyPieceSelected)
					selectedPieceOriginalPos = player2[selectdPieceID]->GetPosition();

				//reset flag
				flag_isAnyPieceSelected = true;

				//----------------------------------------------------------------
				// get possible postions

				if (!flag_possibleMouvemntsAreCalculated)
				{
					UpdateBoardInfo(flag_isPlayer1Turn);
					Position pos = player2[selectdPieceID]->GetPosition();
					ChessCase coords = PosToCase(pos);
					GenerateLegalMoves(coords, m_board, m_possibleMouvement, enActionType::MOUVEMENT);

					flag_possibleMouvemntsAreCalculated = true;
				}


				//----------------------------------------------------------------

				// set centerPiece = mousePos
				SetCenterPieceToCursorPosition(*player2[selectdPieceID], cursorPos);

				// drag piece to cursor position (center of piece folows cursor)
				MovePieceByStep(*player2[selectdPieceID], step);

			}
		}
	}
}

void Chess::ReleasePiece()
{
	if (selectdPieceID != -1 && flag_isAnyPieceSelected) // security check
	{
		// correct draged Piece Position (allow or not the movement )
		CorrectDragedPiecePosition();

	}
}



void Chess::CorrectDragedPiecePosition()
{
	if (flag_isPlayer1Turn) // player1 turn
	{
		// get center pos
		Position centerPos = player1[selectdPieceID]->GetCenterOfPiecePosition();

		// get current pos
		Position currentPos = player1[selectdPieceID]->GetPosition();


		// here we should decide if the piece should move or not
		// in case the piece shouldnt move => we should back to its orignal position (before the drag)

		// get corresponding board case position 
		Position correspondingCase = GetCorrespondingBoardCase(centerPos);
		ChessCase correspondingCell = PosToCase(correspondingCase);


		//----------------------------------
		// check if check position is present				
		//----------------------------------
		ValidateCurrentMove(correspondingCell);


		flag_possibleMouvemntsAreCalculated = false;  //<<******ToDo no need

		//-----------------
		// EXECUTE DECISION
		// -----------------
		// move piece to the corresponding case
		MovePieceToNewCase(*player1[selectdPieceID], correspondingCell);

		// is any castle case
		if (m_currentMoveFlags.isLegal)
		{
			if (m_currentMoveFlags.isKingCastle)
				MovePieceToNewCase(*player1[13], GetCellCoordsFromCaseName("F1"));
			if (m_currentMoveFlags.isQueenCastle)
				MovePieceToNewCase(*player1[12], GetCellCoordsFromCaseName("D1"));
		}

		//----------------------------------------end execute


		// check if piece is moved 
		bool colCondition = selectedPieceOriginalPos.col != player1[selectdPieceID]->GetPosition().col;
		bool rowCondition = selectedPieceOriginalPos.row != player1[selectdPieceID]->GetPosition().row;

		if (colCondition || rowCondition)
		{
			player1[selectdPieceID]->SetNeverMoved(false);
			player1[selectdPieceID]->SetLastPosition(selectedPieceOriginalPos);
		}

	}
	else // player2 turn
	{
		// get center pos
		Position centerPos = player2[selectdPieceID]->GetCenterOfPiecePosition();

		// get current pos
		Position currentPos = player2[selectdPieceID]->GetPosition();


		// here we should decide if the piece should move or not
		// in case the piece shouldnt move => we should back to its orignal position (before the drag)

		// get corresponding board case position 
		Position correspondingCase;
		ChessCase correspondingCell;

		if (flag_CPU_Movement_ON)
		{
			correspondingCell = PosToCase(currentPos);
		}
		else
		{
			correspondingCase = GetCorrespondingBoardCase(centerPos);
			correspondingCell = PosToCase(correspondingCase);
		}


		//----------------------------------
		// check if check position is present				
		//----------------------------------
		ValidateCurrentMove(correspondingCell);


		flag_possibleMouvemntsAreCalculated = false;  //<<******ToDo no need


		//-----------------
		// EXECUTE DECISION
		// -----------------
		// move piece to the corresponding case
		MovePieceToNewCase(*player2[selectdPieceID], correspondingCell);

		// is any castle case
		if (m_currentMoveFlags.isLegal)
		{
			if (m_currentMoveFlags.isKingCastle)
				MovePieceToNewCase(*player2[13], GetCellCoordsFromCaseName("F8"));
			if (m_currentMoveFlags.isQueenCastle)
				MovePieceToNewCase(*player2[12], GetCellCoordsFromCaseName("D8"));
		}

		//----------------------------------------end execute

		// check if piece is moved 
		bool colCondition = selectedPieceOriginalPos.col != player2[selectdPieceID]->GetPosition().col;
		bool rowCondition = selectedPieceOriginalPos.row != player2[selectdPieceID]->GetPosition().row;

		if (colCondition || rowCondition)
		{
			player2[selectdPieceID]->SetNeverMoved(false);
			player2[selectdPieceID]->SetLastPosition(selectedPieceOriginalPos);
		}

	}
}


int Chess::GetSelectedPiece(std::vector<std::unique_ptr<Piece>> const& player)
{
	for (int idx = 0; idx < player.size(); idx++)
	{
		// check if mouse position is inside piece image
		if (CheckCollisionPointRec(GetMousePosition(), player[idx]->GetImageBounds()))
			return idx;
	}
	return -1; // no piece is selected
}


void Chess::SetCenterPieceToCursorPosition(Piece& piece, Position const& cursorPos)
{

	Position delta_center_cursor(0, 0),
		centerPiece(0, 0);

	// get center of piece position
	centerPiece = piece.GetCenterOfPiecePosition();

	//calculate delta_center_cursor
	delta_center_cursor.row = cursorPos.row - centerPiece.row;
	delta_center_cursor.col = cursorPos.col - centerPiece.col;

	MovePieceByStep(piece, delta_center_cursor);
}

Position Chess::GetCorrespondingBoardCase(Position centerPos)
{
	// get the orignal position of the current piece
	Position result = selectedPieceOriginalPos;
	int x = 0, y = 0;

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			// position of each cell
			x = col * cellSize;
			y = row * cellSize;

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

				for (auto const& p : m_possibleMouvement)
					if (p.possiblePosition.col == x && p.possiblePosition.row == y && p.canMove == true)
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
// m_board methods
//------------------------------

Position Chess::GetCellPosition(int i, int j)
{
	// row = i * cellSize + topMargin
	// col = j * cellSize + leftMargin

	return Position((i * cellSize) + topMargin, (j * cellSize) + leftMargin);
}

Position Chess::GetCellCentralPosition(int i, int j)
{
	Position cellPos = GetCellPosition(i, j);

	float halfCellSize = (float)(cellSize / 2);

	return Position(cellPos.row + halfCellSize, cellPos.col + halfCellSize);
}

Position Chess::GetPostionFromBoardCaseName(std::string const& caseName) const
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (m_board.at(i).at(j).caseName == caseName)
				return Position(m_board.at(i).at(j).cellPosition.row, m_board.at(i).at(j).cellPosition.col);
	return Position(0, 0);
}

void Chess::InitBoardInfo()
{
	int cpt = 1; // id cell on board

	for (int i = 0; i < 8; i++) // row : 8 7 6 5 4 3 2 1 
	{
		for (int j = 0; j < 8; j++) // col : A B C D E F G H
		{
			m_board.at(i).at(j).caseName = GetCaseName(i, j);
			m_board.at(i).at(j).empty = true;
			m_board.at(i).at(j).idCell = cpt++;
			m_board.at(i).at(j).idPiece = -1;
			m_board.at(i).at(j).pieceTeamIndex = -1;
			m_board.at(i).at(j).pieceName = "-"; // no piece
			m_board.at(i).at(j).pieceEnName = "-";
			m_board.at(i).at(j).piecePGNName = "-";
			m_board.at(i).at(j).playerSide = -1;
			m_board.at(i).at(j).cellPosition = GetCellPosition(i, j);
			m_board.at(i).at(j).cellCoordinates = ChessCase(i, j);
			m_board.at(i).at(j).cellCentralPosition = GetCellCentralPosition(i, j);

			std::cout << m_board.at(i).at(j).caseName << "  ";
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
		(i == 1) ? "7" :
		(i == 2) ? "6" :
		(i == 3) ? "5" :
		(i == 4) ? "4" :
		(i == 5) ? "3" :
		(i == 6) ? "2" : "1";

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

void Chess::UpdateCellInfo(Piece const& piece, int row, int col, Board& board)
{
	// put cell info according to piece details 
	if (board.at(row).at(col).cellPosition.row == piece.GetPosition().row &&
		board.at(row).at(col).cellPosition.col == piece.GetPosition().col)
	{
		board.at(row).at(col).empty = false;
		board.at(row).at(col).idPiece = piece.getId();
		board.at(row).at(col).pieceTeamIndex = piece.GetTeamIndex();
		board.at(row).at(col).pieceName = piece.GetName();
		board.at(row).at(col).pieceEnName = piece.GetEnName();
		board.at(row).at(col).piecePGNName = piece.GetPGNName();
		board.at(row).at(col).playerSide = piece.GetPlayerSide();
	}

}

void Chess::SetCellInfo(Piece const& piece, int row, int col, Board& board)
{
	// put cell info according to piece details (no need to checkif cellposs = piecePosition)

	board.at(row).at(col).empty = false;
	board.at(row).at(col).idPiece = piece.getId();
	board.at(row).at(col).pieceTeamIndex = piece.GetTeamIndex();
	board.at(row).at(col).pieceName = piece.GetName();
	board.at(row).at(col).pieceEnName = piece.GetEnName();
	board.at(row).at(col).piecePGNName = piece.GetPGNName();
	board.at(row).at(col).playerSide = piece.GetPlayerSide();


}

void Chess::UpdateBoardInfo(bool player1Turn)
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
		if (!piece->IsCaptured()) // piece not captured
		{
			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					if (m_board.at(row).at(col).playerSide == player1Turn)
					{
						m_board.at(row).at(col).setToEmpty();
					}
				}
			}
		}
	}
}

void Chess::SetNewValuesToBoardInfo(bool player1Turn)
{
	for (auto& const piece : (player1Turn ? player1 : player2))
	{
		if (!piece->IsCaptured()) // piece not captured
		{
			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					UpdateCellInfo(*piece, row, col, m_board);
				}
			}
		}
	}
}

bool Chess::Compare2Cells(infoCase const& cell1, infoCase const& cell2)
{

	bool condition = (cell1.caseName == cell2.caseName &&
		cell1.idCell == cell2.idCell &&
		cell1.playerSide == cell2.playerSide &&
		cell1.pieceName == cell2.pieceName &&
		cell1.pieceTeamIndex == cell2.pieceTeamIndex &&
		cell1.idPiece == cell2.idPiece);


	return condition;
}

bool Chess::IsSameCoordinates(ChessCase const& cell1, ChessCase const& cell2)
{
	return (cell1.row == cell2.row && cell1.col == cell2.col);
}

void Chess::GetBoardRowColFromCaseName(std::string caseName, int& row, int& col)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_board.at(i).at(j).caseName == caseName)
			{
				row = i;
				col = j;
				break;
			}
		}
	}
}

ChessCase Chess::GetCellCoordsFromCaseName(std::string const& caseName)
{
	int row = -1, col = -1;
	GetBoardRowColFromCaseName(caseName, row, col);

	if (row == -1 || col == -1)
	{
		std::cerr << " error while using GetBoardRowColFromCaseName";
	}

	ChessCase cell(row, col);

	return cell;
}



void Chess::GetBoardRowColFromPiecePosition(Position const& pos, int& row, int& col, bool beforePieceReleased)  const
{
	Position piecePos;

	// set the mode (before or after release)
	// in case we use this function after piece released the position of the piece is corrected (so we can take it)
	// otherwise we are before piece realeasd so we have to use the original position of the piece

	if (beforePieceReleased)
		piecePos = selectedPieceOriginalPos;
	else
		piecePos = pos;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_board.at(i).at(j).cellPosition.row == piecePos.row &&
				m_board.at(i).at(j).cellPosition.col == piecePos.col) // original pos (not current pos)
			{
				row = i;
				col = j;
				break;
			}
		}
	}
}

std::string Chess::PrintBoardQuickInfo(std::string infoType, Board const& board) const
{
	std::ostringstream oss; // the goal is to get printed results by "cout" into a "oss"

	oss << std::endl;

	if (infoType == "Position")
	{
		oss << "Position" << std::endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				oss << "(" << board.at(i).at(j).cellPosition.row << "," << board.at(i).at(j).cellPosition.col << ") ";
			}
			oss << std::endl;
		}
	}

	else if (infoType == "Names")
	{
		oss << "Names" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				oss << std::setw(4) << board.at(i).at(j).pieceName << " ";
			}
			oss << std::endl;
		}
	}

	else if (infoType == "ENG Names")
	{
		oss << "ENG Names" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				oss << std::setw(4) << board.at(i).at(j).pieceEnName << " ";
			}
			oss << std::endl;
		}
	}

	else if (infoType == "ENG Small Names")
	{
		std::string playerTurn = "";

		oss << " ==================" << std::endl;
		for (int i = 0; i < 8; i++)
		{
			oss << 8 - i << "|";

			for (int j = 0; j < 8; j++)
			{
				oss << std::setw(1) << board.at(i).at(j).piecePGNName << " "; // only the first letter
			}

			// write player turn
				//-------------------

			if (i == 1)
				if (flag_isPlayer1Turn)
					playerTurn = "\tWhite turn";
				else
					playerTurn = "\tBlack turn";
			else
				playerTurn = "";

			oss << "|" << playerTurn << std::endl;
		}
		oss << " ==================" << std::endl;
		oss << "  a b c d e f g h" << std::endl;
	}

	else if (infoType == "empty")
	{
		oss << "empty" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				oss << std::setw(4) << board.at(i).at(j).empty << " ";
			}
			oss << std::endl;
		}
	}

	else if (infoType == "playerSide")
	{
		oss << "playerSide" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				oss << std::setw(3) << board.at(i).at(j).playerSide << " ";
			}
			oss << std::endl;
		}
	}

	else if (infoType == "indexes")
	{
		oss << "indexes" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				oss << std::setw(3) << board.at(i).at(j).pieceTeamIndex << " ";
			}
			oss << std::endl;
		}
	}

	oss << std::endl;

	std::cout << YELLOW_COUT << oss.str() << RESET_COUT;

	return oss.str();
}
