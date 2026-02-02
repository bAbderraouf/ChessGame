#include "Drawable.h"
#include "raylib.h"

Drawable::Drawable(Vector2 const & position)
{
	m_isClicked				= false;
	m_isSelected			= false;
	m_outlineRecColor		= GREEN;
	m_objectPosition		= position;
	m_objectSize			= {0,0};
	m_outlineRecThinkness	= 3;
}

Drawable::~Drawable()
{
}

void Drawable::SetClicked(bool value)
{
	m_isClicked = value;
}

void Drawable::SetSelected(bool value)
{
	m_isSelected = value;
}

bool Drawable::IsClicked()
{
	return m_isClicked;
}

void Drawable::Update(bool const& isLeftMousePressed)
{
	if (IsClickedObject(GetMousePosition(), this->GetOutlineRect(), isLeftMousePressed))
	{
		this->SetSelected(true);
		this->SetClicked(true);
	}

}

void Drawable::SetRectThinkness(int thinkness)
{
	m_outlineRecThinkness = thinkness;
}

Rectangle Drawable::GetOutlineRect()
{
	return Rectangle{m_objectPosition.x , m_objectPosition.y , m_objectSize.x, m_objectSize.y };
}

ImageObject::ImageObject(Vector2 const& position,  const char* fileName) : Drawable(position)
{
	m_image =  LoadImage(fileName);
	
	if (m_image.data == nullptr)
	{
		std::cerr << "error : error while loading image file : " <<  fileName <<std::endl;
		return;
	}

	m_objectSize = { (float)m_image.width , (float)m_image.height };

	m_texture = LoadTextureFromImage(m_image);

}

ImageObject::~ImageObject()
{
	UnloadImage(m_image);
}

void ImageObject::Draw()
{
	// draw image
	DrawTexture(m_texture, m_objectPosition.x, m_objectPosition.y, WHITE);

	// draw outline rect
	if(m_isSelected)
		DrawRectangleLines(m_objectPosition.x, m_objectPosition.y, m_objectSize.x, m_objectSize.y, GREEN);
}


TextObject::TextObject(Vector2 const& position, const char* text, int fontSize) : Drawable(position)
{
	m_objectSize.x = MeasureText(text, fontSize);
	m_objectSize.y = fontSize;

	m_text = text;
	m_fontSize = fontSize;
}

TextObject::~TextObject()
{
	delete m_text;
}

void TextObject::Draw()
{
	// draw Text
	DrawText(m_text, m_objectPosition.x, m_objectPosition.y, m_fontSize, WHITE);

	// draw outline rect
	if (m_isSelected)
		DrawRectangleLines(m_objectPosition.x, m_objectPosition.y, m_objectSize.x, m_objectSize.y, GREEN);
}