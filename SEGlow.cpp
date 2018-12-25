#include "SEGlow.hpp"

C_SEGlow g_SEGlow;

ADD_SCREENSPACE_EFFECT(C_SEGlow, ge_entglow);

static float rBlack[4] = { 0, 0, 0, 1 };
static float rWhite[4] = { 1, 1, 1, 1 };

C_SEGlow* C_SEGlow::GetGlowMgr() {
	return &ge_entglow_effect;
}

ITexture *InitRenderTarget(int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat fmt, MaterialRenderTargetDepth_t depth, bool bHDR, char *pStrOptionalName = NULL)
{

	int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT;
	if (depth == MATERIAL_RT_DEPTH_ONLY)
		textureFlags |= TEXTUREFLAGS_POINTSAMPLE;

	int renderTargetFlags = bHDR ? CREATERENDERTARGETFLAGS_HDR : 0;

	// NOTE: Refcount returned by CreateRenderTargetTexture is 1
	return I::MdlSys->CreateNamedRenderTargetTextureEx(pStrOptionalName, w, h, sizeMode, fmt, depth, textureFlags, renderTargetFlags);
}

static void SetRenderTargetAndViewPort(CMatRenderContextPtr &pRenderContext, ITexture *rt, int w, int h)
{
	pRenderContext->SetRenderTarget(rt);
	pRenderContext->Viewport(0, 0, w, h);
}

void C_SEGlow::Init() {
	// init the textures / materials that we need here rather than doing an
	// expensive lookup each render
	pMatGlowColor = I::MdlSys->FindMaterial("dev/glow_color", TEXTURE_GROUP_OTHER, true);

	// Get material and texture pointers
	pRtQuarterSize1 = I::MdlSys->FindTexture("_rt_SmallFB1", TEXTURE_GROUP_RENDER_TARGET);

	pRtFullFrame = I::MdlSys->FindTexture(FULL_FRAME_TEXTURE, TEXTURE_GROUP_RENDER_TARGET);

	int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_EIGHTBITALPHA;
	int renderTargetFlags = CREATERENDERTARGETFLAGS_HDR;

	// create full size buffers but we might not use the whole size of the
	// texture
	// theoretically if we only use half the buffer for glow the overhead of the
	// bluring *should* be less
	pRenderBuffer1 = I::MdlSys->CreateNamedRenderTargetTextureEx("_f1_buffer_1", pRtFullFrame->GetActualWidth(), pRtFullFrame->GetActualHeight(),
		RT_SIZE_LITERAL, IMAGE_FORMAT_RGB888, MATERIAL_RT_DEPTH_SHARED, textureFlags, renderTargetFlags);
	pRenderBuffer1->IncrementReferenceCount();

	pRenderBuffer2 = I::MdlSys->CreateNamedRenderTargetTextureEx("_f1_buffer_2", pRtFullFrame->GetActualWidth(), pRtFullFrame->GetActualHeight(),
		RT_SIZE_LITERAL, IMAGE_FORMAT_RGB888, MATERIAL_RT_DEPTH_SHARED, textureFlags, renderTargetFlags);
	pRenderBuffer2->IncrementReferenceCount();

	{
		// create the blur textures
		KeyValues *kv = new KeyValues("BlurFilterX");
		kv->SetString("$basetexture", "_f1_buffer_1");
		kv->SetInt("$ignorez", 1);
		kv->SetInt("$translucent", 1);
		kv->SetInt("$alphatest", 1);
		pMatBlurX = I::MdlSys->CreateMaterial_Internal("_f1_blurx", kv);
		pMatBlurX->Refresh();
	}

	{
		KeyValues *kv = new KeyValues("BlurFilterY");
		kv->SetString("$basetexture", "_f1_buffer_2");
		kv->SetInt("$bloomamount", 10);
		kv->SetInt("$bloomscale", 10);
		kv->SetInt("$ignorez", 1);
		kv->SetInt("$translucent", 1);
		kv->SetInt("$alphatest", 1);
		pMatBlurY = I::MdlSys->CreateMaterial_Internal("_f1_blury", kv);
		pMatBlurY->Refresh();
	}

	{
		KeyValues *kv = new KeyValues("UnlitGeneric");
		kv->SetString("$basetexture", "_f1_buffer_1");
		kv->SetInt("$additive", 1);
		pMatHaloAddToScreen = I::MdlSys->CreateMaterial_Internal("__f1_glow_blit", kv);
		pMatHaloAddToScreen->Refresh();
	}
}

