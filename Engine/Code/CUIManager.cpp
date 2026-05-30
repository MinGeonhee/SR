#include "CUIManager.h"
#include "CUI.h"

IMPLEMENT_SINGLETON(CUIManager)

CUIManager::CUIManager()
    : m_BigFont(nullptr), m_MidFont(nullptr), m_SmallFont(nullptr), m_MiniFont(nullptr),
    m_fontsInit(false)
{
}

CUIManager::~CUIManager()
{
    Free();
}

void CUIManager::Add_UI(CUI* pUI)
{
    if (!pUI) return;
    m_vecUI.push_back(pUI);

    const auto& tag = pUI->Get_Tag();
    if (!tag.empty()) {
        auto it = m_byTag.find(tag);
        if (it != m_byTag.end() && it->second != pUI) {
            it->second->Set_UIActive(false);
        }
        m_byTag[tag] = pUI;
    }
}

void CUIManager::Remove_UI(CUI* pUI)
{
    if (!pUI) return;
    m_vecUI.erase(remove(m_vecUI.begin(), m_vecUI.end(), pUI), m_vecUI.end());

    auto it = m_byTag.find(pUI->Get_Tag());
    if (it != m_byTag.end() && it->second == pUI) m_byTag.erase(it);
}

void CUIManager::Clear()
{
    for (auto& pUI : m_vecUI) Safe_Release(pUI);
    m_vecUI.clear();
    m_byTag.clear();
}

void CUIManager::Update_UI(const _float& fTimeDelta)
{
    for (auto& pUI : m_vecUI)
        if (pUI)
            pUI->Update_GameObject(fTimeDelta);
}

void CUIManager::LateUpdate_UI(const _float& fTimeDelta)
{
    for (auto& pUI : m_vecUI)
        if (pUI)
            pUI->LateUpdate_GameObject(fTimeDelta);

    for (auto it = m_vecUI.begin(); it != m_vecUI.end(); )
    {
        CUI* pUI = *it;
        if (pUI && pUI->Is_Finish())
        {
            Remove_UI(pUI);
            it = m_vecUI.begin();
        }
        else ++it;
    }
}

void CUIManager::Render_UI()
{
    for (auto& pUI : m_vecUI)
        if (pUI)
            pUI->Render_GameObject();
}

void CUIManager::Free()
{
    Clear();
    ReleaseFonts();
}

HRESULT CUIManager::InitFonts(LPDIRECT3DDEVICE9 dev)
{
    if (m_fontsInit) return S_OK;

    if (!m_BigFont) {
        if (FAILED(D3DXCreateFont(dev, 50, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            L"Germania One", &m_BigFont))) return E_FAIL;
    }
    if (!m_MidFont) {
        if (FAILED(D3DXCreateFont(dev, 40, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            L"Germania One", &m_MidFont))) return E_FAIL;
    }
    if (!m_SmallFont) {
        if (FAILED(D3DXCreateFont(dev, 30, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            L"Germania One", &m_SmallFont))) return E_FAIL;
    }
    if (!m_MiniFont) {
        if (FAILED(D3DXCreateFont(dev, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            L"Germania One", &m_MiniFont))) return E_FAIL;
    }
    m_fontsInit = true;
    return S_OK;
}

void CUIManager::ReleaseFonts()
{
    if (m_BigFont) { m_BigFont->Release();   m_BigFont = nullptr; }
    if (m_MidFont) { m_MidFont->Release();   m_MidFont = nullptr; }
    if (m_SmallFont) { m_SmallFont->Release(); m_SmallFont = nullptr; }
    if (m_MiniFont) { m_MiniFont->Release(); m_MiniFont = nullptr; }
    m_fontsInit = false;
}

CUI* CUIManager::Find_UI(const std::wstring& tag)
{
    auto it = m_byTag.find(tag);
    return (it != m_byTag.end()) ? it->second : nullptr;
}

bool CUIManager::ActivateUI(const std::wstring& tag)
{
    if (auto* ui = Find_UI(tag)) {
        ui->Set_UIActive(true);
        return true;
    }
    return false;
}

bool CUIManager::DeactivateUI(const std::wstring& tag)
{
    if (auto* ui = Find_UI(tag)) {
        ui->Set_UIActive(false);
        return true;
    }
    return false;
}

void CUIManager::SetGlobalPause(bool b)
{
    if (CUI::IsPaused() == b) return;

    CUI::SetPaused(b);
}

void CUIManager::ForceUnpauseAll()
{
    CUI::SetPaused(false);
}

void CUIManager::PushTag(const wstring& tag)
{
    if (m_Itemtags.size() == 5)
        m_Itemtags.pop_front();
    m_Itemtags.push_back(tag); //5개넘어가면제일오래된거없애고새로넣기
}

bool CUIManager::PopTag(wstring& out)
{
    if (m_Itemtags.empty()) return false;
    out = m_Itemtags.front();
    m_Itemtags.pop_front();
    return true;
}

void CUIManager::ClearTags()
{
    m_Itemtags.clear();
}

void CUIManager::SetCategoryTag(const wstring& category, const wstring& tag)
{
    m_categoryTags[category] = tag;
}

const wstring& CUIManager::GetCategoryTag(const wstring& category) const
{
    auto it = m_categoryTags.find(category);
    if (it != m_categoryTags.end())
        return it->second;

    static const wstring empty;
    return empty;
}

void CUIManager::EraseCategoryTag(const wstring& category)
{
    m_categoryTags.erase(category);
}

int CUIManager::DeactivateAll(bool clearCategoryTags, bool clearItemTags)
{
    int changed = 0;

    for (auto& kv : m_byTag) {
        CUI* ui = kv.second;
        if (!ui) continue;
        ui->Set_UIActive(false);
        ++changed;
    }

    if (clearCategoryTags) 
        m_categoryTags.clear();
    if (clearItemTags)
        m_Itemtags.clear();

    return changed;
}
