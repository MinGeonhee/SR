#include "pch.h"
#include "CMapTool.h"
#include "CFrameManager.h"
#include "CTimeManager.h"
#include "CInputManager.h"
#include "CCameraMan.h"
#include "CRendererManager.h"
#include "CCameraManager.h"
#include "CManagement.h"
#include "CGrid.h"
#include "CToolScene.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CDebugLineHelper.h"
#include "CCollisionEventManager.h"
#include "CCubeManager.h"
#include "CGridManager.h"
#include "CToolManager.h"
#include "CTileManager.h"
#include "CMapObjectManager.h"

CMapTool::CMapTool()
    : m_pGraphicDev(nullptr), m_pManagementClass(CManagement::GetInstance()), m_pDeviceClass(nullptr) , m_pImGui(nullptr)
    , m_FPS(0), m_fTimer(0.f)
{
}
CMapTool::~CMapTool()
{
}

HRESULT CMapTool::Ready_Maptool()
{

    if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
        return E_FAIL;

    if (FAILED(Ready_ImGuiDemo(m_pGraphicDev)))
        return E_FAIL;

    if (FAILED(Ready_Scene(m_pGraphicDev)))
        return E_FAIL;


    return S_OK;

}

int CMapTool::Update_Maptool(const float& fTimeDelta)
{
    CheckFrame(fTimeDelta);

    m_pManagementClass->Update_Scene(fTimeDelta);

    return 0;
}
void CMapTool::LateUpdate_Maptool(const float& fTimeDelta)
{
    CInputManager::GetInstance()->Update();

    //씬 업데이트
    m_pManagementClass->LateUpdate_Scene(fTimeDelta);

    //임구이 RateUpdate
//    m_pImGui->LateUpdate_ImGui();
    m_pImGui->Update_ImGui(fTimeDelta);
}

void CMapTool::Render_Maptool()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pDeviceClass->Render_Begin(D3DCOLOR_ARGB(255, 135, 206, 235));

    //CRendererManager::GetInstance()->Render_GameObject(m_pGraphicDev);
    m_pManagementClass->Render_Scene(m_pGraphicDev);

    m_pImGui->Render_ImGui();
    m_pDeviceClass->Render_End();
}

HRESULT CMapTool::Ready_DefaultSetting(LPDIRECT3DDEVICE9* mGraphicDev)
{
    if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
    {
        MSG_BOX("CGraphicDev Ready Failed");
        return E_FAIL;
    }

    m_pDeviceClass->Add_Ref();

    (*mGraphicDev) = CGraphicDev::GetInstance()->Get_GraphicDev();
    (*mGraphicDev)->AddRef();


    return S_OK;
}
HRESULT CMapTool::Ready_ImGuiDemo(LPDIRECT3DDEVICE9 device)
{
    if (m_pImGui != nullptr)
        return E_FAIL;


    m_pImGui = CImGui::GetInstance();

    m_pImGui->Init(device);


    return S_OK;
}


HRESULT CMapTool::Ready_Scene(LPDIRECT3DDEVICE9 device)
{
    Engine::CScene* pScene = CToolScene::Create(device);
    if (nullptr == pScene)
        return E_FAIL;

    if (FAILED(m_pManagementClass->Set_Scene(pScene)))
    {
        MSG_BOX("Scene Setting Failed");
        return E_FAIL;
    }

    return S_OK;
}

void CMapTool::CheckFrame(const _float& fTimeDelta)
{
    ++m_FPS;


    m_fTimer += fTimeDelta;

    if (m_fTimer >= 1.f)
    {
        TCHAR m_szFPS[32];

        swprintf_s(m_szFPS, L"FPS : %d", m_FPS);

        SetWindowText(g_hWnd, m_szFPS);

        m_FPS = 0;
        m_fTimer = 0.f;
    }
}


CMapTool* CMapTool::Create()
{
    CMapTool* NewMapTool = new CMapTool;
    if (NewMapTool == nullptr)
        return nullptr;


    if (FAILED(NewMapTool->Ready_Maptool()))
    {
        MSG_BOX("MapTool::Create Is Failed In ewMapTool->Ready_Maptool()");
        delete NewMapTool;
        NewMapTool = nullptr;
    }

    return NewMapTool;
}

void CMapTool::Free()
{
    CFrameManager::GetInstance()->DestroyInstance();
    CTimeManager::GetInstance()->DestroyInstance();
    CInputManager::GetInstance()->DestroyInstance();
    CCameraManager::GetInstance()->DestroyInstance();
    CProtoTypeManager::GetInstance()->DestroyInstance();
    CRendererManager::GetInstance()->DestroyInstance();
    CResourceManager::GetInstance()->DestroyInstance();
    CManagement::GetInstance()->DestroyInstance();

    CGridManager::GetInstance()->DestroyInstance();
    CTileManager::GetInstance()->DestroyInstance();
    CCubeManager::GetInstance()->DestroyInstance();
    CToolManager::GetInstance()->DestroyInstance();

    CCollisionManager::GetInstance()->DestroyInstance();
    CDebugLineHelper::GetInstance()->DestroyInstance();
    CCollisionEventManager::GetInstance()->DestroyInstance();
    CMapObjectManager::GetInstance()->DestroyInstance();
    m_pDeviceClass->DestroyInstance();
    
   
    Safe_Release(m_pImGui);
}
