#include "pch.h"
#include <string>
#include "CLoading.h"
#include "CTransform.h"
#include "CRendererManager.h"
#include "CProtoTypeManager.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CObjectPoolingManager.h"
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CMonster_Goblin_Hammer.h"
#include "CMonster_Goblin_Fireball.h"
#include "CMonster_Orge_Axe.h"
#include "CMonster_Rat.h"
#include "CMonster_Boss.h"
#include "CMonster_Skeleton_Sword.h"
#include "CMonster_Wizard.h"
#include "CMonster_Clone_Boss.h"
#include "CParticleSystem_Sparkle.h"
#include "CParticleSystem_Ice.h"
#include "CParticleSystem_Rain.h"
#include "CParticleSystem_Bush.h"
#include "CParticleSystem_Barrel.h"
#include "CParticleSystem_Book.h"
#include "CParticleSystem_Eat.h"
#include "CParticleSystem_Bone.h"
#include "CParticleSystem_BrokenCube.h"
#include "CTerrain.h"
#include "CTile.h"
#include "CCube.h"
#include "CEventCube.h"
#include "CPathFunction.h"
#include "CPlayerTex.h"
#include "CBullet.h"
#include "CDoor.h"
#include "CGroundDecor.h"
#include "CWallDecor.h"
#include "CObstacle.h"
#include "CEventObject.h"
#include "CItem.h"
#include "CSkyBox.h"
#include "CSceneManager.h"
#include "CInputManager.h"
#include "CUIManager.h"
#include "CLoadingBar_UI.h"
#include "CCameraManager.h"
#include "CObjectCamera.h"
#include "CUIBackGround.h"
#include "CBossBarrier.h"
#include "CBossEffect.h"
#include "CMoveCube.h"
#include "CTargetPointCube.h"
#include "CPortalCube.h"
#include "CFogSystem.h"
#include "CMonsterSystem.h"
#include "CStatue.h"
#include "CGroundTorch.h"
#include "CreatureCantGoCube.h"
#include "CWallTorch.h"
#include "CCandle.h"
#include "CTripleCandle.h"
#include "CChandelier.h"
#include "CHangingbrazier.h"
#include "CFirealtar.h"
#include "CDestructibleAttackCube.h"
#include "CDestructibleGimicCube.h"
#include "JsonFunction.h"
#include "CSoundManager.h"

using std::wstring;

// -------------------------------------------------------------------------------------
// ctor/dtor
// -------------------------------------------------------------------------------------
CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
    , m_bFinish(false)
    , m_eLoadingID(LOADING_END)
{
    ZeroMemory(m_szLoading, sizeof(m_szLoading));
}

CLoading::~CLoading() {}

// -------------------------------------------------------------------------------------
// Ready
// -------------------------------------------------------------------------------------
HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
    m_eLoadingID = eID;

    CSoundManager::GetInstance()->StopAll();

    Engine::CGameObject* pCameraObject = nullptr;

    if (CCameraManager::GetInstance()->Get_Camera(L"UI_Camera") == NULL)
    {
        pCameraObject = CObjectCamera::Create(m_pGraphicDev);
        if (nullptr == pCameraObject) return E_FAIL;

        if (FAILED(CCameraManager::GetInstance()->Ready_Camera(L"UI_Camera", pCameraObject)))
            return E_FAIL;
    }

    // UI 카메라 설정
    auto* pObjCam = static_cast<CObjectCamera*>(CCameraManager::GetInstance()->Get_Camera(L"UI_Camera"));
    pObjCam->Get_Camera()->Set_CameraState(CAM_UIMODE);
    CCameraManager::GetInstance()->Set_UICamera(L"UI_Camera");
    CCameraManager::GetInstance()->Set_UICameraCom(pObjCam->Get_Camera());

    // 폰트(로딩 텍스트용)
    {
        wstring fontPath = L"../Bin/Resource/Font/GermaniaOne-Regular.ttf";
        AddFontResourceExW(fontPath.c_str(), FR_PRIVATE, 0);
    }

    // 최초에는 아직 큐가 없음 → Update에서 생성
    m_bLoading = false;
    m_bFinish = false;
    m_jobsBuilt = false;
    m_jobs.clear();
    m_totalJobs = 0;
    m_fLoadCnt = 0.f;

    return S_OK;
}

