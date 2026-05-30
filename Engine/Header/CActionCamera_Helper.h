#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CActionCamera_Helper
{
public:
	static void PlayDeathSequence(CGameObject* object, ObjectHandle TargetHandle);
	static void StartIslandTour(CGameObject* object, ObjectHandle TargetHandle); // 지름 길이
};

END