#pragma once

#include "IPropertyHandler.h"

/// <summary>
/// 정적으로 생성된 멤버 변수를 관리하는 클래스
/// 변수에 직접적으로 영향을 주거나 가져온다.
/// 작성자 : 김동일
/// </summary>
/// <typeparam name="TClass">소유하고 있는 객체의 타입</typeparam>
/// <typeparam name="T">변수 타입</typeparam>
template<typename TClass, typename T>
class StaticPropertyHandler : public IPropertyHandler<T>
{
	GENERATE_CLASS_TYPE_INFO(StaticPropertyHandler)

public:
	// 생성자 소멸자
	StaticPropertyHandler() {}; //  m_defaultConstructor를 초기화 시키기 위해서 넣어줌.
;	explicit StaticPropertyHandler(T* ptr) : m_ptr(ptr) {}
	virtual ~StaticPropertyHandler() = default;

	// 인스턴스화된 객체의 포인터를 받아서
	// 해당 객체의 멤버 변수에 접근해서 값을 가져온다.
	virtual T& Get(void* object) const override
	{
		return *m_ptr;
	}

	// 인스턴스화된 객체의 포인터를 받아서
	// 해당 객체의 멤버 변수에 접근하여 원하는 값으로 변경한다.
	virtual void Set(void* object, const T& value) const override
	{
		*m_ptr = value;
	}

private:
	// 정적인 멤버 변수의 주소를 들고있는 변수
	T* m_ptr = nullptr;
};