struct ShaderStencilState_t
{
	bool                        m_bEnable;
	StencilOperation_t          m_FailOp;
	StencilOperation_t          m_ZFailOp;
	StencilOperation_t          m_PassOp;
	StencilComparisonFunction_t m_CompareFunc;
	int                         m_nReferenceValue;
	uint32                      m_nTestMask;
	uint32                      m_nWriteMask;

	ShaderStencilState_t()
	{
		m_bEnable = false;
		m_PassOp = m_FailOp = m_ZFailOp = STENCILOPERATION_KEEP;
		m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
		m_nReferenceValue = 0;
		m_nTestMask = m_nWriteMask = 0xFFFFFFFF;
	}

	void SetStencilState(CMatRenderContextPtr &pRenderContext)
	{
		pRenderContext->SetStencilEnable(m_bEnable);
		pRenderContext->SetStencilFailOperation(m_FailOp);
		pRenderContext->SetStencilZFailOperation(m_ZFailOp);
		pRenderContext->SetStencilPassOperation(m_PassOp);
		pRenderContext->SetStencilCompareFunction(m_CompareFunc);
		pRenderContext->SetStencilReferenceValue(m_nReferenceValue);
		pRenderContext->SetStencilTestMask(m_nTestMask);
		pRenderContext->SetStencilWriteMask(m_nWriteMask);
	}
};

void C_SEGlow::GlowOutlineAmountChangeCallback(IConVar *var, const char *pOldValue, float flOldValue)
{
	C_SEGlow *pGlow = g_SEGlow.GetGlowMgr();

	IMaterialVar *pBlurYAmmount = pGlow->pMatBlurY->FindVar("$bloomamount", nullptr);
	pBlurYAmmount->SetFloatValue(1.0f);
}

void C_SEGlow::RegisterEnt(EHANDLE hEnt, Color glowColor /*=Color(255,255,255,64)*/, float fGlowScale /*=1.0f*/)
{
	// Don't add duplicates
	if (FindGlowEnt(hEnt) != -1 || !GetBaseEntity(hEnt))
		return;

	GlowEnt *newEnt = new GlowEnt;
	newEnt->m_hEnt = hEnt;
	newEnt->m_fColor[0] = glowColor.r() / 255.0f;
	newEnt->m_fColor[1] = glowColor.g() / 255.0f;
	newEnt->m_fColor[2] = glowColor.b() / 255.0f;
	newEnt->m_fColor[3] = glowColor.a() / 255.0f;
	newEnt->m_fGlowScale = fGlowScale;
	m_vGlowEnts.AddToTail(newEnt);
}

void C_SEGlow::DeregisterEnt(EHANDLE hEnt)
{
	int idx = FindGlowEnt(hEnt);
	if (idx == -1)
		return;

	delete m_vGlowEnts[idx];
	m_vGlowEnts.Remove(idx);
}

void C_SEGlow::SetEntColor(EHANDLE hEnt, Color glowColor)
{
	int idx = FindGlowEnt(hEnt);
	if (idx == -1)
		return;

	m_vGlowEnts[idx]->m_fColor[0] = glowColor.r() / 255.0f;
	m_vGlowEnts[idx]->m_fColor[1] = glowColor.g() / 255.0f;
	m_vGlowEnts[idx]->m_fColor[2] = glowColor.b() / 255.0f;
	m_vGlowEnts[idx]->m_fColor[3] = glowColor.a() / 255.0f;
}


void C_SEGlow::SetEntGlowScale(EHANDLE hEnt, float fGlowScale)
{
	int idx = FindGlowEnt(hEnt);
	if (idx == -1)
		return;

	m_vGlowEnts[idx]->m_fGlowScale = fGlowScale;
}

int C_SEGlow::FindGlowEnt(EHANDLE hEnt)
{
	for (int i = 0; i < m_vGlowEnts.Count(); i++) {
		if (GetBaseEntity(m_vGlowEnts[i]->m_hEnt) == GetBaseEntity(hEnt))
			return i;
	}

	return -1;
}



