#include "Piece.h"

Piece::Piece() 
	: position(4,0) // default pos
{
	id = 0;
	rotationState = 1;  // 1 :0° , 2:90° , 3:180° , 4:270°
	size = 3;	// block size (3x3)
	leftBoundary = 0;
	rightBoundary = size;
	downBoundary = size;
	InitBlock();
}


Piece::Piece(Piece const& p) : position(p.position.col, p.position.row)
{
	id = p.id;
	rotationState = p.rotationState;  // 1 :0° , 2:90° , 3:180° , 4:270°
	size = p.size;	// block size (3x3)
	leftBoundary = p.leftBoundary;
	rightBoundary = p.rightBoundary;
	downBoundary = p.downBoundary;
	//block[][]
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			block[i][j] = p.block[i][j];
		}
	}
}


int Piece::getRotationState()
{
	return this->rotationState;
}

int Piece::getSize()
{
	return this->size;
}

int Piece::getId()
{
	return this->id;
}

void Piece::InitBlock()
{
	for (int i = 0; i < size; i++)
	{
		for (int j=0; j < size; j++)
		{
			block[i][j] = 0;
		}
	}
}


void Piece::Print()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			std::cout << block[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Piece::setRotationState(int rotation)
{
	this->rotationState = rotation;
	this->InitBlock(); // reset to empty (0)
	this->FillBlock(); // refill block
}

Position Piece::getPosition()
{
	return position;
}

void Piece::setPosition(Position pos)
{
	position = pos;
}

int Piece::getLeftBound()
{
	return leftBoundary;
}

int Piece::getRigtBound()
{
	return rightBoundary;
}

int Piece::getDownBound()
{
	return downBoundary;
}

L_piece::L_piece() : Piece()
{
	id = 1;
	size = 3;
	FillBlock();
}

// copy constructor
L_piece::L_piece(L_piece const& p) : Piece(p)
{
}

L_piece* L_piece::Clone() const
{
	return new L_piece(*this);
}

void L_piece::FillBlock()
{
	switch (rotationState)
	{
	case 2 : // 90°
		block[0][2] = id;
		block[1][0] = id;
		block[1][1] = id;
		block[1][2] = id;

		leftBoundary = 0;
		rightBoundary = 2;
		downBoundary = 1;
		break;
	case 3 : // 180°
		block[0][0] = id;
		block[0][1] = id;
		block[1][1] = id;
		block[2][1] = id;
		//bounds
		leftBoundary = 0;
		rightBoundary = 1;
		downBoundary = 2;
		break;
	case 4 : //270°
		block[0][0] = id;
		block[0][1] = id;
		block[0][2] = id;
		block[1][0] = id;
		//bounds
		leftBoundary = 0;
		rightBoundary = 2;
		downBoundary = 1;
		break;
	default:  // 0°
		block[0][0] = id;
		block[1][0] = id;
		block[2][0] = id;
		block[2][1] = id;
		//bounds
		leftBoundary = 0;
		rightBoundary = 1;
		downBoundary = 2;
		break;
	}

}

I_piece::I_piece() : Piece()
{
	id = 2;
	size = 3; //4 
	FillBlock();
}

// copy constructor
I_piece::I_piece(I_piece const& p) : Piece(p)
{
}

I_piece* I_piece::Clone() const
{
	return new I_piece(*this);
}

void I_piece::FillBlock()
{
	// only two states
	if (rotationState == 1 || rotationState == 3) // 0° or 180°
	{
		block[0][1] = id;
		block[1][1] = id;
		block[2][1] = id;
		//bounds
		leftBoundary = 1;
		rightBoundary = 1;
		downBoundary = 2;
	}
	else if (rotationState == 2 || rotationState == 4) // 90° or 270°
	{
		block[1][0] = id;
		block[1][1] = id;
		block[1][2] = id;
		//bounds
		leftBoundary = 1;
		rightBoundary = 2;
		downBoundary = 1;
	}
}

Z_piece::Z_piece():Piece()
{
	
	id = 3;
	size = 3;
	FillBlock();
}

// copy constructor
Z_piece::Z_piece(Z_piece const& p) : Piece(p)
{
}

Z_piece* Z_piece::Clone() const
{
	return new Z_piece(*this);
}

void Z_piece::FillBlock()
{
	// only two states
	if (rotationState == 1 || rotationState == 3) // 0° or 180°
	{
		block[0][0] = id;
		block[0][1] = id;
		block[1][1] = id;
		block[1][2] = id;
		//bounds
		leftBoundary = 0;
		rightBoundary = 2;
		downBoundary = 1;
	}
	else if (rotationState == 2 || rotationState == 4) // 90° or 270°
	{
		block[2][0] = id;
		block[0][1] = id;
		block[1][1] = id;
		block[1][0] = id;
		//bounds
		leftBoundary = 0;
		rightBoundary = 1;
		downBoundary = 2;
	}
}

S_piece::S_piece() :Piece()
{
	id = 4;
	size = 3;
	FillBlock();
}

// copy constructor
S_piece::S_piece(S_piece const& p) : Piece(p)
{
}

S_piece* S_piece::Clone() const
{
	return new S_piece(*this);
}

void S_piece::FillBlock()
{
	//only two states
	if (rotationState == 1 || rotationState == 3) // 0° or 180°
	{
		block[0][1] = id;
		block[0][2] = id;
		block[1][1] = id;
		block[1][0] = id;
		//bounds
		leftBoundary = 0;
		rightBoundary = 2;
		downBoundary = 1;
	}
	else if (rotationState == 2 || rotationState == 4) // 90° or 270°
	{
		block[0][0] = id;
		block[1][0] = id;
		block[1][1] = id;
		block[2][1] = id;
		//bounds
		leftBoundary = 0;
		rightBoundary = 1;
		downBoundary = 2;
	}
}

O_piece::O_piece() :Piece()
{
	id = 5;
	size = 3;
	FillBlock();
}

// copy constructor
O_piece::O_piece(O_piece const& p) : Piece(p)
{
}

O_piece* O_piece::Clone() const
{
	return new O_piece(*this);
}

void O_piece::FillBlock()
{
	// same state for each rotationState
	block[0][0] = id;
	block[0][1] = id;
	block[1][0] = id;
	block[1][1] = id;
	//bounds
	leftBoundary = 0;
	rightBoundary = 1;
	downBoundary = 1;
}

T_piece::T_piece() : Piece()
{
	id = 6;
	size = 3;
	FillBlock();
}
// copy constructor
T_piece::T_piece(T_piece const& p) : Piece(p)
{
}

T_piece* T_piece::Clone() const
{
	return new T_piece(*this);
}

void T_piece::FillBlock()
{
	

	switch (rotationState)
	{
	case 2: // 90°
		block[0][0] = id;
		block[1][0] = id;
		block[2][0] = id;
		block[1][1] = id;
		//bounds
		leftBoundary = 0;
		rightBoundary = 1;
		downBoundary = 2;
		break;
	case 3: // 180°
		block[2][2] = id;
		block[2][1] = id;
		block[2][0] = id;
		block[1][1] = id;
		//bounds
		leftBoundary = 0;
		rightBoundary = 2;
		downBoundary = 2;
		break;
	case 4: //270°
		block[0][2] = id;
		block[1][2] = id;
		block[2][2] = id;
		block[1][1] = id;
		//bounds
		leftBoundary = 1;
		rightBoundary = 2;
		downBoundary = 2;
		break;
	default:  // 0°
		block[0][0] = id;
		block[0][1] = id;
		block[0][2] = id;
		block[1][1] = id;
		//bounds
		leftBoundary = 0;
		rightBoundary = 2;
		downBoundary = 1;
		break;
	}
	
}
