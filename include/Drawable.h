#pragma once

#include "raylib.h"
#include "Utilis.h"
#include <iostream>
#include <string>
#include <memory>


class Drawable
{
protected :

	bool		m_isClicked;
	bool		m_isSelected;		/// selected = clicked + hold selection
	Color		m_outlineRecColor;
	Vector2		m_objectPosition;
	Vector2		m_objectSize;
	int			m_outlineRecThinkness;

public :

	Drawable(Vector2 const & position);
	~Drawable();

	virtual void SetClicked(bool value) ;
	virtual void SetSelected(bool value);
	virtual bool IsClicked();
	virtual void Draw() = 0;	
	virtual void Update(bool const& isLeftMousePressed);
	virtual void SetRectThinkness(int thinkness);
	virtual Rectangle GetOutlineRect();

};


class ImageObject : public Drawable
{
public :
	ImageObject(Vector2 const& position , const char * fileName);
	~ImageObject();

	void Draw() override;

private:
	Image		m_image;
	Texture2D	m_texture;

};


class TextObject : public Drawable
{
public:
	TextObject(Vector2 const& position, const char* text, int fontSize);
	~TextObject();

	void Draw() override;

private:
	const char* m_text;
	int m_fontSize;
};