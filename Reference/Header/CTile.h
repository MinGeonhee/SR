#pragma once

#include "CGameObject.h"
#include "CTileTex.h"

BEGIN(Engine)


class ENGINE_DLL CTile : public CGameObject
{
private:
	explicit CTile(const CTile& cpy);
	explicit CTile(LPDIRECT3DDEVICE9 device);
	virtual	~CTile();

public:

	virtual     HRESULT     Ready_GameObject() override;
	
	virtual     _int        Update_GameObject(const _float& fTimeDelta) override;
	virtual     void        LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual     void        Render_GameObject() override;

public:


public:

	void					Set_TileType(const TILETYPE eType) { m_eTileType = eType; }
	void					Set_IsEventOn(const _bool& event) { m_IsEvent = event; }

private:

	HRESULT					Add_Component();
	HRESULT					Set_TileUV(const _vec2& vCenterUV);
	HRESULT					Set_TilePos(const _vec3& vPos);
	void					Set_BufferIndexCw();

public:

	void					Change_IndexBuffer_To_CCW();
	void					Change_TextureUV(const _vec2& vCenterUV);
	const _vec2&			Get_CenterUV() { return m_pBuffer->Get_CenterUV(); }

public:

	void					SemiUpdate();

	const TILETYPE&			Get_TileType()		{ return m_eTileType; }

public:


	static CTile* Create(LPDIRECT3DDEVICE9 device);

protected:

	TILETYPE				m_eTileType;

	CTileTex*				m_pBuffer;

	bool					m_IsEvent;

private:
	virtual    void         Free() override;
};


END