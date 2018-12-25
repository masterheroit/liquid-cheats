#include "SEGlow.hpp"

CScreenSpaceEffectRegistration *CScreenSpaceEffectRegistration::s_pHead = NULL;

CScreenSpaceEffectRegistration::CScreenSpaceEffectRegistration(const char *pName, IScreenSpaceEffect *pEffect)
{
	m_pEffectName = pName;
	m_pEffect = pEffect;
	m_pNext = s_pHead;
	s_pHead = this;
}
