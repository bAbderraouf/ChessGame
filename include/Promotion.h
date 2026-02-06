#pragma once

#include "raylib.h"
#include "Utilis.h"
#include "Drawable.h"
#include <string>


class Promotion
{
public:
	Promotion(Position const& promotionPos, int const& cellSize, int const& topMargin, int const& leftMargin);
	~Promotion();
	
	void Init();
	void Input();
	void Update();
	void Draw();
	void SetImagesFileNames();
	void SetDrawablePositions();
	bool IsPromotionDone();
	PieceName GetSelectedPiece();

private:
	bool m_flag_leftMouseButtonPressed;
	bool m_flag_promotionDone;
	bool m_teamSide;
	Position m_pawnPos;
	Color m_bgRectColor;
	int m_cellSize, m_topMargin, m_leftMargin;
	PieceName m_selectedPiece;			/// promoted to this piece type

	// promotion pieces 
	std::unique_ptr<Drawable> Queen, Rook, Bishop, Knight;

	// promotion pieces positions
	Position posQueen,posRook, posBishop, posKnight;

	// pieces images files names
	std::string base, fileQueen, fileRook, fileBishop, fileKnight;
};