void C_SEGlow::RenderGlowModels(int nSplitScreenSlot, CMatRenderContextPtr &pRenderContext)
{
	int x, y;

	I::Engine->GetScreenSize(x, y);

	pRenderContext->PushRenderTargetAndViewport();

	// Save modulation color and blend
	Vector vOrigColor;
	I::RenderView->GetColorModulation(vOrigColor.Base());
	float flOrigBlend = I::RenderView->GetBlend();

	SetRenderTargetAndViewPort(pRenderContext, pRenderBuffer1, x, y);

	pRenderContext->ClearColor4ub(0, 0, 0, 0);
	pRenderContext->ClearBuffers(true, false, false);

	// Set override material for glow color

	I::MdlRender->ForcedMaterialOverride(pMatGlowColor, OVERRIDE_NORMAL);

	ShaderStencilState_t stencilState;
	stencilState.m_bEnable = false;
	stencilState.m_nReferenceValue = 0;
	stencilState.m_nTestMask = 0xFF;
	stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
	stencilState.m_PassOp = STENCILOPERATION_KEEP;
	stencilState.m_FailOp = STENCILOPERATION_KEEP;
	stencilState.m_ZFailOp = STENCILOPERATION_KEEP;

	stencilState.SetStencilState(pRenderContext);

	//==================//
	// Draw the objects //
	//==================//
	for (int i = 0; i < m_vGlowEnts.Count(); ++i) {
		c_baseplayer *pEnt = GetBasePlayer(m_vGlowEnts[i]->m_hEnt);
		if (!pEnt || pEnt->IsDormant()) {
			// We don't exist anymore, remove us!
			delete m_vGlowEnts[i];
			m_vGlowEnts.Remove(i);
			continue;
		}

		GlowEnt *&glowEnt = m_vGlowEnts[i];

		I::RenderView->SetBlend(glowEnt->m_fGlowScale);
		Vector vGlowColor = Vector(glowEnt->m_fColor[0], glowEnt->m_fColor[1], glowEnt->m_fColor[2]) * glowEnt->m_fGlowScale;
		I::RenderView->SetColorModulation(&vGlowColor[0]); // This only sets rgb, not alpha

		pEnt->DrawModel(STUDIO_RENDER);
	}

	I::MdlRender->ForcedMaterialOverride(NULL, OverrideType_t::OVERRIDE_NORMAL);
	I::RenderView->SetColorModulation(vOrigColor.Base());
	I::RenderView->SetBlend(flOrigBlend);

	ShaderStencilState_t stencilStateDisable;
	stencilStateDisable.m_bEnable = false;
	stencilStateDisable.SetStencilState(pRenderContext);

	pRenderContext->PopRenderTargetAndViewport();
}

//Q_SNPrintf HL2DM : 55 8B EC 56 8B 75 0C 8D 45 14 57 8B 7D 08 50 FF 75 10 56 57 E8 ? ? ? ? 83 C4 10 85 C0 78 04 3B C6 7C 07 C6 44 37 ? ?

