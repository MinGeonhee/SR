#pragma once
#include "CMapToolMapObject.h"

class CMapToolItem :
    public CMapToolMapObject
{
protected:

	explicit CMapToolItem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMapToolItem(const CMapToolItem& rhs);
	virtual ~CMapToolItem();

public:

	const ITEMTYPE& Get_ItemType()									{ return m_eItemType; }
	void			Set_ItemType(const ITEMTYPE& eType)				{ m_eItemType = eType; }

public:

	virtual     _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual     void		    LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void	        Render_GameObject() override;

protected:
	virtual     HRESULT			Ready_GameObject() override;
	HRESULT						Add_Component();
public:

	static	CMapToolItem*		Create(LPDIRECT3DDEVICE9 device);

protected:
	ITEMTYPE		m_eItemType;

private:
	virtual void		Free();
};

