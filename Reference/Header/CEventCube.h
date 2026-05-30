#pragma once
#include "CGameObject.h"
#include "CCubeTex.h"
#include "CParticleSystem_BrokenCube.h"


BEGIN(Engine)

class ENGINE_DLL CEventCube :
    public CGameObject
{
protected:
	explicit CEventCube(const CEventCube& cpy);
	explicit CEventCube(LPDIRECT3DDEVICE9 device);
	virtual	~CEventCube();

public:

	virtual     HRESULT     Ready_GameObject();
	virtual     _int        Update_GameObject(const _float& fTimeDelta);
	virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
	virtual     void        Render_GameObject();
protected:

	virtual		HRESULT				Add_Component();

public:

	void							SetCubeType(const CUBE_TYPE& eType) { m_eCubeType = eType; }
	const CUBE_TYPE&				GetCubeType() { return m_eCubeType; }
	const _bool& GetIsEvent() { return m_IsEvent; }
	const CUBE_EVENT_TYPE&			GetCubeEventType() { return m_eCubeEventType; }

	_vec2* GetCenterUV();

	void							SetVerFilpArr(_bool* arr);
	void						    SetHorFilpArr(_bool* arr);
	void							SetCubeEventType(const CUBE_EVENT_TYPE& eType) { m_eCubeEventType = eType; }

public:

	void							Change_CubeTextureFaceUV(const CUBE_FACE& eType, const _vec2& vCenterUV);

	static	CEventCube* Create(LPDIRECT3DDEVICE9 device);

	void					SemiUpdate();


public:

	//이벤트 큐브의 이벤트 실행 함수....각자 Update 내부안에서 Event를 실행한다.  
	virtual void					PlayEvent() PURE;
	
protected:
	//나중에 하위 클래스에서 생성 예정...

	_bool							m_isEventTriggered;

	CUBE_TYPE						m_eCubeType;
	CUBE_EVENT_TYPE					m_eCubeEventType;
	CCubeTex*						m_pBuffer;
	bool							m_IsEvent;

protected:
	CParticleSystem_BrokenCube* m_pCubeParticleSystem = nullptr;
public:
	void    Play_BrokenCubeParticleSystem();


protected:
	virtual    void         Free() override;

};

END