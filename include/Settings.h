#pragma once

#include "raylib.h"
#include "Position.h"
#include "Drawable.h"
#include <iostream>

/**
* @brief this class goal is to grab and store chessGame settings 
*/
class Settings
{
public: 
	/**
	* @brief Represents the speed category of a chess game.
	*/
	enum class GameMode {
		Bulet,			/// 1min per player
		Blitz,			/// 5min
		Rapid			/// 20min
	};


	/**
	* @brief Represents different themes existing in this game
	*/
	enum class Theme {
		Blue,			
		Brown,			
		Magenta			
	};



private:

	Color m_backgroundColor;
	bool m_flag_isCpuON;
	bool m_flag_isSoundON;
	bool m_flag_isSaveON;
	GameMode m_gameMode;
	Theme m_theme;


	// drawable object
	std::unique_ptr<Drawable> 
		imTheme1, imTheme2, imTheme3,
		imBtnOK,
		txtSoundON, txtSoundOFF, 
		txtCpuON, txtCpuOFF,
		txtSaveON, txtSaveOFF,
		txtBulet, txtBlitz, txtRapid;

	// positions
	Vector2 posT1, posT2, posT3,
			posSoundON, posSoundOFF,
			posCpuOn, posCpuOFF,
			posSaveON, posSaveOFF,
			posBulet, posBlitz, posRapid,
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
	
	/**------------------------------------------------------------------------------------------------------
	* @brief check if element is selected by mouse cursor (clicked)
	* note : button should be presed
	* @param cursorPos : cursor position coordinates (x,y)
	* @param objectOutlineRect : rectangle defining the object to be selected
	* @param leftMouseBtnClicked : to know if the left mouse button is clicked or not (get the flag state)
	* @return true if selected (left mouse button pressed + cursor in the correct aera).
	* @code 
		Rectangle  rect{ 0, 0, float(windowWidth), float(topMargin) };
		if(Settings::IsClicked(GetMousePosition(),rect, flag_leftMouseButtonPressed))
			std::cout << RED_COUT << "***** clickeed *****"  << RESET_COUT << std::endl;
	* @endcode
	------------------------------------------------------------------------------------------------------*/
	static bool IsClicked(Vector2 const& cursorPos, Rectangle const& objectOutlineRect, bool leftMouseBtnClicked);


private :
	/**
	* @brief check if the selection is done
	* @return true if selection is done
	*/
};



