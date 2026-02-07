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
