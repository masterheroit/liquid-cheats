#include <d3dx9.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <d3d.h>
#include <d3d9.h>

#define DX_BLU D3DCOLOR_ARGB(255, 52, 128, 249)
#define DX_RED D3DCOLOR_ARGB(255, 249, 52, 52)

class dxcolor
{
public:
	static D3DCOLOR _WHITE()
	{
		return D3DCOLOR_ARGB(255, 255, 255, 255);
	}
	static D3DCOLOR _RED()
	{
		return DX_RED;
	}

	static D3DCOLOR _BLU()
	{
		return DX_BLU;
	}

	static D3DCOLOR _BLACK()
	{
		return D3DCOLOR_ARGB(255, 0, 0, 0);
	}

	static D3DCOLOR _GRAY()
	{
		return D3DCOLOR_ARGB(255, 9, 9, 9);
	}
};
class CRender
{
public:
	CRender(IDirect3DDevice9* device)
	{
		this->pDevice = device;
		this->InitFonts();
	}

	static LPD3DXFONT font;
	static IDirect3DDevice9* pDevice;

	void InitFonts()
	{
		if (!pDevice) return;
		if(!font)
			D3DXCreateFontA(pDevice, 0.5, 0, FW_THIN, 1, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "System", &font);
	}
	void RenderText(int x, int y, D3DXCOLOR color, const char* ptr)
	{
		if (!pDevice || !font) return;

		RECT rect;
		SetRect(&rect, x, y, x, y);
		char Buffer[1024] = { '\0' };
		va_list va_alist;
		va_start(va_alist, ptr);
		vsprintf_s(Buffer, ptr, va_alist);
		va_end(va_alist);
		font->DrawTextA(NULL, Buffer, -1, &rect, DT_LEFT | DT_NOCLIP, color);
	}

	void RenderRect(int x, int y, int w, int h, D3DXCOLOR color)
	{
		if (!pDevice || !font) return;

		D3DRECT BarRect = { x, y, x + w, y + h };

		pDevice->Clear(1, &BarRect, 1.0f , color, 0, 0);
	}
};