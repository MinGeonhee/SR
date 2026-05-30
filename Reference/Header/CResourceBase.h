#pragma once
#include "CComponent.h"
#include "Engine_Define.h"

enum class RESOURCETYPE : _uint8
{
	NONE = -1,

	MESH,		// 매쉬
	SHADER,		// 쉐이더


	MATERIAL,	// 마테리얼 -> 조명 연산이 들어갈 때 필수적으로 들어갈 질감 
	TEXTURE,	//텍스처
	//ANIMATION,  // 애니메이션 -> 버퍼로 쓴다. uv 바꾸면서.
	SOUND,		// 사운드

	End
};

enum
{
	RESOURCE_TYPE_COUNT = static_cast<_uint8>(RESOURCETYPE::End)
};

// 컴포넌트와 다른 독자적인 ResoureBase를 씀
class CResourceBase : public CComponent
{

public:
	explicit CResourceBase(RESOURCETYPE _type, LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CResourceBase(const CResourceBase& rhs);
	virtual ~CResourceBase();
	
	RESOURCETYPE		GetType() { return m_eType; }

	void				SetName(const wstring& name) { _name = name; }
	const wstring&		GetName() { return _name; }
	_uint32				GetID() { return _id; } // 리소스의 고유 아이디가 생성 될 수 있음


protected:
	// 리소스를 관리하려면 반드시 파일 입출력을 통해서 관리되어야 함.
	// 모든 것이 파일로 저장할 필요가 있다.
	virtual void Load(const wstring& path) {} // 오버로딩해서 자식이 선언할 수 있게.
	virtual void Save(const wstring& path) {}

	// 리소스를 복사 형태로 넘겨주어야하는게 있다면 Clone 통해 생성해서 넘겨줌.
	virtual CComponent*		Clone() { return nullptr; }// 선언 안함.
	virtual void		   Free();

protected:

	RESOURCETYPE m_eType = RESOURCETYPE::NONE;
	wstring _name;
	wstring _path;
	_uint32 _id = 0;

	bool	m_bClone;
};

