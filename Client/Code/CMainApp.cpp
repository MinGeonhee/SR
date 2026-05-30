#include "pch.h"
#include "CMainApp.h"
#include "CManagement.h"
#include "CProtoTypeManager.h"
#include "CFrameManager.h"
#include "CTimeManager.h"
#include "CLogo.h"
#include "CShape.h"
#include "CCameraManager.h"
#include "CRendererManager.h"
#include "CInputManager.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CDebugLine.h"
#include "CDebugLineHelper.h"
#include "CCollisionEventManager.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CUIManager.h"
#include "CSystemManager.h"
#include "CEventBus.h"
#include "CSceneManager.h"
#include "CStage.h"
#include "CTutorialStage.h"
#include "CBossStage.h"
#include "CBossStage2.h"
#include "CPartitionManager.h"
#include "CSoundManager.h"
#include "CLightManager.h"
#include "CTriggerEventManager.h"
#include "CScoreBoard.h"
#include "CStage2.h"

CMainApp::CMainApp() : m_pGraphicDev(nullptr)
, m_pManagementClass(CManagement::GetInstance())
{

}

CMainApp::~CMainApp()
{

}


HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
		return E_FAIL;

	Ready_ProtoType();
	Ready_SceneSetting();
	Ready_CollisionTable();
	Ready_TextureSetting();
	CPartitionManager::GetInstance()->Start();
	CDebugLine::Initialize(m_pGraphicDev);
	CSoundManager::GetInstance()->Initialize();

	return S_OK; // 함수가 정상적으로 완료됐다는 것을 return 함.
}

HRESULT CMainApp::Ready_SceneSetting()
{
	auto pScene = CSceneManager::GetInstance();

	pScene->RegisterFactory(
		SCENETYPE::LOGO,
		[=]() -> CScene* { return CLogo::Create(m_pGraphicDev); });

	pScene->RegisterFactory(
		SCENETYPE::STAGE1,
		[=]() -> CScene* { return CStage::Create(m_pGraphicDev); });

	pScene->RegisterFactory(
		SCENETYPE::STAGE2,
		[=]() -> CScene* { return CStage2::Create(m_pGraphicDev); });

	pScene->RegisterFactory(
		SCENETYPE::TUTORIAL,
		[=]() -> CScene* { return CTutorialStage::Create(m_pGraphicDev); });

	pScene->RegisterFactory(
		SCENETYPE::BOSS,
		[=]() -> CScene* { return CBossStage::Create(m_pGraphicDev); });

	pScene->RegisterFactory(
		SCENETYPE::BOSS2,
		[=]() -> CScene* { return CBossStage2::Create(m_pGraphicDev); });

	pScene->RegisterLoadingScene(
		SCENETYPE::LOADING,
		[=](LOADINGID eID) -> CScene* { return CLoading::Create(m_pGraphicDev, eID); });

	CSceneManager::GetInstance()->Initalize();

	return S_OK;
}

