#pragma once

#include "Game.h"
#include "Grid.h"
#include "Piece.h"
#include "raylib.h"
#include "Position.h"
#include "ChessCase.h"
#include <string>
#include <iomanip> // for setw() : cout

class Chess : public Game
{
private:
	struct PossibleMouvement {
		Position possiblePosition;
		int movementType;
	};

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

	void DragPiece();
	void ReleasePiece();
	void CorrectPiecePosition(std::vector<std::unique_ptr<Piece>> & player);
	int GetSelectedPiece(std::vector<std::unique_ptr<Piece>> const &player);
	void SetCenterPieceToCursorPosition(Piece& piece , Position const &cursorPos);
	Position GetCorrespondingBoardCase(Position centerPos);
	std::vector<std::unique_ptr<Piece>> InitPlayersPieces(bool player1Side);
	void SetPlayerInitialPositions(std::vector<std::unique_ptr<Piece>>& vectOfPieces, std::string caseNames[]);
	void DrawPossiblePositions(Color color);
	bool GetPossiblePositionsOnBoard(Piece const& piece, std::vector<PossibleMouvement> & PossibleMvt);
	int  CheckIfAnyPossiblePosIsSelected(Piece const& piece);
	bool isCheckPosition();
	void FillCheckPositions( );

	//conversion cell to pos
	ChessCase PosToCase(Position pos);
	Position CaseToPos(ChessCase CCase);


	// m_borad fonctions
	Position GetCellPosition(int i, int j);
	Position GetCellCentralPosition(int i, int j);
	Position GetPostionFromBoardCaseName(std::string const &caseName) const;
	void InitBoardInfo();
	std::string GetCaseName(int i, int j);
	void UpdateCellInfo(Piece const& piece , int row, int col);
	void UpdateBoardInfo(bool player1Turn);
	void SetEmptyBoardInfo(bool player1Turn);
	void SetNewValuesToBoardInfo(bool player1Turn);
	void GetBoardRowColFromCaseName(std::string caseName , int& row, int& col);
	void GetBoardRowColFromPiecePosition(Position const & pos, int& row, int& col , bool beforePieceReleased) const;
	void PrintBoardQuickInfo(std::string infoType) const;
	int  AddPossiblePossition(Piece const& piece , std::vector<PossibleMouvement> &PossibleMvt, int const &row, int const &col ) ;
	bool IsCapturableObstacle(int const &row, int const &col) const;
	bool IsEnemyObstacle(int const& row, int const& col) const;
	bool IsCheckPosition(int const&row , int const &col) const;

private:
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
	Color color1, color2 , hoveredColor;

	//pieces
	std::vector<std::unique_ptr<Piece>> player1, player2;
	int selectdPieceID;
	Position selectedPieceOriginalPos;

	std::vector<PossibleMouvement> m_possibleMouvement;
	//std::vector<Position> m_selectedPiecePossiblePositions;
	//std::vector<int> m_selectedPieceMovementTypes;

	std::vector<int> m_player1InCheckCases;
	std::vector<int> m_player2InCheckCases;
	int m_selectedPieceMovementType;
	

	//Board case
	struct infoCase 
	{
		bool empty;
		int idCell;
		int idPiece;
		int pieceTeamIndex;
		Position cellPosition;
		Position cellCentralPosition;
		std::string pieceName;
		std::string caseName;
		int playerSide;
		//bool caseHovered;
		infoCase(); // construtor
		void setToEmpty();
	};
	infoCase m_Board[8][8];

	//flags
	bool    flag_isPlayer1Turn,
		flag_leftMouseButtonPressed,
		flag_rightMouseButtonPressed,
		flag_leftMouseButtonDown,
		flag_leftMouseButtonReleased,
		flag_isAnyPieceSelected,
		flag_drawPosiblePositions,
		flag_player1InCheck,
		flag_player2InCheck,
		flag_possibleMouvemntsAreCalculated;
	
};


