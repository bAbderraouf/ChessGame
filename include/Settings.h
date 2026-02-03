#pragma once

#include "raylib.h"
#include "Position.h"
#include <memory>
#include <iostream>
#include "Drawable.h"
#include "Utilis.h"

/**
* @brief this class goal is to grab and store chessGame settings 
*/
class Settings
{

private:

	Color m_backgroundColor;
	bool m_flag_isCpuON;
	bool m_flag_isSoundON;
	bool m_flag_isSaveON;
	GameMode m_gameMode;
	Theme m_theme;


	// drawable object
	std::unique_ptr<Drawable> imTheme1;
	std::unique_ptr<Drawable> imTheme2;
	std::unique_ptr<Drawable> imTheme3;
	std::unique_ptr<Drawable> imBtnOK;
	std::unique_ptr<Drawable> txtSoundON;
	std::unique_ptr<Drawable> txtSoundOFF;
	std::unique_ptr<Drawable> txtCpuON; 
	std::unique_ptr<Drawable> txtCpuOFF;
	std::unique_ptr<Drawable> txtSaveON;
	std::unique_ptr<Drawable> txtSaveOFF;
	std::unique_ptr<Drawable> txtBulet;
	std::unique_ptr<Drawable> txtBlitz;
	std::unique_ptr<Drawable> txtRapid;
	std::unique_ptr<Drawable> imgSettings;
	std::unique_ptr<Drawable> imgSettingsTxt;

	// positions
	Vector2 posT1	  , posT2		, posT3,
			posSoundON, posSoundOFF,
			posCpuOn  , posCpuOFF,
			posSaveON , posSaveOFF,
			posBulet  , posBlitz	,posRapid,
			posBtnOK;

	// font
	int m_fontSize;

	// flags
	bool m_flag_leftMouseButtonPressed;
	bool m_flag_isSlectionDone;


public:
	Settings(); // set default values (even without selection)
	~Settings();


	// get settings from outside
	GameMode GetGameMode();
	Theme GetTheme();
	bool GetIsCpuON();
	bool GetIsSoundOn();


	void Init();
	void Input();
	void Update();
	void Draw();


	// calculate settings

	void UpdateCpuMode();
	void UpdateSound();
	void UpdateSave();
	void UpdateTheme();
	void UpdateGameMode();
	void UpdateApplySettings();


	//end of selection
	bool IsSelectionDone();

	

private :
	/**
	* @brief check if the selection is done
	* @return true if selection is done
	*/
};



