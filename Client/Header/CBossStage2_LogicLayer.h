#pragma once
#include "CLayer.h"

class CBossStage2_LogicLayer :
    public CLayer
{
private:
    explicit CBossStage2_LogicLayer();
    virtual ~CBossStage2_LogicLayer() {};

public:
    virtual			HRESULT		Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual			_int		Update_Layer(const _float& fTimeDelta);
    virtual			void		LateUpdate_Layer(const _float& fTimeDelta);
    virtual			void		Render_Layer();

private:


public:
    static CBossStage2_LogicLayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual void                Free() override;
};

