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

	pieceImage = LoadTexture("assets/images/black_queen.png");

	//InitPiece
}


//-------------------------------------------------------------
//			Copy Constructor
//-------------------------------------------------------------
Piece::Piece(Piece const& p) : 
	initialPos(p.initialPos.col, p.initialPos.row),
	currentPos(p.currentPos.col, p.currentPos.row)
{
	id = p.id;

	//InitPiece
	pieceImage = p.pieceImage;
	

}

//-------------------------------------------------------------
//				Adiitional functions
//-------------------------------------------------------------

int Piece::getId()
{
	return this->id;
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
		DrawTexture(pieceImage, currentPos.x + 10 , currentPos.y + 10 , WHITE); 
	else
		DrawTexture(pieceImage, currentPos.x + 10 , currentPos.y + 10  , WHITE);
	
	//10: marge de la grid ,
	//7: 1/2 diff(sizecell,sizepic)
}

//-------------------------------------------------------------
//			Pion
//-------------------------------------------------------------

Pion::Pion(bool player1Side) : Piece(player1Side)
{
	id = 1;

	//InitPiece
	if (player1Side)
		pieceImage = LoadTexture("assets/images/pionb.png");
	else
		pieceImage = LoadTexture("assets/images/pionN.png");
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
	//InitPiece
	if (player1Side)
		pieceImage = LoadTexture("assets/images/foub.png");
	else
		pieceImage = LoadTexture("assets/images/fouN.png");
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
	//InitPiece
	if (player1Side)
		pieceImage = LoadTexture("assets/images/cavalierb.png");
	else
		pieceImage = LoadTexture("assets/images/cavalierN.png");
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
	//InitPiece
	if (player1Side)
		pieceImage = LoadTexture("assets/images/tourb.png");
	else
		pieceImage = LoadTexture("assets/images/tourN.png");
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
	//InitPiece
	if (player1Side)
		pieceImage = LoadTexture("assets/images/dameb.png");
	else
		pieceImage = LoadTexture("assets/images/dameN.png");
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
	//InitPiece
	if (player1Side)
		pieceImage = LoadTexture("assets/images/roib.png");
	else
		pieceImage = LoadTexture("assets/images/roiN.png");
}
// copy constructor
Roi::Roi(Roi const& p) : Piece(p)
{
}

Roi* Roi::Clone() const
{
	return new Roi(*this);
}