// -------------------------------------------------------------------------------------
// Jobs builder (코옵 로딩) - 로고
// -------------------------------------------------------------------------------------
void CLoading::BuildJobs_ForLogo()
{
    auto R = CResourceManager::GetInstance();
    auto OM = CObjectManager::GetInstance();
    auto PM = CObjectPoolingManager::GetInstance();
    auto CM = CCollisionManager::GetInstance();

    // 장면 정리 (가능하다면 Step API 사용, 없다면 원샷)
    m_jobs.push_back([=] { OM->Delete_Scene(); });
    m_jobs.push_back([=] { PM->AllActiveObject_Return(); });
    m_jobs.push_back([=] { CM->Delete_ObjectList(); });
    m_jobs.push_back([=] { CMonsterSystem::GetInstance()->ResetMonsterKill(); });

    // 머티리얼
    m_jobs.push_back([=] { R->Load<CMaterial>(L"Metal", L"../../Client/Bin/Resource/Material/Metal.json", m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CMaterial>(L"Glass", L"../../Client/Bin/Resource/Material/Glass.json", m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CMaterial>(L"Terrain", L"../../Client/Bin/Resource/Material/Terrain.json", m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CMaterial>(L"Unlit", L"../../Client/Bin/Resource/Material/Unlit.json", m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CMaterial>(L"Wood", L"../../Client/Bin/Resource/Material/Wood.json", m_pGraphicDev); });

    // 텍스처
    m_jobs.push_back([=] { R->Load<CTexture>(L"Ground_Decor", CPathFunction::GroundDecor_Png_Path_InClient, m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CTexture>(L"Wall_Decor", CPathFunction::WallDecor_Png_Path_InClient, m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CTexture>(L"Obstacle", CPathFunction::Obstacle_Png_Path_InClient, m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CTexture>(L"Interaction", CPathFunction::Interaction_Png_Path_InClient, m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CTexture>(L"Tile", L"../../Client/Bin/Resource/Texture/Terrain/tileset.png", m_pGraphicDev); });

    // UI 리소스
    const struct { const wchar_t* key; const wchar_t* path; } uiList[] = {
        {L"SelectBook",     L"../Bin/Resource/Texture/UI/book_Ani.png"},
        {L"StageArrow_Left",L"../Bin/Resource/Texture/UI/StageArrow_Left.png"},
        {L"StageArrow_Right",L"../Bin/Resource/Texture/UI/StageArrow_Right.png"},
        {L"Logo",           L"../Bin/Resource/Texture/UI/wizordum_logo.png"},
        {L"Back",           L"../Bin/Resource/Texture/UI/PauseBack.png"},
        {L"MainBackGround", L"../Bin/Resource/Texture/Logo/Wizordum_Cover_170.5x.png"},
        {L"Banner_UI",      L"../Bin/Resource/Texture/UI/Banner_UI.png"},
        {L"Line_UI",        L"../Bin/Resource/Texture/UI/Line_UI.png"},
        {L"SetBack",        L"../Bin/Resource/Texture/UI/SetBack.png"},
        {L"thumb",          L"../Bin/Resource/Texture/UI/thumb_Total.png"},
        {L"Banner_Left",    L"../Bin/Resource/Texture/UI/Banner_UI_Left.png"},
        {L"Banner_Right",   L"../Bin/Resource/Texture/UI/Banner_UI_Right.png"},
        {L"Checking",       L"../Bin/Resource/Texture/UI/Checking.png"},
        {L"checkmark",      L"../Bin/Resource/Texture/UI/checkmark.png"},
        {L"Hit",            L"../Bin/Resource/Texture/UI/Hit.png"},
        {L"Eat",            L"../Bin/Resource/Texture/UI/Eat.png"},
        {L"Get",            L"../Bin/Resource/Texture/UI/Get.png"},
        {L"Prologue",       L"../Bin/Resource/Texture/UI/Prologue.png"},
        {L"Epilogue",       L"../Bin/Resource/Texture/UI/placeholder.png"},
        {L"BossHP",         L"../Bin/Resource/Texture/UI/BossHP.png"},
        {L"ClearIcon",      L"../Bin/Resource/Texture/UI/ClearIcon.png"},
        {L"Dialogue_Box",   L"../Bin/Resource/Texture/UI/Dialogue_Box.png"},
        {L"ArrowRight",     L"../Bin/Resource/Texture/UI/ArrowRight.png"},
        {L"BlackBack",     L"../Bin/Resource/Texture/UI/BlackBack.png"},
    };
    for (auto& it : uiList)
        m_jobs.push_back([=] { R->Load<CTexture>(it.key, it.path, m_pGraphicDev); });

    // 로딩 UI 텍스처 (게이지/바/배경) - MainApp에서도 로드하지만 방어적으로 추가
    m_jobs.push_back([=] { R->Load<CTexture>(L"LoadingBar", L"../Bin/Resource/Texture/UI/LoadingBar.png", m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CTexture>(L"LoadingGage", L"../Bin/Resource/Texture/UI/LoadingGage.png", m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CTexture>(L"LoadBackGround", L"../Bin/Resource/Texture/Logo/Wizordum_Cover_Loading.png", m_pGraphicDev); });
}

// -------------------------------------------------------------------------------------
// Jobs builder - 스테이지
// -------------------------------------------------------------------------------------
void CLoading::BuildJobs_ForStage()
{
    auto R = CResourceManager::GetInstance();
    auto OM = CObjectManager::GetInstance();
    auto PM = CObjectPoolingManager::GetInstance();
    auto CM = CCollisionManager::GetInstance();

    // ====== 1) 정리 작업 ======
    m_jobs.push_back([=] { OM->Delete_Scene(); });
    m_jobs.push_back([=] { PM->AllActiveObject_Return(); });
    m_jobs.push_back([=] { CM->Delete_ObjectList(); });
    m_jobs.push_back([=] { CMonsterSystem::GetInstance()->ResetMonsterKill(); });

    // ====== 2) 리소스 로드 (작은 단위) ======
    m_jobs.push_back([=] { R->Load<CTexture>(L"SkyBox1", L"../Bin/Resource/Texture/SkyBox/skybox-dawn-panorama.png", m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CTexture>(L"SkyBox2", L"../Bin/Resource/Texture/SkyBox/skybox-stars 1.png", m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CTexture>(L"SkyBox3", L"../Bin/Resource/Texture/SkyBox/skybox-dusk-panorama.png", m_pGraphicDev); });

    // 플레이어/맵/아이콘
    const struct { const wchar_t* k; const wchar_t* p; } texA[] = {
        {L"projectiles",     L"../Bin/Resource/Texture/Player/projectiles.png"},
        {L"projectiles2",    L"../Bin/Resource/Texture/Player/projectiles2.png"},
        {L"Player_Mace",     L"../Bin/Resource/Texture/Player/Mace.png"},
        {L"Player_Weapon",   L"../Bin/Resource/Texture/Player/Weapon_Big.png"},
        {L"Terrain",         L"../Bin/Resource/Texture/Terrain/tiletest.png"},
        {L"MapUI",           L"../Bin/Resource/Texture/Player/map_ui.png"},
        {L"NumUI_Tile",      L"../Bin/Resource/Texture/UI/Num_R.png"},
        {L"HpIcon",          L"../Bin/Resource/Texture/UI/Player_Hp.png"},
        {L"DefIcon",         L"../Bin/Resource/Texture/UI/Player_Def.png"},
        {L"ItemIcon",        L"../Bin/Resource/Texture/UI/ICON/ico-decor-user.png"},
        {L"BulIcon",         L"../Bin/Resource/Texture/UI/Player_Bul.png"},
    };
    for (auto& t : texA) m_jobs.push_back([=] { R->Load<CTexture>(t.k, t.p, m_pGraphicDev); });

    // 몬스터/보스/이펙트
    const struct { const wchar_t* k; const wchar_t* p; } texMon[] = {
        {L"Goblin_Hammer_Animation", L"../Bin/Resource/Texture/Monster/goblin-hammer-Anim.png"},
        {L"Goblin_Fireball_Animation", L"../Bin/Resource/Texture/Monster/goblin-fireball-Anim.png"},
        {L"Orge_Axe_Animation", L"../Bin/Resource/Texture/Monster/ogre-axe-Anim.png"},
        {L"Rat_Animation", L"../Bin/Resource/Texture/Monster/rat-Anim.png"},
        {L"Mon_Projectiles", L"../Bin/Resource/Texture/Monster/projectiles.png"},
        {L"Boss_Animation", L"../Bin/Resource/Texture/BossMonster/boss-baron.png"},
        {L"Boss_IcePillar_Wind", L"../Bin/Resource/Texture/BossMonster/ice-wind.png"},
        {L"Boss_Thunder", L"../Bin/Resource/Texture/BossMonster/lightning-big.png"},
        {L"Effect_Thunder", L"../Bin/Resource/Texture/BossMonster/Thunder.png"},
        {L"Effect_Thunder_Anim", L"../Bin/Resource/Texture/BossMonster/Thunder-Anim.png"},
        {L"Wizard_Animation", L"../Bin/Resource/Texture/Monster/wizard-Anim.png"},
        {L"Skeleton_Animation", L"../Bin/Resource/Texture/Monster/skeleton-sword-Anim.png"},
        {L"Buff_Anim", L"../Bin/Resource/Texture/Monster/buff.png"},
    };
    for (auto& t : texMon) m_jobs.push_back([=] { R->Load<CTexture>(t.k, t.p, m_pGraphicDev); });

    // 파티클
    const struct { const wchar_t* k; const wchar_t* p; } texPart[] = {
        {L"ParticleTex",       L"../../Client/Bin/Resource/Texture/Particle/particles.png"},
        {L"ParticleTex_Ground",L"../../Client/Bin/Resource/Texture/Particle/particles-ground.png"},
        {L"ParticleTex_Anim",  L"../../Client/Bin/Resource/Texture/Particle/particle-shine-anim.png"},
    };
    for (auto& t : texPart) m_jobs.push_back([=] { R->Load<CTexture>(t.k, t.p, m_pGraphicDev); });

    // UI
    const struct { const wchar_t* k; const wchar_t* p; } texUI[] = {
        {L"Back",          L"../Bin/Resource/Texture/UI/PauseBack.png"},
        {L"NotifiBcg",     L"../Bin/Resource/Texture/UI/notification-bcg.png"},
        {L"Cross",         L"../Bin/Resource/Texture/UI/crosshairs_8.png"},
        {L"Cursor",        L"../Bin/Resource/Texture/UI/cursor.png"},
        {L"Money",         L"../Bin/Resource/Texture/UI/Money.png"},
        {L"Shop_Hud",      L"../Bin/Resource/Texture/UI/Shop_Hud.png"},
        {L"Shop_Weapon",   L"../Bin/Resource/Texture/UI/Shop_Weapon.png"},
        {L"Shop_Spell",    L"../Bin/Resource/Texture/UI/Shop_Spell.png"},
        {L"Shop_Char",     L"../Bin/Resource/Texture/UI/Shop_Char.png"},
        {L"MainBackGround",L"../Bin/Resource/Texture/Logo/Wizordum_Cover_170.5x.png"},
        {L"Banner_UI",     L"../Bin/Resource/Texture/UI/Banner_UI.png"},
        {L"Line_UI",       L"../Bin/Resource/Texture/UI/Line_UI.png"},
        {L"SetBack",       L"../Bin/Resource/Texture/UI/SetBack.png"},
        {L"donut",         L"../Bin/Resource/Texture/UI/donut.png"},
        {L"ClearFlag",     L"../Bin/Resource/Texture/UI/ClearFlag.png"},
        {L"checkmark",     L"../Bin/Resource/Texture/UI/checkmark.png"},
        {L"DeadScene",     L"../Bin/Resource/Texture/UI/DeadScene.png"},
        {L"Fade_Ground",   L"../Bin/Resource/Texture/UI/Fade_Ground.png"},
        {L"Item_Icon",     L"../Bin/Resource/Texture/UI/Item_Icon.png"},
        {L"Key_Icon",           L"../Bin/Resource/Texture/UI/Key_Icon.png"},
    };
    for (auto& t : texUI) m_jobs.push_back([=] { R->Load<CTexture>(t.k, t.p, m_pGraphicDev); });

    // 문
    const struct { const wchar_t* k; const wchar_t* p; } texDoor[] = {
        {L"bossDoor",        L"../Bin/Resource/Texture/Object/Door/bossDoor.png"},
        {L"defaultWoodDoor", L"../Bin/Resource/Texture/Object/Door/Defualt_Wooddoor.png"},
        {L"defaultIronDoor", L"../Bin/Resource/Texture/Object/Door/Default_Irondoor.png"},
        {L"SwitchWoodDoor",  L"../Bin/Resource/Texture/Object/Door/Switch_Wooddoor.png"},
        {L"SwitchIronDoor",  L"../Bin/Resource/Texture/Object/Door/Switch_Irondoor.png"},
        {L"BronzeDoor",      L"../Bin/Resource/Texture/Object/Door/bronzeKeydoor.png"},
        {L"SilverDoor",      L"../Bin/Resource/Texture/Object/Door/SilverKeydoor.png"},
        {L"GoldDoor",        L"../Bin/Resource/Texture/Object/Door/GoldKeydoor.png"},
        {L"Fade",            L"../Bin/Resource/Texture/UI/fade.png"},
    };
    for (auto& t : texDoor) m_jobs.push_back([=] { R->Load<CTexture>(t.k, t.p, m_pGraphicDev); });

    // 아이템
    const struct { const wchar_t* k; const wchar_t* p; } texItem[] = {
        {L"Item1",        L"../Bin/Resource/Texture/Item/loot-fire-mana.png"},
        {L"Item2",        L"../Bin/Resource/Texture/Item/loot-general.png"},
        {L"GunBullet",    L"../Bin/Resource/Texture/Item/loot-arcane-rocks.png"},
        {L"Armor_Shard",  L"../Bin/Resource/Texture/Item/loot-armor-shard.png"},
        {L"Ice_Wand",     L"../Bin/Resource/Texture/Item/Ice_Wand.png"},
        {L"Gun",          L"../Bin/Resource/Texture/Item/Player_Gun.png"},
    };
    for (auto& t : texItem) m_jobs.push_back([=] { R->Load<CTexture>(t.k, t.p, m_pGraphicDev); });

    // Light Object
    const struct { const wchar_t* k; const wchar_t* p; } texLight[] = {
        {L"STATUE",         L"../Bin/Resource/Texture/Object/Light/STATUE.png"},
        {L"CANDLE",         L"../Bin/Resource/Texture/Object/Light/CANDLE.png"},
        {L"GROUNDTORCH",    L"../Bin/Resource/Texture/Object/Light/GROUNDTORCH.png"},
        {L"WALLTORCH",      L"../Bin/Resource/Texture/Object/Light/WALLTORCH.png"},
        {L"HANGINGBRAZIER", L"../Bin/Resource/Texture/Object/Light/HANGINGBRAZIER.png"},
        {L"CHANDELIER",     L"../Bin/Resource/Texture/Object/Light/CHANDELIER.png"},
        {L"FIREALTAR",      L"../Bin/Resource/Texture/Object/Light/FIREALTAR.png"},
    };
    for (auto& t : texLight) m_jobs.push_back([=] { R->Load<CTexture>(t.k, t.p, m_pGraphicDev); });

    // 기타
    m_jobs.push_back([=] { R->Load<CTexture>(L"EYEOFCHAOS", L"../Bin/Resource/Texture/Object/Event/EyeOfChaos/EyeOfChaos.png", m_pGraphicDev); });
    m_jobs.push_back([=] { R->Load<CTexture>(L"Fog", L"../Bin/Resource/Texture/Particle/mist.png", m_pGraphicDev); });

    // 숫자 UI 텍스처 0~9
    for (int i = 0; i < 10; ++i) {
        wstring key = L"NumUI_" + std::to_wstring(i);
        wstring path = L"../Bin/Resource/Texture/UI/Num_" + std::to_wstring(i) + L"_R.png";
        m_jobs.push_back([=] { CResourceManager::GetInstance()->Load<CTexture>(key, path, m_pGraphicDev); });
    }

    // ====== 3) 팩토리 등록/프리로딩 ======
    m_jobs.push_back([=] { RegisterClientFactories(); });
    m_jobs.push_back([=] { RegisterClientPoolingFactories(); });

    // 플레이어 시작 위치/회전
    m_jobs.push_back([=] {
        _vec3 vPos, vRot;
        JsonFunction::SetPlayerStartAndRot(SCENETYPE::STAGE1, &vPos, &vRot);

        auto pObject = CObjectManager::GetInstance();
        auto player = pObject->Get(OBJ_PLAYER);
        if (!player)
        {
            pObject->Preload(OBJ_PLAYER, 1, vPos.x, vPos.y, vPos.z, vRot.x, vRot.y, vRot.z);
        }
        else
        {
            static_cast<CStatus*>(player->Get_Component(L"Com_Status", ID_DYNAMIC))->Reset_Hp();
            player->Get_Transform()->Set_Pos(vPos.x, vPos.y, vPos.z);
            player->Get_Transform()->Reset_Angle();
            player->Get_Transform()->Set_Angle(vRot.x, vRot.y, vRot.z);
        }

        static_cast<CPlayer*>(pObject->Get(OBJ_PLAYER))->SetPlayerDefaultY(vPos.y);

        pObject->Preload(OBJ_SKYCUBE, 1, vPos.x, vPos.y, vPos.z, vRot.x, vRot.y, vRot.z);
        });

    // 맵 데이터(원샷) ? 무거우면 Begin/Step으로 분할 구현 권장
    m_jobs.push_back([=] { CObjectManager::GetInstance()->PreLoad(CPathFunction::MapDataJsonPath(SCENETYPE::STAGE1)); });
}


void CLoading::BuildJobs_ForStage2()
{
    auto R = CResourceManager::GetInstance();
    auto OM = CObjectManager::GetInstance();
    auto PM = CObjectPoolingManager::GetInstance();
    auto CM = CCollisionManager::GetInstance();

    // ====== 1) 정리 작업 ======
    m_jobs.push_back([=] { OM->Delete_Scene(); });
    m_jobs.push_back([=] { PM->AllActiveObject_Return(); });
    m_jobs.push_back([=] { CM->Delete_ObjectList(); });
    m_jobs.push_back([=] { CMonsterSystem::GetInstance()->ResetMonsterKill(); });

    
    // 숫자 UI 텍스처 0~9
    for (int i = 0; i < 10; ++i) {
        wstring key = L"NumUI_" + std::to_wstring(i);
        wstring path = L"../Bin/Resource/Texture/UI/Num_" + std::to_wstring(i) + L"_R.png";
        m_jobs.push_back([=] { CResourceManager::GetInstance()->Load<CTexture>(key, path, m_pGraphicDev); });
    }

    // ====== 3) 팩토리 등록/프리로딩 ======
    m_jobs.push_back([=] { RegisterClientFactories(); });
    m_jobs.push_back([=] { RegisterClientPoolingFactories(); });

    // 플레이어 시작 위치/회전
    m_jobs.push_back([=] {
        _vec3 vPos, vRot;
        JsonFunction::SetPlayerStartAndRot(SCENETYPE::STAGE2, &vPos, &vRot);

        auto pObject = CObjectManager::GetInstance();
        auto player = pObject->Get(OBJ_PLAYER);
        if (!player)
        {
            pObject->Preload(OBJ_PLAYER, 1, vPos.x, vPos.y, vPos.z, vRot.x, vRot.y, vRot.z);
        }
        else
        {
            static_cast<CStatus*>(player->Get_Component(L"Com_Status", ID_DYNAMIC))->Reset_Hp();
            player->Get_Transform()->Set_Pos(vPos.x, vPos.y, vPos.z);
            player->Get_Transform()->Reset_Angle();
            player->Get_Transform()->Set_Angle(vRot.x, vRot.y, vRot.z);
        }

        static_cast<CPlayer*>(pObject->Get(OBJ_PLAYER))->SetPlayerDefaultY(vPos.y);

        pObject->Preload(OBJ_SKYCUBE, 1, vPos.x, vPos.y, vPos.z, vRot.x, vRot.y, vRot.z);
        });

    // 맵 데이터(원샷) ? 무거우면 Begin/Step으로 분할 구현 권장
    m_jobs.push_back([=] { CObjectManager::GetInstance()->PreLoad(CPathFunction::MapDataJsonPath(SCENETYPE::STAGE2)); });
}
// -------------------------------------------------------------------------------------
// Jobs builder - 보스
// -------------------------------------------------------------------------------------
void CLoading::BuildJobs_ForBoss()
{
    auto R = CResourceManager::GetInstance();
    auto OM = CObjectManager::GetInstance();
    auto PM = CObjectPoolingManager::GetInstance();
    auto CM = CCollisionManager::GetInstance();

    m_jobs.push_back([=] { OM->Delete_Scene(); });
    m_jobs.push_back([=] { PM->AllActiveObject_Return(); });
    m_jobs.push_back([=] { CM->Delete_ObjectList(); });
    m_jobs.push_back([=] { CMonsterSystem::GetInstance()->ResetMonsterKill(); });

    // 숫자 UI
    for (int i = 0; i < 10; ++i) {
        wstring key = L"NumUI_" + std::to_wstring(i);
        wstring path = L"../Bin/Resource/Texture/UI/Num_" + std::to_wstring(i) + L"_R.png";
        m_jobs.push_back([=] { R->Load<CTexture>(key, path, m_pGraphicDev); });
    }
    m_jobs.push_back([=] { R->Load<CTexture>(L"baron-shield", L"../Bin/Resource/Texture/Monster/baron-shield.png", m_pGraphicDev); });

    // 팩토리 등록
    m_jobs.push_back([=] { RegisterClientFactories(); });

    // 보스 맵/객체 프리로드
    m_jobs.push_back([=] { CObjectManager::GetInstance()->PreLoad(CPathFunction::MapDataJsonPath(SCENETYPE::BOSS)); });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Preload(OBJ_MONSTER_BOSS, 1, 26.5f, 2.f, 42.5f, 0.f, 0.f, 0.f); });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS)->Set_Active(false);  });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Preload(OBJ_BARRIER, 1, 26.5f, 2.f, 42.5f, 0.f, 0.f, 0.f); });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Get(OBJ_BARRIER)->Set_Active(false); });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Preload(OBJ_SKYCUBE, 1, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f); });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Preload(OBJ_PARTICLE_SYSTEM_RAIN, 1, 26.5f, 2.f, 42.5f, 0.f, 0.f, 0.f); });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Preload(OBJ_FOG_SYSTEM, 1, 26.5f, 2.f, 42.5f, 0.f, 0.f, 0.f); });

    // 플레이어 위치 맞추기
    m_jobs.push_back([=] {
        _vec3 vPos, vRot;
        JsonFunction::SetPlayerStartAndRot(BOSS, &vPos, &vRot);
        CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->Set_Pos(vPos.x, vPos.y, vPos.z);
        static_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJ_PLAYER))->SetPlayerDefaultY(vPos.y);
        });
}

