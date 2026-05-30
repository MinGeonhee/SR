#pragma once
#include "CLayer.h"

class Stage1_UILayer :
    public CLayer
{
private:
    explicit Stage1_UILayer();
    virtual ~Stage1_UILayer() {};

public:
    virtual			HRESULT		Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual			_int		Update_Layer(const _float& fTimeDelta);
    virtual			void		LateUpdate_Layer(const _float& fTimeDelta);
    virtual			void		Render_Layer();

public:
    static Stage1_UILayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
    virtual void                Free() override;
};

