#pragma once

#include "raylib.h"

#define RED_COUT   "\033[31m"
#define BLUE_COUT  "\033[34m"
#define RESET_COUT "\033[0m"
#define GREEN_COUT "\033[32m"
#define YELLOW_COUT "\033[33m"
#define WHITE_COUT "\033[37m"


/**
* @brief date struct
*/
struct stDate {
	short year;
	short month;
	short day;
	short hour;
	short minute;
	short second;
};


/**
* @brief duration time struct
*/
struct stDuration {
	int seconds;
	int minutes;
	int hours;
};



/**
* @brief Represents different themes existing in this game
*/
enum class Theme {
	Blue,
	Brown,
	Magenta
};



/**
* @brief Represents the speed category of a chess game.
*/
enum class GameMode {
	Bulet,			/// 1min per player
	Blitz,			/// 5min
	Rapid			/// 20min
};


/**
* @brief who wins the game
*/
enum class Winner {
	White,			
	Black,			
	Draw,
	None,
};


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
bool IsClickedObject(Vector2 const& cursorPos, Rectangle const& objectOutlineRect, bool leftMouseBtnClicked);