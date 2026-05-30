#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CCameraManager.h"

BEGIN(Engine)

class ENGINE_DLL CDebugLine
    :public CBase
{
public:
    struct LINE_INFO
    {
        D3DXVECTOR3 vStart;
        D3DXVECTOR3 vEnd;
        D3DXCOLOR   Color;
    };

public:

    // 유니티 최고

    static void Initialize(LPDIRECT3DDEVICE9 pDevice)
    {
        m_pDevice = pDevice;
    }

    static void AddLine(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXCOLOR& color)
    {
        m_vLines.push_back({ start, end, color });
    }
    // CDebugLine::Render()
    static void Render() {
        if (m_vLines.empty() || m_pDevice == nullptr) return;

        // 백업
        D3DXMATRIX oldW, oldV, oldP, I;
        m_pDevice->GetTransform(D3DTS_WORLD, &oldW);
        m_pDevice->GetTransform(D3DTS_VIEW, &oldV);
        m_pDevice->GetTransform(D3DTS_PROJECTION, &oldP);
        DWORD oldLighting, oldZEnable, oldZWrite, oldFVF;
        m_pDevice->GetRenderState(D3DRS_LIGHTING, &oldLighting);
        m_pDevice->GetRenderState(D3DRS_ZENABLE, &oldZEnable);
        m_pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &oldZWrite);
        m_pDevice->GetFVF(&oldFVF);

        D3DXMatrixIdentity(&I);
        // 월드는 항~상 단위행렬
        m_pDevice->SetTransform(D3DTS_WORLD, &I);

        // 카메라 행렬(메인 카메라 가져오는 쪽으로 교체)
        auto* cam = CCameraManager::GetInstance();
        D3DXMATRIX V = cam->Get_MainViewMatrix();
        D3DXMATRIX P = cam->Get_MainProjMatrix();
        m_pDevice->SetTransform(D3DTS_VIEW, &V);
        m_pDevice->SetTransform(D3DTS_PROJECTION, &P);

        // 상태
        m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
        m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);     // 깊이 테스트는 유지
        m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); // 쓰기만 끔
        m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

        for (const auto& line : m_vLines) {
            struct VTX { D3DXVECTOR3 pos; D3DCOLOR color; };
            VTX v[2] = {
                { line.vStart, D3DCOLOR_COLORVALUE(line.Color.r, line.Color.g, line.Color.b, line.Color.a) },
                { line.vEnd,   D3DCOLOR_COLORVALUE(line.Color.r, line.Color.g, line.Color.b, line.Color.a) }
            };
            m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(VTX));
        }

        // 복구
        m_pDevice->SetFVF(oldFVF);
        m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, oldZWrite);
        m_pDevice->SetRenderState(D3DRS_ZENABLE, oldZEnable);
        m_pDevice->SetRenderState(D3DRS_LIGHTING, oldLighting);
        m_pDevice->SetTransform(D3DTS_WORLD, &oldW);
        m_pDevice->SetTransform(D3DTS_VIEW, &oldV);
        m_pDevice->SetTransform(D3DTS_PROJECTION, &oldP);
    }


    static void Clear()
    {
        m_vLines.clear();
    }

private:
    static inline LPDIRECT3DDEVICE9 m_pDevice = nullptr;
    static inline vector<LINE_INFO> m_vLines;
};

END