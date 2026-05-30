#include "CResourceManager.h"

IMPLEMENT_SINGLETON(CResourceManager);


void CResourceManager::Free()
{
	for (auto& _resource : _resources)
	{
		for_each(_resource.begin(), _resource.end(), CDeleteMap());
	}
}