#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

class CPlayerTex;
class CPlayer :
    public CGameObject
{
private:
    //explicit    CPlayer();
    explicit    CPlayer(const CPlayer& rhs);
    explicit    CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual     ~CPlayer();

    virtual void  Free();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();


public:
    void                    SetPlayerDefaultY(_float y) { m_PlayerDefaultY = y; }
    _float                  GetPlayerDefaultY() { return m_PlayerDefaultY; }

private:
    void                     Mouse_Move(const _float& fTimeDelta);

private:

    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT                 Add_Component();
    void                    Key_Input(const _float& fTimeDelta);
public:
    static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    void                    Set_MouseLock(bool btype) { m_bMouseLocked = btype; }
    _float                  Get_Hp() const { return m_pStatusCom->Get_CurrentHp(); }
    _float                  Get_Def() const { return m_pStatusCom->Get_Status().fDefence; }

    _float                  Get_ItemCnt() const { return m_iItemCnt; }

    void                    Set_MoneyCnt(int Money) { m_pWalletCom->Set_Money(Money); }
    _float                  Get_MoneyCnt() const { return m_pWalletCom->Get_Money(); }

    bool                    Get_MouseFixed() const { return m_bFix; }

    void SetPlayerTexUI(CPlayerTex* pPlayerTexUI) { m_pPlayerTexUI = pPlayerTexUI; }
    Engine::CWeaponController* Get_WeaponController() const { return m_pWeaponCom; }

private:

    Engine::CRecTex* m_pBufferCom;
    Engine::CWeaponController* m_pWeaponCom;
    Engine::CWallet* m_pWalletCom;

    CPlayerTex* m_pPlayerTexUI = nullptr;


    _float              m_PlayerDefaultY;

private:
    int                 m_iItemCnt = 5; //아이템

    bool                m_bMouseLocked = false;
    float               m_fSpeed = 0.f;

    _vec3 m_prevPos = { 0,0,0 };
    float m_footTimer = 0.f;
    int   m_lastStepIdx = -1;
private:
    // 마우스 잠금 관련 변수
    bool                m_bCheck = false;
    bool                m_bFix = false;
    bool m_sentStageFail = false;
};

