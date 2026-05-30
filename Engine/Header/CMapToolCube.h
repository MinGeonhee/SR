#pragma once
#include "CCube.h"

BEGIN(Engine)

class ENGINE_DLL CMapToolCube : public CCube
{

private:

	CMapToolCube(LPDIRECT3DDEVICE9 device);
	CMapToolCube(const CMapToolCube& cpy);


private:
	virtual     HRESULT			Ready_GameObject(const FLOOR& eFloor, const _ushort& sRoomNum, const _vec3& vPos);
	virtual     HRESULT			Ready_GameObject();
	HRESULT						Add_Component();

public:

	virtual     _int        Update_GameObject(const _float& fTimeDelta) override;
	virtual     void        LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void        Render_GameObject() override;

public:
	void					SetTargetMakeIndex(const _int& iMakeIndex) { m_iTargetMakeIndex = iMakeIndex; };
	const int&				GetTargetMakeIndex() { return m_iTargetMakeIndex; }

	void					SetCubeTransparent();
	void					SetCubeDefault();

public:

	//이벤트 타입은 나중에 Info에서 바꿔줄수 있도록 해줄예정
	static	CMapToolCube* Create(LPDIRECT3DDEVICE9 device, const FLOOR& eFloor, const _ushort& sRoomNum, const _vec3& vPos);
	static	CMapToolCube* Create(LPDIRECT3DDEVICE9 device);

private:

	int					m_iTargetMakeIndex;
};

END
