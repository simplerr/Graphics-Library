#include "Graphics.h"
#include "StatusText.h"

namespace GLib {

//! Constructor.
StatusText::StatusText()
{
	mColor = 0xff000000;
	SetSize(16);
	SetText("nothing", mColor, 0);
	mPosition = XMFLOAT3(0, 0, 0);
}

//! Constructor.
/**
@param text The text to draw.
@param time How long to draw the text (seconds).
*/
StatusText::StatusText(string text, float x, float y, float time)
{
	SetSize(16);
	SetText(text, 0xff000000, time);
	mPosition = XMFLOAT3(x, y, 0);
}

//! Destructor.
StatusText::~StatusText()
{

}

//! Draws the status text.
void StatusText::Draw(Graphics* pGraphics)
{
	if(mTime > 0)	
		pGraphics->DrawText(mText, mPosition.x, mPosition.y, mSize, mColor);
}

//! Updates the timer.
void StatusText::Update(float dt)
{
	mTime -= dt;
}

//! Set the text and how long to draw it.
void StatusText::SetText(string text, UINT32 color, float time)
{
	mText = text;
	mTime = time;
	mColor = color;
}

//! Resets the timer.
void StatusText::Activate(float time)
{
	mTime = time;
}

void StatusText::SetSize(float size)
{
	mSize = size;
}

}