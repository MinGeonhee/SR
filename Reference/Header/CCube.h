#pragma once
#include "CGameObject.h"
#include "CCubeTex.h"

BEGIN(Engine)

class ENGINE_DLL CCube :
    public CGameObject
{
protected:

	explicit CCube(const CCube& cpy);
	explicit CCube(LPDIRECT3DDEVICE9 device);
	virtual	~CCube(); 

public:

	virtual		HRESULT				Ready_GameObject();
	virtual     _int				Update_GameObject(const _float& fTimeDelta);
	virtual     void				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual     void				Render_GameObject();

public:

	void							SetEvent() { m_IsEvent = true; }
	void							SetCubeType(const CUBE_TYPE& eType) { m_eCubeType = eType; }

public:
	const _bool& 					GetIsEvent() { return m_IsEvent; }
	_vec2*							GetCenterUV();

	const CUBE_TYPE&				GetCubeType() { return m_eCubeType; }
	_bool*							GetCubeHorizontalFilpArr();
	_bool*							GetCubeVerticalFilpArr();

	void							SetCubeHorizontalFilpArr(_bool* arr);
	void							SetCubeVerticalFilpArr(_bool* arr);

protected:

	HRESULT							Add_Component();

public:
	void							Change_CubeTextureFaceUV(const CUBE_FACE& eType ,const _vec2& vCenterUV);
	void							Filp_Horizontal(const CUBE_FACE& etype);
	void							Filp_Vertically(const CUBE_FACE& etype);
public:

	void							SemiUpdate();

public:

	static							CCube* Create(LPDIRECT3DDEVICE9 device);

protected:

	CUBE_TYPE						m_eCubeType;
	CCubeTex*						m_pBuffer;
	bool							m_IsEvent;

private:
	virtual    void					Free() override;
};

END