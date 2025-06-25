#pragma once
#include <iostream>
#include "Position.h"

class Piece 
{
public :
	Piece();
	virtual ~Piece() = default;
	Piece(Piece const& p); // copy constructor
	int id;
	int block[3][3]; // pour tester a modifier (vector)

	virtual Piece* Clone() const = 0;
	virtual int getRotationState();
	virtual int getSize();
	virtual int getId();
	virtual void InitBlock();
	virtual void FillBlock() = 0;
	virtual void Print();
	virtual void setRotationState(int rotation);
	virtual Position getPosition();
	virtual void setPosition(Position pos);
	virtual int getLeftBound();
	virtual int getRigtBound();
	virtual int getDownBound();


protected:
	int rotationState; //  1 :0° , 2:90° , 3:180° , 4:270°
	int size; // block size
	Position position;  // pos on the grid
	
	//---------------------------
	// ajouter une variable pour les boundaries top, left, down and right
	int leftBoundary, rightBoundary, downBoundary;
};

class L_piece :public Piece
{
public:
	L_piece();
	L_piece (L_piece const& p); // copy constructor
	L_piece* Clone() const override;
	void FillBlock() override;
};

class I_piece :public Piece
{
public:
	I_piece();
	I_piece(I_piece const& p); // copy constructor
	I_piece* Clone() const override;
	void FillBlock() override;
	 
};
class Z_piece :public Piece
{
public:
	Z_piece();
	Z_piece(Z_piece const& p); // copy constructor
	Z_piece* Clone() const override;
	void FillBlock() override;
	 
};

class S_piece :public Piece
{
public:
	S_piece();
	S_piece(S_piece const& p); // copy constructor
	S_piece* Clone() const override;
	void FillBlock() override;
	 
};
class O_piece :public Piece
{
public:
	O_piece();
	O_piece(O_piece const& p); // copy constructor
	O_piece* Clone() const override;
	void FillBlock() override;
	 
};

class T_piece :public Piece
{
public:
	T_piece();
	T_piece(T_piece const& p); // copy constructor
	T_piece* Clone() const override;
	void FillBlock() override;
	
};
