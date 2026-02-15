#include "Settings.h"


Settings::Settings(Vector2 const & windowSize)
{
	m_windowSize = windowSize; 

	// init default values
	Init();
}

Settings::~Settings()
{
}

GameMode Settings::GetGameMode()
{
	return m_gameMode;
}

Theme Settings::GetTheme()
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

	

	m_flag_isCpuON		= true;
	m_flag_isSoundON	= true;
	m_flag_isSaveON		= true;
	m_gameMode			= GameMode::Blitz;
	m_theme				= Theme::Blue;

	// init internal variables
	m_backgroundColor = BLACK;
	m_flag_leftMouseButtonPressed = false;
	m_flag_isSlectionDone = false;

	// font
	m_fontSize = 20;
	

	//drawable object declaration

	imTheme1	= std::make_unique<ImageObject>(posT1, "assets/images/theme/Blue.png");
	imTheme2	= std::make_unique<ImageObject>(posT2, "assets/images/theme/Brown.png");
	imTheme3	= std::make_unique<ImageObject>(posT3,	"assets/images/theme/Magenta.png");

	txtSoundON	= std::make_unique<MixedObject>(posSoundON, "Sound ON", m_fontSize, "assets/images/buttons/sound_on.png", 5);
	txtSoundOFF = std::make_unique<MixedObject>(posSoundOFF, "Sound OFF", m_fontSize, "assets/images/buttons/mute.png", 5);

	txtSaveON	= std::make_unique<TextObject>(posSaveON, "Save ON", m_fontSize);
	txtSaveOFF	= std::make_unique<TextObject>(posSaveOFF, "Save OFF", m_fontSize);

	txtCpuON	= std::make_unique<TextObject>(posCpuOn, "1 vs CPU", m_fontSize);
	txtCpuOFF	= std::make_unique<TextObject>(posCpuOFF, "1 vs 1", m_fontSize);

	txtBulet	= std::make_unique<MixedObject>(posBulet, "Bullet", m_fontSize,"assets/images/buttons/1min.png", 5);
	txtBlitz	= std::make_unique<MixedObject>(posBlitz, "Blitz", m_fontSize ,"assets/images/buttons/5min.png", 5);
	txtRapid	= std::make_unique<MixedObject>(posRapid, "Rapid", m_fontSize,"assets/images/buttons/20min.png", 5);


	imgSettings		= std::make_unique<ImageObject>("assets/images/buttons/settings.png");
	imgSettingsTxt	= std::make_unique<ImageObject>( "assets/images/buttons/settings2.png");
	imBtnOK			= std::make_unique<MixedObject>("Apply", m_fontSize, "assets/images/buttons/check_60.png", 7);


	// positions calculations
	posSettings		= GetPosTopRight(m_windowSize, imgSettings->GetSize());
	posSettingsTxt	= { GetXCenter(m_windowSize, imgSettingsTxt->GetSize()) ,  0 };
	posBtnOK		= GetPosBottomRight(m_windowSize, imBtnOK->GetSize());

	// layout // xStep // xStep
	/*---------------------------
		T1		 T2		  T3   
		sounon	 sondoff
		cpu on	 cpu off
		bulet	 blitz	  rapid
	------------------------------*/

	int xStep = 50, 
		yStep = 50;
	int widthLayout		= 3 * imTheme1->GetSize().x + 2 * xStep;
	int heightLayout	= imTheme1->GetSize().y + 3 * txtSoundON->GetSize().y + 3 * yStep;
	Vector2 sizeLayout	= { widthLayout , heightLayout };

	posT1		= {	GetPosCenter(m_windowSize, sizeLayout).x , 
					GetPosCenter(m_windowSize, sizeLayout).y + imgSettingsTxt->GetSize().y/2 };
	posT2		= {	posT1.x + imTheme1->GetSize().x + xStep ,	posT1.y };
	posT3		= { posT2.x + imTheme1->GetSize().x + xStep , 	posT2.y };

	posSoundON	= { posT1.x , posT1.y + yStep + imTheme1->GetSize().y };
	posSoundOFF = {	posT2.x , posSoundON.y};

	posCpuOn	= {	posSoundON.x , posSoundON.y + yStep };
	posCpuOFF	= {	posSoundOFF.x, posCpuOn.y};

	posBulet	= { posT1.x , posCpuOn.y + yStep};
	posBlitz	= { posT2.x , posBulet.y  };
	posRapid	= { posT3.x , posBulet.y  };


	// set positions
	imgSettingsTxt->SetPosition(posSettingsTxt);
	imgSettings->SetPosition(posSettings);
	imBtnOK->SetPosition(posBtnOK);

	imTheme1->SetPosition(posT1);
	imTheme2->SetPosition(posT2);
	imTheme3->SetPosition(posT3);

	txtSoundON->SetPosition(posSoundON);
	txtSoundOFF->SetPosition(posSoundOFF);

	txtCpuOFF->SetPosition(posCpuOFF);
	txtCpuON->SetPosition(posCpuOn);

	txtBulet->SetPosition(posBulet);
	txtBlitz->SetPosition(posBlitz);
	txtRapid->SetPosition(posRapid);


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

		// init flag
		m_flag_leftMouseButtonPressed = false;

	}
}

