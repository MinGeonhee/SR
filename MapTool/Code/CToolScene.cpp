#include "pch.h"
#include "CToolScene.h"
#include "CProtoTypeManager.h"
#include "CCameraManager.h"
#include "CCameraMan.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CGrid.h"
#include "CTerrain.h"
#include "CToolManager.h"
#include "CDebugLine.h"
#include "CMapToolCube.h"
#include "CImGui.h"
#include "CPathFunction.h"

CToolScene::CToolScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : CScene(pGraphicDev) , m_pToolManager(nullptr)
{
}

CToolScene::~CToolScene()
{
}

HRESULT CToolScene::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_CollisionTable()))
        return E_FAIL;

    if (FAILED(Ready_ToolManager()))
        return E_FAIL;

    if (FAILED(Ready_Resource()))
        return E_FAIL;


    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    return S_OK;
}

_int CToolScene::Update_Scene(const _float& fTimeDelta)
{

    Engine::CScene::Update_Scene(fTimeDelta);
    m_pToolManager->ToolManger_Update(fTimeDelta);
    CCameraManager::GetInstance()->Update_MainCamera(fTimeDelta);


    return 0;
}

void CToolScene::LateUpdate_Scene(const _float& fTimeDelta)
{
    //Engine::CScene::LateUpdate_Scene(fTimeDelta);

    m_pToolManager->ToolManager_LateUpdate(fTimeDelta);
    CCameraManager::GetInstance()->LateUpdate_MainCamera(fTimeDelta);
}

void CToolScene::Render_Scene()
{
    m_pToolManager->ToolManger_Render(m_pGraphicDev);

    CDebugLine::Render();
    CDebugLine::Clear();
}

void CToolScene::Ready_ResourceTileImage(LPDIRECT3DDEVICE9 device)
{

    CImGui* Imgui = CImGui::GetInstance();

    for (int i = 0; i < 32; ++i)
    {
        for (int j = 0; j < 32; ++j)
        {
            TexTileImageUV* tInfo = new TexTileImageUV;
           
            TCHAR szFileName[128] = L"";
            wsprintf(szFileName, L"../../Client/Bin/Resource/Texture/Tile/tile_%d_%d.png", i, j);
            wstring wstrFilePath = szFileName;
            
            wsprintf(szFileName, L"tile_%d_%d", i, j);
            wstring wstrKey = szFileName;

            tInfo->texID = (ImTextureID)CResourceManager::GetInstance()->Load<CTexture>(wstrKey,wstrFilePath, device)->GetIDirect3DBaseTexture9();

            tInfo->vCenterUV = { TILE_UV_CENTER + TILE_UV_SIZE * j , TILE_UV_CENTER + TILE_UV_SIZE * i };

            Imgui->Add_ImGuiImageUV(tInfo);
        }   
    }

}

HRESULT CToolScene::Ready_Environment_Layer(const wstring& pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();


    Add_Cameraman(pLayer);


    m_mapLayer.insert({pLayerTag, pLayer});

    return S_OK;
}

HRESULT CToolScene::Ready_GameLogic_Layer(const wstring& pLayerTag)
{
    return S_OK;
}

HRESULT CToolScene::Ready_UI_Layer(const wstring& pLayerTag)
{
    return S_OK;
}