void CLoading::BuildJobs_ForBoss2()
{
    auto R = CResourceManager::GetInstance();
    auto OM = CObjectManager::GetInstance();
    auto PM = CObjectPoolingManager::GetInstance();
    auto CM = CCollisionManager::GetInstance();

    m_jobs.push_back([=] { OM->Delete_Scene(); });
    m_jobs.push_back([=] { PM->AllActiveObject_Return(); });
    m_jobs.push_back([=] { CM->Delete_ObjectList(); });
    m_jobs.push_back([=] { CMonsterSystem::GetInstance()->ResetMonsterKill(); });

    // 숫자 UI
    for (int i = 0; i < 10; ++i) {
        wstring key = L"NumUI_" + std::to_wstring(i);
        wstring path = L"../Bin/Resource/Texture/UI/Num_" + std::to_wstring(i) + L"_R.png";
        m_jobs.push_back([=] { R->Load<CTexture>(key, path, m_pGraphicDev); });
    }
    // 팩토리 등록
    m_jobs.push_back([=] { RegisterClientFactories(); });

    // 보스 맵/객체 프리로드
    m_jobs.push_back([=] { CObjectManager::GetInstance()->PreLoad(CPathFunction::MapDataJsonPath(SCENETYPE::BOSS2)); });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Preload(OBJ_MONSTER_BOSS, 1, 48.f, 3.f, 49.5f, 0.f, 0.f, 0.f); });
    //m_jobs.push_back([=] { CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS)->Set_Active(false);  });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Preload(OBJ_BARRIER, 1, 48.f, 3.f, 49.5f, 0.f, 0.f, 0.f); });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Get(OBJ_BARRIER)->Set_Active(false); });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Preload(OBJ_SKYCUBE, 1, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f); });
    m_jobs.push_back([=] { CObjectManager::GetInstance()->Preload(OBJ_PARTICLE_SYSTEM_RAIN, 1, 48.f, 3.f, 49.5f, 0.f, 0.f, 0.f); });
    //m_jobs.push_back([=] { CObjectManager::GetInstance()->Preload(OBJ_FOG_SYSTEM, 1, 48.f, 3.f, 49.5f, 0.f, 0.f, 0.f); });

    // 플레이어 위치 맞추기
    m_jobs.push_back([=] {
        _vec3 vPos, vRot;
        JsonFunction::SetPlayerStartAndRot(BOSS2, &vPos, &vRot);
        CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->Set_Pos(vPos.x, vPos.y, vPos.z);
        static_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJ_PLAYER))->SetPlayerDefaultY(vPos.y);
        });
}

