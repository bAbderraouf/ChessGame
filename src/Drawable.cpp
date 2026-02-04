#include "Drawable.h"

Drawable::Drawable(Vector2 const & position)
{
	m_isClicked				= false;
	m_outlineRecColor		= GREEN;
	m_objectPosition		= position;
	m_objectSize			= {0,0};
	m_outlineRecThinkness	= 2;

	fontSize = 30;
	spacing = 0; 
	fontCourrier = LoadFontEx("assets/fonts/courrierNew.ttf", fontSize, nullptr, 0);

	//sound
	m_clickedSound = LoadSound("assets/sounds/selection.wav");
}

Drawable::~Drawable()
{
	
}

void Drawable::SetClicked(bool value)
{
	m_isClicked = value;
}


bool Drawable::IsClicked()
{
	return m_isClicked;
}

void Drawable::Update(bool const& isLeftMousePressed)
{
	if (IsClickedObject(GetMousePosition(), this->GetOutlineRect(), isLeftMousePressed))
	{
		this->SetClicked(true);
		PlaySound(m_clickedSound);

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

	m_objectSize = this->GetSize();

	m_texture = LoadTextureFromImage(m_image);

}

ImageObject::~ImageObject()
{
	UnloadImage(m_image);
}

void ImageObject::Draw()
{
	Color selectionColor = { 0, 255, 255 , 200 };

	// draw image
	DrawTextureEx(m_texture, m_objectPosition, 0 , 1, WHITE);

	// define outline rect
	Rectangle rect = { m_objectPosition.x - 5, m_objectPosition.y - 5, m_objectSize.x + 10, m_objectSize.y + 10 };

	// draw outline rect
	if (m_isClicked)
	{
		DrawRectangleLinesEx(rect, m_outlineRecThinkness, selectionColor);
	}
		
}

Vector2 ImageObject::GetSize()
{
	return { (float)m_image.width , (float)m_image.height };
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
	Color selectionColor = { 0, 255, 255 , 200};

	// draw Text
	//DrawText(m_text, m_objectPosition.x, m_objectPosition.y, m_fontSize, WHITE);
	Vector2 size = this->GetSize();
	DrawTextEx(fontCourrier, m_text,{ m_objectPosition.x , m_objectPosition.y  }, fontSize, spacing, WHITE);

	// draw outline rect
	Rectangle rect = { m_objectPosition.x - 5, m_objectPosition.y - 5, size.x + 10, size.y + 10 };
	if (m_isClicked)
	{
		DrawRectangleLinesEx(rect, m_outlineRecThinkness, selectionColor);
	}
}

Vector2 TextObject::GetSize()
{
	return  MeasureTextEx(fontCourrier, m_text, fontSize, spacing);
}

MixedObject::MixedObject(Vector2 const& txtPosition, const char* text, int fontSize,  const char* imgFileName ,int const& distance):Drawable(txtPosition)
{	
	// text initialisation
	m_objectSize.x = MeasureText(text, fontSize);
	m_objectSize.y = fontSize;

	m_text = text;
	m_fontSize = fontSize;
	
	//------------------------------------------------------
	// img initialization
	m_image = LoadImage(imgFileName);

	if (m_image.data == nullptr)
	{
		std::cerr << "error : error while loading image file : " << imgFileName << std::endl;
		return;
	}

	m_imgSize = { (float)m_image.width , (float)m_image.height };

	m_texture = LoadTextureFromImage(m_image);

	m_distance = distance;

}

MixedObject::~MixedObject()
{
}

void MixedObject::Draw()
{
	Color selectionColor = { 0, 255, 255 , 200 };

	// draw Text
	DrawTextEx(fontCourrier, m_text, { m_objectPosition.x , m_objectPosition.y }, fontSize, spacing, WHITE);


	//calcul total size
	Vector2 imgSize = { (float)m_image.width , (float)m_image.height };
	Vector2 textSize = MeasureTextEx(fontCourrier, m_text, fontSize, spacing);
	m_objectSize = { textSize.x + imgSize.x + m_distance , std::max(textSize.y , imgSize.y) };

	// ecart de taille (the heighest - the smallest)/2
	float delta = (std::max(imgSize.y, textSize.y) - std::min(imgSize.y, textSize.y)) / 2;

	Vector2 imgPosition = { m_objectPosition.x + textSize.x + m_distance , m_objectPosition.y - delta};

	// draw image
	DrawTextureEx(m_texture, imgPosition, 0, 1, WHITE);

	// draw outline rect
	Rectangle rect = { m_objectPosition.x - 5, m_objectPosition.y - 5, m_objectSize.x + 10, m_objectSize.y + 10 };
	if (m_isClicked)
	{
		DrawRectangleLinesEx(rect, m_outlineRecThinkness, selectionColor);
	}
		

}

Vector2 MixedObject::GetSize()
{
	//calcul total size
	Vector2 imgSize = { (float)m_image.width , (float)m_image.height };
	Vector2 textSize = MeasureTextEx(fontCourrier, m_text, fontSize, spacing);
	m_objectSize = { textSize.x + imgSize.x + m_distance , std::max(textSize.y , imgSize.y) };

	return m_objectSize;
}
