#pragma once
#include "CLayer.h"

class CBossStage2_UILayer :
    public CLayer
{
private:
    explicit CBossStage2_UILayer();
    virtual ~CBossStage2_UILayer() {};

public:
    virtual			HRESULT		Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual			_int		Update_Layer(const _float& fTimeDelta);
    virtual			void		LateUpdate_Layer(const _float& fTimeDelta);
    virtual			void		Render_Layer();

public:
    static CBossStage2_UILayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual void                Free() override;
};