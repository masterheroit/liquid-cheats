#include "CDrawManager.h"
//===================================================================================
CDrawManager gDrawManager;
CDrawManager gDrawManager_Courier;

#define ESP_HEIGHT 10
//===================================================================================
void CDrawManager::Initialize( const char* lpFont)
{
	if ( I::Surface == NULL )
		return;

	I::Engine->GetScreenSize( gScreenSize.iScreenWidth, gScreenSize.iScreenHeight );

	m_Arial = I::Surface->CreateFont();
	I::Surface->SetFontGlyphSet(m_Arial, "Arial", 15, 15, 0, 0, FONTFLAG_OUTLINE);

	m_System = I::Surface->CreateFont();
	I::Surface->SetFontGlyphSet(m_System, "System", 10, 10, 0, 0, FONTFLAG_OUTLINE);

	m_Courier = I::Surface->CreateFont();
	I::Surface->SetFontGlyphSet(m_Courier, "Courier", 15, 15, 0, 0, FONTFLAG_OUTLINE);

	m_Tahoma = I::Surface->CreateFont();
	I::Surface->SetFontGlyphSet(m_Tahoma, "Tahoma", 15, 15, 0, 0, FONTFLAG_OUTLINE);

	m_Hl2Mp = I::Surface->CreateFont();
	I::Surface->SetFontGlyphSet(m_Hl2Mp, "hl2mp", 50, 50, 0, 0, FONTFLAG_OUTLINE);

	m_Font = m_System; //Default;
}
//===================================================================================
void CDrawManager::DrawString( int x, int y, Color color, const wchar_t *pszText)
{
	if( pszText == NULL )
		return;

	I::Surface->DrawSetTextPos( x, y );
	I::Surface->DrawSetTextFont( m_Font );
	I::Surface->DrawSetTextColor(color.r(), color.g(), color.b(), color.a());
	I::Surface->DrawPrintText( pszText, wcslen( pszText ) );
}
//===================================================================================
void CDrawManager::DrawLine(int xbegin, int ybegin, int xend, int yend, Color col)
{
	I::Surface->DrawSetColor(col.r(), col.g(), col.b(), col.a());
	I::Surface->DrawLine(xbegin, ybegin, xend, yend);
}
void CDrawManager::DrawString( int x, int y, Color color, const char *pszText, ... )
{
	if( !pszText )
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start( va_alist, pszText );
	vsprintf_s( szBuffer, pszText, va_alist );
	va_end( va_alist );

	wsprintfW( szString, L"%S", szBuffer );

	I::Surface->DrawSetTextPos( x, y );
	I::Surface->DrawSetTextFont( m_Font );
	I::Surface->DrawSetTextColor(color.r(), color.g(), color.b(), color.a());
	I::Surface->DrawPrintText( szString, wcslen( szString ) );
}
//===================================================================================
byte CDrawManager::GetESPHeight( )
{
	return ESP_HEIGHT;
}
//===================================================================================
void CDrawManager::DrawRect( int x, int y, int w, int h, Color color )
{
	I::Surface->DrawSetColor(color.r(), color.g(), color.b(), color.a());
	I::Surface->DrawFilledRect( x, y, x + w, y + h );
}
//===================================================================================
void CDrawManager::OutlineRect( int x, int y, int w, int h, Color color)
{
	I::Surface->DrawSetColor(color.r(), color.g(), color.b(), color.a());
	I::Surface->DrawOutlinedRect( x, y, x + w, y + h );
}
//===================================================================================
void CDrawManager::DrawBox( Vector vOrigin, int r, int g, int b, int alpha, int box_width, int radius )
{
	Vector vScreen;

	if( !WorldToScreen( vOrigin, vScreen ) )
		return;

	int radius2 = radius<<1;

	OutlineRect( vScreen.x - radius + box_width, vScreen.y - radius + box_width, radius2 - box_width, radius2 - box_width, Color(255, 255, 255, 255) );
	OutlineRect( vScreen.x - radius - 1, vScreen.y - radius - 1, radius2 + ( box_width + 2 ), radius2 + ( box_width + 2 ), Color(255, 255, 255, 255) );
	DrawRect( vScreen.x - radius + box_width, vScreen.y - radius, radius2 - box_width, box_width,Color( r, g, b, alpha ));
	DrawRect( vScreen.x - radius, vScreen.y + radius, radius2, box_width,Color( r, g, b, alpha ));
	DrawRect( vScreen.x - radius, vScreen.y - radius, box_width, radius2,Color( r, g, b, alpha ));
	DrawRect( vScreen.x + radius, vScreen.y - radius, box_width, radius2 + box_width, Color( r, g, b, alpha ) );
}
//===================================================================================
bool CDrawManager::WorldToScreen( Vector &vOrigin, Vector &vScreen )
{
	const matrix3x4& worldToScreen = I::Engine->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	vScreen.z = 0; //Screen doesn't have a 3rd dimension.

	if( w > 0.001 ) //If the object is within view.
	{
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		vScreen.x = (gScreenSize.iScreenWidth / 2) + ( 0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * gScreenSize.iScreenWidth + 0.5); //Get the X dimension and push it in to the Vector.
		vScreen.y = (gScreenSize.iScreenHeight / 2) - ( 0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * gScreenSize.iScreenHeight + 0.5); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}
//==================================================
void CDrawManager::SetFont(unsigned long id) {
	m_Font = id;
}

void CDrawManager::DrawCornerBox(int x, int y, int w, int h, int cx, int cy, Color col) {
	DrawLine(x, y, x + (w / cx), y, col);
	DrawLine(x, y, x, y + (h / cy), col);

	DrawLine(x + w, y, x + w - (w / cx), y, col);
	DrawLine(x + w, y, x + w, y + (h / cy), col);

	DrawLine(x, y + h, x + (w / cx), y + h, col);
	DrawLine(x, y + h, x, y + h - (h / cy), col);

	DrawLine(x + w, y + h, x + w - (w / cx), y + h, col);
	DrawLine(x + w, y + h, x + w, y + h - (h / cy), col);
}