#pragma once

#include "IPropertyHandler.h"

/// <summary>
/// 멤버 변수를 관리하는 Handler 클래스
/// 변수에 직접적으로 영향을 주거나 가져온다.
/// 작성자 : 김동일
/// </summary>
/// <typeparam name="TClass">소유하고 있는 객체의 타입</typeparam>
/// <typeparam name="T">변수 타입</typeparam>
template<typename TClass, typename T>
class PropertyHandler : public IPropertyHandler<T>
{
	GENERATE_CLASS_TYPE_INFO(PropertyHandler)

	// TClass 내의 T 타입 멤버 변수 포인터
	// 템플릿 인자가 컴파일 타임에 결정되어야 하기 때문에
	// 멤버 변수 포인터 타입으로 주소를 저장한다.
	using MemberPtr = T TClass::*;

public:
	// 생성자 소멸자
	PropertyHandler() {}; // m_defaultConstructor를 초기화 시키기 위해서 넣어줌.
	explicit PropertyHandler(MemberPtr ptr) : m_ptr(ptr) {}
	virtual ~PropertyHandler() = default;

	// 인스턴스화된 객체의 포인터를 받아서
	// 해당 객체의 멤버 변수에 접근해서 값을 가져온다.
	virtual T& Get(void* object) const override
	{
		return static_cast <TClass*>(object)->*m_ptr;
	}

	// 인스턴스화된 객체의 포인터를 받아서
	// 해당 객체의 멤버 변수에 접근하여 원하는 값으로 변경한다.
	virtual void Set(void* object, const T& value) const override
	{
		static_cast<TClass*>(object)->*m_ptr = value;
	}

private:
	MemberPtr m_ptr = nullptr;
};