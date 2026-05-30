#pragma once

#include "Engine_Define.h"
#include "CBase.h"

class CGrid;

namespace Engine
{
	class CRendererManager;
}

class CGridManager : public CBase
{
	DECLARE_SINGLETON(CGridManager)
private:
	explicit	CGridManager();
	virtual		~CGridManager();

public:

	void		Init();
	void		GridManager_Update(const _float& DT);
	void		GridManager_LateUpdate(const _float& DT);
	void		GridManager_Render(LPDIRECT3DDEVICE9 device);


	void	SetFloor(const FLOOR& eFloor);
	void	SetGridScale(const _vec3 vScale) { m_vGridScale = vScale; }


	const _vec3& GetGridScal() { return m_vGridScale; }
private:
	
	void MakeGrid_OnAllFloor();

public:


private:
	FLOOR			  m_eCurFloor;
	CGrid*			  m_pGrid[FLOOR_END];

	CRendererManager* m_pRendererManager;
	_vec3		      m_vGridScale;

	virtual			void	Free();
};