HRESULT CMainApp::Ready_ProtoType()
{
	// =============== 버퍼 컴포넌트 ===============

	REGISTER_PROTOTYPE(PROTO_RECTCOL, CRectCol, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_PLANECOL, CPlaneColl, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_CUBECOL, CCubeCol, m_pGraphicDev);

	// =============== 버퍼 컴포넌트 ===============

	REGISTER_PROTOTYPE(PROTO_SKYCUBETEX, CSkyCube, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_RECTEX, CRecTex, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_TERRAINTEX, CTerrainTex, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_UIRECTEX, CUIRecTex, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_TILETEX, CTileTex, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_CUBETEX, CCubeTex, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_PARTICLERECTEX, CParticleRecTex, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_PARTICLEONFLOORTEX, CParticleOnFloorRecTex, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_MAPOBJECTTEX, CMapObjectTex, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_PARTICLERECTEX_ALPHA, CParticleRecTex_Alpha, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_UITILETEX, CUITileTex, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_BOSS_APPEAR_MESH_TEX, CBossAppearMeshTex, m_pGraphicDev);

	// =============== 기타 컴포넌트 ===============
	REGISTER_PROTOTYPE(PROTO_TRANS, CTransform, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_CAMERA, CCamera, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_BILLBOARD, CBillboard, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_ANIMATION, CAnimation, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_MESH_ANIMATION, CMeshAnimation, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_BOSS_EFFECT_ANIMATION, CBossEffectAnimation, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_STATUS, CStatus, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_WEAPON, CWeaponController, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_DOOR, CDoor_Component, m_pGraphicDev);	// 문 컴포넌트
	REGISTER_PROTOTYPE(PROTO_HOVERITEM, CHoverItem, m_pGraphicDev);	// 둥실 컴포넌트
	REGISTER_PROTOTYPE(PROTO_DROPITEM, CDropItem, m_pGraphicDev);	// 둥실 컴포넌트
	REGISTER_PROTOTYPE(PROTO_WALLET, CWallet, m_pGraphicDev);	// 둥실 컴포넌트
	REGISTER_PROTOTYPE(PROTO_BARRIER, CBarrier, m_pGraphicDev);	// 둥실 컴포넌트

	// =============== 충돌 컴포넌트 ===============
	REGISTER_PROTOTYPE(PROTO_COLLIDER_AABB, CAABB, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB, m_pGraphicDev);
	REGISTER_PROTOTYPE(PROTO_COLLIDER_OBB, COBB, m_pGraphicDev);

	REGISTER_PROTOTYPE(PROTO_COLLIDER_SPHERE, CSphere, m_pGraphicDev);

	return S_OK;
}

