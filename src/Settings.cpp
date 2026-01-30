#include "Settings.h"
#include "Settings.h"


bool Settings::IsClicked(Vector2 const& cursorPos, Rectangle const& objectOutlineRect, bool leftMouseBtnClicked)
{
	if (leftMouseBtnClicked)
		return  CheckCollisionPointRec(cursorPos, objectOutlineRect);
	else
		return false;
}

Settings::Settings()
{
	// init default values

	Init();
}

Settings::~Settings()
{
}

Settings::GameMode Settings::GetGameMode()
{
	return m_gameMode;
}

Settings::Theme Settings::GetTheme()
{
	return m_theme;
}

bool Settings::GetIsCpuON()
{
	return m_flag_isCpuON;
}

bool Settings::GetIsSoundOn()
{
	return m_flag_isSoundON;
}

void Settings::Init()
{

	// init default values
	// cpu ON
	// theme = blue
	// time = 5min
	// sound ON

	m_flag_isCpuON = true;
	m_flag_isSoundON = true;
	m_gameMode = GameMode::Blitz;
	m_theme = Theme::Blue;

	// init internal variables
	m_backgroundColor = BLACK;
	m_flag_leftMouseButtonPressed = false;
	m_flag_isSlectionDone = false;
}

void Settings::Input()
{

	// LEFT mouse mouse PRESSED
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		m_flag_leftMouseButtonPressed = true;
	}
}

void Settings::Update()
{
	if (m_flag_leftMouseButtonPressed) // selection attempt
	{
		
		// cpu
		UpdateCpuMode();

		// mode
		UpdateGameMode();

		// sound
		UpdateSound();

		// theme
		UpdateTheme();


		// apply btn
		UpdateApplySettings();

		// init flag
		m_flag_leftMouseButtonPressed = false;

	}
}

void Settings::Draw()
{
	// cpu
	DrawRectangle(0, 0, 100, 100, GREEN);  
	DrawRectangle(100, 0, 100, 100, RED);

	// sound
	DrawRectangle(0, 150, 100, 100, GREEN);
	DrawRectangle(100, 150, 100, 100, RED);

	// theme
	DrawRectangle(0, 300, 200, 200, BLUE);
	DrawRectangle(250, 300, 200, 200, BROWN);
	DrawRectangle(500, 300, 200, 200, MAGENTA);


	// mode
	DrawRectangle(0, 550, 100, 100, YELLOW);
	DrawRectangle(150, 550, 100, 100, GRAY);
	DrawRectangle(300, 550, 100, 100, ORANGE);

	// apply btn
	DrawRectangle(500, 0, 100, 50, GREEN);
}

void Settings::UpdateCpuMode()
{
	// cpu  ON OFF
	Rectangle rectCpuON{ 0, 0 , 100, 100};
	Rectangle rectCpuOFF{ 100, 0 , 100, 100 };

	if (IsClicked(GetMousePosition(), rectCpuON, m_flag_leftMouseButtonPressed))
		m_flag_isCpuON = true;
	if (IsClicked(GetMousePosition(), rectCpuOFF, m_flag_leftMouseButtonPressed))
		m_flag_isCpuON = false;

	std::cout << "cpu on ? : " << m_flag_isCpuON << std::endl;
}

void Settings::UpdateSound()
{
	// sound  ON OFF
	Rectangle rectSoundON{ 0, 150 , 100, 100 };
	Rectangle rectSoundOFF{ 100, 150 , 100, 100 };

	if (IsClicked(GetMousePosition(), rectSoundON, m_flag_leftMouseButtonPressed))
		m_flag_isSoundON = true;
	if (IsClicked(GetMousePosition(), rectSoundOFF, m_flag_leftMouseButtonPressed))
		m_flag_isSoundON = false;

	std::cout << "sound on ? : " << m_flag_isSoundON << std::endl;

}

void Settings::UpdateTheme()
{
	// theme1 theme2 theme3  (200x200 px) + 50space
	Rectangle rectTheme1{ 0, 300 , 200, 200 };
	Rectangle rectTheme2{ 250, 300 , 200, 200 };
	Rectangle rectTheme3{ 500, 300 , 200, 200 };

	if (IsClicked(GetMousePosition(), rectTheme1, m_flag_leftMouseButtonPressed))
		m_theme = Theme::Blue;
	if (IsClicked(GetMousePosition(), rectTheme2, m_flag_leftMouseButtonPressed))
		m_theme = Theme::Brown;
	if (IsClicked(GetMousePosition(), rectTheme3, m_flag_leftMouseButtonPressed))
		m_theme = Theme::Magenta;

	std::string r = (m_theme == Theme::Blue) ? "blue" : (m_theme == Theme::Brown ? "brown" : "magenta");
	std::cout << "theme type ? : " << r << std::endl;
}

void Settings::UpdateGameMode()
{
	// game mode  (100x100 px) + 50space
	Rectangle rectBulet{ 0, 550 , 100, 100 };
	Rectangle rectBlitz{ 150, 550 , 100, 100 };
	Rectangle rectRapid{ 300, 550 , 100, 100 };

	if (IsClicked(GetMousePosition(), rectBulet, m_flag_leftMouseButtonPressed))
		m_gameMode = GameMode::Bulet;
	if (IsClicked(GetMousePosition(), rectBlitz, m_flag_leftMouseButtonPressed))
		m_gameMode = GameMode::Blitz;
	if (IsClicked(GetMousePosition(), rectRapid, m_flag_leftMouseButtonPressed))
		m_gameMode = GameMode::Rapid;


	std::string r = (m_gameMode == GameMode::Blitz) ? "blitz" : (m_gameMode == GameMode::Bulet ? "bulet" : "rapid");
	std::cout << "game mode  : " << r << std::endl;

}

void Settings::UpdateApplySettings()
{
	// ok button
	Rectangle applyBtn{ 500, 0 , 100, 50 };
	if (IsClicked(GetMousePosition(), applyBtn, m_flag_leftMouseButtonPressed))
		m_flag_isSlectionDone = true;
}

bool Settings::IsSelectionDone()
{
	return m_flag_isSlectionDone;
}
