#include "CRendererManager.h"
#include "CCameraManager.h"
#include "CResourceManager.h"
#include "CDebugLine.h"

IMPLEMENT_SINGLETON(CRendererManager)

CRendererManager::CRendererManager()
{

}

CRendererManager::~CRendererManager()
{
    Free();
}

void CRendererManager::Add_RenderGroup(RENDERID eType, CGameObject* pGameObject)
{
    if (RENDER_END <= eType || nullptr == pGameObject)
        return;

    m_RenderGroup[eType].push_back(pGameObject);
    pGameObject->Add_Ref();
}

void CRendererManager::Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
    Render_SkyBox(pGraphicDev);
    Render_Priority(pGraphicDev);
    Render_TileTex(pGraphicDev);
    /*Render_GroundDecor(pGraphicDev);
    Render_WallDecor(pGraphicDev);
    Render_Obstacle(pGraphicDev);
    Render_Event(pGraphicDev);*/
    Render_AlphaTest(pGraphicDev);
    Render_NonAlpha(pGraphicDev);
    Render_AlphaBlend(pGraphicDev);
    Render_UI(pGraphicDev);

    // 랜더 다 출력하고 나면 정리해주어야한다.
    Clear_RenderGroup();
}

void CRendererManager::Clear_RenderGroup()
{
    for (size_t i = 0; i < RENDER_END; ++i)
    {
        for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
        m_RenderGroup[i].clear();
    }
}

void CRendererManager::Render_SkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
{
    if (FAILED(pGraphicDev->SetTransform(D3DTS_VIEW, &CCameraManager::GetInstance()->Get_MainViewMatrix())))
        MSG_BOX("카메라 뷰 행렬 적용 실패");

    if (FAILED(pGraphicDev->SetTransform(D3DTS_PROJECTION, &CCameraManager::GetInstance()->Get_MainProjMatrix())))
        MSG_BOX("카메라 투영 행렬 적용 실패");

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

    for (auto& pObj : m_RenderGroup[RENDERID::RENDER_SKYBOX])
    {
        if (pObj && pObj->Get_Active())
            pObj->Render_GameObject();
    }

    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CRendererManager::Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev)
{
    for (auto& pObj : m_RenderGroup[RENDER_PRIORITY])
    {
        if (pObj && pObj->Get_Active())
            pObj->Render_GameObject();
    }
}

void CRendererManager::Render_TileTex(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 250);
    pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    auto texture = CResourceManager::GetInstance()->Get<CTexture>(L"Tile");

    if (texture)
        texture->Set_Texture();


    for (auto& pObj : m_RenderGroup[RENDER_TILETEX])
    {
        if (pObj && pObj->Get_Active())
            pObj->Render_GameObject();
    }

    pGraphicDev->SetTexture(0, nullptr);

    pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CRendererManager::Render_GroundDecor(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    CResourceManager::GetInstance()->Get<CTexture>(L"Ground_Decor")->Set_Texture();


    for (auto& pObj : m_RenderGroup[RENDER_MAPOBJECT_GROUND_DECOR])
    {
        if (pObj && pObj->Get_Active())
            pObj->Render_GameObject();
    }


    pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pGraphicDev->SetTexture(0, nullptr);

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRendererManager::Render_WallDecor(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    CResourceManager::GetInstance()->Get<CTexture>(L"Wall_Decor")->Set_Texture();


    for (auto& pObj : m_RenderGroup[RENDER_MAPOBJECT_WALL_DECOR])
    {
        if (pObj && pObj->Get_Active())
            pObj->Render_GameObject();
    }


    pGraphicDev->SetTexture(0, nullptr);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


void CRendererManager::Render_Obstacle(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    CResourceManager::GetInstance()->Get<CTexture>(L"Obstacle")->Set_Texture();

    for (auto& pObj : m_RenderGroup[RENDER_MAPOBJECT_OBSTACLE])
    {
        if (pObj && pObj->Get_Active())
            pObj->Render_GameObject();
    }


    pGraphicDev->SetTexture(0, nullptr);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRendererManager::Render_Event(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


    for (auto& pObj : m_RenderGroup[RENDER_MAPOBJECT_EVENT])
    {
        if (pObj && pObj->Get_Active())
            pObj->Render_GameObject();
    }


    pGraphicDev->SetTexture(0, nullptr);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}


void CRendererManager::Render_NonAlpha(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    for (auto& pObj : m_RenderGroup[RENDER_NONALPHA])
    {
        if (pObj && pObj->Get_Active())
            pObj->Render_GameObject();
    }



    pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRendererManager::Render_AlphaTest(LPDIRECT3DDEVICE9 Device)
{
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    Device->SetRenderState(D3DRS_ZENABLE, TRUE);
    Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    // 비교 함수 설정 (예: 128 이상이면 출력)
    Device->SetRenderState(D3DRS_ALPHAREF, 128);// 기준값 (0~255)
    Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    for (auto& pObj : m_RenderGroup[RENDER_ALPHATESTING])
    {
        if (pObj && pObj->Get_Active())
            pObj->Render_GameObject();
    }

    Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


void CRendererManager::Render_AlphaBlend(LPDIRECT3DDEVICE9 pGraphicDev)
{
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);

    // 알파 블렌딩 켜기
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


    // 선생님 저 알파 블랜딩 좀 쓰겠습니다.
    m_RenderGroup[RENDER_ALPHABLEND].sort([](CGameObject* pDst, CGameObject* pSrc)->bool
        {
            return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
        });


    for (auto& pObj : m_RenderGroup[RENDER_ALPHABLEND])
    {
        if (pObj && pObj->Get_Active())
            pObj->Render_GameObject();
    }


    // 알파 블랜딩이 끝나면 끈다.


    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}



void CRendererManager::Render_UI(LPDIRECT3DDEVICE9 pGraphicDev)
{
    if (FAILED(pGraphicDev->SetTransform(D3DTS_VIEW, &CCameraManager::GetInstance()->Get_UIViewMatrix()))) // 모든 객체한테 이 뷰 행렬 적용할랭
        MSG_BOX("카메라 뷰 행렬 적용 실패");

    if (FAILED(pGraphicDev->SetTransform(D3DTS_PROJECTION, &CCameraManager::GetInstance()->Get_UIProjMatrix())))
        MSG_BOX("카메라 투영 행렬 적용 실패");

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	for (auto& pObj : m_RenderGroup[RENDER_UI])
	{
		if (pObj && pObj->Get_Active())
		pObj->Render_GameObject();
	}

    pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

    CDebugLine::Render();
    CDebugLine::Clear(); // 반드시 렌더 이후 자워줘야함. 안지우면 계속 남는다.
}

void CRendererManager::Free()
{
    Clear_RenderGroup();
}