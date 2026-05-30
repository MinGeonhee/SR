#pragma once
#include "CGameObject.h"
#include "CCubeTex.h"

BEGIN(Engine)

class ENGINE_DLL CreatureCantGoCube
	:	public CGameObject
{
protected:
	explicit CreatureCantGoCube(const CreatureCantGoCube& cpy);
	explicit CreatureCantGoCube(LPDIRECT3DDEVICE9 device);
	virtual	~CreatureCantGoCube();

public:

	virtual     HRESULT     Ready_GameObject();
	virtual     _int        Update_GameObject(const _float& fTimeDelta);
	virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
	virtual     void        Render_GameObject();
protected:

	virtual		HRESULT				Add_Component();

public:

	void							SetCubeType(const CUBE_TYPE& eType) { m_eCubeType = eType; }
	const				CUBE_TYPE&  GetCubeType() { return m_eCubeType; }
	const _bool&					GetIsEvent() { return m_IsEvent; }


public:

	void							Change_CubeTextureFaceUV(const CUBE_FACE& eType, const _vec2& vCenterUV);

	static	CreatureCantGoCube*		Create(LPDIRECT3DDEVICE9 device);

	void					SemiUpdate();

private:
	virtual  void					Free();

private:
	CUBE_TYPE						m_eCubeType;
	bool							m_IsEvent;

};

END