// -------------------------------------------------------------------------------------
// Jobs builder - 튜토리얼
// -------------------------------------------------------------------------------------
void CLoading::BuildJobs_ForTutorial()
{
    auto R = CResourceManager::GetInstance();
    auto OM = CObjectManager::GetInstance();
    auto PM = CObjectPoolingManager::GetInstance();
    auto CM = CCollisionManager::GetInstance();

    m_jobs.push_back([=] { OM->Delete_Scene(); });
    m_jobs.push_back([=] { PM->AllActiveObject_Return(); });
    m_jobs.push_back([=] { CM->Delete_ObjectList(); });
    m_jobs.push_back([=] { CMonsterSystem::GetInstance()->ResetMonsterKill(); });

    const struct { const wchar_t* k; const wchar_t* p; } texUI[] = {
        {L"Back",      L"../Bin/Resource/Texture/UI/PauseBack.png"},
        {L"NotifiBcg", L"../Bin/Resource/Texture/UI/notification-bcg.png"},
        {L"Cross",     L"../Bin/Resource/Texture/UI/crosshairs_8.png"},
        {L"Cursor",    L"../Bin/Resource/Texture/UI/cursor.png"},
    };
    for (auto& t : texUI) m_jobs.push_back([=] { R->Load<CTexture>(t.k, t.p, m_pGraphicDev); });
}

