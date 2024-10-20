#pragma once


#include "Property.h"

/// 멤버 변수 정보를 생성하는 매크로
///
/// 변수 정보는 각 변수마다 정의된 등록 구조체의 생성자에 선언된 
/// PropertyRegister 객체를 통해 생성된다.
/// 템플릿 클래스는 PropertyRegister는 아래와 같은 정보를 담는다.
/// 
/// 1. ThisType : 소유하고 있는 객체의 타입
/// 2. decltype(Name) : 변수의 타입
/// 3. decltype(&ThisType::Name) : 변수 포인터 타입
/// 4. &ThisType::Name : 변수 포인터
/// 
/// 3번과 4번같은 경우 클래스::Name(멤버 변수 포인터 타입) 으로 저장하는데
/// 템플릿 인자가 컴파일 타임에 결정되어야 하기 때문에 
/// 정적으로 해석되는 상수 표현식으로 멤버 변수 포인터 타입으로 사용한다.
/// 
/// 만약 &Name(포인터 타입) 으로 하게 된다면 
/// 런타임 시점에 결정되는 주소를 사용하게 되고 오류가 난다.
/// 
/// 중요한 점은 멤버 변수 포인터로 Property를 생성하고 있기에
/// 추후에 멤버 변수를 조작하는 클래스인 PropertyHandler 클래스에서 
/// 멤버 변수의 주소를 담는 타입도 마찬가지로 멤버 변수 포인터 타입이어야 한다.
/// 
 #define PROPERTY(Name, ...) \
inline static struct RegisterPropertyExecutor_##Name \
	{ \
		RegisterPropertyExecutor_##Name() \
		{ \
			static PropertyRegister \
			< \
				ThisType \
				, decltype(Name) \
				, decltype(&ThisType::##Name) \
				, &ThisType::##Name \
				, ##__VA_ARGS__ \
			> property_register_##Name \
			{ #Name, ThisType::StaticTypeInfo()}; \
		} \
\
	} register_##Name; \

//#define PROPERTY(Name) \
//inline static struct RegisterPropertyExecutor_##Name \
//	{ \
//		RegisterPropertyExecutor_##Name() \
//		{ \
//			static PropertyRegister<ThisType, decltype(m_##Name), \
//			decltype(&ThisType::m_##Name), &ThisType::m_##Name> property_register_ ## Name \
//			{ "m_"#Name, ThisType::StaticTypeInfo()}; \
//		} \
//\
//	} register_##Name; \

//#define PROPERTY(Name) \
//inline static struct RegisterPropertyExecutor_##Name \
//{ \
//    RegisterPropertyExecutor_##Name() \
//    { \
//        static PropertyRegister<ThisType, decltype(&ThisType::m_##Name), \
//            decltype(&ThisType::m_##Name), &ThisType::m_##Name> property_register_##Name \
//            { #Name, ThisType::StaticTypeInfo()}; \
//    } \
//\
//} register_##Name;