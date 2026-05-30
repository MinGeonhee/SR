#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;
class CTerrainTex;

class ENGINE_DLL TerrainCollision :
    public CBase
{
    DECLARE_SINGLETON(TerrainCollision)

    enum OBJLIST{OBJ_NONE, OBJ_PLAYER, OBJ_TERRAIN, OBJ_END};

private:

    explicit TerrainCollision();
    virtual ~TerrainCollision() { Free(); };

public:
    void        Collision_Update();
    void        Terrain_IndexCalculate();

    void        Add_Player(CGameObject* pPlayer) { m_pPlayer = pPlayer; }
    void        Add_TerrainTex(CTerrainTex* pTex) { m_pTex = pTex;}

private:
    CGameObject* m_pPlayer = nullptr;
    CTerrainTex* m_pTex = nullptr;


private:
    virtual void                Free();
};

END