// -------------------------------------------------------------------------------------
// 시작(큐 생성)
// -------------------------------------------------------------------------------------
void CLoading::Loading_Scene(LOADINGID eID)
{
    m_bLoading = true;
    m_bFinish = false;
    m_jobs.clear();
    m_totalJobs = 0;
    m_jobsBuilt = false;

    switch (eID)
    {
    case LOADING_LOGO:          BuildJobs_ForLogo();     break;
    case LOADING_TUTORIALSTAGE: BuildJobs_ForTutorial(); break;
    case LOADING_STAGE:         BuildJobs_ForStage();    break;
    case LOADING_STAGE2:        BuildJobs_ForStage2();    break;
    case LOADING_BOSS:          BuildJobs_ForBoss();     break;
    case LOADING_BOSS2:         BuildJobs_ForBoss2();     break;
    default: break;
    }
    m_totalJobs = m_jobs.size();
    m_jobsBuilt = true;
}

// -------------------------------------------------------------------------------------
// Update / LateUpdate / Render
// -------------------------------------------------------------------------------------
void CLoading::SpawnUIIfNeeded()
{
    if (m_uiSpawned) return;

    // 배경
    if (auto* pBG = CUIBackGround::Create(m_pGraphicDev, L"LoadBackGround"))
        CUIManager::GetInstance()->Add_UI(pBG);

    // 로딩 바
    if (auto* pBar = CLoadingBar_UI::Create(m_pGraphicDev)) {
        pBar->SetLoading(this);
        CUIManager::GetInstance()->Add_UI(pBar);
        m_pLoadingUI = pBar;
    }
    m_uiSpawned = true;
}

void CLoading::StepJobsPerFrame(int stepsPerFrame)
{
    for (int i = 0; i < stepsPerFrame && !m_jobs.empty(); ++i) {
        m_jobs.front()();
        m_jobs.pop_front();
    }

    m_fLoadCnt = (m_totalJobs == 0) ? 100.f
        : 100.f * static_cast<float>(m_totalJobs - m_jobs.size()) / static_cast<float>(m_totalJobs);

    if (m_jobs.empty()) {
        m_bLoading = false;
        m_bFinish = true;
        m_fLoadCnt = 100.f;
    }
}

_int CLoading::Update_Scene(const float& dt)
{
    // UI 카메라/스폰/업데이트
    CCameraManager::GetInstance()->Update_UICamera(dt);
    SpawnUIIfNeeded();
    CUIManager::GetInstance()->Update_UI(dt);

    // 아직 큐를 안 만들었다면 생성
    if (!m_bLoading && !m_bFinish && !m_jobsBuilt)
        Loading_Scene(m_eLoadingID);

    // 작업 큐가 준비되었다면, 프레임당 몇 스텝만 처리
    if (m_bLoading && m_jobsBuilt && !m_jobs.empty())
        StepJobsPerFrame(/*stepsPerFrame=*/6); // 필요 시 조절

    // 완료 후 입력 대기 → 씬 전환
    if (m_bFinish) {
        if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON)) {
            SCENETYPE nextScene =
                (m_eLoadingID == LOADING_LOGO) ? SCENETYPE::LOGO :
                (m_eLoadingID == LOADING_STAGE) ? SCENETYPE::STAGE1 :
                (m_eLoadingID == LOADING_STAGE2) ? SCENETYPE::STAGE2 :
                (m_eLoadingID == LOADING_TUTORIALSTAGE) ? SCENETYPE::TUTORIAL :
                (m_eLoadingID == LOADING_BOSS) ? SCENETYPE::BOSS :
                (m_eLoadingID == LOADING_BOSS2) ? SCENETYPE::BOSS2 :
                SCENETYPE::LOGO;
            m_fLoadCnt = 0.f;
            CSceneManager::GetInstance()->Change_Scene(nextScene, LOADINGID::LOADING_NONE);
        }
    }
    return 0;
}

void CLoading::LateUpdate_Scene(const float& dt)
{
    CCameraManager::GetInstance()->LateUpdate_UICamera(dt);
}

