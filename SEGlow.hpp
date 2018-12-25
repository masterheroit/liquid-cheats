#pragma once

#include "SDK.h"
#include "CBasePlayer.hpp"

#define EHANDLE int

#define FULL_FRAME_TEXTURE "_rt_FullFrameFB"

class C_SEGlow : public IScreenSpaceEffect {
public:
	C_SEGlow(void) {

	}

	//....;
	virtual void Init(void);
	virtual void Shutdown(void);
	virtual void SetParameters(KeyValues *params) {};
	virtual void Enable(bool bEnable)
	{
		m_bEnabled = bEnable;
	}
	virtual bool IsEnabled()
	{
		return m_bEnabled;
	}

	virtual void RegisterEnt(EHANDLE hEnt, Color glowColor = Color(255, 255, 255, 64), float fGlowScale = 1.0f);
	virtual void DeregisterEnt(EHANDLE hEnt);

	virtual void SetEntColor(EHANDLE hEnt, Color glowColor);
	virtual void SetEntGlowScale(EHANDLE hEnt, float fGlowScale);

	virtual void Render(int x, int y, int w, int h);

	int FindGlowEnt(EHANDLE hEnt);

	static void GlowOutlineAmountChangeCallback(IConVar *var, const char *pOldValue, float flOldValue);
	static void GlowOutlineScaleChangeCallback(IConVar *var, const char *pOldValue, float flOldValue);

	C_SEGlow* GetGlowMgr();

protected:
	void RenderToStencil(int idx, IMatRenderContext *pRenderContext);
	void RenderToGlowTexture(int idx, IMatRenderContext *pRenderContext);

	void ApplyEntityGlowEffects(int nSplitScreenSlot, CMatRenderContextPtr &pRenderContext, float flBloomScale, int x, int y, int w, int h);
	void RenderGlowModels(int nSplitScreenSlot, CMatRenderContextPtr &pRenderContext);

private:
	bool m_bEnabled;

	struct GlowEnt
	{
		EHANDLE m_hEnt;
		float   m_fColor[4];
		float   m_fGlowScale;
	};

	CUtlVector<GlowEnt *> m_vGlowEnts;

	IMaterial *pMatGlowColor = NULL;
	ITexture * pRtFullFrame = NULL;

	ITexture *pRenderBuffer1 = NULL;
	ITexture *pRenderBuffer2 = NULL;

	int iRenderBufferDivisor = 1;

	IMaterial *pMatBlurX = NULL;
	IMaterial *pMatBlurY = NULL;

	ITexture * pRtQuarterSize1 = NULL;
	IMaterial *pMatHaloAddToScreen = NULL;

	IMaterialVar *pDimVar = NULL;
};

extern C_SEGlow g_SEGlow;