HRESULT CMainApp::Ready_CollisionTable()
{	
	auto collision = CCollisionManager::GetInstance();

	collision->InitCollisionTable();

	// 여기에 충돌 조합식을 쓴다.
	collision->CheckGroup(OBJ_PLAYER, OBJ_MONSTER_GOBLIN_HAMMER); // 플레이어와 몬스터 충돌할거에요 조합식 씀.
	collision->CheckGroup(OBJ_PLAYER, OBJ_MONSTER_GOBLIN_FIREBALL);
	collision->CheckGroup(OBJ_PLAYER, OBJ_MONSTER_ORGE_AXE);
	collision->CheckGroup(OBJ_PLAYER, OBJ_MONSTER_RAT);
	collision->CheckGroup(OBJ_PLAYER, OBJ_MONSTER_SKELETON_SWORD);
	collision->CheckGroup(OBJ_PLAYER, OBJ_MONSTER_WIZARD);
	collision->CheckGroup(OBJ_PLAYER, OBJ_MONSTER_BOSS);

	collision->CheckGroup(OBJ_BULLET, OBJ_MAPEVENT);
	collision->CheckGroup(OBJ_PLAYER, OBJ_DOOR); 
	collision->CheckGroup(OBJ_MONSTER_GOBLIN_HAMMER, OBJ_DOOR);
	collision->CheckGroup(OBJ_MONSTER_GOBLIN_FIREBALL, OBJ_DOOR);
	collision->CheckGroup(OBJ_MONSTER_ORGE_AXE, OBJ_DOOR);
	collision->CheckGroup(OBJ_MONSTER_RAT, OBJ_DOOR);
	collision->CheckGroup(OBJ_MONSTER_SKELETON_SWORD, OBJ_DOOR);
	collision->CheckGroup(OBJ_MONSTER_WIZARD, OBJ_DOOR);

	// 이펙트 충돌 조합식
	collision->CheckGroup(OBJ_PARTICLE, OBJ_TILE);
	collision->CheckGroup(OBJ_PARTICLE, OBJ_CUBE);
	collision->CheckGroup(OBJ_PARTICLE, OBJ_EVENTCUBE);

	// 총알 충돌 조합식
	collision->CheckGroup(OBJ_PLAYER, OBJ_BULLET);
	collision->CheckGroup(OBJ_MONSTER_GOBLIN_FIREBALL, OBJ_BULLET);
	collision->CheckGroup(OBJ_MONSTER_GOBLIN_HAMMER, OBJ_BULLET);
	collision->CheckGroup(OBJ_MONSTER_ORGE_AXE, OBJ_BULLET);
	collision->CheckGroup(OBJ_MONSTER_RAT, OBJ_BULLET);
	collision->CheckGroup(OBJ_MONSTER_SKELETON_SWORD, OBJ_BULLET);
	collision->CheckGroup(OBJ_MONSTER_WIZARD, OBJ_BULLET);
	collision->CheckGroup(OBJ_MONSTER_BOSS, OBJ_BULLET);
	collision->CheckGroup(OBJ_MONSTER_BOSS_CLONE, OBJ_BULLET);
	collision->CheckGroup(OBJ_TILE, OBJ_BULLET);
	collision->CheckGroup(OBJ_CUBE, OBJ_BULLET);
	collision->CheckGroup(OBJ_EVENTCUBE, OBJ_BULLET);
	collision->CheckGroup(OBJ_BARRIER, OBJ_BULLET);

	// 벽 충돌 조합식
	collision->CheckGroup(OBJ_PLAYER, OBJ_CUBE);
	collision->CheckGroup(OBJ_PLAYER, OBJ_EVENTCUBE);
	collision->CheckGroup(OBJ_PLAYER, OBJ_MAPOBSTACLE);
	collision->CheckGroup(OBJ_PLAYER, OBJ_MAPEVENT);
	collision->CheckGroup(OBJ_PLAYER, OBJ_CREATURECANTGO);
	collision->CheckGroup(OBJ_PLAYER, OBJ_LIGHT);

	collision->CheckGroup(OBJ_MONSTER_GOBLIN_HAMMER, OBJ_CUBE);
	collision->CheckGroup(OBJ_MONSTER_GOBLIN_HAMMER, OBJ_CREATURECANTGO);

	collision->CheckGroup(OBJ_MONSTER_GOBLIN_FIREBALL, OBJ_CUBE);
	collision->CheckGroup(OBJ_MONSTER_GOBLIN_FIREBALL, OBJ_CREATURECANTGO);

	collision->CheckGroup(OBJ_MONSTER_ORGE_AXE, OBJ_CUBE);
	collision->CheckGroup(OBJ_MONSTER_ORGE_AXE, OBJ_CREATURECANTGO);

	collision->CheckGroup(OBJ_MONSTER_RAT, OBJ_CUBE);
	collision->CheckGroup(OBJ_MONSTER_RAT, OBJ_CREATURECANTGO);

	collision->CheckGroup(OBJ_MONSTER_SKELETON_SWORD, OBJ_CUBE);
	collision->CheckGroup(OBJ_MONSTER_SKELETON_SWORD, OBJ_CREATURECANTGO);

	collision->CheckGroup(OBJ_MONSTER_WIZARD, OBJ_CUBE);
	collision->CheckGroup(OBJ_MONSTER_WIZARD, OBJ_CREATURECANTGO);


	// 아이템 충돌 조합식
	collision->CheckGroup(OBJ_PLAYER, OBJ_ITEM);

	// ================ 광선 충돌 조합식 =================
	collision->CheckRaycastGroup(MACE_RAY, OBJ_MONSTER_GOBLIN_HAMMER);
	collision->CheckRaycastGroup(MACE_RAY, OBJ_MONSTER_GOBLIN_FIREBALL);
	collision->CheckRaycastGroup(MACE_RAY, OBJ_MONSTER_ORGE_AXE);
	collision->CheckRaycastGroup(MACE_RAY, OBJ_MONSTER_RAT);
	collision->CheckRaycastGroup(MACE_RAY, OBJ_MONSTER_SKELETON_SWORD);
	collision->CheckRaycastGroup(MACE_RAY, OBJ_MONSTER_WIZARD);
	collision->CheckRaycastGroup(MACE_RAY, OBJ_MONSTER_BOSS);
	collision->CheckRaycastGroup(MACE_RAY, OBJ_MONSTER_BOSS_CLONE);

	collision->CheckRaycastGroup(INTERACTION_RAY, OBJ_DOOR);
	collision->CheckRaycastGroup(EVENT_CUBE_RAY, OBJ_EVENTCUBE);
	collision->CheckRaycastGroup(EYEOFCHAOS_RAY, OBJ_MAPEVENT);

	//collision->CheckRaycastGroup(INTERACTION_RAY, OBJ_DOOR);
	collision->CheckRaycastGroup(INTERACTION_RAY, OBJ_MAPEVENT);
	collision->CheckRaycastGroup(MACE_RAY, OBJ_MAPEVENT);
	collision->CheckRaycastGroup(MACE_RAY, OBJ_EVENTCUBE);
	return S_OK;
}

