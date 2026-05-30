#include "pch.h"
#include "Stage1_UILayer.h"
#include "CCameraManager.h"
#include "CObjectCamera.h"
#include "CPlayerTex.h"
#include "CMap_UI.h"
#include "CPlayerUI.h"
#include "CPauseUI.h"
#include "CUIManager.h"
#include "CUI_NoticeBanner.h"
#include "CUI_Interaction.h"
#include "CShop_UI.h"
#include "CUI_Weapon.h"
#include "CStageClear_UI.h"
#include "CUI_Fade.h"
#include "CBoss_UI.h"
#include "CPlayerStatPanel_UI.h"
#include "CGameOver.h"
#include "CSetting_UI.h"
#include "CWeaponGet_UI.h"
#include "CItemGet_UI.h"
#include "CDialogue_UI.h"
#include "CStageName_UI.h"

Stage1_UILayer::Stage1_UILayer()
{

}

HRESULT Stage1_UILayer::Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev)
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
    
    Engine::CUI* pUI = nullptr;
    // 맵 유아이
    pUI = CMap_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"Map_UI");
    CUIManager::GetInstance()->Add_UI(pUI);

    // 플레이어 스프라이트
    pUI = CPlayerTex::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"PlayerTex");
    CUIManager::GetInstance()->Add_UI(pUI);

    // 플레이어 ui
    pUI = CPlayerUI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"PlayerUI");
    CUIManager::GetInstance()->Add_UI(pUI);

    // 일시정지창
    pUI = CPauseUI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"PauseUI");
    CUIManager::GetInstance()->Add_UI(pUI);

    //무기ui
    pUI = CUI_Weapon::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"UI_Weapon");
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CStageClear_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"StageClearUI");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CPlayerStatPanel_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"PlayerStatPanel_UI");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CUI_NoticeBanner::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"UI_NoticeBanner");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CShop_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"Shop_UI");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CSetting_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"Setting_UI");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CUI_Fade::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"UI_FadeIn");
    CUIManager::GetInstance()->Add_UI(pUI);

    if (auto* fade = dynamic_cast<CUI_Fade*>(CUIManager::GetInstance()->Find_UI(L"UI_FadeIn")))
    {
        fade->StartFadeIn(0.6f, D3DCOLOR_ARGB(255, 0, 0, 0), true);
    }

    pUI = CGameOver::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"GameOver");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CWeaponGet_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"WeaponGet_UI");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CUI_Interaction::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"UI_Interaction");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CItemGet_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"ItemGet_UI");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CDialogue_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"Dialogue_UI");
    pUI->Set_UIActive(false);
    CUIManager::GetInstance()->Add_UI(pUI);

    pUI = CStageName_UI::Create(pGraphicDev);
    if (!pUI) return E_FAIL;
    pUI->Set_Tag(L"StageName_UI");
    pUI->Set_UIActive(true);
    CUIManager::GetInstance()->Add_UI(pUI);

    return S_OK;
}

_int	Stage1_UILayer::Update_Layer(const _float& fTimeDelta)
{
    CLayer::Update_Layer(fTimeDelta);
    CCameraManager::GetInstance()->Update_UICamera(fTimeDelta);
    CUIManager::GetInstance()->Update_UI(fTimeDelta);

    return 0;
}

void	Stage1_UILayer::LateUpdate_Layer(const _float& fTimeDelta)
{
    CLayer::LateUpdate_Layer(fTimeDelta);
    CCameraManager::GetInstance()->LateUpdate_UICamera(fTimeDelta);
    CUIManager::GetInstance()->LateUpdate_UI(fTimeDelta);
}

void	Stage1_UILayer::Render_Layer()
{

}

Stage1_UILayer* Stage1_UILayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    Stage1_UILayer* pStUI = new Stage1_UILayer();

    if (FAILED(pStUI->Ready_Layer(pGraphicDev)))
    {
        MSG_BOX("Stage UI Layer Create Failed");
        Safe_Release(pStUI);
        return nullptr;
    }

    return pStUI;
}

void Stage1_UILayer::Free()
{

}