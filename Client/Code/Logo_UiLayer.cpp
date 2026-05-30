#include "pch.h"
#include "Logo_UiLayer.h"
#include "CCameraManager.h"
#include "CPlayer.h"
#include "CObjectCamera.h"
#include "CInputManager.h"
#include "CBackGround.h"
#include "CMain_UI.h"
#include "CUIManager.h"
#include "CLogo_UI.h"
#include "CUIBackGround.h"
#include "CSelect_UI.h"
#include "CSelect_Achive_UI.h"
#include "CUI_Image.h"
#include "CUI_Fade.h"
#include "CSetting_UI.h"
#include "CStoryScript_UI.h"

Logo_UiLayer::Logo_UiLayer()
{

}

HRESULT Logo_UiLayer::Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev)
{
    Engine::CGameObject* pCameraObject = nullptr;

    // UI 전용 카메라 설정
    if (CCameraManager::GetInstance()->Get_Camera(L"UI_Camera") == NULL)
    {
        pCameraObject = CObjectCamera::Create(pGraphicDev);

        if (nullptr == pCameraObject)
            return E_FAIL;

        if (FAILED(CCameraManager::GetInstance()->
            Ready_Camera(L"UI_Camera", pCameraObject)))
            return E_FAIL;

        // 저 UI 카메라인데요. 선언하기
        static_cast<CObjectCamera*>(pCameraObject)->Get_Camera()->Set_CameraState(CAM_UIMODE);
        CCameraManager::GetInstance()->Set_UICamera(L"UI_Camera");
        CCameraManager::GetInstance()->Set_UICameraCom(static_cast<CObjectCamera*>(pCameraObject)->Get_Camera());
    }

    // ============ 원래 우리 로고 =============

    Engine::CGameObject* pGameObject = nullptr;

    // BackGround
    pGameObject = CBackGround::Create(pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(Add_GameObject(L"BackGround", pGameObject)))
        return E_FAIL;


    Engine::CUI* pUI = nullptr;

    pUI = CUIBackGround::Create(pGraphicDev, L"Back");
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"UIBackGround");
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CLogo_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"Logo_UI");
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CMain_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"Main_UI");
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CSetting_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"Setting_UI");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CStoryScript_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"StoryScript_UI");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CUI_Fade::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"UI_Fade");
    CUIManager::GetInstance()->Add_UI(pUI);

    if (auto* fade = dynamic_cast<CUI_Fade*>(CUIManager::GetInstance()->Find_UI(L"UI_Fade")))
    {
        fade->StartFadeIn(0.6f, D3DCOLOR_ARGB(255, 0, 0, 0), true);
    }

    return S_OK;
}

_int	Logo_UiLayer::Update_Layer(const _float& fTimeDelta)
{
    CLayer::Update_Layer(fTimeDelta);
    CCameraManager::GetInstance()->Update_UICamera(fTimeDelta);
    CUIManager::GetInstance()->Update_UI(fTimeDelta);
    return 0;
}

void	Logo_UiLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
    CLayer::LateUpdate_Layer(fTimeDelta);
    CCameraManager::GetInstance()->LateUpdate_UICamera(fTimeDelta);
    CUIManager::GetInstance()->LateUpdate_UI(fTimeDelta);
}

void	Logo_UiLayer::Render_Layer()
{

}

Logo_UiLayer* Logo_UiLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    Logo_UiLayer* pStUI = new Logo_UiLayer();

    if (FAILED(pStUI->Ready_Layer(pGraphicDev)))
    {
        MSG_BOX("Stage UI Layer Create Failed");
        Safe_Release(pStUI);
        return nullptr;
    }

    return pStUI;
}

void Logo_UiLayer::Free()
{
    CLayer::Free();
}