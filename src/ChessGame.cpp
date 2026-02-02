#include "raylib.h"
#include "Grid.h"
#include "Position.h"
#include "Game.h"
#include "Chess.h"
#include <iostream>
#include <vector>



int main()
{
	double	lastTime = 0, currentTime = 0, 	timeInterval = 0.25; 

	int w = 800, h = 625, nRows = 8, nCols = 8, cSize =75, fps = 60 , leftMargin = 10, topMargin = 10;

	//--------------------
	// window : w, h, fps  
	//----------------------------------------------------------------------
	// grid : nRows, nCols, cSize ,leftMargin, topMargin
	//----------------------------------------------------------------------
	Chess chessGame(w, h, fps, nRows, nCols, cSize, leftMargin, topMargin);

	// tests
	Image queenIm = LoadImage("assets/images/black_queen.png");
	ImageResize(&queenIm, queenIm.width * 0.75, queenIm.height * 0.75);
	Texture queen = LoadTextureFromImage(queenIm);


	while (WindowShouldClose() == false)
	{
		//--------------
		// check inputs 
		//--------------
		chessGame.Input();

		//-----------------
		//update gameMusic
		//-----------------
		//UpdateMusicStream(chessGame.GetGameMusic());

		//---------
		// update
		//---------
		// Set Game speed 
		// (no need to set game speed in chess game)
		//currentTime = GetTime();
		//if (currentTime - lastTime > timeInterval)
		{
			//lastTime = currentTime;
			chessGame.Update();
		}

		//---------
		// draw
		//---------
		BeginDrawing();
		ClearBackground(chessGame.GetBackgroundColor());
		chessGame.Draw();
		EndDrawing();

	
	}

	CloseWindow();
}