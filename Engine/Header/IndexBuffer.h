#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	LPDIRECT3DINDEXBUFFER9* Get_IndexcomPtr() { return _Indexbuffer; }
	_uint32 GetStride() { return _stride; }
	_uint32 GetOffset() { return _offset; }
	_uint32 GetCount() { return _count; }

	void Create(const vector<_uint32>& indices);

private:
	LPDIRECT3DINDEXBUFFER9* _Indexbuffer;

	_uint32 _stride = 0;
	_uint32 _offset = 0;
	_uint32 _count = 0;
};


END
