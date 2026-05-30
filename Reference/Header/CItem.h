#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

BEGIN(Engine)

class ENGINE_DLL CItem :
    public CGameObject
{
protected:
    explicit    CItem(const CItem& rhs);
    explicit    CItem(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CItem();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta) override;
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta) override;
    virtual     void        Render_GameObject() override;

    static  CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:

    ITEMTYPE    Get_ItemType() { return m_eItemtype; }
    void        Set_ItemType(ITEMTYPE type) { m_eItemtype = type; }

protected:
    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT     Add_Component();

private:

    void        Item_StatusSetting();
    void        Item_Texture_Setting();
    void        ITem_Animation_Setting();
    void        ADD_HoverComponent();
    void        ADD_DropComponent();

    bool        IsItemTypeExempt(); // 예외처리 함수
    bool        ExcludeDropTable(); // 예외처리 함수

protected:
    virtual void    Free();

private:

    Engine::CAnimation* m_pAnimationCom;
    Engine::CBillboard* m_pBillboardCom;
    Engine::CHoverItem* m_pHoverCom;
    Engine::CDropItem*  m_pDropCom;

protected:

    ITEMTYPE    m_eItemtype;
    bool        m_bStatusSetting = false;   // 초기 Status 세팅 했니
    bool        m_bAnimationSetting = false;    // 초기 Animation 세팅 했니
    bool        m_bHoverComponentSetting = false; // HoverComponent 세팅 했니.
    bool        m_bDropComponentSetting = false; // HoverComponent 세팅 했니.

private:
    bool        m_bIsDropItem = false;
public:
    void        Set_IsDropItem(bool _b) { m_bIsDropItem = _b; }
private:
    CTransform* m_pTargetTransform;
public:
    void        Set_TargetTransform(CTransform* _p) { m_pTargetTransform = _p; }
};

END