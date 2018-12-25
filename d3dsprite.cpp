#include "d3dsprite.h"

void Texture::draw(IDirect3DDevice9* pDevice, int classnum, int x, int y, int w, int h)
{
	static LPD3DXSPRITE sprite;

	D3DXCreateSprite(pDevice, &sprite);

	sprite->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXVECTOR3 pos = D3DXVECTOR3();
	pos.x = x;
	pos.y = y;
	pos.z = 0;
}