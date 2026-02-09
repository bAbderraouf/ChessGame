#include "Utilis.h"



bool IsClickedObject(Vector2 const& cursorPos, Rectangle const& objectOutlineRect, bool leftMouseBtnClicked)
{
	if (leftMouseBtnClicked)
		return  CheckCollisionPointRec(cursorPos, objectOutlineRect);
	else
		return false;
}

Vector2 PosToVect2(Position const & pos)
{
	return { (float)(pos.col), (float)(pos.row) };
}

std::string GetPGNNameFromPieceName(PieceName const& name)
{
	switch (name)
	{
	case PieceName::Pawn:
		return "P";
	case PieceName::Rook:
		return "R";
	case PieceName::Knight:
		return "N";
	case PieceName::Bishop:
		return "B";
	case PieceName::Queen:
		return "Q";
	case PieceName::King:
		return "K";
	}
}

void MoveFlags::ResetFlags()
{
	isNormal = false;			/// move to empty cell 
	isCapture = false;			/// move to not empty cell (containing enemy piece 
	isPromotion = false;		/// pawn arived to board limit (could be capture)
	isCheck = false;			/// if we move to this cell the other player will be in check position
	isLegal= false;				/// can move :  legal move (current team will not still in check position) 
	isSameCell = false;			/// cant move :  attempt to move to the same cell
	isTeamCell = false;
	isKingCastle = false;
	isQueenCastle = false;
}



stDate GetSystemDateTime(void)
{
	time_t  t_now = time(NULL); // curent time

	// tm* now = localtime(&t); // unsafe (using static memory)

	struct tm now;  // a buffer to store the curent time
	localtime_s(&now, &t_now);

	stDate date;

	date.day = now.tm_mday;
	date.month = now.tm_mon + 1;
	date.year = now.tm_year + 1900;
	date.hour = now.tm_hour;
	date.minute = now.tm_min;
	date.second = now.tm_sec;

	return date;
}

std::string GetDateTimeToString(stDate const& date, std::string sep)
{
	std::string dateTime = "";

	dateTime = std::string(date.hour < 10 ? "0" : "") + std::to_string(date.hour) + sep +
		std::string(date.minute < 10 ? "0" : "") + std::to_string(date.minute) + sep +
		std::string(date.second < 10 ? "0" : "") + std::to_string(date.second);

	return dateTime;
}

stDuration SecondsToDuration(time_t seconds)
{
	stDuration diff;
	int m = 0, s = 0;

	m = seconds / 60;
	s = seconds % 60;

	setDuration(diff, m, s);

	return diff;
}

std::string GetDurationToString(stDuration const& duration, std::string sep)
{
	std::string time = (duration.minutes < 10 ? "0" : "") + std::to_string(duration.minutes) + sep
		+ (duration.seconds < 10 ? "0" : "") + std::to_string(duration.seconds);


	return time;
}

void setDuration(stDuration& duration, int const& min, int const& sec)
{
	duration.minutes = min;
	duration.seconds = sec;
}