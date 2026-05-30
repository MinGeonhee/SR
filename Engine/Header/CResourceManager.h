#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CResourceBase.h"
#include "CTexture.h"
#include "CAnimation.h"
#include "CMaterial.h"

BEGIN(Engine)

//  리소스 매니저를 Template 함수로 떡칠을 한다.
class Sound;

class ENGINE_DLL CResourceManager :
    public CBase
{
    DECLARE_SINGLETON(CResourceManager)

private:
    explicit CResourceManager() {}
    ~CResourceManager() { Free(); }

public:

    // 각 리소스들을 전부 Load 시킬 수 있게 template화 시킨다.
    // cpp 구현하면 Linker 오류 생길 수 있으니 이 바로 밑에 전부 선언
    template<typename T>
    T* Load(const wstring& key, const wstring& path, LPDIRECT3DDEVICE9 pGraphicDev);

    template<typename T>
    bool Add(const wstring& key, T* obejct);

    template<typename T>
    T* Get(const wstring& key);

    template<typename T>
    RESOURCETYPE GetResourceType();

    virtual void    Free() override;

private:
    wstring _resourcePath;

private:

    // 리소스 개수만큼 map을 가진 array 배열을 생성

    using KeyObjMap = map<wstring, CResourceBase*>;
     
    KeyObjMap       _resources[RESOURCE_TYPE_COUNT];

};

// ~~~~~~~~~~~~~~~~~템플릿 선언 ~~~~~~~~~~~~~~~~~~~~~~

// Json에서 Load되서 받아올 때
// Key, Value 값을 넣어주면 그만.
// ex) Key : Player, path : "~~~~~~~~~. txt"

template<typename T>

T* CResourceManager::Load(const wstring& key, const wstring& path, LPDIRECT3DDEVICE9 pGraphicDev)
{
    auto objectType = GetResourceType<T>();
    KeyObjMap& keyObjMap = _resources[static_cast<_uint8>(objectType)];

    auto findIt = keyObjMap.find(key);
    if (findIt != keyObjMap.end())
        return static_cast<T*>(findIt->second);

    T* object = T::Create(pGraphicDev, path); 
    if (object == nullptr)
    {
        MSG_BOX("리소스 생성 실패!");
        return nullptr;
    }

    object->Load(path);
    keyObjMap[key] = object;
    
    return object;
}

template<typename T>
bool CResourceManager::Add(const wstring& key, T* object)
{
    RESOURCETYPE resourceType = GetResourceType<T>();
    KeyObjMap& keyObjMap = _resources[static_cast<_uint8>(resourceType)];

    auto findIt = keyObjMap.find(key);
    if (findIt != keyObjMap.end())
        return false;

    keyObjMap[key] = object;
    return true;
}

template<typename T>
T* CResourceManager::Get(const wstring& key)
{
    RESOURCETYPE resourceType = GetResourceType<T>();
    KeyObjMap& keyObjMap = _resources[static_cast<_uint8>(resourceType)];

    auto findIt = keyObjMap.find(key);
    if (findIt != keyObjMap.end())

        return static_cast<T*>(findIt->second);

    return nullptr;
}

template<typename T>
RESOURCETYPE CResourceManager::GetResourceType()
{
    // 17부터 지원하는 함수인 is_same_v 
    // 즉, std::is_same_v<T, U>는 T와 U가 같은 타입이면 true, 아니면 false를 반환하는 bool 상수입니다.
    // 라고 하네요. 사기잖아.

    if (std::is_same_v<T, CTexture>)
        return RESOURCETYPE::TEXTURE;
    if (std::is_same_v<T, Sound>)
        return RESOURCETYPE::SOUND;
    if (std::is_same_v<T, CMaterial>)
        return RESOURCETYPE::MATERIAL;

    return RESOURCETYPE::NONE;
}



END
