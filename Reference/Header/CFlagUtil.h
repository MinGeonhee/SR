#pragma once

#include "Engine_Define.h"
#include <type_traits>
#include <cstdint>


BEGIN(Engine)
// 타입 트레잇은 단순 꼬리표 이라고 보면된다 어떤 타입 T <어떤 타입이든간에> 꼬리표를 달 수 있게 해주어 해당 타입에 해당하는 꼬리표를 생성해준다.
// constexpr ->


// 이 구조체를 사용하면 해당 타입에 꼬리표가 붙는데 그 꼬리표는 기본적으로 false를 달고태어난다
// 단순한게 생각해서 struct 는 단순 껍데기이고 어느 한 변수형태의 E에대해서 True 값과 false값을 달아주는 꼬리표 구조체라고보면된다.
// enable_bitmask_operators 이라는 이름의 꼬리표를 만든것

template <class E>
struct enable_bitmask_operators : std::false_type {};

//이 매크로를 통해 특정 타입 E에대해서 꼬리표의 값을 true 로 바꿔준다. 
#define ENABLE_BITMASK_OPERATORS(E) \
	template<>	struct enable_bitmask_operators<E> : std::true_type {}

//enable_bitmask_operators<E>가 true 타입만 이 연산자 템플릿을 사용가능하게 막아준다.
//std::enable_if_t<enable_bitmask_operators<E>::value ,int > = 0  조건이 true 면 int 가 되고 , false 면 치환 실패 발생
template<typename E , std::enable_if_t<enable_bitmask_operators<E>::value ,int > = 0>
constexpr E operator|(E a, E b) {
	using U = std::underlying_type_t(E);  //내가 enum타입에 지정해준 int 형이라던지 u_int32 뭐 이런 타입을 using U로 받아와서
	return static_cast<E>(static_cast<U>(a) | static_cast<U>(b)); //밑에서 그 값으로 형변환 이후 비트연산 해주기
}

template<typename E , std::enable_if_t<enable_bitmask_operators<E>::value ,int > = 0>
constexpr E operator&(E a, E b)
{
	using U = std::underlying_type_t(E);
	return static_cast<E>(static_cast<U>(a) & static_cast<U>(b));
}

template<typename E, std::enable_if_t<enable_bitmask_operators<E>::value, int > = 0>
constexpr E operator^(E a, E b)
{
	using U = std::underlying_type_t(E);
	return static_cast<E>(static_cast<U>(a) ^ static_cast<U>(b));
}


template<typename E, std::enable_if_t<enable_bitmask_operators<E>::value, int > = 0>
constexpr E operator<<(E a, E b)
{
	using U = std::underlying_type_t(E);
	return static_cast<E>(static_cast<U>(a) << static_cast<U>(b));
}

template<typename E, std::enable_if_t<enable_bitmask_operators<E>::value, int > = 0>
constexpr E operator>>(E a, E b)
{
	using U = std::underlying_type_t(E);
	return static_cast<E>(static_cast<U>(a) >> static_cast<U>(b));
}




END
