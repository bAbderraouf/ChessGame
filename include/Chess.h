#pragma once

#include "Game.h"
#include "Grid.h"
#include "Piece.h"
#include "raylib.h"
#include "Position.h"
#include "ChessCase.h"
#include "Settings.h"
#include "Utilis.h"
#include <string>
#include <iomanip> // for setw() : cout
#include <array>
#include <ctime>
#include <chrono>
#include <cctype>
#include <cstdlib>	// rand
#include <fstream>
#include <sstream>  // std::ostringstream
#include <memory>


/**
* @brief Class that manage all steps of a 2D chess game
* note : The Game Class is the interface
*/
class Chess : public Game
{
private:


	/**
	* @brief elementary possible movement variable type (for a defined piece)
	* it contains informations about that possible movement
	*/
	struct PossibleMouvement {
		Position possiblePosition;	/// a possible position on the board (x,y)
		ChessCase possibleCase;		/// a possible position on the board (i,j)
		int movementType;			/// 4 types : -1:check; 0:empty cell; 1:obstacle team side; 2:obstacle enemy side (capture)
		bool canMove;				/// can make the move or not ?
	};
	


	/**
	* @brief to select Mode (game or settings)
	*/
	enum class enWindow {
		GAME = 0,
		SETTINGS = 1,
	};



	/**
	* @brief to know wich type of action we need
	*/
	enum class enActionType {
		MOUVEMENT = 0,		/// to make normal moves
		CHECK_CASES = 1,	/// to calculate check cases

	};


	/**
	* @brief to know wich player team side we need
	*/
	enum class enPlayerNum {
		PLAYER1 = 0,
		PLAYER2 = 1,
	};


	/**
	* @brief elementary move structure, from cell to cell,
	*/
	struct stMove {
		ChessCase fromCell;			
		ChessCase toCell;
		// piece info
		int pieceTeamIndex;			
		enPlayerNum pieceTeamSide;
	};


	/**
	* @brief we can define a piece by its teamSide and teamIndex
	*/
	struct stPiece {
		bool pieceTeamSide;		/// 0 : black (player2), 1: white
		int pieceTeamIdx;		/// from 0 to 15 (16 pieces)
	};

	Grid* grid;		/// an instance from Grid class to make the board
	Piece* piece;	/// an instance from Piece class to generate pieces


	//window
	//---------
	int windowWidth, 
		windowHeigh;
	int gameFps;		/// frames/sec
	Image m_screenshot; /// a simple screenshot for the window

	//grid
	//--------
	int numRows;				///8 rows
	int numCols;				///8 columns
	int cellSize;				/// cell size in pixel
	int leftMargin, topMargin;  /// margins outside of the 8x8 board

	Color m_color1, m_color2,	/// board chess colors (black & white)
		  m_hoveredColor,		/// color of possible positions
		  m_backgroundColor;	/// margins colors (outisde of the board)


	/**
	* @brief theme structure (needed colors)
	*/
	struct stTheme {
		Color color1;
		Color color2;
		Color heveredColor;
		Color backgroundColor;
	};


	stTheme m_defaultTheme;		/// default set of colors used for the game
	Theme m_currentTheme;
	
	std::vector<stTheme> m_themeList;  /// list of themes used in the app



	// window type
	// ------------
	enWindow m_currentWindowType;		/// current window type (Game, Settings ...)

	
	
	// settings 
	//----------
	//Settings m_settings;
	std::unique_ptr<Settings> m_settings;


	//pieces
	//----------
	std::vector<std::unique_ptr<Piece>> player1, player2;		/// player1 (white), player2 (black) 16pieces for each one
	int selectdPieceID; 										/// selected (current) piece id (from 0 to 15)
	Position selectedPieceOriginalPos ,selectedPieceCurrentPos; /// original positon for the current piece (x,y) & (i,j)

	std::vector<PossibleMouvement> m_possibleMouvement;			/// possible movement vecteur for each piece.

	//moves history 
	//--------------
	std::string m_WhiteMove,	/// white move text history
				m_BlackMove,	/// black move text history
				m_turnMove;		/// current turn move text history

	std::vector<std::string> m_allMoves;	/// vector of all moves text history (concatenation of moves)

	stMove m_currentMove;		/// current move (from , to) 
	int m_moveIdx;				/// move index from 0 to N, where N: number of moves in the game

	//text 
	//-------
	float m_xText;			/// lateral text position on the window (x axis)
	int m_sizeText,			/// size of lateral text
		m_spacingText;		/// spacing of lateral text
	Font m_fontText;		/// used font for lateral text



	//drawable( clickable)
	//---------
	std::unique_ptr<Drawable>	m_imgNewGame,			/// button to start a new game at any moment
								m_imgChngeTheme;		/// button to change theme from selection

	//drawable positions
	Vector2 m_posNewGame, m_posChgeTheme;


