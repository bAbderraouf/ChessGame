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
	Color		m_outlineRecColor;
	Vector2		m_objectPosition;
	Vector2		m_objectSize;
	int			m_outlineRecThinkness;
	
	Sound m_clickedSound;

	Font fontCourrier;
	int fontSize, spacing;

public :

	Drawable(Vector2 const & position);
	~Drawable();

	virtual void SetClicked(bool value) ;
	virtual bool IsClicked();
	virtual void Draw() = 0;	
	virtual void Update(bool const& isLeftMousePressed);
	virtual void SetRectThinkness(int thinkness);
	virtual Rectangle GetOutlineRect();
	virtual Vector2 GetSize() = 0;

};


class ImageObject : public Drawable
{
public :
	ImageObject(Vector2 const& position , const char * fileName);
	~ImageObject();

	void Draw() override;
	Vector2 GetSize() override;

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
	Vector2 GetSize() override;

private:
	const char* m_text;
	int m_fontSize;
};

class MixedObject : public Drawable	// image + text
{
public:
	MixedObject(Vector2 const& txtPosition, const char* text, int fontSize,  const char* imgFileName , int const & distance);
	~MixedObject();

	void Draw() override;
	Vector2 GetSize() override;

private:
	const char* m_text;
	int m_fontSize;
	Image		m_image;
	Texture2D	m_texture;
	Vector2		m_imgSize;
	int		m_distance;
};