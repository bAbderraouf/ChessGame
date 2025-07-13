#include "Piece.h"

//-------------------------------------------------------------
//			PIECE CONTRUCTOR
//-------------------------------------------------------------
Piece::Piece(bool player1Side)
	: initialPos(0, 0),
	pos(0, 0), // default pos
	centerPos(0, 0)
{
	id = 0;
	player1 = player1Side;
	flag_isSelected = false;
	flag_neverMoved = true;
	pieceName = "";
	imageSize = 75; // real image size
	centerPos = GetCenterOfPiecePosition();

	//InitPiece
	LoadPieceImage();


}


//-------------------------------------------------------------
//			Copy Constructor
//-------------------------------------------------------------
Piece::Piece(Piece const& p) : 
	initialPos(p.initialPos.col, p.initialPos.row),
	pos(p.pos.col, p.pos.row),
	centerPos(p.centerPos.col, p.centerPos.row)
{
	id = p.id;
	pieceName = p.pieceName;


	//InitPiece
	pieceTexture = p.pieceTexture;
	

}

//-------------------------------------------------------------
//				Adiitional functions
//-------------------------------------------------------------

int Piece::getId() const
{
	return this->id;
}

std::string Piece::GetName() const
{
	return this->pieceName;
}

bool Piece::GetPlayerSide() const
{
	return this->player1;
}

//-------------------------------------------------------------
void Piece::InitPiece()
{

}

//-------------------------------------------------------------
Position Piece::GetInitialPosition() const
{
	return initialPos;
}

//-------------------------------------------------------------
Position Piece::GetPosition() const
{
	return pos;
}

Position Piece::GetCenterOfPiecePosition() const
{
	Position centerOfPiece(0, 0);

	//calculate the center pos
	centerOfPiece.row = pos.row + pieceTexture.height / 2;
	centerOfPiece.col = pos.col + pieceTexture.width / 2;

	return centerOfPiece;
}

Position Piece::GetLastPosition() const
{
	return this->lastPos;
}

//-------------------------------------------------------------
void Piece::SetInitialPosition(Position pos)
{
	initialPos = pos;
}

//-------------------------------------------------------------
void Piece::SetPosition(Position pos)
{
	this->pos = pos;
}

void Piece::SetCenterOfPiecePosition(Position pos)
{
	centerPos = pos;

	// convert center pos to normal pos
	Position normalPos(0, 0);
	normalPos.row = centerPos.row - pieceTexture.height / 2;
	normalPos.col = centerPos.col - pieceTexture.width / 2;

	//set corresponding normal pos
	SetPosition(normalPos);
}

void Piece::SetLastPosition(Position pos)
{
	this->lastPos = pos;
}

//-------------------------------------------------------------
bool Piece::IsPlayer1Side()
{
	return player1;
}

//-------------------------------------------------------------
void Piece::Draw()
{
	Vector2 pos;
	pos.x = GetPosition().col;
	pos.y = GetPosition().row;

	if(player1)
		DrawTexture(pieceTexture, pos.x  , pos.y  , WHITE); 
	else
		DrawTexture(pieceTexture, pos.x  , pos.y  , WHITE);
	
}

Rectangle Piece::GetImageBounds()
{
	/*-----------------------------------------------------
	raylib doesnt contains GlobalBounds function.
	Manual calcuation : 
		 - position.x, position.y → coin supérieur gauche de l'image.
		 - texture.width, texture.height → dimensions de l'image.
	--------------------------------------------------------*/

	return Rectangle{ (float)pos.col, (float)pos.row, (float)pieceTexture.width, (float)pieceTexture.height};
}

void Piece::SetImageSize(int size)
{
	imageSize = size;
	// reste image and texture sizes
	LoadPieceImage(); 
}

void Piece::LoadPieceImage()
{
	//set path
	std::string side = player1 ? "B" : "N";
	std::string fullPath = "assets/images/" + pieceName + side + ".png";

	//convert to char*
	const char* c_fullPath = fullPath.c_str();

	//set ratio
	const int realImageSize = 75; //75x75px
	float ratio = (float)imageSize / (float)realImageSize;

	// load image
	pieceImage = LoadImage(c_fullPath);

	//resize image
	ImageResize(&pieceImage, pieceImage.width * ratio, pieceImage.height * ratio);

	//set texture
	pieceTexture = LoadTextureFromImage(pieceImage);
}

void Piece::SetSlected(bool selected)
{
	flag_isSelected = selected;
}

bool Piece::IsSelected() const
{
	return flag_isSelected;
}

void Piece::SetNeverMoved(bool neverMoved)
{
	this->flag_neverMoved = neverMoved;
}

bool Piece::IsNeverMoved() const
{
	return this->flag_neverMoved;
}

