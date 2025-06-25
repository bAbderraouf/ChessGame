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

	Chess(int w, int h, int fps, int nRows, int nCols, int cSize);
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
	
};


