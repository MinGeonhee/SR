#include "IndexBuffer.h"
#include "CGraphicDev.h"

IndexBuffer::IndexBuffer()
	:_Indexbuffer(nullptr)
{

}

IndexBuffer::~IndexBuffer()
{
	if (_Indexbuffer)
	{
		(*_Indexbuffer)->Release();
		delete _Indexbuffer;
		_Indexbuffer = nullptr;
	}
}

void IndexBuffer::Create(const vector<_uint>& indices)
{
	_stride = sizeof(_uint32);
	_count = static_cast<_uint32>(indices.size());
	_offset = 0;

	// 인덱스 버퍼 생성

	LPDIRECT3DINDEXBUFFER9 pIndexBuffer = nullptr;

	HRESULT hr = DEVICE->
		CreateIndexBuffer(
		_stride * _count,
		0,
		D3DFMT_INDEX32,			// 32비트 인덱스 사용
		D3DPOOL_MANAGED,
		&pIndexBuffer,
		nullptr
	);

	if (FAILED(hr))
	{
		MSG_BOX("인덱스 버퍼 생성 실패");
		return;
	}

	// 데이터 복사 루키스님 충성충성

	void* pData = nullptr;
	hr = pIndexBuffer->Lock(0, 0, &pData, 0);

	if (SUCCEEDED(hr))
	{
		memcpy(pData, indices.data(), _stride * _count);
		pIndexBuffer->Unlock();
	}
	
	else
	{
		MSG_BOX("인덱스 버퍼 잠금 실패");
		pIndexBuffer->Release();
		return;
	}

	_Indexbuffer = new LPDIRECT3DINDEXBUFFER9(pIndexBuffer);
}