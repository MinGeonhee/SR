#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

// 다이렉트 11에서는 상수 버퍼라는 개념이 있지만
// 다이렉트 9에서는 상수 버퍼 개념 X // 
// DX9에서 직접 float 배열을 쎼이더 상수 레지스터에 보내는 방식으로 대체함.

template<typename T>
class ENGINE_DLL ShaderConstantBuffer
{
public:

	ShaderConstantBuffer()
		: _registerIndex(0)
	{
		ZeroMemory(&_data, sizeof(T));
	}

	~ShaderConstantBuffer() {}

	// 상수 설정 (기본 레지스터 인덱스 설정 가능)
	void SetRegister(UINT regIndex) { _registerIndex = regIndex; }

	// 데이터를 복사해두기
	void CopyData(const T& data)
	{
		_data = data;
	}

	// 실제로 GPU에 보내는 동작
	void UploadToShader(LPDIRECT3DDEVICE9 device, bool isVertexShader = true)
	{
		if (isVertexShader)
		{
			device->SetVertexShaderConstantF(_registerIndex, reinterpret_cast<const float*>(&_data), sizeof(T) / sizeof(float));
		}
		else
		{
			device->SetPixelShaderConstantF(_registerIndex, reinterpret_cast<const float*>(&_data), sizeof(T) / sizeof(float));
		}
	}

private:
	T _data;
	UINT _registerIndex; // c0, c1, c2... 등 레지스터 번호
};

END


// 셰이더 버퍼 사용하는 방법

// 셰이더 구조를 HLSL로 매핑
// HLSL 을 사용할 수 있어야 함.