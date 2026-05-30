#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

enum
{
    KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX + 1),
    KEY_STATE_COUNT = static_cast<int32>(KEY_STATE::K_END)
};

class ENGINE_DLL CInputManager :
    public CBase
{
    DECLARE_SINGLETON(CInputManager)
private:

    explicit CInputManager() {};
    virtual ~CInputManager() { Free();};

public:

    HRESULT Init(HINSTANCE hInst, HWND hwnd);
    void Update();

    // 누르고 있을 때
    bool GetButtonPress(KEY_TYPE key) 
    { 
        if (m_bKeyLock && key != KEY_TYPE::LBUTTON) return false;
        return GetState(key) == KEY_STATE::PRESS; 
    }
    // 맨 처음 눌렀을 때
    bool GetButtonDown(KEY_TYPE key) 
    { 
        if (m_bKeyLock && key != KEY_TYPE::LBUTTON) return false;
        return GetState(key) == KEY_STATE::DOWN; 
    }
    // 맨 처음 눌렀다 땠을 때
    bool GetButtonUp(KEY_TYPE key)
    { 
        if (m_bKeyLock && key != KEY_TYPE::LBUTTON) return false;
        return GetState(key) == KEY_STATE::UP; 
    }

    inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<_uint8>(key)]; }

public:

    void Set_KeyLock(bool b) { m_bKeyLock = b; }
    bool Get_KeyLock() { return m_bKeyLock; }

public:
    
    // 현재 마우스의 특정 축 좌표를 반환
    _long   Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
    {
        return *(((_long*)&m_tMouseState) + eMouseState);
    }

    _byte   Get_DIMouseState(MOUSEKEYSTATE eMouse)
    {
        return m_tMouseState.rgbButtons[eMouse];
    }  

    const POINT& GetMousePos() { 
        int a = 0;
        return _mousePos; }
    void         Mouse_Fix();

private:
    LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
    HWND _hwnd;
    DIMOUSESTATE    m_tMouseState;
    POINT _mousePos = {};

    LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;
    vector<KEY_STATE> _states;

private:
    // 이벤트 발생했을 때 Key 잠금을 위해.

    _bool               m_bKeyLock = false;

private:
    virtual void        Free();
};

END