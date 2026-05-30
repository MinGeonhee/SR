#include "pch.h"
#include "CUI_Number.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"

CUI_Number::CUI_Number(LPDIRECT3DDEVICE9 pGraphicDev)
    :CUI(pGraphicDev)
{

}

CUI_Number::CUI_Number(const CUI_Number& rhs)
    :CUI(rhs)
{

}

CUI_Number::~CUI_Number()
{

}

HRESULT CUI_Number::Ready_GameObject()
{
    CUI::Ready_GameObject();

    m_tHandle.m_eType = OBJ_UI;
    m_pTransform->Set_Scale({ 0.05f, 0.1f, 1.f });
    Set_Number(m_iNumber);
    m_bActive = true;
    return S_OK;
}


_int CUI_Number::Update_GameObject(const _float& fTimeDelta)
{
    CUI::Update_GameObject(fTimeDelta);

    return 0;
}

void CUI_Number::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CUI::LateUpdate_GameObject(fTimeDelta);

    CInputManager* pInput = CInputManager::GetInstance();
}


void CUI_Number::Render_GameObject()
{
    if (!m_bActive)
        return;
    CUI::Render_GameObject();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    if (m_vecDigits.empty())
        return;

    const float fDigitWidth = 25.f;   //글자크기 지정해줘야 간격 제대로잡힘 
    _matrix matBaseWorld = *m_pTransform->Get_World();    //월드행렬가져오기
     
    CResourceManager::GetInstance()->Get<CTexture>(L"NumUI_Tile")->Set_Texture();

    for (size_t i = 0; i < m_vecDigits.size(); ++i)
    {
        //숫자긁어와서 맞는 텍스쳐 찾기
        int digit = m_vecDigits[i];
        //자릿수에 따라다음숫자 이동시켜야됨
        _matrix matTranslate;
        D3DXMatrixTranslation(&matTranslate, fDigitWidth * i, 0.f, 0.f);
        _matrix matWorld = matBaseWorld * matTranslate;
        m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
        _vec2 vCenterUV = { digit * 0.1f + 0.05f, 0.5f };
        m_pBufferCom->Change_UV(digit, 10, 1, 1);
        m_pBufferCom->Render_Buffer();
    }

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CUI_Number::Set_Number(int number)
{
    m_vecDigits.clear();

    if (number == 0)
    {
        m_vecDigits.push_back(0);
        return;
    }

    while (number > 0)
    {
        m_vecDigits.insert(m_vecDigits.begin(), number % 10); 
        number /= 10;
    }
}

CUI_Number* CUI_Number::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CUI_Number* pNum = new CUI_Number(pGraphicDev);

    if (FAILED(pNum->Ready_GameObject()))
    {
        Safe_Release(pNum);
        MSG_BOX("CUI_Number Create Failed");
        return nullptr;
    }

    return pNum;
}

void CUI_Number::Free()
{
    CUI::Free();
}
