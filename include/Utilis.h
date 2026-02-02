#pragma once

#include "raylib.h"

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