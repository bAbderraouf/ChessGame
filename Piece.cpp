#include "Piece.h"

//-------------------------------------------------------------
//			PIECE CONTRUCTOR
//-------------------------------------------------------------
Piece::Piece(bool player1Side)
	: initialPos(0,0), 
	  currentPos(0,0) // default pos
{
	id = 0;
	player1 = player1Side;
	pieceName = "";
	imageSize = 75; // real image size

	//InitPiece
	LoadPieceImage();


}


//-------------------------------------------------------------
//			Copy Constructor
//-------------------------------------------------------------
Piece::Piece(Piece const& p) : 
	initialPos(p.initialPos.col, p.initialPos.row),
	currentPos(p.currentPos.col, p.currentPos.row)
{
	id = p.id;
	pieceName = p.pieceName;


	//InitPiece
	pieceTexture = p.pieceTexture;
	

}

//-------------------------------------------------------------
//				Adiitional functions
//-------------------------------------------------------------

int Piece::getId()
{
	return this->id;
}

std::string Piece::GetName()
{
	return this->pieceName;
}

//-------------------------------------------------------------
void Piece::InitPiece()
{

}

//-------------------------------------------------------------
Position Piece::GetInitialPosition()
{
	return initialPos;
}

//-------------------------------------------------------------
Position Piece::GetCurrentPosition()
{
	return currentPos;
}

Position Piece::GetCenterOfPiecePosition()
{
	Position centerOfPiece(0, 0);

	//calculate center pos
	centerOfPiece.row = currentPos.row + pieceTexture.height / 2;
	centerOfPiece.col = currentPos.col + pieceTexture.width / 2;

	return centerOfPiece;
}

//-------------------------------------------------------------
void Piece::SetInitialPosition(Position pos)
{
	initialPos = pos;
}

//-------------------------------------------------------------
void Piece::SetCurrentPosition(Position pos)
{
	currentPos = pos;
}

//-------------------------------------------------------------
bool Piece::IsPlayer1Side()
{
	return player1;
}

//-------------------------------------------------------------
void Piece::Draw()
{
	Vector2 currentPos;
	currentPos.x = GetCurrentPosition().col;
	currentPos.y = GetCurrentPosition().row;

	if(player1)
		DrawTexture(pieceTexture, currentPos.x  , currentPos.y  , WHITE); 
	else
		DrawTexture(pieceTexture, currentPos.x  , currentPos.y  , WHITE);
	
}

Rectangle Piece::GetImageBounds()
{
	/*-----------------------------------------------------
	raylib doesnt contains GlobalBounds function.
	Manual calcuation : 
		 - position.x, position.y → coin supérieur gauche de l'image.
		 - texture.width, texture.height → dimensions de l'image.
	--------------------------------------------------------*/

	return Rectangle{ (float)currentPos.col, (float)currentPos.row, (float)pieceTexture.width, (float)pieceTexture.height};
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
	pieceName = "Cavalier";

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
