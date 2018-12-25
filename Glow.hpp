#pragma once

#ifndef __GLOW_HPP
#define __GLOW_HPP

#include "SDK.h"
#include <utlvector.h>
#include "CBasePlayer.hpp"
#include <basehandle.h>

#define END_OF_FREE_LIST -1
#define ENTRY_IN_USE -2

class CGlowObjectDefinition_t {
public:
	inline CGlowObjectDefinition_t(){
		m_vGlowColor = { 255.f, 255.f, 255.f };
		m_nNextFreeSlot = 0;
		m_nSplitScreenSlot = 0;
	}

	//Glowobjects;
	CBaseHandle m_hEntity;
	Vector m_vGlowColor;
	float m_flGlowAlpha;
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	int m_nSplitScreenSlot;
	int m_nNextFreeSlot;

	inline bool IsUnused() {
		if (!this || !m_nNextFreeSlot) return false;

		return m_nNextFreeSlot != ENTRY_IN_USE;
	}
};

class CGlowObjectManager {
public:
	CUtlVector<CGlowObjectDefinition_t> m_GlowObjectDefinitions;
	int m_nFirstFreeSlot;
};

#define END_OF_FREE_LIST -1
#define ENTRY_IN_USE -2

class C_Glow
{
public:
	void init();
	void fsn();

	int registerGlowObject(c_baseplayer *ent, float r, float g, float b, float a, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot);
	void UnregisterGlowObject(int nGlowObjectHandle);

	CGlowObjectManager* pGlow;

	static bool update;
};

extern C_Glow pGlow_t;
#endif