void C_SEGlow::Render(int x, int y, int w, int h)
{
	CMatRenderContextPtr pRenderContext{ I::MdlSys->GetRenderContext() };

	// Set override shader to the same simple shader we use to render the glow models
	I::MdlRender->ForcedMaterialOverride(pMatGlowColor, OVERRIDE_NORMAL);

	ShaderStencilState_t stencilStateDisable;
	stencilStateDisable.m_bEnable = false;
	float flSavedBlend = I::RenderView->GetBlend();

	// Set alpha to 0 so we don't touch any color pixels
	I::RenderView->SetBlend(0.0f);
	pRenderContext->OverrideDepthEnable(true, false);

	int iNumGlowObjects = 0;

	// TODO: shouldnt the push / pop render pointer be here instead of in this
	// loop
	for (int i = 0; i < m_vGlowEnts.Count(); ++i) {

		c_baseplayer *pEnt = GetBasePlayer(m_vGlowEnts[i]->m_hEnt);
		if (!pEnt) {
			// We don't exist anymore, remove us!
			delete m_vGlowEnts[i];
			m_vGlowEnts.Remove(i);
			continue;
		}

		int  removedCond = 0;
		int cond = pEnt->GetConds();
		int  oldcond = cond;
		if (cond & tf_cond::TFCond_Cloaked) {
			cond &= ~tf_cond::TFCond_Cloaked;
		}
		if (cond & tf_cond::TFCond_Disguised) {
			cond &= tf_cond::TFCond_Disguised;
		}

		ShaderStencilState_t stencilState;
		stencilState.m_bEnable = true;
		stencilState.m_nReferenceValue = 1;
		stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
		stencilState.m_PassOp = STENCILOPERATION_REPLACE;
		stencilState.m_FailOp = STENCILOPERATION_KEEP;
		stencilState.m_ZFailOp = STENCILOPERATION_REPLACE;
		stencilState.SetStencilState(pRenderContext);

		pEnt->DrawModel(STUDIO_RENDER);

		// Need to do a 2nd pass to warm stencil for objects which are rendered
		// only when occluded
		stencilState.m_bEnable = true;
		stencilState.m_nReferenceValue = 2;
		stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
		stencilState.m_PassOp = STENCILOPERATION_REPLACE;
		stencilState.m_FailOp = STENCILOPERATION_KEEP;
		stencilState.m_ZFailOp = STENCILOPERATION_KEEP;
		stencilState.SetStencilState(pRenderContext);

		pEnt->DrawModel(STUDIO_RENDER);

		pEnt->set_cond(oldcond);

		iNumGlowObjects++;
	}

	pRenderContext->OverrideDepthEnable(false, false);
	I::RenderView->SetBlend(flSavedBlend);
	stencilStateDisable.SetStencilState(pRenderContext);
	I::MdlRender->ForcedMaterialOverride(NULL, OVERRIDE_NORMAL);

	// If there aren't any objects to glow, don't do all this other stuff
	// this fixes a bug where if there are glow objects in the list, but none of them are glowing, the whole screen blooms.
	if (iNumGlowObjects <= 0)
		return;

	RenderGlowModels(-1, pRenderContext);

	// TODO: if we use buffers that arent of a 1:1 size then these need to change

	// src as in from the solid glow objects
	int nSrcWidth = w;
	int nSrcHeight = h;

	int nViewportX = x;
	int nViewportY = y;
	int nViewportWidth = w;
	int nViewportHeight = h;
	{

		float divisor = 1;
		int   iQuarterWidth = nSrcWidth / divisor;
		int   iQuarterHeight = nSrcHeight / divisor;

		/*			pRenderContext->PushRenderTargetAndViewport();

			SetRenderTargetAndViewPort(pRenderContext, pRenderBuffer2, iQuarterWidth, iQuarterHeight);
			pRenderContext->DrawScreenSpaceRectangle(pMatBlurX, 0, 0, iQuarterWidth, iQuarterHeight, 0.0f, 0, iQuarterWidth - 1, iQuarterHeight - 1, iQuarterWidth, iQuarterHeight);

			SetRenderTargetAndViewPort(pRenderContext, pRenderBuffer1, iQuarterWidth, iQuarterHeight);
			pRenderContext->DrawScreenSpaceRectangle(pMatBlurY, 0, 0, iQuarterWidth, iQuarterHeight, 0.0f, 0, iQuarterWidth - 1, iQuarterHeight - 1, iQuarterWidth, iQuarterHeight);

			pRenderContext->PopRenderTargetAndViewport();*/

		//=======================================================================================================//
		// At this point, pRtQuarterSize0 is filled with the fully colored glow
		// around everything as solid glowy //
		// blobs. Now we need to stencil out the original objects by only
		// writing pixels that have no            //
		// stencil bits set in the range we care about. //
		//=======================================================================================================//

		ShaderStencilState_t stencilState;
		stencilState.m_bEnable = true;
		stencilState.m_nWriteMask = 0x0; // We're not changing stencil
		stencilState.m_nTestMask = 0xFF;
		stencilState.m_nReferenceValue = 0x0;
		stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_EQUAL;
		stencilState.m_PassOp = STENCILOPERATION_KEEP;
		stencilState.m_FailOp = STENCILOPERATION_KEEP;
		stencilState.m_ZFailOp = STENCILOPERATION_KEEP;
		stencilState.SetStencilState(pRenderContext);

		pRenderContext->DrawScreenSpaceRectangle(pMatHaloAddToScreen, 0, 0, nViewportWidth, nViewportHeight,
			0.0f, -0.5f, iQuarterWidth - 1, iQuarterHeight - 1, iQuarterWidth, iQuarterHeight);

		stencilStateDisable.SetStencilState(pRenderContext);
	}
}

void C_SEGlow::Shutdown() {

}