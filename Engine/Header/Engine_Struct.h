#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Function.h"

namespace Engine
{
	typedef struct tagVertexColor
	{
		_vec3		vPosition;
		_ulong		dwColor;

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec2		vTexUV;

	}VTXTEX;
	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;


	typedef struct tagVertexNormalTexture
	{
		_vec3 vPosition;  // 0..11
		_vec3 vNormal;    // 12..23  ← Normal이 반드시 Texcoords보다 먼저 와야 함
		_vec2 vTexUV;     // 24..31
	} VTXNORMALTEX;

	const _ulong FVF_NORMALTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); 
	// 텍스처의 UV 좌표 값을 FLOAT형 3개로 표현하겠다는 매크로
	// (괄호안의 숫자 0의 의미는 본래 버텍스에 텍스쳐 UV값이 여러개가 올 수 있는데 
	// 그중 0번째 값을 지정하겠다는 의미)

	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;


	typedef struct typeStatus
	{
		float       fMaxHp = 0.f;
		float       fCurrentHp = 0.f;
		float		fAttack = 0.f;
		float       fDefence = 0.f;
		float       fSpeed = 0.f;

	}STATUS;

	typedef struct typeHandle
	{
		OBJORIGINTYPE                           m_eOriginType = OBJORIGINTYPE::ORIGIN_STATIC;
		OBJTYPE                                 m_eType = OBJTYPE::OBJ_NONE;
		FLOOR					                m_eFloor = FLOOR::FLOOR_END;
		int                                     m_iIndex = 0;
		_int									m_sEventIndex = 0;
		_ushort                                 m_sRoomNum = 0;
		int                                     m_iGeneration =0;

	}ObjectHandle;


	struct SpriteRect
	{
		_float x;   // 시작 X
		_float y;   // 시작 Y
		_float w;   // 잘라낸 영역의 너비
		_float h;   // 잘라낸 영역의 높이
		_float Mw;  // 전체 원본 이미지 너비 (Meta Width)
		_float Mh;  // 전체 원본 이미지 높이 (Meta Height)
	public:
		SpriteRect()
			: x(0.f) , y(0.f) , w(0.f) , h(0.f) , Mw(0.f) , Mh(0.f)
		{
			
		}
		SpriteRect(_float _x, _float _y, _float _w, _float _h, _float _Mw, _float _Mh)
			: x(_x), y(_y), w(_w), h(_h), Mw(_Mw), Mh(_Mh) {
		}
	public:
		void	Clear()
		{
			x = 0; y = 0; w = 0; h = 0; Mw = 0;  Mh = 0;
		}
	};

	struct UVRect
	{
	public:
		_vec2 vUV[UV_END];
		_vec3 vScale;
	public:
		UVRect()
		{
			for (int i = 0; i < UV_END; i++) 
			{
				vUV[i].x = 0.f;
				vUV[i].y = 0.f;
			}
			for (int i = 0; i < END_SPRITE; ++i)
			{
				vScale.x = 0;
				vScale.y = 0;
				vScale.z = 0;
			}
		}
		void	SettingFromSprite(const SpriteRect& tSpriteRect)
		{
			float fx = static_cast<float>(tSpriteRect.x);
			float fy = static_cast<float>(tSpriteRect.y);
			float fw = static_cast<float>(tSpriteRect.w);
			float fh = static_cast<float>(tSpriteRect.h);
			float fMw = static_cast<float>(tSpriteRect.Mw);
			float fMh = static_cast<float>(tSpriteRect.Mh);

			// LT
			vUV[UV_LT] = _vec2(fx / fMw, fy / fMh);
			// RT
			vUV[UV_RT] = _vec2((fx + fw) / fMw, fy / fMh);
			// RB
			vUV[UV_RB] = _vec2((fx + fw) / fMw, (fy + fh) / fMh);
			// LB
			vUV[UV_LB] = _vec2(fx / fMw, (fy + fh) / fMh);

			vScale.x = (fw) / FIXEL_SIZE;
			vScale.y = (fh) / FIXEL_SIZE;
			vScale.z = 1.f;
		}
	};


	struct MAPTOOLTILE_INFO
	{
		_int					iMakeIndex;
		_int					iTargetMakeIndex;

		_bool					IsCubeOnTop;
		_bool					IsHaveCeiling;
	};


	//맵툴용
	struct MapToolActive
	{
		bool		isActiveFloorTile;
		bool		isActiveTileCeiling;
		bool		isActiveCube;
		bool		isActiveEventCube;
		bool		isActiveCreatureCantGoCube;
		bool		isActiveDecor;
		bool		isActiveObstacle;
		bool		isActiveEventObject;
		bool		isActiveItem;
		bool		isActiveMonster;
	};


	//맵툴용
	enum class ACTIVE_OBJECT
	{
		TILE_FLOOR,
		TILE_CELILING,

		CUBE,
		EVENTCUBE,
		CREATURECANTGOCUBE,

		DECOR,
		OBSTACLE,
		EVENTOBJECT,

		ITEM,
		MONSTER,

		END_ENUM
	};
}


#endif // Engine_Struct_h__