	//sound
	//---------
	Sound	m_normalMoveSound,		/// sound to use for each move
			m_checkSound,
			m_checkmateSound,
			m_capturepieceSound;

	//time
	//---------
	using clock = std::chrono::time_point<std::chrono::steady_clock> ;

	time_t	m_tMaxPlayer1, /// maximum game time for player 1 in seconds
			m_tMaxPlayer2; /// maximum game time for player 2 in seconds

	time_t	m_t1, /// internal variables to calcul elapsed time for each player
			m_t2;  


	//output files
	//-------------

	std::string m_gameHistoryFileName;


	//Board case
	//------------

	/**
	* @brief elementary information structure, used to keep informations about each cell case on the board,
	*/
	struct infoCase
	{
		bool empty;						/// is cell empty (no pieces)
		int idCell;						/// id of the current cell case (i,j)
		int idPiece;					/// id of the piece contained in this cell case (from 0 to 6) : exple 6 => King
		int pieceTeamIndex;				/// piece team index (from 0 to 15) 
		Position cellPosition;			/// cell position in pexels (x,y)
		Position cellCentralPosition;	/// cell central position in pixels (x,y)
		ChessCase cellCoordinates;		/// cell coordinates (i,j)
		std::string pieceName;			/// piece name (Pion, Fou, Tour, ....)
		std::string pieceEnName;		/// piece english name (Pawn, Bishop, Rook, ....)
		std::string piecePGNName;		/// piecePGNName : Portable Game Notation K Q R B N P
		std::string caseName;			/// cell case name exple : A1, E2 , F4 ...
		int playerSide;					/// piece team side (1 : white , 0 : black)

		infoCase();						/// construtor
		void setToEmpty();				/// internal function to set case empty (default values)
	};


	using Board = std::array <std::array<infoCase, 8>, 8>;  // using = type def (modern) : Boerd is the new type 
	
	
	Board	m_board,			/// a matrix of type infoCase containing all cell cases informations
			m_Initial_Board;	/// initial state of that matrix stored after initialization

	//flags
	//--------

	bool	flag_isPlayer1Turn,					/// 1 : white , 0 : black
			flag_leftMouseButtonPressed,
			flag_rightMouseButtonPressed,
			flag_leftMouseButtonDown,
			flag_leftMouseButtonReleased,
			flag_isAnyPieceSelected,			/// 1 : piece selected , 0 : no piece selected
			flag_player1InCheck,				/// 1 : a check position for player1 (white)
			flag_player2InCheck,
			flag_checkMate,						/// 1 : checkmate => game over
			flag_isPat,							/// 1 : draw => game over
			flag_timeOverPlayer1,				/// 1 : white time is finished => game over
			flag_timeOverPlayer2,
			flag_GameOver,						/// 1 : gmae is finished, no moves allowed anymore
			flag_isMovementAllowed,				/// 1 : current move is legal (no check possition, & can move )
			flag_isAnyPieceCaptured,			/// 1 : if any piece is captured (movement type == 2)
			flag_possibleMouvemntsAreCalculated,
			flag_CPU_Movement_ON,				/// 1 : player2 = CPU (computer is playing)
			flag_isCPU_turn,					/// 1 : player2 (black) turn
			flag_isRoundFinished,				/// 1 : if both players are played (white turn + black turn)
			flag_isNewGame,
			flag_isSoundON;

public :  //<<*******ToDo reset public & private fct
	
	
	/*--------------------------------------------------------------------------------
	* @brief constructor : init all varibles for chess game object
	* @param w,h : window's width & height
	* @param nRows, nCols : grid number of rows & columns (respectivly).
	* @param cSize : grid cell size in pixel.
	* @param lMargin, tMargin : left margin & top margin (space to add outside of the grid board).
	* --------------------------------------------------------------------------------*/
	Chess(int w, int h, int fps, int nRows, int nCols, int cSize, int lMargin, int tMargin);


	/*--------------------------------------------------------------------------------
	* @brief chess game destructor
	*---------------------------------------------------------------------------------*/
	~Chess();

	//----------------------
	// principal functions
	//----------------------

	/*--------------------------------------------------------------------------------
	*  @brief init : window + grid + pieces + internal variables
	*---------------------------------------------------------------------------------*/
	void Init();


	/*--------------------------------------------------------------------------------
	*  @brief reset game values :  pieces + internal variables 
		(fixed values are initialized in Init() function)
	*---------------------------------------------------------------------------------*/
	void StartNewGame();

	/*--------------------------------------------------------------------------------
	*  @brief wait for inputs : drag piece, release, ...
	*---------------------------------------------------------------------------------*/
	void Input();

	/*--------------------------------------------------------------------------------
	*  @brief update chess game states.
	*---------------------------------------------------------------------------------*/
	void Update(); 