void Settings::Draw()
{
	//drawable
	imTheme1->Draw();
	imTheme2->Draw();
	imTheme3->Draw();
	imBtnOK->Draw();

	txtSoundON->Draw();
	txtSoundOFF->Draw();

	//txtSaveON->Draw();
	//txtSaveOFF->Draw();

	txtCpuON->Draw();
	txtCpuOFF->Draw();
	txtBulet->Draw();
	txtBlitz->Draw();
	txtRapid->Draw();

	imgSettings->Draw();
	imgSettingsTxt->Draw();
}

void Settings::UpdateSound()
{
	// cpu  ON OFF
	txtSoundON->Update(m_flag_leftMouseButtonPressed);
	if (  txtSoundON->IsClicked())
	{
		txtSoundON->SetClicked(true);
		m_flag_isSoundON = true;

		// ignore selection (radio btn effect)
		txtSoundOFF->SetClicked(false);
	}

	txtSoundOFF->Update(m_flag_leftMouseButtonPressed);
	if ( txtSoundOFF->IsClicked())
	{
		txtSoundOFF->SetClicked(true);
		m_flag_isSoundON = false;

		// ignore selection (radio btn effect)
		txtSoundON->SetClicked(false);
	}

	std::cout << "Sound on ? : " << m_flag_isSoundON << std::endl;
}

void Settings::UpdateCpuMode()
{
	txtCpuON->Update(m_flag_leftMouseButtonPressed);
	if ( txtCpuON->IsClicked())
	{
		txtCpuON->SetClicked(true); 
		m_flag_isCpuON = true;
		// ignore selection (radio btn effect)
		txtCpuOFF->SetClicked(false);
	}

	txtCpuOFF->Update(m_flag_leftMouseButtonPressed);
	if (txtCpuOFF->IsClicked())
	{
		txtCpuOFF->SetClicked(true); 
		m_flag_isCpuON = false;
		// ignore selection (radio btn effect)
		txtCpuON->SetClicked(false);
	}

	std::cout << "Cpu on ? : " << m_flag_isSoundON << std::endl;

}

void Settings::UpdateSave()
{
	txtSaveON->Update(m_flag_leftMouseButtonPressed);
	if (  txtSaveON->IsClicked())
	{
		txtSaveON->SetClicked(true); 
		m_flag_isSaveON = true;
		// ignore selection (radio btn effect)
		txtSaveOFF->SetClicked(false);
	}

	txtSaveOFF->Update(m_flag_leftMouseButtonPressed);
	if (  txtSaveOFF->IsClicked())
	{
		txtSaveOFF->SetClicked(true); 
		m_flag_isSaveON = false;
		// ignore selection (radio btn effect)
		txtSaveON->SetClicked(false);
	}

	std::cout << "Save on ? : " << m_flag_isSaveON << std::endl;
}

void Settings::UpdateTheme()
{
	imTheme1->Update(m_flag_leftMouseButtonPressed);
	if ( imTheme1->IsClicked())
	{
		imTheme1->SetClicked(true);
		m_theme = Theme::Blue;
		imTheme2->SetClicked(false);
		imTheme3->SetClicked(false);
	}

	imTheme2->Update(m_flag_leftMouseButtonPressed);
	if ( imTheme2->IsClicked())
	{
		imTheme2->SetClicked(true);
		m_theme = Theme::Brown;
		imTheme1->SetClicked(false);
		imTheme3->SetClicked(false);
	}

	imTheme3->Update(m_flag_leftMouseButtonPressed);
	if ( imTheme3->IsClicked())
	{
		m_theme = Theme::Magenta;
		imTheme3->SetClicked(true);
		imTheme2->SetClicked(false);
		imTheme1->SetClicked(false);
	}

	std::string r = (m_theme == Theme::Blue) ? "blue" : (m_theme == Theme::Brown ? "brown" : "magenta");
	std::cout << "theme type ? : " << r << std::endl;
}

void Settings::UpdateGameMode()
{
	txtBulet->Update(m_flag_leftMouseButtonPressed);
	if (txtBulet->IsClicked())
	{
		txtBulet->SetClicked(true); 
		m_gameMode = GameMode::Bulet;

		// ignore selection (radio btn effect)
		txtBlitz->SetClicked(false);
		txtRapid->SetClicked(false);
	}

	txtBlitz->Update(m_flag_leftMouseButtonPressed);
	if (txtBlitz->IsClicked())
	{
		txtBlitz->SetClicked(true); 
		m_gameMode = GameMode::Blitz;

		// ignore selection (radio btn effect)
		txtBulet->SetClicked(false);
		txtRapid->SetClicked(false);
	}

	txtRapid->Update(m_flag_leftMouseButtonPressed);
	if (txtRapid->IsClicked())
	{
		txtRapid->SetClicked(true); 
		m_gameMode = GameMode::Rapid;

		// ignore selection (radio btn effect)
		txtBulet->SetClicked(false);
		txtBlitz->SetClicked(false);
	}

	std::string r = (m_gameMode == GameMode::Blitz) ? "blitz" : (m_gameMode == GameMode::Bulet ? "bulet" : "rapid");
	std::cout << "game mode  : " << r << std::endl;

}

void Settings::UpdateApplySettings()
{
	imBtnOK->Update(m_flag_leftMouseButtonPressed);
	if (imBtnOK->IsClicked())
	{
		imBtnOK->SetClicked(false);
		m_flag_isSlectionDone = true;
	}
}

bool Settings::IsSelectionDone()
{
	return m_flag_isSlectionDone;
}
