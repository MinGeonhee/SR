#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CGameObject.h"
#include "CCamera.h"

BEGIN(Engine)

class ENGINE_DLL CCameraManager :
    public CBase
{
    DECLARE_SINGLETON(CCameraManager)

private:

    explicit CCameraManager();
    virtual ~CCameraManager();

public:

    HRESULT         Ready_Camera(const wstring& pCameraTag, CGameObject* pCamera);
    void            Set_MainCamera(const wstring& pCameraTag);
    CGameObject*    Get_Camera(const wstring& CameraTag) { return Find_Camera(CameraTag); }

    void            Set_UICamera(const wstring& pCameraTag);
    void            Set_CameraTarget(CGameObject* pObject);
    void            Set_MainCameraCom(CCamera* pCamera) { m_pMainCameraCom = pCamera; }
    void            Set_UICameraCom(CCamera* pCamera) { m_pUiCameraCom = pCamera; }

    _uint           Update_MainCamera(const _float& _fTimeDelta);
    void            LateUpdate_MainCamera(const _float& _fTimeDelta);

    _uint           Update_UICamera(const _float& _fTimeDelta);
    void            LateUpdate_UICamera(const _float& _fTimeDelta);


public:
    const _matrix& Get_MainViewMatrix();
    const _matrix& Get_MainProjMatrix();

    const _matrix& Get_UIViewMatrix();
    const _matrix& Get_UIProjMatrix();

private:

    CGameObject*     Find_Camera(const wstring& pCameraTag);

private:

    unordered_map<wstring, CGameObject*>           m_mapCameraType; // 원본 컴포넌트
    CGameObject*                                   m_pMainCamera = nullptr;
    CGameObject*                                   m_pUICamera = nullptr;
    CCamera*                                       m_pMainCameraCom = nullptr;
    CCamera*                                       m_pUiCameraCom = nullptr;

private:
    virtual void    Free();
};

END

