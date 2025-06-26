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

	Chess(int w, int h, int fps, int nRows, int nCols, int cSize, Color c1, Color c2);
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
	Color color1, color2;

	//pieces
	std::vector<std::unique_ptr<Piece>> player1, player2;
	
};


