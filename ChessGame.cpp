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

	Color	brown = { 128, 43, 0 ,255},
			brown2 = { 225, 135, 64, 255 },
			beige = { 255, 204, 156,255 },
			/// theme 2
			ligthBlue = { 204, 204, 255, 255 }, // with blue
			vdarkBlue = { 0, 0, 102 , 255},

			color1 = brown2,
			color2 = beige,
			background = brown;
	int w = 800, h = 620, nRows = 8, nCols = 8, cSize = 75, fps = 60;


	Chess chessGame(w, h, fps, nRows, nCols, cSize , color1, color2); 

	// tests
	Image queenIm = LoadImage("assets/images/black_queen.png");
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
		ClearBackground(background);
		chessGame.Draw();
		//DrawTexture(queen, 10 + 75 * 4 , 10 + 75 * 7 , YELLOW);
		//DrawTexture(queen, 10 + 75 * 4, 10 + 75 * 0, RED);
		//DrawTexture(queen, 10 + 75 * 7, 10 + 75 * 7, GRAY);
		EndDrawing();

	}

	UnloadTexture(queen);
	CloseWindow();
}