HRESULT CToolScene::Ready_Prototype()
{
    REGISTER_PROTOTYPE(PROTO_CAMERA, CCamera, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_TRANS , CTransform, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_TERRAINTEX, CTerrainTex, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_COLLIDER_AABB, CAABB, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB, m_pGraphicDev);
    
    REGISTER_PROTOTYPE(PROTO_CUBECOL, CCubeCol, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_RECTCOL, CRectCol, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_CUBETEX, CCubeTex, m_pGraphicDev);
    REGISTER_PROTOTYPE(PROTO_MAPOBJECTTEX, CMapObjectTex , m_pGraphicDev);


    if (FAILED(CProtoTypeManager::GetInstance()->Ready_Prototype(PROTO_CENTERLINECOL, CCenterLineCol::Create(m_pGraphicDev, 0))))
        return E_FAIL;

    if (FAILED(CProtoTypeManager::GetInstance()->Ready_Prototype(PROTO_TILETEX, CTileTex::Create(m_pGraphicDev, _vec2{0.f,0.f}))))
        return E_FAIL;

    return S_OK;
}
HRESULT CToolScene::Ready_CollisionTable()
{
    //조합식 설정
    // 클릭하지않고 단순 마우스 위치로만 발생할 충돌레이 
    // 클릭x 타일레이 - > 타일 = > 노란색 투명 타일 생성
    // 클릭x 큐브레이 - > 큐브 = > 노란색 투명 큐브 생성
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_TILE_FLOOR_NONE_CLICK_RAY, OBJTYPE::OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_TILE_CEILING_NONE_CLICK_RAY, OBJTYPE::OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CUBE_NONE_CLICK_RAY, OBJTYPE::OBJ_TILE);

    ////클릭할때 발생할 충돌레이
    ////클릭 타일레이 -> 타일 => 타일 생성  타일과 충돌처리후 그자리에 생성인아닌 그 타일의 타입을 변경해줄 예정 타입
    //// NONE 타입이면 버퍼 출력  x 이였다다가 Floor 나 Celling으로 들어가게 되면 텍스처 렌더링 ON
    ////클릭 큐브레이 -> 타일 => 큐브 생성  타일과충돌처리후 그자리에 생성할예정
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_TILE_FLOOR_CLICK_RAY, OBJTYPE::OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_TILE_CEILING_CLICK_RAY, OBJTYPE::OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CUBE_CLICK_RAY, OBJTYPE::OBJ_TILE);
    ////큐브 설치일때도 큐브 설치로

    //// 텍스처 모드일때 클릭 X
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_TILE_FLOOR_NONE_CLICK_TEX_RAY, OBJTYPE::OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_TILE_CEILING_NONE_CLICK_TEX_RAY, OBJTYPE::OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CUBE_NONE_CLICK_TEX_RAY, OBJTYPE::OBJ_CUBE);


    //// 텍스처 모드일때 클릭 
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_TILE_FLOOR_CLICK_TEX_RAY, OBJTYPE::OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_TILE_CEILING_CLICK_TEX_RAY, OBJTYPE::OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CUBE_CLICK_TEX_RAY, OBJTYPE::OBJ_CUBE);


    //  삭제모드일때
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_NONE_CLICK_RAY,OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_NONE_CLICK_RAY,OBJ_CUBE);

    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_NONE_CLICK_RAY, OBJ_MAPOBSTACLE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_NONE_CLICK_RAY, OBJ_MAPDECOR);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_NONE_CLICK_RAY, OBJ_MAPEVENT);

    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_CLICK_RAY, OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_CLICK_RAY, OBJ_CUBE);

    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_CLICK_RAY, OBJ_MAPOBSTACLE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_CLICK_RAY, OBJ_MAPDECOR);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_CLICK_RAY, OBJ_MAPEVENT);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_CLICK_RAY, OBJ_ITEM);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_CLICK_RAY, OBJ_MONSTER);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_CLICK_RAY, OBJ_DOOR);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_DELETE_CLICK_RAY, OBJ_LIGHT);

    //InFo 모드 일때
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_NONE_CLICK_INFO_RAY, OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_NONE_CLICK_INFO_RAY, OBJ_CUBE);

    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CLICK_INFO_RAY, OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CLICK_INFO_RAY, OBJ_CUBE);

    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CLICK_INFO_RAY, OBJ_MAPDECOR);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CLICK_INFO_RAY, OBJ_MAPOBSTACLE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CLICK_INFO_RAY, OBJ_MAPEVENT);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CLICK_INFO_RAY, OBJ_ITEM);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CLICK_INFO_RAY, OBJ_MONSTER);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CLICK_INFO_RAY, OBJ_DOOR);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_CLICK_INFO_RAY, OBJ_LIGHT);

    // 오브젝트 관련 모드일때
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_OBJECT_NONE_CLICK_RAY, OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_OBJECT_NONE_CLICK_RAY, OBJ_CUBE);

    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_OBJECT_CLICK_RAY, OBJ_TILE);
    CCollisionManager::GetInstance()->CheckRaycastGroup(MAPTOOL_OBJECT_CLICK_RAY, OBJ_CUBE);



    return S_OK;
}
HRESULT CToolScene::Ready_Resource()
{
    auto Mgr = CResourceManager::GetInstance();

   Mgr->Load<CTexture>(L"Tile", L"../../Client/Bin/Resource/Texture/Terrain/tileset.png", m_pGraphicDev);
   Mgr->Load<CTexture>(L"Ground_Decor", CPathFunction::GroundDecor_Png_Path_InMapTool, m_pGraphicDev);
   Mgr->Load<CTexture>(L"Wall_Decor", L"../../Client/Bin/Resource/Texture/Object/Decor/Wall_Decor.png", m_pGraphicDev);

   //오브젝트
   Mgr->Load<CTexture>(L"Obstacle", CPathFunction::Obstacle_Png_Path_InMapTool, m_pGraphicDev);
   Mgr->Load<CTexture>(L"Ground_Event",CPathFunction::GroundEvent_Png_Path_InMapTool, m_pGraphicDev);
   Mgr->Load<CTexture>(L"Wall_Event", CPathFunction::WallEvent_Png_Path_InMapTool, m_pGraphicDev);
   
   
   //아이템
   Mgr->Load<CTexture>(L"Item", CPathFunction::MapToolItem_Png_Path_InMapTool, m_pGraphicDev);
   //몬스터
   Mgr->Load<CTexture>(L"Monster", CPathFunction::MapToolMonster_Png_Path_InMapTool, m_pGraphicDev);
   Mgr->Load<CTexture>(L"Monster2", CPathFunction::MapToolMonster2_Png_Path_InMapTool, m_pGraphicDev);
   //문
   Mgr->Load<CTexture>(L"Door", CPathFunction::MapToolDoor_Png_Path_InMapTool, m_pGraphicDev);
   //조명관련
   Mgr->Load<CTexture>(L"Light", CPathFunction::MapToolLight_Png_Path_InMapTool, m_pGraphicDev);


  return S_OK;
}

HRESULT CToolScene::Ready_ToolManager()
{
    m_pToolManager = CToolManager::GetInstance();
    m_pToolManager->Init();

    CDebugLine::Initialize(m_pGraphicDev);

    return S_OK;
}

HRESULT CToolScene::Add_Cameraman(CLayer* pLayer)
{
    //카메라맨 추가
    CCameraMan* newObj = nullptr;
    newObj = CCameraMan::Create(m_pGraphicDev);
    if (newObj == nullptr)
        return E_FAIL;
    CCameraManager::GetInstance()->Ready_Camera(L"Main_Camera", newObj);
    CCameraManager::GetInstance()->Set_MainCamera(L"Main_Camera");
    CCameraManager::GetInstance()->Set_MainCameraCom(newObj->Get_Camera());


    return S_OK;
}

CToolScene* CToolScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CToolScene* pScene = new CToolScene(pGraphicDev);

    if (pScene == nullptr)
        return nullptr;

    if (FAILED(pScene->Ready_Scene()))
    {
        MSG_BOX("Create CToolScene Is Failed");
        Safe_Release(pScene);
        return nullptr;
    }

    return pScene;
}

void CToolScene::Free()
{
    CScene::Free();
}
