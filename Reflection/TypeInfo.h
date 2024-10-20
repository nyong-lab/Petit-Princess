#pragma once


#include "TypeInfoManager.h"
#include "TypeEnumDefinitions.h"

class Property;
class Component;
class Collider;
class CollisionManager;

/// <summary>
/// TypeInfo 초기화용으로 사용되는 구조체
/// 부모가 있다면 부모 TypeInfo를 가져오고, 부모가 없다면 nullptr
/// </summary>
template <typename T>
struct TypeInfoInitializer
{
	TypeInfoInitializer(const char* name)
		: m_name(name)
	{
		if (m_name == "FoodTableScript")
		{
			int a = 0;
		}

		// 부모 타입이 있는지 체크
		if constexpr (HasSuper<T>)
		{
			// 있다면 부모 타입 정보 객체를 저장
			m_super = &(typename T::Super::StaticTypeInfo());

			/// TODO::동일 - 구조 개선 요소 2 
			/// 괴물 코드
			std::string superName = m_super->GetName();
			if ((superName == "Component" ||
				superName == "Renderer" ||
				superName == "Collider" || 
				superName == "Script" ||
				superName == "FSM") &&
				(m_name != "Renderer" && 
					m_name != "Collider" &&
					m_name != "Component" &&
					m_name != "Script" && 
					m_name != "FSM"))
			{
				m_tag = TypeTag::Component;

				/// TODO::동일 - 구조 개선 요소 3
				// 추상클래스인지 체크해서 아니라면 기본 생성자를 만드는 함수포인터를 만든다.
				// 컴포넌트 추가할 때 사용할 함수이지만 propertyHandler 라던가 기본생성자가 없는 클래스에 대해서도 만들어진다거나 하는 불필요한 작동을 하기에 개선해야한다.
				// (추상클래스가 아니면서 기본생성자가 없다면 이 구문은 컴파일 에러를 일으키기에 기본 생성자를 만들어줘야한다.)
				if constexpr (!std::is_abstract_v<T>)
				{
					m_componentDefaultConstructor = &TypeInfoInitializer::ComponentDefaultConstructor<T>;
				}
			}
		}
	}

	template <typename U>
	static std::shared_ptr<Component> ComponentDefaultConstructor()
	{
		if constexpr (std::is_base_of<Component, U>::value)
		{
			std::shared_ptr<Component> component = std::make_shared<U>();
			return component;
		}
		else
		{
			return nullptr;
		}
	}

	const char* m_name;
	TypeTag m_tag = TypeTag::Default;
	std::shared_ptr<Component>(*m_componentDefaultConstructor)() = nullptr;
	const TypeInfo* m_super = nullptr;
};

/// <summary>
/// 실제 타입 정보를 담는 TypeInfo 클래스
/// 부모의 TypeInfo에 대한 주소를 가지고 있는 부분이 중요하다.
/// 이를 통해서 TypeInfo는 타입의 상속 관계를 표현. (다중 상속 x)
/// 작성자 : 김동일
/// </summary>
class TypeInfo
{
public:
	// 생성자
	template <typename T>
	explicit TypeInfo(const TypeInfoInitializer<T>& initializer);

	// 프렌드 선언
	friend class Property;

	// 멤버 변수에 대한 정보를 가지고 있는 Property 클래스를 추가
	void AddProperty(Property* property);

	// m_properties 반환
	const std::vector<const Property*> GetProperties() const { return m_properties; }

	// 키값을 활용하여 해당 멤버 변수를 반환
	const Property* GetProperty(const char* name) const;

	// 부모 타입정보 객체를 반환
	const TypeInfo* GetSuper() const { return m_super; }

	// 클래스 또는 변수 이름 반환
	const char* GetName() const { return m_name; }

	// 타입까지 붙어있는 전체 이름 반환
	std::string GetFullName() const { return m_fullName; }

	// 나 자신과 매개변수 TypeInfo가 동일한 타입인지 검사
	bool IsSameTypeInfo(const TypeInfo& other) const;

	// 나 자신이 매개변수 TypeInfo을 상속받는 타입인지 검사
	bool IsChildOf(const TypeInfo& other) const;

	// 태그 반환
	TypeTag GetTypeTag() const { return m_tag; }

	// 카테고리 반환
	PropertyTypeCategory GetCategory() const { return m_typeCategory; }

	std::shared_ptr<Component> InvokdDefaultConstructor() const { return m_defaultConstructor(); }

	template <typename T>
	bool IsSameTypeInfo() const { return IsSameTypeInfo(GetStaticTypeInfo<T>()); }

	template <typename T>
	bool IsChildOf() const { return IsChildOf(GetStaticTypeInfo<T>()); }

	// T타입이 정적 타입 정보 객체를 가지고 있다면 반환 
	template <typename T> requires HasStaticTypeInfo<T>
	static const TypeInfo& GetStaticTypeInfo()
	{
		return T::StaticTypeInfo();
	}

	// T가 포인터 타입이고 && 포인터를 벗긴 T가 정적 타입 정보 객체를 가지고 있다면 반환
	template <typename T> requires std::is_pointer_v<T>&& HasStaticTypeInfo<std::remove_pointer_t<T>>
	static const TypeInfo& GetStaticTypeInfo()
	{
		return std::remove_pointer_t<T>::StaticTypeInfo();
	}

	// T가 타입 정보 객체가 없다면 UnReflectedType 객체 생성 후 반환
	template <typename T> requires (!HasStaticTypeInfo<T>) && (!HasStaticTypeInfo<std::remove_pointer_t<T>>)
		static const TypeInfo& GetStaticTypeInfo(PropertyTypeCategory typeCategory = PropertyTypeCategory::None)
	{
		static TypeInfo typeInfo{ TypeInfoInitializer<T>("unreflected_type_variable") };
		typeInfo.m_typeCategory = typeCategory;
		return typeInfo;
	}

private:
	size_t m_typeHash;
	const char* m_name;
	std::string m_fullName;
	const TypeInfo* m_super;
	TypeTag m_tag;

	// 기본생성자를 호출시켜주는 람다 함수를 저장한 함수 포인터
	std::shared_ptr<Component>(*m_defaultConstructor)() = nullptr;

	// 타입 객체를 가진 클래스가 누군가의 변수가 아니라 클래스 라는 뜻  
	PropertyTypeCategory m_typeCategory = PropertyTypeCategory::Owner;

	std::vector<const Property*> m_properties;

	using PropertyMap = std::unordered_map<std::string_view, const Property*>;
	PropertyMap m_propertyMap;
};

template <typename T>
TypeInfo::TypeInfo(const TypeInfoInitializer<T>& initializer)
	: m_typeHash(typeid(T).hash_code())
	, m_name(initializer.m_name)
	, m_fullName(typeid(T).name())
	, m_super(initializer.m_super)
	, m_tag(initializer.m_tag)
	, m_defaultConstructor(initializer.m_componentDefaultConstructor)
{
	TypeInfoManager::GetInstance()->AddTypeInformation(typeid(T).name(), { std::type_index(typeid(T)), this });
}
