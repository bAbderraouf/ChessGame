#pragma once
#include <iostream>
#include "Position.h"
#include <raylib.h>
#include <vector>

//-------------------------------------------------------------
//				Base Class
//-------------------------------------------------------------
class Piece 
{
public :
	Piece(bool player1Side );
	virtual ~Piece() = default;

	Piece(Piece const& p); // copy constructor
	int id;

	virtual Piece* Clone() const = 0;
	virtual int getId() const;
	virtual std::string GetName() const;
	virtual bool GetPlayerSide() const;

	virtual void InitPiece();

	virtual Position GetInitialPosition() const;
	virtual Position GetPosition() const;
	virtual Position GetCenterOfPiecePosition() const;
	virtual Position GetLastPosition() const;
	virtual void SetInitialPosition(Position pos);
	virtual void SetPosition(Position pos);
	virtual void SetCenterOfPiecePosition(Position pos);
	virtual void SetLastPosition(Position pos);
	virtual bool IsPlayer1Side();
	virtual void Draw();
	virtual Rectangle GetImageBounds();
	virtual void SetImageSize(int size);
	virtual void LoadPieceImage();
	virtual void SetSlected(bool selected);
	virtual bool IsSelected() const;
	virtual void SetNeverMoved(bool neverMoved);
	virtual bool IsNeverMoved() const;
	virtual std::vector<Position> GetPosiblePositionsOnBoard(int cellSize) const;


protected:
	Position pos , centerPos, initialPos , lastPos;  // pos on the grid
	Image pieceImage;
	Texture pieceTexture;
	int imageSize;
	std::string pieceName;

	// flags
	bool player1;
	bool flag_isSelected;
	bool flag_neverMoved;
	
};

//-------------------------------------------------------------
//				Pion
//-------------------------------------------------------------
class Pion :public Piece
{
public:
	Pion(bool player1Side);
	Pion(Pion const& p); // copy constructor
	Pion* Clone() const override;
};

//-------------------------------------------------------------
//				Fou
//-------------------------------------------------------------
class Fou :public Piece
{
public:
	Fou(bool player1Side);
	Fou(Fou const& p); // copy constructor
	Fou* Clone() const override;
	 
};

//-------------------------------------------------------------
//					Cavalier
//-------------------------------------------------------------
class Cavalier :public Piece
{
public:
	Cavalier(bool player1Side);
	Cavalier(Cavalier const& p); // copy constructor
	Cavalier* Clone() const override;
	 
};

//-------------------------------------------------------------
//					Tour
//-------------------------------------------------------------
class Tour :public Piece
{
public:
	Tour(bool player1Side);
	Tour(Tour const& p); // copy constructor
	Tour* Clone() const override;
	 
};

//-------------------------------------------------------------
//					Dame
//-------------------------------------------------------------
class Dame :public Piece
{
public:
	Dame(bool player1Side);
	Dame(Dame const& p); // copy constructor
	Dame* Clone() const override;
	 
};

//-------------------------------------------------------------
//					Roi
//-------------------------------------------------------------
class Roi :public Piece
{
public:
	Roi(bool player1Side);
	Roi(Roi const& p); // copy constructor
	Roi* Clone() const override;
	
};