void CLoading::Render_Scene()
{
    // (필요시) Z 비활성화해서 UI가 가리지 않게
    // m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    CUIManager::GetInstance()->Render_UI();
    // m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

// -------------------------------------------------------------------------------------
// (레거시) 즉시 로딩 함수 ? 참고용/보존용
// -------------------------------------------------------------------------------------
_uint CLoading::Loading_ForLogo()
{
    auto Resource = CResourceManager::GetInstance();

    CObjectManager::GetInstance()->Delete_Scene();
    CObjectPoolingManager::GetInstance()->AllActiveObject_Return();
    CCollisionManager::GetInstance()->Delete_ObjectList();
    CMonsterSystem::GetInstance()->ResetMonsterKill();

    Resource->Load<CMaterial>(L"Metal", L"../../Client/Bin/Resource/Material/Metal.json", m_pGraphicDev);
    Resource->Load<CMaterial>(L"Glass", L"../../Client/Bin/Resource/Material/Glass.json", m_pGraphicDev);
    Resource->Load<CMaterial>(L"Terrain", L"../../Client/Bin/Resource/Material/Terrain.json", m_pGraphicDev);
    Resource->Load<CMaterial>(L"Unlit", L"../../Client/Bin/Resource/Material/Unlit.json", m_pGraphicDev);
    Resource->Load<CMaterial>(L"Wood", L"../../Client/Bin/Resource/Material/Wood.json", m_pGraphicDev);

    Resource->Load<CTexture>(L"Ground_Decor", CPathFunction::GroundDecor_Png_Path_InClient, m_pGraphicDev);
    Resource->Load<CTexture>(L"Wall_Decor", CPathFunction::WallDecor_Png_Path_InClient, m_pGraphicDev);
    Resource->Load<CTexture>(L"Obstacle", CPathFunction::Obstacle_Png_Path_InClient, m_pGraphicDev);
    Resource->Load<CTexture>(L"Interaction", CPathFunction::Interaction_Png_Path_InClient, m_pGraphicDev);
    Resource->Load<CTexture>(L"Tile", L"../../Client/Bin/Resource/Texture/Terrain/tileset.png", m_pGraphicDev);

    const struct { const wchar_t* k; const wchar_t* p; } uiList[] = {
        {L"SelectBook",     L"../Bin/Resource/Texture/UI/book_Ani.png"},
        {L"StageArrow_Left",L"../Bin/Resource/Texture/UI/StageArrow_Left.png"},
        {L"StageArrow_Right",L"../Bin/Resource/Texture/UI/StageArrow_Right.png"},
        {L"Logo",           L"../Bin/Resource/Texture/UI/wizordum_logo.png"},
        {L"Back",           L"../Bin/Resource/Texture/UI/PauseBack.png"},
        {L"MainBackGround", L"../Bin/Resource/Texture/Logo/Wizordum_Cover_170.5x.png"},
        {L"Banner_UI",      L"../Bin/Resource/Texture/UI/Banner_UI.png"},
        {L"Line_UI",        L"../Bin/Resource/Texture/UI/Line_UI.png"},
        {L"SetBack",        L"../Bin/Resource/Texture/UI/SetBack.png"},
        {L"thumb",          L"../Bin/Resource/Texture/UI/thumb_Total.png"},
        {L"Banner_Left",    L"../Bin/Resource/Texture/UI/Banner_UI_Left.png"},
        {L"Banner_Right",   L"../Bin/Resource/Texture/UI/Banner_UI_Right.png"},
        {L"Checking",       L"../Bin/Resource/Texture/UI/Checking.png"},
        {L"checkmark",      L"../Bin/Resource/Texture/UI/checkmark.png"},
        {L"Hit",            L"../Bin/Resource/Texture/UI/Hit.png"},
        {L"Eat",            L"../Bin/Resource/Texture/UI/Eat.png"},
        {L"Get",            L"../Bin/Resource/Texture/UI/Get.png"},
        {L"Prologue",       L"../Bin/Resource/Texture/UI/Prologue.png"},
        {L"BossHP",         L"../Bin/Resource/Texture/UI/BossHP.png"},
        {L"ClearIcon",      L"../Bin/Resource/Texture/UI/ClearIcon.png"},
        {L"Dialogue_Box",   L"../Bin/Resource/Texture/UI/Dialogue_Box.png"},
        {L"ArrowRight",     L"../Bin/Resource/Texture/UI/ArrowRight.png"},
    };
    for (auto& it : uiList)
        Resource->Load<CTexture>(it.k, it.p, m_pGraphicDev);

    m_bFinish = true;
    m_fLoadCnt = 100.f;
    return 0;
}

_uint CLoading::Loading_ForStage()
{
	CObjectManager::GetInstance()->Delete_Scene();
	CObjectPoolingManager::GetInstance()->AllActiveObject_Return();
	CCollisionManager::GetInstance()->Delete_ObjectList();

	auto collision = CCollisionManager::GetInstance();	// CollisionManager
	auto Resource = CResourceManager::GetInstance();
	auto pObject = CObjectManager::GetInstance();

	// 스카이박스
	Resource->Load<CTexture>(L"SkyBox1", L"../Bin/Resource/Texture/SkyBox/skybox-dawn-panorama.png", m_pGraphicDev);

	//맵 텍스처
	Resource->Load<CTexture>(L"projectiles", L"../Bin/Resource/Texture/Player/projectiles.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"projectiles2", L"../Bin/Resource/Texture/Player/projectiles2.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Player_Mace", L"../Bin/Resource/Texture/Player/Mace.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Player_Weapon", L"../Bin/Resource/Texture/Player/Weapon_Big.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Terrain", L"../Bin/Resource/Texture/Terrain/tiletest.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"MapUI", L"../Bin/Resource/Texture/Player/map_ui.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"NumUI_Tile", L"../Bin/Resource/Texture/UI/Num_R.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"HpIcon", L"../Bin/Resource/Texture/UI/Player_Hp.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"DefIcon", L"../Bin/Resource/Texture/UI/Player_Def.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"ItemIcon", L"../Bin/Resource/Texture/UI/ICON/ico-decor-user.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"BulIcon", L"../Bin/Resource/Texture/UI/Player_Bul.png", m_pGraphicDev);
	m_fLoadCnt = 20;
	// 몬스터
	Resource->Load<CTexture>(L"Goblin_Hammer_Animation", L"../Bin/Resource/Texture/Monster/goblin-hammer-Anim.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Goblin_Fireball_Animation", L"../Bin/Resource/Texture/Monster/goblin-fireball-Anim.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Orge_Axe_Animation", L"../Bin/Resource/Texture/Monster/ogre-axe-Anim.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Rat_Animation", L"../Bin/Resource/Texture/Monster/rat-Anim.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Mon_Projectiles", L"../Bin/Resource/Texture/Monster/projectiles.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Boss_Animation", L"../Bin/Resource/Texture/BossMonster/boss-baron.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Boss_IcePillar_Wind", L"../Bin/Resource/Texture/BossMonster/ice-wind.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Boss_Thunder", L"../Bin/Resource/Texture/BossMonster/lightning-big.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Effect_Thunder", L"../Bin/Resource/Texture/BossMonster/Thunder.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Effect_Thunder_Anim", L"../Bin/Resource/Texture/BossMonster/Thunder-Anim.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Wizard_Animation", L"../Bin/Resource/Texture/Monster/wizard-Anim.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Skeleton_Animation", L"../Bin/Resource/Texture/Monster/skeleton-sword-Anim.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Buff_Anim", L"../Bin/Resource/Texture/Monster/buff.png", m_pGraphicDev);


	// 파티클
	Resource->Load<CTexture>(L"ParticleTex", L"../../Client/Bin/Resource/Texture/Particle/particles.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"ParticleTex_Ground", L"../../Client/Bin/Resource/Texture/Particle/particles-ground.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"ParticleTex_Anim", L"../../Client/Bin/Resource/Texture/Particle/particle-shine-anim.png", m_pGraphicDev);
	
	// UI 
	Resource->Load<CTexture>(L"Back", L"../Bin/Resource/Texture/UI/PauseBack.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"NotifiBcg", L"../Bin/Resource/Texture/UI/notification-bcg.png", m_pGraphicDev);

	Resource->Load<CTexture>(L"Cross", L"../Bin/Resource/Texture/UI/crosshairs_8.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Cursor", L"../Bin/Resource/Texture/UI/cursor.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Money", L"../Bin/Resource/Texture/UI/Money.png", m_pGraphicDev);
	m_fLoadCnt = 40;
	Resource->Load<CTexture>(L"Shop_Hud", L"../Bin/Resource/Texture/UI/Shop_Hud.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Shop_Weapon", L"../Bin/Resource/Texture/UI/Shop_Weapon.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Shop_Spell", L"../Bin/Resource/Texture/UI/Shop_Spell.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Shop_Char", L"../Bin/Resource/Texture/UI/Shop_Char.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"MainBackGround", L"../Bin/Resource/Texture/Logo/Wizordum_Cover_170.5x.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Banner_UI", L"../Bin/Resource/Texture/UI/Banner_UI.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Line_UI", L"../Bin/Resource/Texture/UI/Line_UI.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"SetBack", L"../Bin/Resource/Texture/UI/SetBack.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"donut", L"../Bin/Resource/Texture/UI/donut.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"ClearFlag", L"../Bin/Resource/Texture/UI/ClearFlag.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"checkmark", L"../Bin/Resource/Texture/UI/checkmark.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"DeadScene", L"../Bin/Resource/Texture/UI/DeadScene.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Fade_Ground", L"../Bin/Resource/Texture/UI/Fade_Ground.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Item_Icon", L"../Bin/Resource/Texture/UI/Item_Icon.png", m_pGraphicDev);

	// 문
	Resource->Load<CTexture>(L"bossDoor", L"../Bin/Resource/Texture/Object/Door/bossDoor.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"defaultWoodDoor", L"../Bin/Resource/Texture/Object/Door/Defualt_Wooddoor.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"defaultIronDoor", L"../Bin/Resource/Texture/Object/Door/Default_Irondoor.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"SwitchWoodDoor", L"../Bin/Resource/Texture/Object/Door/Switch_Wooddoor.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"SwitchIronDoor", L"../Bin/Resource/Texture/Object/Door/Switch_Irondoor.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"BronzeDoor", L"../Bin/Resource/Texture/Object/Door/bronzeKeydoor.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"SilverDoor", L"../Bin/Resource/Texture/Object/Door/SilverKeydoor.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"GoldDoor", L"../Bin/Resource/Texture/Object/Door/GoldKeydoor.png", m_pGraphicDev);

	Resource->Load<CTexture>(L"Fade", L"../Bin/Resource/Texture/UI/fade.png", m_pGraphicDev);
	m_fLoadCnt = 60;
	// 아이템
	Resource->Load<CTexture>(L"Item1", L"../Bin/Resource/Texture/Item/loot-fire-mana.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Item2", L"../Bin/Resource/Texture/Item/loot-general.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"GunBullet", L"../Bin/Resource/Texture/Item/loot-arcane-rocks.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Armor_Shard", L"../Bin/Resource/Texture/Item/loot-armor-shard.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Ice_Wand", L"../Bin/Resource/Texture/Item/Ice_Wand.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"Gun", L"../Bin/Resource/Texture/Item/Player_Gun.png", m_pGraphicDev);
	//Light Object
	
	Resource->Load<CTexture>(L"STATUE", L"../Bin/Resource/Texture/Object/Light/STATUE.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"CANDLE", L"../Bin/Resource/Texture/Object/Light/CANDLE.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"GROUNDTORCH", L"../Bin/Resource/Texture/Object/Light/GROUNDTORCH.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"WALLTORCH", L"../Bin/Resource/Texture/Object/Light/WALLTORCH.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"HANGINGBRAZIER", L"../Bin/Resource/Texture/Object/Light/HANGINGBRAZIER.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"CHANDELIER", L"../Bin/Resource/Texture/Object/Light/CHANDELIER.png", m_pGraphicDev);
	Resource->Load<CTexture>(L"FIREALTAR", L"../Bin/Resource/Texture/Object/Light/FIREALTAR.png", m_pGraphicDev);


	/* Eye Of Chaos  */
	Resource->Load<CTexture>(L"EYEOFCHAOS", L"../Bin/Resource/Texture/Object/Event/EyeOfChaos/EyeOfChaos.png", m_pGraphicDev);



	//포그
	Resource->Load<CTexture>(L"Fog", L"../Bin/Resource/Texture/Particle/mist.png", m_pGraphicDev);
	//	================================================ 애니메이션 등록 ==========================================

	for (int i = 0; i < 10; ++i)
	{
		wstring wstrKey = L"NumUI_" + to_wstring(i);
		wstring wstrPath = L"../Bin/Resource/Texture/UI/Num_" + to_wstring(i) + L"_R.png";

		CResourceManager::GetInstance()->Load<CTexture>(wstrKey, wstrPath, m_pGraphicDev);
	}



	// =============== 오브젝트 풀링 미리 하는 공간 ==============

	_vec3 vPos; _vec3 vRot;

	JsonFunction::SetPlayerStartAndRot(SCENETYPE::STAGE1,&vPos,&vRot);

	RegisterClientFactories();

	if (!CObjectManager::GetInstance()->Get(OBJ_PLAYER))
		pObject->Preload(OBJ_PLAYER, 1, vPos.x, vPos.y, vPos.z, vRot.x, vRot.y, vRot.z);  // 반드시 플레이어가 먼저 호출되어야 한다.

	pObject->Preload(OBJ_SKYCUBE, 1, vPos.x , vPos.y, vPos.z , vRot.x, vRot.y, vRot.z);

	static_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJ_PLAYER))->SetPlayerDefaultY(vPos.y);

	RegisterClientPoolingFactories();
	CObjectManager::GetInstance()->PreLoad("../Bin/Resource/MapData/Stage1.json");

	// =============== CobjectManager & CObjectPoolingManager 가 objectlist를 다 작성했다면 ==================

    // (즉시형 ? 이전 코드 보존)
    BuildJobs_ForStage(); // 참고: 이제는 코옵 빌더가 메인
    m_bFinish = true; m_fLoadCnt = 100.f;
    return 0;
}

_uint CLoading::Loading_ForStage2()
{
    m_fLoadCnt = 60;
    CObjectManager::GetInstance()->Delete_Scene();
    CObjectPoolingManager::GetInstance()->AllActiveObject_Return();
    CCollisionManager::GetInstance()->Delete_ObjectList();

    auto collision = CCollisionManager::GetInstance();	// CollisionManager
    auto Resource = CResourceManager::GetInstance();
    auto pObject = CObjectManager::GetInstance();


    //	================================================ 애니메이션 등록 ==========================================

    for (int i = 0; i < 10; ++i)
    {
        wstring wstrKey = L"NumUI_" + to_wstring(i);
        wstring wstrPath = L"../Bin/Resource/Texture/UI/Num_" + to_wstring(i) + L"_R.png";

        CResourceManager::GetInstance()->Load<CTexture>(wstrKey, wstrPath, m_pGraphicDev);
    }



    // =============== 오브젝트 풀링 미리 하는 공간 ==============

    _vec3 vPos; _vec3 vRot;

    JsonFunction::SetPlayerStartAndRot(SCENETYPE::STAGE2, &vPos, &vRot);

    RegisterClientFactories();

    if (!CObjectManager::GetInstance()->Get(OBJ_PLAYER))
        pObject->Preload(OBJ_PLAYER, 1, vPos.x, vPos.y, vPos.z, vRot.x, vRot.y, vRot.z);  // 반드시 플레이어가 먼저 호출되어야 한다.

    pObject->Preload(OBJ_SKYCUBE, 1, vPos.x, vPos.y, vPos.z, vRot.x, vRot.y, vRot.z);

    static_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJ_PLAYER))->SetPlayerDefaultY(vPos.y);

    RegisterClientPoolingFactories();
    CObjectManager::GetInstance()->PreLoad("../Bin/Resource/MapData/Stage2.json");

    // =============== CobjectManager & CObjectPoolingManager 가 objectlist를 다 작성했다면 ==================


    // (즉시형 ? 이전 코드 보존)
    BuildJobs_ForStage2(); // 참고: 이제는 코옵 빌더가 메인
    m_bFinish = true; m_fLoadCnt = 100.f;
    return 0;
}

_uint CLoading::Loading_ForBossStage()
{
    BuildJobs_ForBoss();
    m_bFinish = true; m_fLoadCnt = 100.f;
    return 0;
}

_uint CLoading::Loading_ForBossStage2()
{
    BuildJobs_ForBoss2();
    m_bFinish = true; m_fLoadCnt = 100.f;
    return 0;
}

_uint CLoading::Loading_ForTutorialStage()
{
    BuildJobs_ForTutorial();
    m_bFinish = true; m_fLoadCnt = 100.f;
    return 0;
}

// -------------------------------------------------------------------------------------
// 팩토리/풀 등록 (원본 그대로)
// -------------------------------------------------------------------------------------
void CLoading::RegisterClientPoolingFactories()
{
    auto pPoolManager = CObjectPoolingManager::GetInstance();

    pPoolManager->RegisterFactory(OBJ_MONSTER_GOBLIN_HAMMER, [=]() { return CMonster_Goblin_Hammer::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_MONSTER_ORGE_AXE, [=]() { return CMonster_Orge_Axe::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_MONSTER_GOBLIN_FIREBALL, [=]() { return CMonster_Goblin_Fireball::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_MONSTER_SKELETON_SWORD, [=]() { return CMonster_Skeleton_Sword::Create(m_pGraphicDev); });

    pPoolManager->RegisterFactory(OBJ_PARTICLE, [=]() { return CParticle::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_PARTICLE_SYSTEM_BLOOD, [=]() { return CParticleSystem_Blood::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_PARTICLE_SYSTEM_SPARKLE, [=]() { return CParticleSystem_Sparkle::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_PARTICLE_SYSTEM_ICE, [=]() { return CParticleSystem_Ice::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_PARTICLE_SYSTEM_BUSH, [=]() { return CParticleSystem_Bush::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_PARTICLE_SYSTEM_BARREL, [=]() { return CParticleSystem_Barrel::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_PARTICLE_SYSTEM_BOOK, [=]() { return CParticleSystem_Book::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_PARTICLE_SYSTEM_EAT, [=]() { return CParticleSystem_Eat::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_PARTICLE_SYSTEM_BONE, [=]() { return CParticleSystem_Bone::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_PARTICLE_SYSTEM_BROKENCUBE, [=]() { return CParticleSystem_BrokenCube::Create(m_pGraphicDev); });

    pPoolManager->RegisterFactory(OBJ_BULLET, [=]() { return CBullet::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_ITEM, [=]() { return CItem::Create(m_pGraphicDev); });
    pPoolManager->RegisterFactory(OBJ_MONSTER_BOSS_CLONE, [=]() { return CMonster_Clone_Boss::Create(m_pGraphicDev); });

    pPoolManager->Preload(OBJ_PARTICLE_SYSTEM_BLOOD, 100);
    pPoolManager->Preload(OBJ_PARTICLE_SYSTEM_SPARKLE, 100);
    pPoolManager->Preload(OBJ_PARTICLE_SYSTEM_ICE, 100);
    pPoolManager->Preload(OBJ_PARTICLE_SYSTEM_BUSH, 50);
    pPoolManager->Preload(OBJ_PARTICLE_SYSTEM_BARREL, 50);
    pPoolManager->Preload(OBJ_PARTICLE_SYSTEM_BOOK, 50);
    pPoolManager->Preload(OBJ_PARTICLE_SYSTEM_EAT, 50);
    pPoolManager->Preload(OBJ_PARTICLE_SYSTEM_BONE, 50);
    pPoolManager->Preload(OBJ_PARTICLE_SYSTEM_BROKENCUBE, 100);
    pPoolManager->Preload(OBJ_PARTICLE, 1000);

    pPoolManager->Preload(OBJ_BULLET, 1000);
    pPoolManager->Preload(OBJ_MONSTER_GOBLIN_HAMMER, 50);
    pPoolManager->Preload(OBJ_MONSTER_ORGE_AXE, 50);
    pPoolManager->Preload(OBJ_MONSTER_GOBLIN_FIREBALL, 50);
    pPoolManager->Preload(OBJ_ITEM, 500);
    pPoolManager->Preload(OBJ_MONSTER_BOSS_CLONE, 50);
    pPoolManager->Preload(OBJ_MONSTER_SKELETON_SWORD, 50);
}

void CLoading::PreloadClientPoolingFactories() {}

void CLoading::RegisterClientFactories()
{
    auto pObject = CObjectManager::GetInstance();

    pObject->RegisterFactory(OBJ_PLAYER, [=]() { return CPlayer::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_TERRAIN, [=]() { return CTerrain::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_TILE, [=]() { return CTile::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_SKYCUBE, [=]() { return CSkyBox::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_CUBE, [=]() { return CCube::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_EVENTCUBE, [=]() { return CEventCube::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_CREATURECANTGO, [=]() { return CreatureCantGoCube::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_MONSTER_GOBLIN_HAMMER, [=]() { return CMonster_Goblin_Hammer::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_MONSTER_GOBLIN_FIREBALL, [=]() { return CMonster_Goblin_Fireball::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_MONSTER_ORGE_AXE, [=]() { return CMonster_Orge_Axe::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_MONSTER_RAT, [=]() { return CMonster_Rat::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_MONSTER_BOSS, [=]() { return CMonster_Boss::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_MONSTER_SKELETON_SWORD, [=]() { return CMonster_Skeleton_Sword::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_MONSTER_WIZARD, [=]() { return CMonster_Wizard::Create(m_pGraphicDev); });

    pObject->RegisterFactory(OBJ_BARRIER, [=]() { return CBossBarrier::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_DOOR, [=]() { return CDoor::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_MAPOBSTACLE, [=]() { return CObstacle::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_ITEM, [=]() { return CItem::Create(m_pGraphicDev); });
    //pObject->RegisterFactory(OBJ_EFFECT_BOSS_APPEAR, [=]() { return CBossAppearEffect::Create(m_pGraphicDev); });
    //pObject->RegisterFactory(OBJ_BOSS_EFFECT, [=]() { return CBossEffect::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_PARTICLE_SYSTEM_RAIN, [=]() { return CParticleSystem_Rain::Create(m_pGraphicDev); });
    pObject->RegisterFactory(OBJ_FOG_SYSTEM, [=]() { return CFogSystem::Create(m_pGraphicDev); });

    // 이벤트 큐브
    pObject->RegisterEventCubeFactory(CUBE_EVENT_TYPE::POSITION, [=]() { return CTargetPointCube::Create(m_pGraphicDev); });
    pObject->RegisterEventCubeFactory(CUBE_EVENT_TYPE::PORTAL, [=]() { return CPortalCube::Create(m_pGraphicDev); });
    pObject->RegisterEventCubeFactory(CUBE_EVENT_TYPE::MOVE, [=]() { return CMoveCube::Create(m_pGraphicDev); });
    pObject->RegisterEventCubeFactory(CUBE_EVENT_TYPE::DESTRUCTIBLE_ATTACK, [=]() { return CDestructibleAttackCube::Create(m_pGraphicDev); });
    pObject->RegisterEventCubeFactory(CUBE_EVENT_TYPE::DESTRUCTIBLE_GIMIC, [=]() { return CDestructibleGimicCube::Create(m_pGraphicDev); });

    // Light Object
    pObject->RegisterLightObjectFactory(LIGHT_OBJECT_TYPE::STATUE, [=]() { return CStatue::Create(m_pGraphicDev); });
    pObject->RegisterLightObjectFactory(LIGHT_OBJECT_TYPE::GROUNDTORCH, [=]() { return CGroundTorch::Create(m_pGraphicDev); });
    pObject->RegisterLightObjectFactory(LIGHT_OBJECT_TYPE::WALLTORCH, [=]() { return CWallTorch::Create(m_pGraphicDev); });
    pObject->RegisterLightObjectFactory(LIGHT_OBJECT_TYPE::CANDLE, [=]() { return CCandle::Create(m_pGraphicDev); });
    pObject->RegisterLightObjectFactory(LIGHT_OBJECT_TYPE::CANDLETRIPLE, [=]() { return CTripleCandle::Create(m_pGraphicDev); });
    pObject->RegisterLightObjectFactory(LIGHT_OBJECT_TYPE::CHANDELIER, [=]() { return CChandelier::Create(m_pGraphicDev); });
    pObject->RegisterLightObjectFactory(LIGHT_OBJECT_TYPE::HANGINGBRAZIER, [=]() { return CHangingbrazier::Create(m_pGraphicDev); });
    pObject->RegisterLightObjectFactory(LIGHT_OBJECT_TYPE::FIREALTAR, [=]() { return CFirealtar::Create(m_pGraphicDev); });
}

void CLoading::PreloadClientFactories() {}

// -------------------------------------------------------------------------------------
// Create / Free
// -------------------------------------------------------------------------------------
CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
    CLoading* pLoading = new CLoading(pGraphicDev);
    if (FAILED(pLoading->Ready_Loading(eID)))
    {
        Safe_Release(pLoading);
        MSG_BOX("Loading Create Failed");
        return nullptr;
    }
    return pLoading;
}

void CLoading::Free()
{
    Engine::CScene::Free();
}
