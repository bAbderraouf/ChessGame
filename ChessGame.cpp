#include "raylib.h"
#include "Grid.h"
#include "Position.h"
#include "Game.h"
#include "Chess.h"
#include <iostream>
#include <vector>



int main()
{
	double	lastTime = 0,
		currentTime = 0,
		timeInterval = 0.25;
	Color brown = { 165, 42, 42 , 255 };

				// (w, h, fps, nRows,nCols, cSize)
	Chess chessGame(800, 620, 60, 8, 8, 75); 

	// tests
	Image queenIm = LoadImage("assets/images/pion.png");
	ImageResize(&queenIm, queenIm.width * 0.75, queenIm.height * 0.75);
	Texture queen = LoadTextureFromImage(queenIm);


	while (WindowShouldClose() == false)
	{
		// check inputs 
		chessGame.Input();

		//update gameMusic
		//UpdateMusicStream(chessGame.GetGameMusic());

		// update
		currentTime = GetTime();
		if (currentTime - lastTime > timeInterval)
		{
			lastTime = currentTime;
			chessGame.Update();
		}

		// draw
		BeginDrawing();
		ClearBackground(DARKBLUE);
		chessGame.Draw();
		DrawTexture(queen, 10 + 75 * 4 , 10 + 75 * 7 , YELLOW);
		DrawTexture(queen, 10 + 75 * 4, 10 + 75 * 0, RED);
		DrawTexture(queen, 10 + 75 * 7, 10 + 75 * 7, GRAY);
		EndDrawing();

	}

	UnloadTexture(queen);
	CloseWindow();
}