	/*--------------------------------------------------------------------------------
	*  @brief draw screen : window + grid + pieces + possible movements
	*---------------------------------------------------------------------------------*/
	void Draw();




	//----------------------
	// additional functions
	//----------------------

	/*--------------------------------------------------------------------------------
	* @brief set game Frames per seconds.
	* @param fps : numbere of frames/sec
	*---------------------------------------------------------------------------------*/
	void setGameFPS(int fps);


	/*--------------------------------------------------------------------------------
	* @brief draw all player's pieces on the board.
	* @param player : player1 or player2
	*---------------------------------------------------------------------------------*/
	void DrawPlayerPieces(std::vector<std::unique_ptr<Piece>> const &player);


	/*--------------------------------------------------------------------------------
	* @brief move selected piece by step (add a step to its position)
	* @param piece : selected piece
	* @param step : delta position for that piece.
	*---------------------------------------------------------------------------------*/
	void MovePieceByStep(Piece& piece, Position step);


	/*--------------------------------------------------------------------------------
	* @brief move selected piece to a new positon
	* @param piece : the selected piece
	* @param newPos : the new position for this piece.
	*---------------------------------------------------------------------------------*/
	void MovePieceToNewPos(Piece& piece, Position newPos);


	/*--------------------------------------------------------------------------------
	* @brief move selected piece to a new positon by selecting cell coordinates
	* @param piece : the selected piece
	* @param newPos : the new position for this piece in cell coordinates (raw, col).
	*---------------------------------------------------------------------------------*/
	void MovePieceToNewCase(Piece& piece, ChessCase CCase);



	/*--------------------------------------------------------------------------------
	* @brief game is starting (no moves yet)
	* @return true if game is starting without any move
	*---------------------------------------------------------------------------------*/
	bool IsNewGame();



	/*--------------------------------------------------------------------------------
	* @brief change the player side after each round (init necessary flags)
	*---------------------------------------------------------------------------------*/
	void ChangeTurn();


	/*--------------------------------------------------------------------------------
	* @brief check if round is finished (both players played)
	* @return true if round is finished
	*---------------------------------------------------------------------------------*/
	bool IsRoundFinished();



	/*--------------------------------------------------------------------------------
	* @brief change the theme of the game (use existing themes, new set of colors).
	* @param generalColor : the selected theme
	*---------------------------------------------------------------------------------*/
	void ChangeTheme(Theme const& theme);



	/*--------------------------------------------------------------------------------
	* @brief change the theme of the game (use existing themes, new set of colors).
	* @param generalColor : the selected theme
	*---------------------------------------------------------------------------------*/
	void SwitchThemes();



	/*--------------------------------------------------------------------------------
	* @brief get default theme fot the game (new set of colors).
	* @return stTheme : the default theme
	*---------------------------------------------------------------------------------*/
	stTheme GetDefaultTheme();



	/*--------------------------------------------------------------------------------
	* @brief set different colors from a selected theme
	* @param theme : a selected theme to be used
	*---------------------------------------------------------------------------------*/
	void SetColorsFromTheme( stTheme const & theme);



	/*--------------------------------------------------------------------------------
	* @brief drag the selected piece and move it to the wanted position
	*---------------------------------------------------------------------------------*/
	void DragPiece();


	/*--------------------------------------------------------------------------------
	* @brief drag then release the selected piece, 
	* this function will correct draged Piece Position (allow or not the movement according to game rules)
	*---------------------------------------------------------------------------------*/
	void ReleasePiece();


	/*--------------------------------------------------------------------------------
	
	* @brief  coorect draged piece possitions, and put it alligned inside the correct case afted the drag
	* otherwise the piece should comme back to its last position before the drag
	
	----------------------------------------------------------------------------------*/
	void CorrectDragedPiecePosition();


	/*--------------------------------------------------------------------------------
	
	* @brief This function gets the selected piece to drag using the cursor.
	* @param piece : a vector of pieces (player1 or 2).
	* @return : The index of the selected piece from the correspondig vector of pieces,
	* returns -1 if no piece is selected.
	
	*---------------------------------------------------------------------------------*/
	int GetSelectedPiece(std::vector<std::unique_ptr<Piece>> const &player);


	/*--------------------------------------------------------------------------------

	* @brief This function sets the center of the piece on the same positon as the cursor.
	* @param piece : the selected piece to drag
	* @param cursoPos : the current position of the cursor.
		
	*---------------------------------------------------------------------------------*/
	void SetCenterPieceToCursorPosition(Piece& piece , Position const &cursorPos);


	/*--------------------------------------------------------------------------------

	* @brief This function gets the corresponding board case (cell) position.
	* @param centerPos : the draged piece central position
	* @return the corresponding board case (cell) position (coords in pixels)

	*---------------------------------------------------------------------------------*/
	Position GetCorrespondingBoardCase(Position centerPos);



