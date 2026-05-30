#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#define	WINCX		1280
#define	WINCY		720


#define FILEPATH	"../../Client/Bin/Resource/MapData/"


#define	VTXCNTX		129
#define	VTXCNTZ		129

	// 파티션 갯수
#define PARTX			500
#define PARTZ			500

	// 파티션 크기
#define	PARTXSIZE		2
#define	PARTZSIZE		2


#define VTXITV		1


#define TILE_UV_CENTER  0.015625f
#define TILE_UV_SIZE    0.03125f 
#define TILE_CNTX		32
#define TILE_CNTY		32


#define DEFUALT_TILE_UV_CENTERX  0.015625f
#define DEFUALT_TILE_UV_CENTERY  0.078125f

#define DEFUALT_CUBE_UV_CENTERX 0.515625f
#define DEFUALT_CUBE_UV_CENTERY 0.015625f
	//투명값 위치
#define DEFUALT_NAGATIVEY_CENTER_X 0.265625f
#define DEFUALT_NAGATIVEY_CENTER_Y 0.953125f

#define DEFUALT_NAGATIVEY_CENTER_BOSS_X 0.203125f
#define DEFUALT_NAGATIVEY_CENTER_BOSS_Y 0.671875f



#define TRANSPARENT_TILE_UV_X				   0.984375f
#define TRANSPARENT_TILE_UV_Y				   0.484375f

#define MARGIN				0.003

#define	FIRST_SPRITE	0
#define LAST_SPRITE		1
#define	END_SPRITE		2

#define BUFFER_LT 0
#define BUFFER_RT 1
#define BUFFER_RB 2
#define BUFFER_LB 3

#define UV_LT 0
#define UV_RT 1
#define UV_RB 2
#define UV_LB 3
#define UV_END 4


#define FIXEL_SIZE		64.f

#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;

#define			NULLPTR_RETURN(pInstance)	if( !pInstance ) return;

#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif

#define NO_COPY(CLASSNAME)								\
		private:										\
		CLASSNAME(const CLASSNAME&) = delete;			\
		CLASSNAME& operator = (const CLASSNAME&)= delete;		

#define DECLARE_SINGLETON(CLASSNAME)					\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(NULL == m_pInstance) {					\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		void CLASSNAME::DestroyInstance( void ) {		\
			if(NULL != m_pInstance)	{					\
				delete m_pInstance;						\
				m_pInstance = NULL;						\
			}											\
		}

	// 디바이스 가져오는 매크로.
#define GET_SINGLE(classname)	classname::GetInstance()

#define GRAPHICS	GET_SINGLE(CGraphicDev)
#define DEVICE		GRAPHICS->Get_GraphicDev()

#define REGISTER_PROTOTYPE(_enumkey, CLASSNAME, graphicDev)					\
		if(FAILED(CProtoTypeManager::GetInstance()->						\
		Ready_Prototype(_enumkey, Engine::CLASSNAME::Create(graphicDev))))	\
		return E_FAIL;														\

#define CLONE_PROTOTYPE(_enumkey, CLASSNAME)						\
		static_cast<CLASSNAME*>(CProtoTypeManager::GetInstance()->  \
		Clone_Prototype(_enumkey));									\


}

#endif // Engine_Macro_h__
