#pragma once
#include "CScene.h"
class CBossStage2 :
    public CScene
{
public:
	explicit CBossStage2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossStage2();

public:

	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

private:
	HRESULT			Ready_Environment_Layer(const wstring& pLayerTag);
	HRESULT			Ready_GameLogic_Layer(const wstring& pLayerTag);
	HRESULT			Ready_UI_Layer(const wstring& pLayerTag);

private:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_Manager();

public:
	static CBossStage2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	float Get_ElapsedTime() const { return m_fTimer; }
	float Get_ClearTime()  const { return m_fClearTime; }
	float Get_ParTime()    const { return m_fParTime; }
	float Get_ClearScore()    const { return m_iClearScore; }
	bool  IsCleared()      const { return m_bClear; }

	int ComputeScore(float clear, float par, int treasure);

protected:
	virtual void			Free();
	bool Clear();
	float m_fTimer = 0.f;
	float m_fParTime = 0.f;
	float m_fClearTime = 0.f;

	int   m_iTreasure = 0;
	int   m_iClearScore = 0;

	bool  m_bClear = false;
	float m_fCarryTime = 0.f;
};