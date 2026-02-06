#include "Promotion.h"

Promotion::Promotion(Position const& promotionPos, int const & cellSize , int const & topMargin , int const & leftMargin)
{
	m_bgRectColor = { 255, 255, 255, 200 };
	m_pawnPos = promotionPos;
	m_cellSize = cellSize;
	m_topMargin = topMargin;
	m_leftMargin = leftMargin;
	m_teamSide = (m_pawnPos.row == topMargin) ? true : false;
	m_flag_promotionDone = false;

    m_selectedPiece = PieceName::Pawn;	 // default (no promotion)	
	m_flag_leftMouseButtonPressed = false;

	// init drawables
	Init();
	
}

Promotion::~Promotion()
{
}

void Promotion::Init()
{
	// 1- positoins :
	SetDrawablePositions();

	// 2- set images file names :
	SetImagesFileNames();

	// 3- make drawable
	Queen = std::make_unique<ImageObject>(PosToVect2(posQueen), fileQueen.c_str());
	Rook = std::make_unique<ImageObject>(PosToVect2(posRook), fileRook.c_str());
	Bishop = std::make_unique<ImageObject>(PosToVect2(posBishop), fileBishop.c_str());
	Knight = std::make_unique<ImageObject>(PosToVect2(posKnight), fileKnight.c_str());
}

void Promotion::Input()
{
	// LEFT mouse mouse PRESSED
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		m_flag_leftMouseButtonPressed = true;
	}
}


void Promotion::Update( )
{
	if (m_flag_leftMouseButtonPressed)
	{
		// update queen
		Queen->Update(m_flag_leftMouseButtonPressed);
		if (Queen->IsClicked())
		{
			Queen->SetClicked(false);
			m_selectedPiece = PieceName::Queen;
			m_flag_promotionDone = true;
		}

		// update Bishop
		Bishop->Update(m_flag_leftMouseButtonPressed);
		if (Bishop->IsClicked())
		{
			Bishop->SetClicked(false);
			m_selectedPiece = PieceName::Bishop;
			m_flag_promotionDone = true;
		}

		// update knight
		Knight->Update(m_flag_leftMouseButtonPressed);
		if (Knight->IsClicked())
		{
			Knight->SetClicked(false);
			m_selectedPiece = PieceName::Knight;
			m_flag_promotionDone = true;
		}

		// update rook
		Rook->Update(m_flag_leftMouseButtonPressed);
		if (Rook->IsClicked())
		{
			Rook->SetClicked(false);
			m_selectedPiece = PieceName::Rook;
			m_flag_promotionDone = true;
		}
			
	}
}

void Promotion::Draw()
{
	// pos = top lef side of rect
	if(m_teamSide)
		DrawRectangle(m_pawnPos.col, m_pawnPos.row, m_cellSize, 4 * m_cellSize, m_bgRectColor);
	else
		DrawRectangle(m_pawnPos.col, m_pawnPos.row - 3*m_cellSize, m_cellSize, 4 * m_cellSize, m_bgRectColor);

	// draw 
	Queen->Draw();
	Rook->Draw();
	Bishop->Draw();
	Knight->Draw();
}

void Promotion::SetImagesFileNames()
{
	if (m_teamSide) // player1
	{
		base = "assets/images/";
		fileQueen = base + "DameB.png";
		fileRook = base + "TourB.png";
		fileBishop = base + "FouB.png";
		fileKnight = base + "CavaB.png";
	}
	else			// player2
	{
		base = "assets/images/";
		fileQueen = base + "DameN.png";
		fileRook = base + "TourN.png";
		fileBishop = base + "FouN.png";
		fileKnight = base + "CavaN.png";
	}
}

void Promotion::SetDrawablePositions()
{
	if (m_teamSide)		//player1
	{
		posQueen  = m_pawnPos;
	}
	else               //player2
	{
		posQueen = { m_pawnPos.row - 3 * m_cellSize ,  m_pawnPos.col };
	}

	// other pieces follows Queen position

	posRook   = Position(posQueen.row + m_cellSize, posQueen.col);
	posBishop = Position(posQueen.row + 2 * m_cellSize, posQueen.col);
	posKnight = Position(posQueen.row + 3 * m_cellSize, posQueen.col);
}

bool Promotion::IsPromotionDone()
{
	return m_flag_promotionDone;
}

PieceName Promotion::GetSelectedPiece()
{
	return  m_selectedPiece;			
}