	/*--------------------------------------------------------------------------------

	* @brief This function generates all player pieces (intialize vector of pieces player1 & player2.
	* @param player1Side : true if the current player side is player1
	* 
	*---------------------------------------------------------------------------------*/
	std::vector<std::unique_ptr<Piece>> InitPlayersPieces(bool player1Side);


	/*--------------------------------------------------------------------------------

	* @brief This function sets all player pieces on their initial positions on the board.
	* @param vectOfPieces : player1 or player2 vector.
	* @param caseNames[] : corresponding case names string list for all pieces from this list.
		A8  B8  C8  D8  E8  F8  G8  H8
		A7  B7  C7  D7  E7  F7  G7  H7
		A6  B6  C6  D6  E6  F6  G6  H6
		A5  B5  C5  D5  E5  F5  G5  H5
		A4  B4  C4  D4  E4  F4  G4  H4
		A3  B3  C3  D3  E3  F3  G3  H3
		A2  B2  C2  D2  E2  F2  G2  H2
		A1  B1  C1  D1  E1  F1  G1  H1

	*---------------------------------------------------------------------------------*/
	void SetPlayerInitialPositions(std::vector<std::unique_ptr<Piece>>& vectOfPieces, std::string caseNames[]);


	/*--------------------------------------------------------------------------------

	* @brief for each selected piece, we should set the possible movement cells to hovered (transparent color)
	*	to sho the possible moves for the selected piece.
	* @param color : hovered color.
	*
	*---------------------------------------------------------------------------------*/
	void DrawPossiblePositions(Color color);


	/*--------------------------------------------------------------------------------

	* @brief for the current piece get the possible positions (movements) on the board, 
	* according to the game rules
	* @param piece : the current piece
	* @param possibleMvt : a vector of possible movements for the selected piece.
	* @param ActionType : are we generating for a movement type or checkPosition type ?
	
	*---------------------------------------------------------------------------------*/
	bool GetPossiblePositionsOnBoard(	Piece const& piece, 
										std::vector<PossibleMouvement> & PossibleMvt , 
										enActionType const & ActionType);



	/*--------------------------------------------------------------------------------

	* @brief generate the possible positions (movements) on the board, for the current piece
	* according to the game rules
	* @param board : the current board matrix state
	* @param possibleMvt : a vector of possible movements for the selected piece.
	* @param ActionType : are we generating for a movement type or checkPosition type ?

	*---------------------------------------------------------------------------------*/
	bool GenerateLegalMoves(	ChessCase const & cellCoords, 
								Board const & board, 
								std::vector<PossibleMouvement>& PossibleMvt, 
								enActionType const& ActionType);


	/*--------------------------------------------------------------------------------

	* @brief after releasing the current piece, we should check if the selected movement type is "piece captured"
	* to put the piece to the captured list of pieces.
	* @param piece : the current piece (selected piece by the player).

	*---------------------------------------------------------------------------------*/
	bool IsAnyPieceCaptured(Piece const& piece);



	/*--------------------------------------------------------------------------------

	* @brief check if this player is in check possition,
	* note : player 1 turn => we should check player2 pieces if they present a check position for player1
	* @param board : the current board matrix state
	* @parma palyerNum : is player1 turn or player2 ?
	* @return ture if the current player is in check position

	*---------------------------------------------------------------------------------*/
	bool IsPlayerInCheckPosition(Board const& board , enPlayerNum const & playerNum);



	/*--------------------------------------------------------------------------------

	* @brief the turn will be finished if movement is allowed in release function or if piece is captured
	* otherwise the player side should be kept as is.
	* note : maybe only the drag action is finished maybe the turn still present.
	* @return true if movement is finished then side should be changed
	
	*---------------------------------------------------------------------------------*/
	bool IsTurnFinished();


	/*--------------------------------------------------------------------------------

	* @brief get window's background color
	
	*---------------------------------------------------------------------------------*/
	Color GetBackgroundColor();



	/*--------------------------------------------------------------------------------

	* @brief update window's background color according to current window type

	*---------------------------------------------------------------------------------*/
	void UpdateBackgroundColor(enWindow const & window);



	
	/*--------------------------------------------------------------------------------

	* @brief convert cell coordinates to position  (i,j) to (x,y)
	* 	convert a Position type to a ChessCase type
	*	position : is the coordinates of that point on the window
	*	chessCase : is the coordinates of that case on the board (0 to 7)

	*---------------------------------------------------------------------------------*/
	ChessCase PosToCase(Position pos);


	/*--------------------------------------------------------------------------------

	* @brief convert x,y coordinates to cell coordinates  (x,y) to (i,j)
		convert a ChessCase type to Position type
		position : is the coordinates of that point on the window
		chessCase : is the coordinates of that case on the board (0 to 7)

	*---------------------------------------------------------------------------------*/
	Position CaseToPos(ChessCase CCase);



