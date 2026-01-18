#pragma once

#include "Game.h"
#include "Grid.h"
#include "Piece.h"
#include "raylib.h"
#include "Position.h"
#include "ChessCase.h"
#include <string>
#include <iomanip> // for setw() : cout
#include <array>


#define RED   "\033[31m"
#define BLUEE  "\033[34m"
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define WHITE "\033[37m"

class Chess : public Game
{
private:
	struct PossibleMouvement {
		Position possiblePosition;
		ChessCase possibleCase;
		int movementType;
		bool canMove;
	};
	
	enum enActionType {
		MOUVEMENT = 0,
		CHECK_CASES = 1,
	};

	enum enPlayerNum {
		PLAYER1 = 0,
		PLAYER2 = 1,
	};

	struct stPiece {
		bool pieceTeamSide;
		int pieceTeamIdx;
	};

	Grid* grid;
	Piece* piece;

	//window
	int windowWidth, windowHeigh;
	int gameFps;
	Image m_screenshot;

	//grid
	int numRows;
	int numCols;
	int cellSize;
	int leftMargin, topMargin;
	Color color1, color2, hoveredColor;

	//pieces
	std::vector<std::unique_ptr<Piece>> player1, player2;
	int selectdPieceID;;
	Position selectedPieceOriginalPos;

	std::vector<PossibleMouvement> m_possibleMouvement;


	//Board case
	struct infoCase
	{
		bool empty;
		int idCell;
		int idPiece;
		int pieceTeamIndex;
		Position cellPosition;
		Position cellCentralPosition;
		ChessCase cellCoordinates;
		std::string pieceName;
		std::string caseName;
		int playerSide;
		//bool caseHovered;
		infoCase(); // construtor
		void setToEmpty();
	};
	//infoCase m_Board[8][8];
	using Board = std::array <std::array<infoCase, 8>, 8>;  // using = type def (modern) : Boerd is the new type 
	Board m_board , m_Initial_Board;

	//flags
	bool    flag_isPlayer1Turn,
			flag_leftMouseButtonPressed,
			flag_rightMouseButtonPressed,
			flag_leftMouseButtonDown,
			flag_leftMouseButtonReleased,
			flag_isAnyPieceSelected,
			flag_player1InCheck,
			flag_player2InCheck,
			flag_isMovementAllowed,
			flag_isAnyPieceCaptured,
			flag_possibleMouvemntsAreCalculated;


public :
	Chess(int w, int h, int fps, int nRows, int nCols, int cSize, int lMargin, int tMargin, Color c1, Color c2);
	~Chess();

	//----------------------
	// principal functions
	//----------------------
	void Init();  
	void Input();
	void Update(); 
	void Draw();

	//----------------------
	// additional functions
	//----------------------
	void setGameFPS(int fps);
	void DrawPlayerPieces(std::vector<std::unique_ptr<Piece>> const &player);
	void MovePieceByStep(Piece& piece, Position step);
	void MovePieceToNewPos(Piece& piece, Position newPos);
	void MovePieceToNewCase(Piece& piece, ChessCase CCase);
	void ChangeTurn();

	void DragPiece();
	void ReleasePiece();
	void CorrectPiecePosition(std::vector<std::unique_ptr<Piece>> & player);
	int GetSelectedPiece(std::vector<std::unique_ptr<Piece>> const &player);
	void SetCenterPieceToCursorPosition(Piece& piece , Position const &cursorPos);
	Position GetCorrespondingBoardCase(Position centerPos);
	std::vector<std::unique_ptr<Piece>> InitPlayersPieces(bool player1Side);
	void SetPlayerInitialPositions(std::vector<std::unique_ptr<Piece>>& vectOfPieces, std::string caseNames[]);
	void DrawPossiblePositions(Color color);
	bool GetPossiblePositionsOnBoard(	Piece const& piece, 
										std::vector<PossibleMouvement> & PossibleMvt , 
										enActionType const & ActionType);
	bool GetPossiblePositionsOnBoardFromBoard(	ChessCase const & cellCoords, 
												Board const & board, 
												std::vector<PossibleMouvement>& PossibleMvt, 
												enActionType const& ActionType);
	bool  IsAnyPieceCaptured(Piece const& piece);
	bool IsPlayerInCheckPosition(Board const& board , enPlayerNum const & playerNum);
	bool IsTurnFinished();

	//conversion cell to pos
	ChessCase PosToCase(Position pos);
	Position CaseToPos(ChessCase CCase);


	// m_borad fonctions
	Position GetCellPosition(int i, int j);
	Position GetCellCentralPosition(int i, int j);
	Position GetPostionFromBoardCaseName(std::string const &caseName) const;
	void InitBoardInfo();
	std::string GetCaseName(int i, int j);
	void UpdateCellInfo(Piece const& piece , int row, int col, Board &board);
	void SetCellInfo(Piece const& piece, int row, int col, Board& board);
	void UpdateBoardInfo(bool player1Turn);
	void SetEmptyBoardInfo(bool player1Turn);
	void SetNewValuesToBoardInfo(bool player1Turn);
	bool Compare2Cells(infoCase const& cell1, infoCase const& cell2);
	bool IsSameCoordinates(ChessCase const& cell1, ChessCase const& cell2);
	void GetBoardRowColFromCaseName(std::string caseName , int& row, int& col);
	void GetBoardRowColFromPiecePosition(Position const & pos, int& row, int& col , bool beforePieceReleased) const;
	void PrintBoardQuickInfo(std::string infoType , Board const & board) const;
	int  AddPossiblePossition(	enActionType const& ActionType ,
								Piece const& piece , 
								std::vector<PossibleMouvement> &PossibleMvt, 
								int const &row, 
								int const &col ) ;
	int AddPossiblePossitionFromBoard(  enActionType const& ActionType,
										Board const& board,
										std::vector<PossibleMouvement>& PossibleMvt,
										int const& row,
										int const& col,
										ChessCase const& attackerCoords);
	bool IsCapturableObstacle(int const &row, int const &col) const;
	bool IsCapturableObstacle_2(int const& row, int const& col, Board const &board) const;
	bool IsEnemyObstacle(int const& row, int const& col) const;
	bool IsEnemyObstacle_2(int const& row, int const& col, ChessCase const& attackerCoords , Board const& board) const;
	bool IsCheckPosition(int const&row , int const &col, Piece const& piece) const;
	bool IsCheckPosition_2(int const& row, int const& col, ChessCase const& attackerCoords, Board const & board) const;
	bool IsAnyCheckPossitionExists(std::vector<PossibleMouvement> const & PossibleMvt);  // for each piece
	ChessCase GetAttackedKingCoordsOnTheBoard(bool const & attackedSide, Board const& board) const;
	bool IsNextMoveValid(Piece const &piece, ChessCase const & nextPosition , Board const & board , enPlayerNum const& playerNum);

	void ValidateCurrentMove( ChessCase & selectedMoveCell );
	stPiece GetPieceFromBoardCell(infoCase const& cell);

	bool IsValidIdx(int const &row, int const &col) const;
	void ErrorIndex(int row, int col);
};