HRESULT CMainApp::Ready_TextureSetting()
{
	auto Resource = CResourceManager::GetInstance();
	Resource->Load<CTexture>(L"LoadingBar", L"../Bin/Resource/Texture/UI/LoadingBar.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"LoadingGage", L"../Bin/Resource/Texture/UI/LoadingGage.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"LoadBackGround", L"../Bin/Resource/Texture/Logo/Wizordum_Cover_Loading.png", m_pGraphicDev);

	return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
	CInputManager::GetInstance()->Update();
	CSceneManager::GetInstance()->Update_Scene(fTimeDelta);
	CTriggerEventManager::GetInstance()->Update_Event(fTimeDelta);
	CDebugLineHelper::GetInstance()->Debug_Update();

	CSystemManager::GetInstance()->System_Update(fTimeDelta);


	return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fTimeDelta)
{
	CSceneManager::GetInstance()->LateUpdate_Scene(fTimeDelta);
	CTriggerEventManager::GetInstance()->Late_Update(fTimeDelta);
	CSystemManager::GetInstance()->System_LateUpdate(fTimeDelta);
	// 모든 이벤트 삭제.
	CEventBus::GetInstance()->Delete_Event();
	ShowFPS(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.05f, 0.05f, 0.08f, 1.f));

	CSceneManager::GetInstance()->Render_Scene(m_pGraphicDev);
	m_pDeviceClass->Render_End();
}



HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd,
		MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
	{
		MSG_BOX("CGraphicDev Ready Failed");
		return E_FAIL;
	}

	m_pDeviceClass->Add_Ref();

	(*ppGraphicDev) = CGraphicDev::GetInstance()->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp;

	// E_FAIL는 0x80004005
	// E_FAIL이 뜨면 FAILED가 참이 되면서 → 음수값이기 때문에 삭제함.
	if (FAILED(pMainApp->Ready_MainApp()))
	{
		delete pMainApp;
		pMainApp = nullptr;
	}
	return pMainApp;
}

void CMainApp::Free()
{
	// 제거를 할 때 참조 reference count를 내린다.
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pGraphicDev);

	CProtoTypeManager::GetInstance()->DestroyInstance();
	CTimeManager::GetInstance()->DestroyInstance();
	CFrameManager::GetInstance()->DestroyInstance();
	CCameraManager::GetInstance()->DestroyInstance();
	CRendererManager::GetInstance()->DestroyInstance();
	CInputManager::GetInstance()->DestroyInstance();
	CResourceManager::GetInstance()->DestroyInstance();
	CDebugLineHelper::GetInstance()->DestroyInstance();

	CCollisionEventManager::GetInstance()->DestroyInstance();
	CTriggerEventManager::GetInstance()->DestroyInstance();
	
	CUIManager::GetInstance()->DestroyInstance();
	CSceneManager::GetInstance()->DestroyInstance();

	CScoreBoard::DestroyInstance();
	// Event 
	CSystemManager::GetInstance()->DestroyInstance();
	CEventBus::GetInstance()->DestroyInstance();

	// objectManager들은 제일 후순위에서 파괴
	CPartitionManager::GetInstance()->Stop();
	CPartitionManager::GetInstance()->DestroyInstance();
	CObjectManager::GetInstance()->DestroyInstance();
	CObjectPoolingManager::GetInstance()->DestroyInstance();
	CCollisionManager::GetInstance()->DestroyInstance();
	CSoundManager::GetInstance()->DestroyInstance();
	CLightManager::GetInstance()->DestroyInstance();

	m_pManagementClass->DestroyInstance();
	m_pDeviceClass->DestroyInstance();
}

void CMainApp::ShowFPS(const float& fTimeDelta)
{
	++fps;

	timer += fTimeDelta;

	if (timer >= 1.f)
	{
		TCHAR m_szFPS[32];

		swprintf_s(m_szFPS, L"FPS : %d", fps);

		SetWindowText(g_hWnd, m_szFPS);

		fps = 0;
		timer = 0.f;
	}
}