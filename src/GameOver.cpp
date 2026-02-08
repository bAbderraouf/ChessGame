#include "GameOver.h"

GameOver::GameOver(Vector2 const& windowSize, Winner const& winner)
{
	m_windowSize = windowSize;
	m_whoWins = winner;

	// init default values
	Init();
}

GameOver::~GameOver()
{
}



void GameOver::Init()
{
	//flgs
	m_flag_leftMouseButtonPressed = false;
	m_flag_isRestartGameRequested = false;

	// font
	m_fontSize = 20;

	// positions
	posSShot = { 0 , 0 };			// background
	posWinnerPic = { 75 , 75 };
	posWinnerTxt = { m_windowSize.x / 2 - 100 , posWinnerPic.y + 500 + 10 };
	posRestartBtn = { m_windowSize.x - 180 ,  m_windowSize.y - 60 };

	GetWinnerPicFileName();

	// drawable objects
	lastMoveSShot = std::make_unique<ImageObject>(posSShot, "assets/images/capture/capture.png");
	winnerPic = std::make_unique<ImageObject>(posWinnerPic, winnerPicFileName.c_str());
	winnerText = std::make_unique<TextObject>(posWinnerTxt, "White wins!", m_fontSize);
	restartGameBtn = std::make_unique<MixedObject>(posRestartBtn, "Restart", m_fontSize, "assets/images/buttons/restartGame.png", 5);
}

void GameOver::Input()
{
	// LEFT mouse mouse PRESSED
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		m_flag_leftMouseButtonPressed = true;
	}
}

void GameOver::Update()
{
	// check if restart is requested
	UpdateRestartGameRequest();

	// init flag
	m_flag_leftMouseButtonPressed = false;
}

void GameOver::Draw()
{
	lastMoveSShot->Draw();
	winnerPic->Draw();
	//winnerText->Draw();
	restartGameBtn->Draw();
}

void GameOver::SetWindowSize(Vector2 const& size)
{
	m_windowSize = size;
}


void GameOver::UpdateRestartGameRequest()
{
	restartGameBtn->Update(m_flag_leftMouseButtonPressed);
	if (restartGameBtn->IsClicked())
	{
		restartGameBtn->SetClicked(false);
		m_flag_isRestartGameRequested = true;
	}
}

void GameOver::GetWinnerPicFileName()
{
	// get winnerPicFileName
	switch (m_whoWins)
	{
	case Winner::Black:
		winnerPicFileName = "assets/images/buttons/blackWins.png";
		break;

	case Winner::White:
		winnerPicFileName = "assets/images/buttons/whiteWins.png";
		break;

	case Winner::Draw:
		winnerPicFileName = "assets/images/buttons/draw.png";
		break;
	}
}

bool GameOver::IsRestartGameRequested()
{
	return m_flag_isRestartGameRequested;
}
