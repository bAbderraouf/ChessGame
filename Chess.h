#pragma once

#include "Game.h"
#include "Grid.h"
#include "Piece.h"
#include "raylib.h"
#include "Position.h"
#include <string>
#include <iomanip> // for setw() : cout

class Chess : public Game
{
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
	void MovePiece(Piece& piece, Position step);
	void DragPiece();
	void ReleasePiece();
	void CorrectPiecePosition(std::vector<std::unique_ptr<Piece>> & player);
	int GetSelectedPiece(std::vector<std::unique_ptr<Piece>> const &player);
	void SetCenterPieceToCursorPosition(Piece& piece , Position const &cursorPos);
	Position GetCorrespondingBoardCase(Position centerPos);
	std::vector<std::unique_ptr<Piece>> InitPlayersPieces(bool player1Side);
	void SetPlayerInitialPositions(std::vector<std::unique_ptr<Piece>>& vectOfPieces, std::string caseNames[]);
	void DrawPossiblePositions(Color color);
	std::vector<Position> GetPossiblePositionsOnBoard(Piece const & piece) const;

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
	void GetBoardRowColFromPiecePosition(Position const & pos, int& row, int& col) const;
	void PrintBoardQuickInfo(std::string infoType) const;
	int AddPossiblePossition(std::vector<Position>& positions, Piece const& piece, int const &row, int const &col ) const;
	bool IsCapturableObstacle(int const &row, int const &col) const;

private:
	Grid* grid;
	Piece* piece;

	//window
	int windowWidth, windowHeigh;
	int gameFps;

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
	std::vector<Position> m_selectedPiecePossiblePositions;

	//Board case
	struct infoCase 
	{
		bool empty;
		int idCell;
		int idPiece;
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
		flag_drawPosiblePositions;

	
};