	//====================
	// m_borad fonctions
	//===================

	/*--------------------------------------------------------------------------------

	* @brief get (x,y) position from cell coordinates (i,j)
	* @param i,j : cell coordinates (row, col)
	* @return x,y coordinates 

	*---------------------------------------------------------------------------------*/
	Position GetCellPosition(int i, int j);


	/*--------------------------------------------------------------------------------

	* @brief get the central position for the corresponding cell case
	* @param i,j : cell coordinates (row, col)
	* @return x,y coordinates

	*---------------------------------------------------------------------------------*/
	Position GetCellCentralPosition(int i, int j);


	/*--------------------------------------------------------------------------------

	* @brief get the corresponding positon (x,y) from cell case name
	* @param caseName : cell case name exple : E2, A2 ...
	* @return x,y coordinates 

	*---------------------------------------------------------------------------------*/
	Position GetPostionFromBoardCaseName(std::string const &caseName) const;


	/*--------------------------------------------------------------------------------

	* @brief initialize the board info internal variable with default values

	*---------------------------------------------------------------------------------*/
	void InitBoardInfo();



	/*--------------------------------------------------------------------------------

	* @brief get the corresponding positon (x,y) from cell case name
	* @param i,j : cell coordinates on the board (row, col)
	* @return cell case name exple : E2, A2 ...

	*---------------------------------------------------------------------------------*/
	std::string GetCaseName(int i, int j);


	/*--------------------------------------------------------------------------------

	* @brief update the current cell case on the board with the curent piece informations (team side, piece id, ...)
	* @param piece : current piece
	* @param row,col : cell coordinates (i,j)
	* @param board : the borad internal varialbe

	*---------------------------------------------------------------------------------*/
	void UpdateCellInfo(Piece const& piece , int row, int col, Board &board);


	/*--------------------------------------------------------------------------------

	* @brief update the current cell case with the curent piece informations (team side, piece id, ...)
	* @param piece : current piece
	* @param row,col : cell coordinates (i,j)
	* @param board : the borad internal varialbe

	*---------------------------------------------------------------------------------*/
	void SetCellInfo(Piece const& piece, int row, int col, Board& board);



	/*--------------------------------------------------------------------------------

	* @brief update  the borad internal varialbe with new values (team side, piece id, ...)
	* @param player1Turn : current team turn

	*---------------------------------------------------------------------------------*/
	void UpdateBoardInfo(bool player1Turn);



	/*--------------------------------------------------------------------------------

	* @brief set the borad internal varialbe with initial values (team side, piece id, ...)
	* @param player1Turn : current team turn

	*---------------------------------------------------------------------------------*/
	void SetEmptyBoardInfo(bool player1Turn);



	/*--------------------------------------------------------------------------------

	* @brief update all cells of the internal board variable with new variables  (team side, piece id, ...)
	* @param player1Turn : current team turn

	*---------------------------------------------------------------------------------*/
	void SetNewValuesToBoardInfo(bool player1Turn);



	/*--------------------------------------------------------------------------------

	* @brief compare 2 cells from the internal board variable 
	* @param cell1,2 : cell1 & cell2

	*---------------------------------------------------------------------------------*/
	bool Compare2Cells(infoCase const& cell1, infoCase const& cell2);


	/*--------------------------------------------------------------------------------

	* @brief compare coordinates of 2 cells from the internal board variable
	* @param cell1,2 : cell1 & cell2
	* @return true if same coordinates

	*---------------------------------------------------------------------------------*/
	bool IsSameCoordinates(ChessCase const& cell1, ChessCase const& cell2);



	/*--------------------------------------------------------------------------------

	* @brief compare coordinates of 2 cells from the internal board variable
	* @param row,col : i,j coordinates for current cell case
	* @param caseName : case name E2,A4 ....
	* @return row & col

	*---------------------------------------------------------------------------------*/
	void GetBoardRowColFromCaseName(std::string caseName , int& row, int& col);



	/*--------------------------------------------------------------------------------

	* @brief get cell case coordinates (i,j) from a piece position (x,y)
	* @param row,col : i,j coordinates for current cell case
	* @param pos : piece's current position
	* @param beforePieceReleased : set the mode, in case we use this function after piece released the position of the piece is corrected (so we can take it)
	  otherwise we are before piece realeasd so we have to use the original position of the piece
	* @return row & col

	*---------------------------------------------------------------------------------*/
	void GetBoardRowColFromPiecePosition(Position const & pos, int& row, int& col , bool beforePieceReleased) const;



	/*--------------------------------------------------------------------------------

	* @brief print current board info on the terminal
	* @param infoType : the info name that we need to print (Position, Indexes, ....)
	* @param caseName : the needed board info variable
	* @return printed output as a string 

	*---------------------------------------------------------------------------------*/
	std::string PrintBoardQuickInfo(std::string infoType , Board const & board) const;





