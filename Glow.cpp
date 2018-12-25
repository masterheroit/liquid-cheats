#include "Glow.hpp"
#include "SEGlow.hpp"

bool C_Glow::update;

C_Glow pGlow_t;

int C_Glow::registerGlowObject(c_baseplayer *ent, float r, float g, float b, float a, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot)
{
	g_SEGlow.GetGlowMgr()->RegisterEnt(ent->GetIndex(), Color(r, g, b, a), 1.0f);

	return 0;
}

void C_Glow::UnregisterGlowObject(int nGlowObjectHandle)
{
	pGlow->m_GlowObjectDefinitions[nGlowObjectHandle].m_nNextFreeSlot = pGlow->m_nFirstFreeSlot;
	pGlow->m_GlowObjectDefinitions[nGlowObjectHandle].m_hEntity = NULL;
	pGlow->m_nFirstFreeSlot = nGlowObjectHandle;
}

class GlowEntity_t
{
public:
	inline c_baseplayer* GetEntity() {
		return this->m_pEnt;
	}
	inline GlowEntity_t(c_baseplayer* ent) {
		this->m_pEnt = ent;
		this->curtime = 0.0f;
	}
	inline float GetLastUpdate() {
		return this->curtime;
	}
	inline void SetLastUpdate(float f) {
		this->curtime = f;
	}
	inline bool GetRegistered() {
		return this->Registered;
	}
	inline void SetRegistered(bool v) {
		this->Registered = v;
	}
private:
	c_baseplayer* m_pEnt;

	float curtime;

	bool Registered;
};


static GlowEntity_t* entities[34];

void C_Glow::fsn()
{
	ConVar* on = I::CVars->FindVar("glow_outline_effect_enable");

	if (on->GetInt() != 1)
		on->SetValue(1);

	//static int oldcount = AcquirePlayerCount_t();

	//Remove and Re-register the glow objects every frame, because otherwise you gonna end up with doubled entries and the game will lag hard and likely crash. (test)dddd
	//Also, If you register once via bool, then not all entities are glowing. (g.g,, if new players join)

	if (!gOpts.glow_active)
	{
		pGlow->m_GlowObjectDefinitions.RemoveAll();
		return;
	}

	for (int i = 0; i <= I::EntList->GetHighestEntityIndex(); i++)
	{
		auto entity = GetBasePlayer(i);

		if (!entity || entity->GetIndex() == me || entity->IsPlayer() != true || entity->IsDormant())
			continue;

		if (!pGlow)
			continue;

		if (!entities[i]) {
			entities[i] = new GlowEntity_t(entity);
		}

		float __diff = abs(I::Globals->curtime - entities[i]->GetLastUpdate());

		switch (entity->Team()) {
		case BLU_TEAM:
			this->registerGlowObject(entities[i]->GetEntity(), 25.f, 25.f, 255.f, 255.f, false, true, 0);
			break;
		case RED_TEAM:
			this->registerGlowObject(entities[i]->GetEntity(), 255.f, 25.f, 25.f, 255.f, false, true, 0);
			break;
		default:
			break;
		}

		entities[i]->SetLastUpdate(I::Globals->curtime);

		entities[i]->SetRegistered(true);
	}

	static float last_clear = 0.0f;

	if (last_clear <= 5.0f) {
		++last_clear;
	}
	else {
		pGlow->m_GlowObjectDefinitions.RemoveAll();
		last_clear = 0;
	}
}

void C_Glow::init()
{
	DWORD dwGlowLoc = sigs::GetClientSignature("8B 0D ? ? ? ? A1 ? ? ? ? 56 8B 37") + 0x2;
	XASSERT(dwGlowLoc);
	pGlow = *(CGlowObjectManager**)dwGlowLoc;
	XASSERT(pGlow);
}