#include "CMaterial.h"
#include <fstream>
#include <filesystem>

using namespace std;
using json = nlohmann::json;

USING(Engine)

CMaterial::CMaterial(LPDIRECT3DDEVICE9 pGraphicDev)
    : CResourceBase(RESOURCETYPE::MATERIAL, pGraphicDev), m_Material{}
{
    ZeroMemory(&m_Material, sizeof(m_Material));
}

CMaterial::CMaterial(const CMaterial& rhs)
    : CResourceBase(rhs), m_Material(rhs.m_Material)
{
}

CMaterial::~CMaterial()
{
}

HRESULT CMaterial::Ready_Material(const Desc& d)
{
    ZeroMemory(&m_Material, sizeof(m_Material));
    m_Material.Diffuse = d.Diffuse;
    m_Material.Specular = d.Specular;
    m_Material.Ambient = d.Ambient;
    m_Material.Emissive = d.Emissive;
    m_Material.Power = d.Power;
    return S_OK;
}

void CMaterial::Set_Material() const
{
    // 라이팅이 꺼져 있으면 머티리얼 효과가 안 보임에 주의 (D3DRS_LIGHTING = TRUE 필요)
    m_pGraphicDev->SetMaterial(&m_Material);
}

static D3DXCOLOR _readColor(const json& j, const char* key, const D3DXCOLOR& def)
{
    if (!j.contains(key) || !j[key].is_array() || j[key].size() < 4)
        return def;
    return D3DXCOLOR(
        j[key][0].get<float>(),
        j[key][1].get<float>(),
        j[key][2].get<float>(),
        j[key][3].get<float>());
}

void CMaterial::Load(const wstring& path)
{
    std::ifstream ifs{ std::filesystem::path(path) }; 
    if (!ifs.is_open()) { MSG_BOX("CMaterial 파일 열기 실패!"); return; }

    nlohmann::json j;              
    try { ifs >> j; }
    catch (...) { MSG_BOX("CMaterial JSON 파싱 실패!"); return; }

    Desc d;
    d.Diffuse = _readColor(j, "Diffuse", d.Diffuse);
    d.Specular = _readColor(j, "Specular", d.Specular);
    d.Ambient = _readColor(j, "Ambient", d.Ambient);
    d.Emissive = _readColor(j, "Emissive", d.Emissive);
    if (j.contains("Power")) d.Power = j["Power"].get<float>();

    Ready_Material(d);
}


CMaterial* CMaterial::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& path)
{
    CMaterial* p = new CMaterial(pGraphicDev);
    return p;
}

CResourceBase* CMaterial::Clone()
{
    return new CMaterial(*this);
}

void CMaterial::Free()
{
    CResourceBase::Free();
}