	/*--------------------------------------------------------------------------------

	* @brief add possible possition to corresponing vector "possbleMvt" for the current piece
	* @param piece : the current piece
	* @param ActionType : are we generating for a movement type or checkPosition type ?
	* @param PossibleMvt ! a vector of possible movements for the selected piece.
	* @param row,col : current coordinates (to be checked)

	* @return
	   -1 : obstacle present (enemy side but Roi) : its a check possition : cant move to this case
		0 : obstacle present (team side) : cant move to this case
		1 : no obstacle : a possible empty position can move to it
		2 : obstacle present (enemy side) : can capture this piece and move to its case (the piece is not Roi).

	*---------------------------------------------------------------------------------*/
	int  AddPossiblePossition(	enActionType const& ActionType ,
								Piece const& piece , 
								std::vector<PossibleMouvement> &PossibleMvt, 
								int const &row, 
								int const &col ) ;





	/*--------------------------------------------------------------------------------

	* @brief add possible possition to corresponing vector "possbleMvt" for the current piece 
	* @param board : the needed board info variable
	* @param ActionType : are we generating for a movement type or checkPosition type ?
	* @param PossibleMvt ! a vector of possible movements for the selected piece.
	* @param row,col : current coordinates (to be checked)
	* @param attackerCoords : attacker coordinates for checkCases
	* @return 
	   -1 : obstacle present (enemy side but Roi) : its a check possition : cant move to this case
		0 : obstacle present (team side) : cant move to this case
		1 : no obstacle : a possible empty position can move to it
		2 : obstacle present (enemy side) : can capture this piece and move to its case (the piece is not Roi).
	 
	*---------------------------------------------------------------------------------*/
	int AddPossiblePossitionFromBoard(  enActionType const& ActionType,
										Board const& board,
										std::vector<PossibleMouvement>& PossibleMvt,
										int const& row,
										int const& col,
										ChessCase const& attackerCoords);



	/*--------------------------------------------------------------------------------

	* @brief check if its about a capturable obstacle
	* @param row,col : current coordinates (to be checked)

	*---------------------------------------------------------------------------------*/
	bool IsCapturableObstacle(int const &row, int const &col) const;




	/*--------------------------------------------------------------------------------

	* @brief check if its about a capturable obstacle
	* @param row,col : current coordinates (to be checked)
	* @param board : the board internal variable

	*---------------------------------------------------------------------------------*/
	bool IsCapturableObstacle_2(int const& row, int const& col, Board const &board) const;




	/*--------------------------------------------------------------------------------

	* @brief check if its about an enemy obstacle
	* @param row,col : current coordinates (to be checked)

	*---------------------------------------------------------------------------------*/
	bool IsEnemyObstacle(int const& row, int const& col) const;



	/*--------------------------------------------------------------------------------

	* @brief check if its about an enemy obstacle
	* @param row,col : current coordinates (to be checked)
	* @param board : the board internal variable
	* @param attackerCoords : attacker coordinates for checkCases

	*---------------------------------------------------------------------------------*/
	bool IsEnemyObstacle_2(int const& row, int const& col, ChessCase const& attackerCoords , Board const& board) const;



	/*--------------------------------------------------------------------------------

	* @brief check if its about a check position
	* @param row,col : current coordinates (to be checked)
	* @param piece : the current piece

	*---------------------------------------------------------------------------------*/
	bool IsCheckPosition(int const&row , int const &col, Piece const& piece) const;



	/*--------------------------------------------------------------------------------

	* @brief check if its about a check position
	* @param row,col : current coordinates (to be checked)
	* @param board : the board internal variable
	* @param attackerCoords : attacker coordinates for checkCases

	*---------------------------------------------------------------------------------*/
	bool IsCheckPosition_2(int const& row, int const& col, ChessCase const& attackerCoords, Board const & board) const;

	/*
	* 	check if any possible position exists for this current piece 
	=> check values of the "PossibleMouvement" vector of this piece
	   if PossibleMouvement.MouvementType == -1 => check possition exists
	*/
	bool IsAnyCheckPossitionExists(std::vector<PossibleMouvement> const & PossibleMvt);  // for each piece



	/*--------------------------------------------------------------------------------

	* @brief get attacker cell case coordinates on the board
	* @param attackedSide : team side of that attacker 
	* @param board : the board internal variable
	* @return  attacker coordinates 

	*---------------------------------------------------------------------------------*/
	ChessCase GetAttackedKingCoordsOnTheBoard(bool const & attackedSide, Board const& board) const;


