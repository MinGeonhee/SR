#include "CObjectPoolingManager.h"
#include "CSceneManager.h"
#include "CObjectManager.h"
#include "CRendererManager.h"
#include "CScene.h"
#include "CUIManager.h"
#include "CCollisionManager.h"
#include "CEventBus.h"
IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager() : m_SceneType(MAIN), m_nowScene(nullptr)
{

}

CSceneManager::~CSceneManager()
{
	Free();
}

void CSceneManager::RegisterFactory(SCENETYPE type, SceneFactory factory)
{
	m_mSceneList[type] = factory;
}

void CSceneManager::RegisterLoadingScene(SCENETYPE type, LoadingFactory factory)
{
	m_mLoading[type] = factory;
}

void CSceneManager::Initalize()
{
	if (!m_nowScene)
	{
		auto it = m_mLoading.find(SCENETYPE::LOADING);

		if (it != m_mLoading.end())
		{
			m_nowScene = it->second(LOADINGID::LOADING_LOGO);
		}
	}
}

void CSceneManager::Update_Scene(const float& fTimeDelta)
{
	if (m_nowScene)
		m_nowScene->Update_Scene(fTimeDelta);
}

void CSceneManager::LateUpdate_Scene(const float& fTimeDelta)
{
	if (m_nowScene)
		m_nowScene->LateUpdate_Scene(fTimeDelta);
}

void CSceneManager::Change_Scene(SCENETYPE _sceneType, LOADINGID eID) // 씬을 교체하는 함수
{
	if (_sceneType == m_SceneType)	// 현재 씬의 타입과 동일하면 return
		return;

	EVENT event
	{
		PHASE::PHASE_BEGIN,
		TOPIC::TIME_RESUME,
		CHANNEL::Global,
		nullptr,
		nullptr,
		{},
		{}
	};

	CEventBus::GetInstance()->publish(event);

	CUIManager::GetInstance()->Clear();

	CScene* scene = nullptr;		// 씬의 주소값을 담을 지역변수

	switch (_sceneType)				// Type에 따라 포인터 지역 변수에 해당 씬 클래스를 할당
	{
		case LOGO:
		{
			auto it = m_mSceneList.find(SCENETYPE::LOGO);

			if (it != m_mSceneList.end()) scene = it->second();
			else
			{
				MSG_BOX("씬 전환 실패!!!");
				return;
			}
			break;
		}

		case MAIN:
		{
			auto it = m_mSceneList.find(SCENETYPE::MAIN);

			if (it != m_mSceneList.end()) scene = it->second(); 
			else
			{
				MSG_BOX("씬 전환 실패!!!");
				return;
			}
			break;
		}

		case STAGE1:
		{
			auto it = m_mSceneList.find(SCENETYPE::STAGE1);

			if (it != m_mSceneList.end()) scene = it->second();
			else
			{
				MSG_BOX("씬 전환 실패!!!");
				return;
			}
			break;
		}

		case STAGE2:
		{
			auto it = m_mSceneList.find(SCENETYPE::STAGE2);

			if (it != m_mSceneList.end()) scene = it->second();
			else
			{
				MSG_BOX("씬 전환 실패!!!");
				return;
			}
			break;
		}


		case LOADING:
		{
			auto it = m_mLoading.find(SCENETYPE::LOADING);

			if (it != m_mLoading.end()) scene = it->second(eID);
			else
			{
				MSG_BOX("씬 전환 실패!!!");
				return;
			}
			break;
		}
		case TITLE:
		{
			auto it = m_mSceneList.find(SCENETYPE::TITLE);

			if (it != m_mSceneList.end()) scene = it->second();
			else
			{
				MSG_BOX("씬 전환 실패!!!");
				return;
			}
			break;
		}
		case TUTORIAL:
		{
			auto it = m_mSceneList.find(SCENETYPE::TUTORIAL);

			if (it != m_mSceneList.end()) scene = it->second();
			else
			{
				MSG_BOX("씬 전환 실패!!!");
				return;
			}
			break;
		}

		case BOSS:
		{
			auto it = m_mSceneList.find(SCENETYPE::BOSS);

			if (it != m_mSceneList.end()) scene = it->second();
			else
			{
				MSG_BOX("씬 전환 실패!!!");
				return;
			}
			break;
		}

		case BOSS2:
		{
			auto it = m_mSceneList.find(SCENETYPE::BOSS2);

			if (it != m_mSceneList.end()) scene = it->second();
			else
			{
				MSG_BOX("씬 전환 실패!!!");
				return;
			}
			break;
		}
	}

	CRendererManager::GetInstance()->Clear_RenderGroup();

	Safe_Release(m_nowScene);

	m_nowScene = scene;				// type에 의해 정해진 씬을 대입

	m_SceneType = _sceneType;		// 바뀐 type을 대입 --> ChangeScene에서 비교를 위해
}

void CSceneManager::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRendererManager::GetInstance()->Render_GameObject(pGraphicDev);
	m_nowScene->Render_Scene();
}

void CSceneManager::Free()
{
	Safe_Release(m_nowScene);
}

