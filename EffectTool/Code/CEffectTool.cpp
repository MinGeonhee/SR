#include "pch.h"
#include "CEffectTool.h"
#include "CManagement.h"
#include "CFrameManager.h"
#include "CTimeManager.h"
#include "CInputManager.h"
#include "CImGuiDemo.h"
#include "CMainScene.h"
#include "CResourceManager.h"
#include "CRendererManager.h"
#include "CCameraManager.h"

CEffectTool::CEffectTool()
    : m_pGraphicDev(nullptr), 
	m_pManagementClass(CManagement::GetInstance()), m_pDeviceClass(nullptr), m_pImGui(nullptr)
{
}

CEffectTool::~CEffectTool()
{
}

void CEffectTool::ShowFPS(const float& fTimeDelta)
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

HRESULT CEffectTool::Ready_EffectTool()
{
    if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
        return E_FAIL;

    if (FAILED(Ready_Scene(m_pGraphicDev)))
        return E_FAIL;

    if (FAILED(Ready_ImGuiDemo(m_pGraphicDev)))
        return E_FAIL;



    return S_OK;
}

int CEffectTool::Update_EffectTool(const float& fTimeDelta)
{
    m_pManagementClass->Update_Scene(fTimeDelta);
    m_pImGui->Update_ImGui(fTimeDelta);
	ShowFPS(fTimeDelta);
    return 0;
}

void CEffectTool::LateUpdate_EffectTool(const float& fTimeDelta)
{
    m_pManagementClass->LateUpdate_Scene(fTimeDelta);
    m_pImGui->LateUpdate_ImGui();
}

void CEffectTool::Render_EffectTool()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
 
    m_pDeviceClass->Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));

    m_pManagementClass->Render_Scene(m_pGraphicDev);
    
    m_pImGui->Render_ImGui();
    m_pDeviceClass->Render_End();
}

HRESULT CEffectTool::Ready_DefaultSetting(LPDIRECT3DDEVICE9* mGraphicDev)
{
    if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
    {
        MSG_BOX("CGraphicDev Ready Failed");
        return E_FAIL;
    }

    m_pDeviceClass->Add_Ref();

    (*mGraphicDev) = CGraphicDev::GetInstance()->Get_GraphicDev();
    (*mGraphicDev)->AddRef();

    //_matrix		matView, matProj;
    //
    //_vec3	vEye{ 0.f, 0.0f, 0.f };
    //_vec3	vAt{ 0.f, 0.f, 1.f };
    //_vec3	vUp{ 0.f , 1.f, 0.f };
    //
    //D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
    //(*mGraphicDev)->SetTransform(D3DTS_VIEW, &matView);
    //
    //D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), float(WINCX) / WINCY, 0.1f, 1000.f);
    //(*mGraphicDev)->SetTransform(D3DTS_PROJECTION, &matProj);


    return S_OK;
}

HRESULT CEffectTool::Ready_Scene(LPDIRECT3DDEVICE9 mGraphicDev)
{
    Engine::CScene* pScene = CMainScene::Create(mGraphicDev);
    if (nullptr == pScene)
        return E_FAIL;

    if (FAILED(m_pManagementClass->Set_Scene(pScene)))
    {
        MSG_BOX("Scene Setting Failed");
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CEffectTool::Ready_ImGuiDemo(LPDIRECT3DDEVICE9 device)
{
   if (m_pImGui != nullptr)
        return E_FAIL;
   
   m_pImGui = new CImGuiDemo;
   
   m_pImGui->Init(device);

    return S_OK;
}

CEffectTool* CEffectTool::Create()
{
    CEffectTool* pEffectTool = new CEffectTool;
    if (pEffectTool == nullptr)
        return nullptr;


    if (FAILED(pEffectTool->Ready_EffectTool()))
    {
        MSG_BOX("EffectTool::Create Is Failed In CEffectTool->Ready_Effecttool()");
        delete pEffectTool;
        pEffectTool = nullptr;
    }

    return pEffectTool;
}

void CEffectTool::Free()
{
    Engine::Safe_Release(m_pDeviceClass);
    Engine::Safe_Release(m_pGraphicDev);

    CProtoTypeManager::GetInstance()->DestroyInstance();
    CFrameManager::GetInstance()->DestroyInstance();
    CTimeManager::GetInstance()->DestroyInstance();
    CInputManager::GetInstance()->DestroyInstance();
    CCameraManager::GetInstance()->DestroyInstance();
    CRendererManager::GetInstance()->DestroyInstance();
    CResourceManager::GetInstance()->DestroyInstance();


    m_pManagementClass->DestroyInstance();
    m_pDeviceClass->DestroyInstance();

    Safe_Delete(m_pImGui);
}
