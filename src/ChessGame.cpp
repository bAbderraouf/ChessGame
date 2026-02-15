#include "raylib.h"
#include "Grid.h"
#include "Position.h"
#include "Game.h"
#include "Chess.h"
#include <iostream>
#include <vector>



int main()
{
	float scaleX = 0.6, scaleY = 0.85;
	int cSize = 75, winH = 0, winW = 0;				/// grid CellSize,  window height & width

	GetWindowDimensions(winH, winW, scaleX, scaleY);

	Chess chessGame(winH, winW, cSize);

	while (WindowShouldClose() == false)
	{
		//--------------
		// check inputs 
		//--------------
		chessGame.Input();

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