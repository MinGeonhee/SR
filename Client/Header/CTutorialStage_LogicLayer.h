#pragma once
#include "CLayer.h"
class CTutorialStage_LogicLayer :
    public CLayer
{
private:
    explicit CTutorialStage_LogicLayer();
    virtual ~CTutorialStage_LogicLayer();

public:
    virtual			HRESULT		Ready_Layer(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual			_int		Update_Layer(const _float& fTimeDelta);
    virtual			void		LateUpdate_Layer(const _float& fTimeDelta);
    virtual			void		Render_Layer();

public:
    static CTutorialStage_LogicLayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    
protected:
    virtual void                Free() override;
};

