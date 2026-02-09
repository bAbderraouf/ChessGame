#pragma once

#include "raylib.h"
#include "Position.h"
#include <string>
#include <ctime>


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

/**
* @brief defines pieces names
*/
enum class PieceName {
	Pawn = 1,
	Rook = 2,
	Knight = 3,
	Bishop = 4,
	Queen = 5,
	King = 6,
};



/**
* @brief defines different moves flags
*/
struct MoveFlags {
	bool isNormal;			/// move to empty cell 
	bool isCapture;			/// move to not empty cell (containing enemy piece 
	bool isPromotion;		/// pawn arived to board limit (could be capture)
	bool isCheck;			/// if we move to this cell the other player will be in check position
	bool isLegal;			/// can move :  legal move (current team will not still in check position) 
	bool isSameCell;		/// cant move :  attempt to move to the same cell
	bool isTeamCell;		/// cant move :  cell occupied by team side 
	bool isKingCastle;		/// king castle (king side)
	bool isQueenCastle;		/// queen castle (queen side)
	
	// checkmat & pat will be checked after allowing the move
	void ResetFlags();
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



/**------------------------------------------------------------------------------------------------------
* @brief convert position object to vector2
* @param pos : possition to be converted
* @return the converted vector2 value
------------------------------------------------------------------------------------------------------*/
Vector2 PosToVect2(Position const & pos);


/**------------------------------------------------------------------------------------------------------
* @brief get PGN notation from pice name exple : "King" => "K"
* @param name : piece english name
* @return PGN notation
------------------------------------------------------------------------------------------------------*/
std::string GetPGNNameFromPieceName(PieceName const& name);




/*--------------------------------------------------------------------------------

* @brief get system date
* @return system date hh:min:sec in stDate format

*---------------------------------------------------------------------------------*/
stDate GetSystemDateTime(void);



/*--------------------------------------------------------------------------------

* @brief convert Date to string
* @param sep : separator between min and seconds, default ":"
* @return system date in string format

*---------------------------------------------------------------------------------*/
std::string GetDateTimeToString(stDate const& date, std::string sep = ":");



/*--------------------------------------------------------------------------------

* @brief convert seconds to duration
* @param seconds : number of seconds
* @return stDuration

*---------------------------------------------------------------------------------*/
stDuration SecondsToDuration(time_t seconds);



/*--------------------------------------------------------------------------------

* @brief convert duration to string
* @param sep : separator between min and seconds, default ":"
* @return duration in string format

*---------------------------------------------------------------------------------*/
std::string GetDurationToString(stDuration const& duration, std::string sep = ":");


/*--------------------------------------------------------------------------------

* @brief set duration in min & sec
* @param duration structure

*---------------------------------------------------------------------------------*/
void setDuration(stDuration& duration, int const& min, int const& sec);