	/*
	* when having a check position the player cant move until he removes the check position
	* check if the next move is valid (check position is removed)
	* return true to allow the move / false to say that check position is still present
	* piece : current piece selected
	* nextPosition : the selected move by the player
	* board is m_bord (containing all positions of pieces)
	*/
	bool IsSelectedMoveLegal(Piece const &piece, ChessCase const & nextPosition , Board const & board , enPlayerNum const& playerNum);




	/*--------------------------------------------------------------------------------

	* @brief check if the current move is legal (valid : no check position + can move to this case)
	* @param move : current move to be checked
	* @return true if the move is legal

	*---------------------------------------------------------------------------------*/
	bool IsLegalMove(stMove const & move, Board const& board);



	/*--------------------------------------------------------------------------------

	* @brief fill move variable with corresponding informations
	* @return filled move variable

	*---------------------------------------------------------------------------------*/
	stMove SetMoveInfo(ChessCase const& fromCell, ChessCase const& toCell, int pieceTeamIndex ,enPlayerNum playerSide);



	/*--------------------------------------------------------------------------------

	* @brief get text of current move (for moves history)
	* @param move : current move to be checked
	* @return string describing current move according to game rules

	*---------------------------------------------------------------------------------*/
	std::string GetCurrentMoveText(stMove const & move);




	/*--------------------------------------------------------------------------------

	* @brief convert piece names from French to English
	* @param pieceName : french piece name
	* @return english piece name

	*---------------------------------------------------------------------------------*/
	std::string GetEnglishPieceName(std::string const& pieceName);


	/*
	* @brief validate the current move selected:
	* 1 - in case no check position =>  the move is valid
	* 2 - otherwise we should back to the original position of the piece (before the drag)
	* @param selectedMoveCell : the corresponding case to the draged piece (current move selected by the player),
	* this variable is modified and retrned
	* @return validated piece position
	*
	*/



	/*--------------------------------------------------------------------------------

	* @brief after check if the current move is legal we should take action according to given results
	* @param selectedMoveCell : current move 

	*---------------------------------------------------------------------------------*/
	void ValidateCurrentMove( ChessCase & selectedMoveCell );




	/*--------------------------------------------------------------------------------

	* @brief extract piece  informations from cell case (from board matrix)
	* @param cell : current cell case
	* @return piece team index and piece team side

	*---------------------------------------------------------------------------------*/
	stPiece GetPieceFromBoardCell(infoCase const& cell);



	/*--------------------------------------------------------------------------------

	* @brief check if any promotion is present, and promote it to queen

	*---------------------------------------------------------------------------------*/
	void PionPromotion();



	/*--------------------------------------------------------------------------------

	* @brief check if any check position is present
	* @return true check position is present

	*---------------------------------------------------------------------------------*/
	bool IsCheck();



	/*--------------------------------------------------------------------------------

	* @brief check if checkmate position is present
	* @return true checkmate position is present

	*---------------------------------------------------------------------------------*/
	bool IsCheckmat();



	/*--------------------------------------------------------------------------------

	* @brief check if pat (draw) position is present
	* @return true pat (draw) position is present

	*---------------------------------------------------------------------------------*/
	bool IsPat();  //<<********ToDo to be finished




	/*--------------------------------------------------------------------------------

	* @brief get indexes of remaining pieces (not captured pieces)
	* @return vector of indexes of reamaining pieces

	*---------------------------------------------------------------------------------*/
	std::vector<int> GetRemainingPiecesIds(enPlayerNum const & side);



	/*--------------------------------------------------------------------------------

	* @brief check if index is out of range (7x7)
	* @return true if index is valid

	*---------------------------------------------------------------------------------*/
	bool IsValidIdx(int const &row, int const &col) const;



	/*--------------------------------------------------------------------------------

	* @brief in case of any invalid index, generate an error

	*---------------------------------------------------------------------------------*/
	void ErrorIndex(int row, int col);

	// update moves history
	//-----------------------


	/*--------------------------------------------------------------------------------

	* @brief update movements history according to game rules

	*---------------------------------------------------------------------------------*/
	void UpdateMovesHistory();



	/*--------------------------------------------------------------------------------

	* @brief utility function to convert a string to lowercases
	* @return input in lower cases

	*---------------------------------------------------------------------------------*/
	std::string ToLowerCase(std::string& const str);


	//time
	//-------

	/*--------------------------------------------------------------------------------

	* @brief helpful function to calculate elapsed time for each player

	*---------------------------------------------------------------------------------*/
	void InitT1();


	/*--------------------------------------------------------------------------------

	* @brief update tempo (timer) for each player
	
	*---------------------------------------------------------------------------------*/
	void UpdateTempo();



	/*--------------------------------------------------------------------------------

	* @brief update buttons states on the lateral screen

	*---------------------------------------------------------------------------------*/
	void UpdateButtons();



	// text
	//-----------

	/*--------------------------------------------------------------------------------

	* @brief draw player turn text

	*---------------------------------------------------------------------------------*/
	void DrawPlayerTurn();


