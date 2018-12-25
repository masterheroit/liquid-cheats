#include "SDK.h"

unsigned char* get_ptr(int iIndex) {
	HRSRC hResourceData = FindResourceA(G::hDll, MAKEINTRESOURCE(iIndex), "raw");
	uintptr_t dwSize = SizeofResource(G::hDll, hResourceData);
	HGLOBAL hResource = LoadResource(G::hDll, hResourceData);
	void* pFirstByte = LockResource(hResource);

	unsigned char* szData = new unsigned char[dwSize];
	memcpy(szData, pFirstByte, dwSize);
	return szData;

}

class TextureHolder
{
public:
	typedef std::shared_ptr<TextureHolder> Ptr;
	TextureHolder()
		: m_pSurface(I::Surface), m_iH(0), m_iW(0), m_bgColor(255, 255, 255, 255), m_bValid(false)
	{ };

	TextureHolder(const unsigned char* pRawRGBAData, uint32 W, uint32 H)
		: m_pSurface(I::Surface), m_iH(H), m_iW(W), m_bgColor(255, 255, 255, 255), m_bValid(false)
	{
		m_iTexture = m_pSurface->CreateNewTextureId(true);
		if (!m_iTexture)
			return;
		m_pSurface->DrawSetTextureRgba(m_iTexture, pRawRGBAData, W, H, false, false);
		m_bValid = true;
	};

	bool IsValid() const
	{
		return m_bValid;
	};

	int GetTextureId() const
	{
		return m_iTexture;
	};

	bool Draw(int x, int y, float scale = 1.0)
	{
		if (!m_pSurface->IsTextureIdValid(m_iTexture))
			return false;
		m_pSurface->DrawSetColor(m_bgColor.r(), m_bgColor.g(), m_bgColor.b(), m_bgColor.a());
		m_pSurface->DrawSetTexture(m_iTexture);
		m_pSurface->DrawTexturedRect(x, y, x + m_iW * scale, y + m_iH * scale);
		return true;
	};

protected:
	uint32 m_iTexture;
	uint32 m_iW, m_iH;
	Color  m_bgColor;
	bool   m_bValid;

	ISurface* m_pSurface;
};