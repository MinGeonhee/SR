#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTransform :
    public CComponent
{
private:
	explicit CTransform();
    explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:

	// RIGHT, UP, LOOK 벡터를 월드 행렬에서 떼어간다.

	// 회전된 축을 구하고 싶다면 해당 Get_Info를 통해서 회전된 축의 Vector를 가져오면 됨.
	// 즉, 이건 Rotate_Axis 이다.

	// 내가 건내준 D3DXVECTOR3 자료형의 pInfo에 memcpy형태로 들고온다.
	void		Get_Info(INFO eType, _vec3* pInfo)
	{
		int a = 0;
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
	}

	_vec3* GetInfoPos()
	{
		return &m_vInfo[INFO_POS];
	}


	// 내 중점 좌표를 Dir (방향) * (speed 값) * (FDT) 만큼 옮기겠다.
	void		Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
	{
		m_vInfo[INFO_POS] += *pDir * fSpeed * fTimeDelta;
	}

	// 가독성은 떨어지지만 switch - case 문에 비해 더 빠를 수 있다.
	// switch - case 문은 분기문이라서 조금 더 느릴 수 있음. -> 현대 컴파일러는 최적화를 알아서 해주기 때문에 괜찮다.

	// m_vAngle 자체도 하나의 float 형 배열이다.
	// 배열의 주소는 첫 인덱스의 주소이기도 하다. -> 배열 시간에 항상 배우던 것. 주소 + a 연산을 통해 다음 포인터로 건너뛸 수 있다. 
	// 배열은 연속된 메모리이기 때문이지.

	// 그리고 m_vAngle의 주소를 나는 _float*형으로 읽어들이겠다. 
	// _float* 형으로 변환한 후에 나는 eType 이라는 enum 값을 통해 주소 옮기기를 할 것이고 최종적으로 옮긴 주소의 변수를 *()통해 접근하며
	// 접근한 주소의 값에 fAngle 값을 더하겠다.

	void		Rotation(ROTATION eType, const _float& fAngle)
	{
		*(((_float*)&m_vAngle) + eType) += fAngle;
	}
	// 생성한 월드 행렬의 주소를 뱉어내겠다.

	// 내 위치를 정하는 것.

	void		Set_Pos(_float _fX, _float _fY, _float _fZ)
	{
		m_vInfo[INFO_POS] = { _fX, _fY, _fZ };
	}

	void		Set_Scale(_vec3 scale) { m_vScale = scale; }

	void		Set_Angle(_float _fX, _float _fY, _float _fZ)
	{
		m_vAngle = { _fX, _fY, _fZ };
	}

	void		Reset_Angle() { m_vAngle = { 0.f, 0.f, 0.f }; }

	const _vec3 Get_Angle() { return m_vAngle; }
	const _vec3 Get_Scale() { return m_vScale; }
public:
    HRESULT         Ready_Transform();
    virtual _int    Update_Component(const _float& fTimeDelta) override;
    
    // 얘는 왜 LateUpdate에 시간 값이 안들어가냐.
	virtual void    LateUpdate_Component(const _float& fTimeDelta) override;

public:

	void			Make_ScaleMatrix();
	void			Make_RotateMatrix();

	void			Set_TransMatrix(_matrix pTrans) { m_matTrans = pTrans; };
	void			Set_ScaleMatrix(_matrix pScale) {m_matScale = pScale;}
	void			Set_RotateMatix(_matrix pRotate) { m_matRotate = pRotate; }
	void			Set_World(_matrix pWorld) { m_matWorld = pWorld; }
	void			Set_World(_matrix* pWorld) { m_matWorld = *pWorld; }


	const _matrix			Get_TransMatrix() const { return m_matTrans; };
	const _matrix			Get_ScaleMatrix() const { return m_matScale; }
	const _matrix			Get_RotateMatrix() const { return m_matRotate; };
	const _matrix*			Get_World() const { return &m_matWorld; }

	// 인자로 행렬 주소를 넣어주면 해당 행렬 주소에 계산해둔 월드 행렬을 넣어주겠다.
	void	Get_World(_matrix* pWorld) const
	{
		*pWorld = m_matWorld;
	}

public:
    _vec3           m_vInfo[INFO_END];
    _vec3           m_vScale;
    _vec3           m_vAngle;	// Pitch, Yaw, ROll 을 담고 있는 것.

    _matrix         m_matWorld;
	_matrix			m_matRotate;
	_matrix			m_matScale;
	_matrix			m_matTrans;

public:
    virtual CComponent* Clone();
    static  CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

};

END