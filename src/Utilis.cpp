#include "Utilis.h"



bool IsClickedObject(Vector2 const& cursorPos, Rectangle const& objectOutlineRect, bool leftMouseBtnClicked)
{
	if (leftMouseBtnClicked)
		return  CheckCollisionPointRec(cursorPos, objectOutlineRect);
	else
		return false;
}
