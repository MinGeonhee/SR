#include "CGameObject.h"
#include "CTileTex.h"

BEGIN(Engine)


class ENGINE_DLL CMapToolTile : public CGameObject
{
private:
	explicit CMapToolTile(const CMapToolTile& cpy);
	explicit CMapToolTile(LPDIRECT3DDEVICE9 device);
	virtual	~CMapToolTile();

public:

	virtual     HRESULT     Ready_GameObject(const  FLOOR& eFloor, const _vec2& vCenterUV, const _ushort& sRoomNum, const _vec3& vPos);
	virtual     HRESULT     Ready_GameObject();

	virtual     _int        Update_GameObject(const _float& fTimeDelta);
	virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
	virtual     void        Render_GameObject();

public:

	const _bool&			IsHaveCeiling() { return m_tMapToolTile_Info.IsHaveCeiling; }

public:
	void					Set_IsHaveCeiling(const _bool& isHave) { m_tMapToolTile_Info.IsHaveCeiling = isHave; }
	void					Set_TileType(const TILETYPE eType) { m_eTileType = eType; }
	void					Set_IsEventOn(const _bool& event) { m_IsEvent = event; }
	void					Set_OnTopCube(const _bool& IsCubeOnTop) { m_tMapToolTile_Info.IsCubeOnTop = IsCubeOnTop; }
	void					Set_MakeIndex(const _int& iMakeIndex) { m_tMapToolTile_Info.iMakeIndex = iMakeIndex; }
	void					Set_MapToolTile_Info(const MAPTOOLTILE_INFO& tInfo) { m_tMapToolTile_Info = tInfo; }

private:
	HRESULT					Add_Component();
	HRESULT					Set_TileUV(const _vec2& vCenterUV);
	HRESULT					Set_TilePos(const _vec3& vPos);
public:
	void					Change_TextureUV(const _vec2& vCenterUV);
	const _vec2&			Get_CenterUV() { return m_pBuffer->Get_CenterUV(); }
public:

	void					SemiUpdate();


	const _int&				Get_MakeIndex() { return m_tMapToolTile_Info.iMakeIndex; }
	const TILETYPE&			Get_TileType() { return m_eTileType; }
	const MAPTOOLTILE_INFO& Get_MapToolTile_Info() { return m_tMapToolTile_Info; }

	const _bool&			IsCubeOnTop() { return m_tMapToolTile_Info.IsCubeOnTop; }
	const _bool&			IsEvent() { return m_IsEvent; }

public:
	void							SetTargetMakeIndex(const _int& iTargetMakeIndex) { m_tMapToolTile_Info.iTargetMakeIndex = iTargetMakeIndex; };
	const int&				GetTargetMakeIndex() { return m_tMapToolTile_Info.iTargetMakeIndex; }

public:

	static	CMapToolTile*	Create(LPDIRECT3DDEVICE9 device, const FLOOR& eFloor, const _ushort& sRoomNum
		, const _vec3& vPos, const _vec2& vCenterUV = { DEFUALT_TILE_UV_CENTERX, DEFUALT_TILE_UV_CENTERY });

	static CMapToolTile*	Create(LPDIRECT3DDEVICE9 device);

protected:

	TILETYPE				m_eTileType;

	MAPTOOLTILE_INFO		m_tMapToolTile_Info;

	CTileTex*				m_pBuffer;

	bool					m_IsEvent;

private:
	virtual    void         Free() override;
};


END