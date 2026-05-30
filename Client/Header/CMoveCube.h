#pragma once
#include "CEventCube.h"
#include "CMapObject.h"



class CMoveCube :
    public CEventCube
{
private:
	explicit CMoveCube(LPDIRECT3DDEVICE9 Device);
	explicit CMoveCube(const CMoveCube& cpy);
	virtual ~CMoveCube();


public:

	virtual     _int        Update_GameObject(const _float& fTimeDelta);
	virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
	virtual     void        Render_GameObject();

public:

	void		PlayEvent() override;

private:

	void		MoveToTargetPos(const _float& DT);
	void		CheckMoveEnd();

	virtual		HRESULT		Add_Component();
	virtual     HRESULT     Ready_GameObject();
	virtual		void		Free();

private:

	_vec3					m_vTargetPos;
	_vec3					m_vDir;
	_bool					m_isMoveEnd;

	_bool					m_isHaveWallDecor;
	CMapObject* m_pTargetWallDecor;

	_float					m_fMoveSpeed;

public:
	static CMoveCube* Create(LPDIRECT3DDEVICE9 Device);

};

