#pragma once

#include "CMapToolMapObject.h"

class CMapToolLight :
    public CMapToolMapObject
{
	explicit CMapToolLight(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMapToolLight(const CMapToolLight& rhs);
	virtual ~CMapToolLight();

public:

	const LIGHT_OBJECT_TYPE& Get_LightType() { return m_eLight_Object_Type; }
	void			Set_LightType(const LIGHT_OBJECT_TYPE& eType) { m_eLight_Object_Type = eType; }

public:

	virtual     _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual     void		    LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void	        Render_GameObject() override;

protected:
	virtual     HRESULT			Ready_GameObject() override;
	HRESULT						Add_Component();
public:

	static	CMapToolLight* Create(LPDIRECT3DDEVICE9 device);

protected:

	LIGHT_OBJECT_TYPE				m_eLight_Object_Type;

private:
	virtual void		Free();
};

