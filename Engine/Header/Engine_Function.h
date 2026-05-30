#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"
#include "Engine_Struct.h"

namespace Engine
{
	// 템플릿은 기능의 정해져있으나 자료형은 정해져있지 않은 것
	// 기능을 인스턴스화 하기 위하여 만들어두는 틀

	template<typename T>
	void	Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void	Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete[] Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = NULL;
		}

		return dwRefCnt;
	}


	// Functor
	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag) : m_pTargetTag(pTag) {}
		~CTag_Finder(void) {}

	public:
		template<typename T>
		_bool		operator()(const T& pair)
		{
			if (0 == lstrcmpW(m_pTargetTag, pair.first))
				return true;

			return false;
		}

	private:
		const _tchar* m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	// 연관컨테이너 삭제용
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};


	inline const _vec2& LT(int x, int y , int w , int h , int Mw, int Mh)
	{
		return _vec2(x / (float)Mw, y / (float)Mh);
	}
	inline const _vec2 LT(const SpriteRect& tSpriteRect)
	{
		return _vec2(tSpriteRect.x / (float)tSpriteRect.Mw, tSpriteRect.y / (float)tSpriteRect.Mh);
	}

	inline const  _vec2& RT(int x, int y, int w, int h ,int Mw, int Mh)
	{
		return _vec2( (x + w) / (float)Mw, y / (float)Mh);
	}
	inline const _vec2 RT(const SpriteRect& tSpriteRect)
	{
		return _vec2((tSpriteRect.x + tSpriteRect.w )/ (float)tSpriteRect.Mw, tSpriteRect.y / (float)tSpriteRect.Mh);
	}

	inline const _vec2& RB(int x, int y, int w, int h, int Mw, int Mh)
	{
		return _vec2((x + w) / (float)Mw, (y + h) / (float)Mh);
	}

	inline const _vec2& LB(int x, int y , int w, int h , int Mw, int Mh)
	{
		return _vec2(x / (float)Mw, (y +h) / (float)Mh);
	}

	inline const _vec3& PixelToWorldScale(int w, int h )
	{
		if (w == 0 || h == 0)
			return _vec3(0, 0,0);

		return _vec3(  w / FIXEL_SIZE  , h / FIXEL_SIZE , 1.f);
	}
}

#endif // Engine_Function_h__