std::vector<Position> Piece::GetPosiblePositionsOnBoard(int cellSize) const
{
	// without margings (only board positions)

	std::vector<Position> positions;
	int step = player1 ? -cellSize : cellSize;
	Position p(0, 0);
	int r = 0, c = 0;
	
	switch (id)
	{
	case 1:  // piion
		positions.push_back(Position(pos.row + step, pos.col));
		positions.push_back(Position(pos.row + step, pos.col +step));
		positions.push_back(Position(pos.row + step, pos.col -step));
		break;
	case 2: // fou
		for(int n = -7 ; n <=7 ; n++)
		{
			
			r= pos.row + n* step;
			c = pos.col + n* step;

			if (r <= 7 * cellSize && r >= 0 && c <= 7 * cellSize && c >= 0)
			{
				positions.push_back(Position(r, c));
			}
			
			c = pos.col - n* step;

			if (r <= 7 * cellSize && r >= 0 && c <= 7 * cellSize && c >= 0)
			{
				positions.push_back(Position(pos.row + step, pos.col - step));
			}

		}
		break;
	case 3: // cavalier
		positions.push_back(Position(pos.row + step, pos.col + 2*step));
		positions.push_back(Position(pos.row + step, pos.col - 2*step));
		positions.push_back(Position(pos.row + 2 * step, pos.col + step));
		positions.push_back(Position(pos.row + 2 * step, pos.col - step));
		positions.push_back(Position(pos.row - step, pos.col + 2 * step));
		positions.push_back(Position(pos.row - step, pos.col - 2 * step));
		positions.push_back(Position(pos.row - 2 * step, pos.col + step));
		positions.push_back(Position(pos.row - 2 * step, pos.col - step));
		break;
	case 4: //tour
		for (int n = -7; n <= 7; n++)
		{

			r = pos.row + n * step;
			c = pos.col ;

			if (r <= 7 * cellSize && r >= 0 && c <= 7 * cellSize && c >= 0)
			{
				positions.push_back(Position(r, c));
			}

			r = pos.row ;
			c = pos.col + n * step;

			if (r <= 7 * cellSize && r >= 0 && c <= 7 * cellSize && c >= 0)
			{
				positions.push_back(Position(pos.row + step, pos.col - step));
			}

		}
		break;

	//case 5:
	//case 6:
	}

	return positions;
}

//-------------------------------------------------------------
//			Pion
//-------------------------------------------------------------

Pion::Pion(bool player1Side) : Piece(player1Side)
{
	id = 1;
	pieceName = "Pion";

	//InitPiece
	LoadPieceImage();
}

// copy constructor
Pion::Pion(Pion const& p) : Piece(p)
{
}

Pion* Pion::Clone() const
{
	return new Pion(*this);
}

//-------------------------------------------------------------
//			Fou
//-------------------------------------------------------------

Fou::Fou(bool player1Side) : Piece(player1Side)
{
	id = 2;
	pieceName = "Fou";

	//InitPiece
	LoadPieceImage();
}

// copy constructor
Fou::Fou(Fou const& p) : Piece(p)
{
}

Fou* Fou::Clone() const
{
	return new Fou(*this);
}

//-------------------------------------------------------------
//			Cavalier
//-------------------------------------------------------------

Cavalier::Cavalier(bool player1Side):Piece( player1Side)
{
	id = 3;
	pieceName = "Cava";//lier";

	//InitPiece
	LoadPieceImage();
}

// copy constructor
Cavalier::Cavalier(Cavalier const& p) : Piece(p)
{
}

Cavalier* Cavalier::Clone() const
{
	return new Cavalier(*this);
}
//-------------------------------------------------------------
//				Tour
//-------------------------------------------------------------

Tour::Tour(bool player1Side) :Piece( player1Side)
{
	id = 4;
	pieceName = "Tour";

	//InitPiece
	LoadPieceImage();
}

// copy constructor
Tour::Tour(Tour const& p) : Piece(p)
{
}

Tour* Tour::Clone() const
{
	return new Tour(*this);
}

//-------------------------------------------------------------
//				Dame
//-------------------------------------------------------------

Dame::Dame(bool player1Side) :Piece( player1Side)
{
	id = 5;
	pieceName = "Dame";

	//InitPiece
	LoadPieceImage();
}

// copy constructor
Dame::Dame(Dame const& p) : Piece(p)
{
}

Dame* Dame::Clone() const
{
	return new Dame(*this);
}

//-------------------------------------------------------------
//			Roi
//-------------------------------------------------------------

Roi::Roi(bool player1Side) : Piece( player1Side)
{
	id = 6;
	pieceName = "Roi";

	//InitPiece
	LoadPieceImage();
}
// copy constructor
Roi::Roi(Roi const& p) : Piece(p)
{
}

Roi* Roi::Clone() const
{
	return new Roi(*this);
}
