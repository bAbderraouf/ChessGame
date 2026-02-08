#pragma once

#include "GameOver.h"
#include "raylib.h"
#include <memory>
#include <iostream>
#include "Drawable.h"
#include "Utilis.h"

/**
* @brief this class goal is to represent game over window
*/
class GameOver
{
private:

	// drawable object
	std::unique_ptr<Drawable> lastMoveSShot;
	std::unique_ptr<Drawable> winnerPic;
	std::unique_ptr<Drawable> winnerText;
	std::unique_ptr<Drawable> restartGameBtn;

	//window size
	Vector2 m_windowSize;

	// positions
	Vector2 posSShot, posWinnerPic, posWinnerTxt, posRestartBtn;

	std::string winnerPicFileName;

	Winner m_whoWins;

	// font
	int m_fontSize;

	// flags
	bool m_flag_leftMouseButtonPressed;
	bool m_flag_isRestartGameRequested;

public:
	GameOver() = delete;
	GameOver(Vector2 const& windowSize, Winner const& winner);
	~GameOver();

	void Init();
	void Input();
	void Update();
	void Draw();

	// calculate settings

	void SetWindowSize(Vector2 const& size);
	bool IsRestartGameRequested();
	void UpdateRestartGameRequest();

private:
	void GetWinnerPicFileName();

};



