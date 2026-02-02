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
	m_flag_isSaveON = true;
	m_gameMode = GameMode::Blitz;
	m_theme = Theme::Blue;

	// init internal variables
	m_backgroundColor = BLACK;
	m_flag_leftMouseButtonPressed = false;
	m_flag_isSlectionDone = false;


	// font
	m_fontSize = 20;

	// positions
	posT1 =  { 50,100 };
	posT2 =  { 250,100 };
	posT3 =  { 450,100 };

	posSoundON = { 50,300 };
	posSoundOFF  = { 250,300 };

	posCpuOn = { 50,350 };
	posCpuOFF = { 250,350 };

	posBulet = { 50,400 };
	posBlitz = { 250,400 };
	posRapid = { 460,400 };

	posSaveON = { 50,450 };
	posSaveOFF = { 250,450 };

	posBtnOK = { 690,550 };

	//drawable object

	imTheme1 = std::make_unique<ImageObject>(posT1, "assets/images/theme/Blue.png");
	imTheme2 = std::make_unique<ImageObject>(posT2, "assets/images/theme/Brown.png");
	imTheme3 = std::make_unique<ImageObject>(posT3, "assets/images/theme/Magenta.png");
	imBtnOK = std::make_unique<ImageObject>(posBtnOK, "assets/images/buttons/check_60.png");

	txtSoundON	= std::make_unique<TextObject>(posSoundON, "Sound ON", m_fontSize);
	txtSoundOFF = std::make_unique<TextObject>(posSoundOFF, "Sound OFF", m_fontSize);

	txtSaveON = std::make_unique<TextObject>(posSaveON, "Save ON", m_fontSize);
	txtSaveOFF = std::make_unique<TextObject>(posSaveOFF, "Save OFF", m_fontSize);

	txtCpuON	= std::make_unique<TextObject>(posCpuOn, "CPU ON", m_fontSize);
	txtCpuOFF	= std::make_unique<TextObject>(posCpuOFF, "CPU OFF", m_fontSize);
	txtBulet	= std::make_unique<TextObject>(posBulet, "Bulet 1min", m_fontSize);
	txtBlitz	= std::make_unique<TextObject>(posBlitz, "Blitz 5min", m_fontSize);
	txtRapid	= std::make_unique<TextObject>(posRapid, "Rapid 20min", m_fontSize);
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

		// save
		UpdateSave();

		// theme
		UpdateTheme();


		// apply btn
		UpdateApplySettings();


		//update(... if un est activé les autres doivent etre non actifs)

		// init flag
		m_flag_leftMouseButtonPressed = false;

	}
}

void Settings::Draw()
{
	/*
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

	*/
	//drawable
	//img1->Draw();
	//txt1->Draw();
	imTheme1->Draw();
	imTheme2->Draw();
	imTheme3->Draw();
	imBtnOK->Draw();

	txtSoundON->Draw();
	txtSoundOFF->Draw();

	txtSaveON->Draw();
	txtSaveOFF->Draw();

	txtCpuON->Draw();
	txtCpuOFF->Draw();
	txtBulet->Draw();
	txtBlitz->Draw();
	txtRapid->Draw();
}

void Settings::UpdateCpuMode()
{
	// cpu  ON OFF
	if (IsClicked(GetMousePosition(), txtSoundON->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		txtSoundON->SetSelected(true);
		m_flag_isSoundON = true;
	}
	if (IsClicked(GetMousePosition(), txtSoundOFF->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		txtSoundOFF->SetSelected(true);
		m_flag_isSoundON = false;
	}

	std::cout << "Sound on ? : " << m_flag_isSoundON << std::endl;
}

void Settings::UpdateSound()
{
	if (IsClicked(GetMousePosition(), txtCpuON->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		txtCpuON->SetSelected(true); m_flag_isCpuON = true;
	}
	if (IsClicked(GetMousePosition(), txtCpuOFF->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		txtCpuOFF->SetSelected(true); m_flag_isCpuON = false;
	}

	std::cout << "Cpu on ? : " << m_flag_isSoundON << std::endl;

}

void Settings::UpdateSave()
{
	if (IsClicked(GetMousePosition(), txtSaveON->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		txtSaveON->SetSelected(true); m_flag_isSaveON = true;
	}
	if (IsClicked(GetMousePosition(), txtSaveOFF->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		txtSaveOFF->SetSelected(true); m_flag_isSaveON = false;
	}

	std::cout << "Save on ? : " << m_flag_isSaveON << std::endl;
}

void Settings::UpdateTheme()
{
	if (IsClicked(GetMousePosition(), imTheme1->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		imTheme1->SetSelected(true);
		m_theme = Theme::Blue;
	}
	if (IsClicked(GetMousePosition(), imTheme2->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		imTheme2->SetSelected(true);
		m_theme = Theme::Brown;
	}
	if (IsClicked(GetMousePosition(), imTheme3->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		m_theme = Theme::Magenta;
		imTheme3->SetSelected(true);
	}

	std::string r = (m_theme == Theme::Blue) ? "blue" : (m_theme == Theme::Brown ? "brown" : "magenta");
	std::cout << "theme type ? : " << r << std::endl;
}

void Settings::UpdateGameMode()
{
	if (IsClicked(GetMousePosition(), txtBulet->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		txtBulet->SetSelected(true); m_gameMode = GameMode::Bulet;
	}
	if (IsClicked(GetMousePosition(), txtBlitz->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		txtBlitz->SetSelected(true); m_gameMode = GameMode::Blitz;
	}
	if (IsClicked(GetMousePosition(), txtRapid->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		txtRapid->SetSelected(true); m_gameMode = GameMode::Rapid;
	}

	std::string r = (m_gameMode == GameMode::Blitz) ? "blitz" : (m_gameMode == GameMode::Bulet ? "bulet" : "rapid");
	std::cout << "game mode  : " << r << std::endl;

}

void Settings::UpdateApplySettings()
{
	// ok button
	if (IsClicked(GetMousePosition(), imBtnOK->GetOutlineRect(), m_flag_leftMouseButtonPressed))
	{
		imBtnOK->SetSelected(true);
		m_flag_isSlectionDone = true;
	}
	
		
}

bool Settings::IsSelectionDone()
{
	return m_flag_isSlectionDone;
}
