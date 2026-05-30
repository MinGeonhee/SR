#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CUI;

class ENGINE_DLL CUIManager : public CBase
{
	DECLARE_SINGLETON(CUIManager)

private:
	explicit CUIManager();
	virtual ~CUIManager();

public:
	void Update_UI(const _float& fTimeDelta);
	void LateUpdate_UI(const _float& fTimeDelta);
	void Render_UI();

	void Add_UI(CUI* pUI);
	void Remove_UI(CUI* pUI);
	void Clear();

	const vector<CUI*>& Get_UI() const { return m_vecUI; }
	void Set_UI(const vector<CUI*>& vecUI) { m_vecUI = vecUI; }

	HRESULT InitFonts(LPDIRECT3DDEVICE9 dev);
	void    ReleaseFonts();

	LPD3DXFONT GetBig()   const { return m_BigFont; }
	LPD3DXFONT GetMid()   const { return m_MidFont; }
	LPD3DXFONT GetSmall() const { return m_SmallFont; }
	LPD3DXFONT GetMini() const { return m_MiniFont; }

	CUI* Find_UI(const wstring& tag);
	bool ActivateUI(const wstring& tag);
	bool DeactivateUI(const wstring& tag);
	int  DeactivateAll(bool clearCategoryTags = false, bool clearItemTags = false);

	void SetGlobalPause(bool b);
	void ForceUnpauseAll();

	void SetCategoryTag(const wstring& category, const wstring& tag);
	const wstring& GetCategoryTag(const wstring& category) const;
	void EraseCategoryTag(const wstring& category);

	void PushTag(const wstring& tag);
	bool PopTag(wstring& out);
	void ClearTags();
	size_t TagCount() const { return m_Itemtags.size(); }

	bool IsCleared() { return m_bClear; }
	void SetCleared(bool c) { m_bClear = c; }

private:
	vector<CUI*> m_vecUI;
	unordered_map<wstring, CUI*> m_byTag;
	unordered_map<wstring, wstring> m_categoryTags;

	LPD3DXFONT m_BigFont;
	LPD3DXFONT m_MidFont;
	LPD3DXFONT m_SmallFont;
	LPD3DXFONT m_MiniFont;
	bool       m_fontsInit;

	deque<wstring> m_Itemtags;
	bool m_bClear = false;
public:
	virtual void Free() override;
};

END