#pragma once
#include "CMapToolMapObject.h"

class CMapToolDoor :
    public CMapToolMapObject
{

	explicit CMapToolDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMapToolDoor(const CMapToolDoor& rhs);
	virtual ~CMapToolDoor();

public:

	const DOORTYPE& Get_DoorType() { return m_eDoorType; }
	void			Set_DoorType(const DOORTYPE& eType) { m_eDoorType = eType; }

	void			Set_DoorSwitchCount(const _int& Count)  { m_iDoorSwitchCount = Count; }
	const _int&		Get_DoorSwitchCount()					{ return m_iDoorSwitchCount; }

public:

	virtual     _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual     void		    LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void	        Render_GameObject() override;

protected:
	virtual     HRESULT			Ready_GameObject() override;
	HRESULT						Add_Component();
public:

	static	CMapToolDoor* Create(LPDIRECT3DDEVICE9 device);

protected:

	DOORTYPE				m_eDoorType;
	_int					m_iDoorSwitchCount;

	

private:
	virtual void		Free();
};

