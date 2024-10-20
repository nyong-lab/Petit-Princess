#pragma once


/// <summary>
/// Handler 클래스들의 최상단 부모 클래스
/// Property 클래스에서 멤버 변수인 m_handler를 관리하기 위한 타입으로
/// IPropertyHandler 클래스를 사용하려 했지만 템플릿 클래스였기에 
/// IPropertyHandler 에서 typename T를 추론하기 위한 방법을 잘 모르겠다.
/// 그렇기에 템플릿 클래스가 아닌 일반적인 클래스 PropertyHandlerBase 클래스를 만들었고
/// 이를 IPropertyHandler 클래스가 상속받게끔 만들어서 필요할 때 다운캐스팅을 하여
/// 다형적 동작을 하는 방법을 선택했다.
/// 작성자 : 김동일
/// </summary>
class PropertyHandlerBase
{
	GENERATE_CLASS_TYPE_INFO(PropertyHandlerBase);

public:
	virtual ~PropertyHandlerBase() = default;
};

/// <summary>
/// Handler 클래스의 인터페이스
/// 이 인터페이스를 사용하여 Property 클래스가 들고 있는
/// PropertyHandlerBase 클래스를 다운캐스팅해서 다형적 동작을 하게 만든다.
/// 작성자 : 김동일
/// </summary>
template <typename T>
class IPropertyHandler : public PropertyHandlerBase
{
	GENERATE_CLASS_TYPE_INFO(IPropertyHandler)

public:
	virtual ~IPropertyHandler() = default;

	// 인스턴스화된 객체의 포인터를 받아서
	// 해당 객체의 멤버 변수에 접근해서 값을 가져온다.
	virtual T& Get(void* object) const abstract;

	// 인스턴스화된 객체의 포인터를 받아서
	// 해당 객체의 멤버 변수에 접근하여 원하는 값으로 변경한다.
	virtual void Set(void* object, const T& value) const abstract;
};