	/*--------------------------------------------------------------------------------

	* @brief draw state text, check, checkmate, draw ...

	*---------------------------------------------------------------------------------*/
	void DrawState();



	/*--------------------------------------------------------------------------------

	* @brief draw timer for both players & system date time

	*---------------------------------------------------------------------------------*/
	void DrawTime();



	/*--------------------------------------------------------------------------------

	* @brief draw move history text (4 last moves)

	*---------------------------------------------------------------------------------*/
	void DrawLastMoves();



	/*--------------------------------------------------------------------------------

	* @brief draw board letters A1, F1 ....

	*---------------------------------------------------------------------------------*/
	void DrawLetters();
	


	/*--------------------------------------------------------------------------------

	* @brief draw board letters A1, F1 ....

	*---------------------------------------------------------------------------------*/
	void DrawButtons();


	/*--------------------------------------------------------------------------------

	* @brief draw all texts 

	*---------------------------------------------------------------------------------*/
	void DrawLateralTexts();

	// time
	//----------

	/*--------------------------------------------------------------------------------

	* @brief get system date
	* @return system date hh:min:sec in stDate format

	*---------------------------------------------------------------------------------*/
	stDate GetSystemDateTime(void);



	/*--------------------------------------------------------------------------------

	* @brief convert Date to string
	* @param sep : separator between min and seconds, default ":" 
	* @return system date in string format

	*---------------------------------------------------------------------------------*/
	std::string GetDateTimeToString(stDate const& date, std::string sep = ":");



	/*--------------------------------------------------------------------------------

	* @brief convert seconds to duration
	* @param seconds : number of seconds
	* @return stDuration

	*---------------------------------------------------------------------------------*/
	stDuration SecondsToDuration(time_t seconds);



	/*--------------------------------------------------------------------------------

	* @brief convert duration to string
	* @param sep : separator between min and seconds, default ":" 
	* @return duration in string format

	*---------------------------------------------------------------------------------*/
	std::string GetDurationToString(stDuration const& duration , std::string sep = ":");


	/*--------------------------------------------------------------------------------

	* @brief set duration in min & sec
	* @param duration structure

	*---------------------------------------------------------------------------------*/
	void setDuration(stDuration& duration, int const& min, int const& sec);


	/*--------------------------------------------------------------------------------

	* @brief check if timer of any player is over.
	* @return true if any timer is = to 0 seconds

	*---------------------------------------------------------------------------------*/
	bool IsTimeOver();

	// cpu
	//---------

	/*--------------------------------------------------------------------------------

	* @brief generate CPU (black team) move automaticaly

	*---------------------------------------------------------------------------------*/
	bool GenerateCPUMove();



	/*--------------------------------------------------------------------------------

	* @brief check if its CPU (black team) turn
	* @return true if this turn is the CPU turn

	*---------------------------------------------------------------------------------*/
	bool IsCPUTurn();



	/*--------------------------------------------------------------------------------

	* @brief load settings from the first configuration before the game

	*---------------------------------------------------------------------------------*/
	void LoadSettings();



	/*--------------------------------------------------------------------------------

	* @brief Save game steps on a text file (all board states)

	*---------------------------------------------------------------------------------*/
	void SaveGameSteps();

	void GameOver();
};


/*check mat
* 1 player1 turn + player1 in check // should be the first iff
* 2 all roi positions are invalide + ( its own position // no need)
* 3 all player1 pieces possible positions are invalid
*/

/* rock roi side : to be added to roi possible positions
* 1 selectedPiece = king // no need
* 2 king never moved && tour 13 never moved
* 3 empty cells between king & tour && no check on this cases
* 4 king moves to cavalier 11 case
* 5 tour moves to fou 9 case
*/

// isPat
// priseEnPassant
// 3 times same position
// materiel insufisant roi vs roi

// x temp, fin de partie en temps,
// son, mvt, echec, mat
// orientation du plateaux

// x historique des coups,
// annuler / refaire un coups / redemarrer la partie / abondon
// sauvegarde / chargement de partie

// x bug  pion 2 cases + obstacle
// x bug time white while settinngs
// bug in start new game (check renit board)

/* setings
* sound on/off	x
* time player   x
* theme			x
* with cpu ?	x
* save file txt 
* show possible mvt
*/


/* ===> reste
  x 1- start new game
	2- game over window
  x 3- change theme during game
	4- rock
	5- promotion full feature
	6- draw (3times ,...)
  x 7- finish settings
  x 8- change chess construtor (no need de c1,c2, make it internalà
	9- son de chargement / win /
*/

/*
* @brief → Description courte, obligatoire.

@param → Pour chaque paramètre.

@return → Pour les fonctions non-void.

@throw → Si la fonction peut lever une exception.

@code ... @endcode → Exemple d’utilisation (très apprécié).
*/

