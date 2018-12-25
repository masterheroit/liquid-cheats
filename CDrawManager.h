#pragma once
//===================================================================================
#include "SDK.h"
#include "Panels.h"
#include <Color.h>

#define RED(COLORCODE)	((int) ( COLORCODE >> 24) )
#define BLUE(COLORCODE)	((int) ( COLORCODE >> 8 ) & 0xFF )
#define GREEN(COLORCODE)	((int) ( COLORCODE >> 16 ) & 0xFF )
#define ALPHA(COLORCODE)	((int) COLORCODE & 0xFF )
#define COLORCODE(r,g,b,a)((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)))

struct TAD
{
	int w;
	int h;
};
//===================================================================================
class CDrawManager
{
public:
	void Initialize( const char* lpFont );
	TAD GetTextAddiction(const wchar* lptext)
	{
		int a, b;
		I::Surface->GetTextSize(m_Font, lptext, a, b);

		return TAD{ a, b };
	}

	void SetFont(unsigned long id);
	void DrawString( int x, int y, Color color, const wchar_t *pszText);
	void DrawString( int x, int y, Color color, const char *pszText, ... );
	byte GetESPHeight( );
	int GetPixelTextSize ( const char *pszText );
	void DrawLine(int xbegin, int ybegin, int xend, int yend, Color col);
	int GetPixelTextSize ( wchar_t *pszText );
	void DrawBox( Vector vOrigin, int r, int g, int b, int alpha, int box_width, int radius );
	void DrawRect( int x, int y, int w, int h, Color color );
	void OutlineRect( int x, int y, int w, int h, Color color );
	bool WorldToScreen( Vector &vOrigin, Vector &vScreen );
	void DrawCornerBox(int x, int y, int w, int h, int cx, int cy, Color col);
	Color GetTeamColor(int i)
	{
		switch (i)
		{
		case BLU_TEAM: return Color(45, 45, 255, 255);
			break;
		case RED_TEAM: return Color(255, 45, 45, 255);
			break;
		default:
			return Color(0, 0, 0, 255);
		}

		return Color(0, 0, 0, 100);
	}

	unsigned long m_Font;

	//Fonts;
	unsigned long m_System;
	unsigned long m_Arial;
	unsigned long m_Tahoma;
	unsigned long m_Courier;
	unsigned long m_Hl2Mp;
};
//===================================================================================
extern CDrawManager gDrawManager;
extern CDrawManager gDrawManager_Courier;
//===================================================================================