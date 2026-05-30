#pragma once
#include "CUI.h"
class CUI_Number : public CUI
{
private:

    explicit CUI_Number(const CUI_Number& rhs);
    explicit CUI_Number(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUI_Number();

public:
    static CUI_Number* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

    void Set_Number(int number);

    void Set_Scale(const _vec3& vScale) { if (m_pTransform) m_pTransform->Set_Scale(vScale); }
    void Set_Position(float x, float y, float z) { if (m_pTransform) m_pTransform->Set_Pos(x, y, z); }

private:

    vector<int> m_vecDigits;
    int m_iNumber = 0;

private:
    void Free() override;
};