#pragma once
#include "CMapToolMapObject.h"


class CMapToolMonster :
    public CMapToolMapObject
{
protected:

	explicit CMapToolMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMapToolMonster(const CMapToolMonster& rhs);
	virtual ~CMapToolMonster();

public:

	const MONSTER_TYPE& Get_MonsterType() { return m_eMonsterType; }
	void			Set_MonsterType(const MONSTER_TYPE& eType) { m_eMonsterType = eType; }

public:

	virtual     _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual     void		    LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void	        Render_GameObject() override;

protected:
	virtual     HRESULT			Ready_GameObject() override;
	HRESULT						Add_Component();
	void						Set_Texture();
public:
	static	CMapToolMonster* Create(LPDIRECT3DDEVICE9 device);



protected:
	MONSTER_TYPE		m_eMonsterType;



private:
	virtual void		Free();
};

