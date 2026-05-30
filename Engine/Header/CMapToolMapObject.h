#pragma once
#include "CMapObject.h"


BEGIN(Engine)

class CMapObjectTex;

class ENGINE_DLL CMapToolMapObject : public CMapObject
{
protected:

    explicit CMapToolMapObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMapToolMapObject(const CMapToolMapObject& rhs);
    virtual ~CMapToolMapObject();


	HRESULT						Add_Component();

public:
	const SpriteRect& Get_SpriteRect() { return m_tSpriteRect; }
	const EVENT_PATH& Get_EventPath() { return m_eEventPath; }
public:
	void					Set_SpriteRect(const SpriteRect& tSpriteRect) { m_tSpriteRect = tSpriteRect; }
	void					Setting(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale,
		const MAPOBJECT_CATEGORY& eCat, const MAPOBJECT_PLACEMENT& ePlacement, const SpriteRect& tSpriteRect);
	void					Set_EventPath(const EVENT_PATH& eType) { m_eEventPath = eType; }
public:

	static	CMapToolMapObject* Create(LPDIRECT3DDEVICE9 device);

protected:
	SpriteRect			m_tSpriteRect;
	EVENT_PATH			m_eEventPath;
};

END
