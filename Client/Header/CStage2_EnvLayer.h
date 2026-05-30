#pragma once
#include "CLayer.h"

class CStage2_EnvLayer :
    public CLayer
{
private:
    explicit CStage2_EnvLayer();
    virtual ~CStage2_EnvLayer() {};

public:
    virtual			HRESULT		Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual			_int		Update_Layer(const _float& fTimeDelta);
    virtual			void		LateUpdate_Layer(const _float& fTimeDelta);
    virtual			void		Render_Layer();

public:
    static CStage2_EnvLayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual void                Free() override;
};
