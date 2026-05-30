#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CScene.h"

BEGIN(Engine)

using SceneFactory = std::function<CScene* ()>;

using LoadingFactory = std::function<CScene* (LOADINGID eID)>;

class ENGINE_DLL CSceneManager :
    public CBase
{
	DECLARE_SINGLETON(CSceneManager)

public:
	enum SCENEID { SC_LOGO, SC_MENU, SC_EDIT, SC_STAGE, SC_END };

private:

	CSceneManager();
	virtual ~CSceneManager();

public:
	// 각각 Scene을 MainGame처럼 쓰기 위한 함수
	SCENETYPE Get_CurrentSceneType() { return m_SceneType; }
	CScene* Get_CurrentScene() { return m_nowScene; }

public:
	void RegisterLoadingScene(SCENETYPE type, LoadingFactory factory);	// ㅋㅋ 비효율 끝판왕
	void RegisterFactory(SCENETYPE type, SceneFactory factory);
	void Initalize();
	void Update_Scene(const float& fTimeDelta);
	void LateUpdate_Scene(const float& fTimeDelta);
	void Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

	void Change_Scene(SCENETYPE _sceneType, LOADINGID eID);

private:
	CScene* m_nowScene; // 현재 씬

	SCENETYPE m_SceneType; // 씬 타입

	// 1차 팀과제 때 형준이 생각나서 std:: 붙여본다. 추억이다 그지
	std::unordered_map<SCENETYPE, SceneFactory> m_mSceneList;

	std::unordered_map<SCENETYPE, LoadingFactory> m_mLoading;

private:
	virtual	void		Free();


};

END
