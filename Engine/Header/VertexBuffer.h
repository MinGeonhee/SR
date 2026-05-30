#pragma once
#include "Engine_Define.h"
#include "CGraphicDev.h"

BEGIN(Engine)

class ENGINE_DLL VertexBuffer
{
private:
	LPDIRECT3DVERTEXBUFFER9* _vertexBuffer;

	_uint32					 _stride;
	_uint32					 _offset;
	_uint32					 _count;

public:
	VertexBuffer()
		:_vertexBuffer(nullptr), _stride(0), _offset(0), _count(0)
	{
	}

	~VertexBuffer()
	{
		if (_vertexBuffer)
		{
			(*_vertexBuffer)->Release();
			delete _vertexBuffer;
			_vertexBuffer = nullptr;
		}
	}

	LPDIRECT3DVERTEXBUFFER9* GetComPtr() { return _vertexBuffer; }
	_uint32					 GetStride() const { return _stride; }
	_uint32				     GetOffset() const { return _offset; }
	_uint32					 GetCount() const { return _count; }

	template<typename T>
	void Create(const vector<T>& vertices)
	{
		_stride = sizeof(T);
		_count = static_cast<_uint32>(vertices.size());
		_offset = 0;

		LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;

		HRESULT hr = DEVICE->CreateVertexBuffer(
			_stride * _count,
			0,
			0,
			D3DPOOL_MANAGED,
			&pVB,
			nullptr);

		if (FAILED(hr))
		{
			MSG_BOX("VertexBuffer 생성 실패 !");
			return;
		}

		void* pData = nullptr;
		hr = pVB->Lock(0, 0, &pData, 0);
		if (SUCCEEDED(hr))
		{
			memcpy(pData, vertices.data(), _stride * _count);
			pVB->Unlock();
		}

		else
		{
			MSG_BOX("VertexBuffer 잠금 실패 !");
			pVB->Release();
			return;
		}

		_vertexBuffer = new LPDIRECT3DVERTEXBUFFER9(pVB);
	}


};

END