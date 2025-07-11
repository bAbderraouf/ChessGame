#pragma once

#include "Game.h"
#include "Grid.h"
#include "Piece.h"
#include "raylib.h"
#include "Position.h"
#include <string>

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
	void AddPieceToGrid(Piece& const piece );
	void EreasePieceFromGrid(Piece& const piece);
	void DrawPlayerPieces(std::vector<std::unique_ptr<Piece>> const &player);
	std::vector<std::unique_ptr<Piece>> InitPlayersPieces(bool player1Side);
	void MovePiece(Piece& piece, Position step);
	void DragPiece();
	void ReleasePiece();
	int GetSelectedPiece(std::vector<std::unique_ptr<Piece>> const &player);
	void SetCenterPieceToCursorPosition(Piece& piece , Position const &cursorPos);
	Position GetCorrespondingBoardCase(Position centerPos);

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
	Color color1, color2;

	//pieces
	std::vector<std::unique_ptr<Piece>> player1, player2;
	int selectdPieceID;

	//Board case
	struct infoCase 
	{
		bool empty;
		int idPiece;
		std::string pieceName;
		std::string caseName;
		bool caseHovered;
		bool playerSide;
	};
	infoCase strCaseInfo;
	infoCase board[8][8];

	//flags
	bool    flag_isPlayer1Turn,
			flag_leftMouseButtonPressed,
			flag_rightMouseButtonPressed,
			flag_leftMouseButtonDown,
			flag_leftMouseButtonReleased,
			flag_isAnyPieceSelected;
	
};


