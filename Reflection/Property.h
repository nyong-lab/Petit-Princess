#pragma once

#include "IPropertyHandler.h"
#include "PropertyHandler.h"
#include "StaticPropertyHandler.h"
#include "TypeEnumDefinitions.h"


struct PropertyInitializer;

/// <summary>
/// Property 클래스를 등록하는 클래스
/// Property 클래스에 알맞는 Handler를 골라내고 
/// 생성시 필요한 초기화 구조체를 만들고 생성시킨다.
/// 작성자 : 김동일
/// </summary>
/// <typeparam name="TClass">소유하고 있는 객체의 타입</typeparam>
/// <typeparam name="T">변수 타입</typeparam>
/// <typeparam name="TPtr">변수 포인터 타입</typeparam>
/// <typeparam name="ptr">변수 주소</typeparam>
template<typename TClass, typename T, typename TPtr, TPtr ptr, bool isShowGui = true>
class PropertyRegister
{
public:
	PropertyRegister(const char* name, TypeInfo& typeInfo)
	{
		/// TODO::동일 - 구조 개선 요소 1
		// 변수 타입에 따라서 분류한다.
		// std::vector나 map 같은경우 클래스이기 때문에 if문 순서에 주의하자. 
		// 포인터 타입에 대해서는 false를 반환하기에 포인터를 벗겨내는 작업이 필요하다.

		// int, float, bool, double 등
		if constexpr (std::is_fundamental_v<std::remove_pointer_t<T>>)
		{
			m_propertyTypeCategory = PropertyTypeCategory::Primitive;
		}
		// 사용자 정의 구조체 Vector4
		else if constexpr (IsVector4<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::Primitive;
		}
		// 사용자 정의 구조체 Vector3
		else if constexpr (IsVector3<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::Primitive;
		}
		// 사용자 정의 구조체 Vector2
		else if constexpr (IsVector2<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::Primitive;
		}

		// 사용자 정의 구조체 Quaternion
		else if constexpr (IsQuaternion<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::Primitive;
		}

		// std::string
		else if constexpr (IsString<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::String;
		}

		// std::vector
		else if constexpr (IsVector<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::Vector;
		}

		// std::map
		else if constexpr (IsMap<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::Map;
		}

		// std::unorderedMap
		else if constexpr (IsUnorderedMap<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::UnorderedMap;
		}

		// enumClass
		else if constexpr (IsEnumClass<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::EnumClass;;
		}

		// class
		else if constexpr (std::is_class_v<std::remove_pointer_t<T>>)
		{
			m_propertyTypeCategory = PropertyTypeCategory::Class;
		}



		// 클래스 내에 static 변수인지 아닌지 체크
		// (클래스 안에 있는 static 변수도 엄연히 멤버 변수라고 부를 수는 있으나 개념이 다르다)
		if constexpr (std::is_member_pointer_v<TPtr>)
		{
			// true : 멤버 변수 핸들러
			static PropertyHandler<TClass, T> handler(ptr);

			// Property 생성시 초기화 해주는 구조체
			static PropertyInitializer initializer =
			{
				.m_name = name,
				.m_type = TypeInfo::GetStaticTypeInfo<T>(),
				.m_handler = handler,
				.m_typeCategory = m_propertyTypeCategory,
				.m_ownerType = TypeInfo::GetStaticTypeInfo<TClass>(),
				.m_isShowGui = isShowGui
			};

			// Property 생성자 호출
			static Property property(typeInfo, initializer);
		}
		else
		{
			// false : 정적 변수 핸들러 (static)
			static StaticPropertyHandler<TClass, T> handler(ptr);

			// Property 생성시 초기화 해주는 구조체
			static PropertyInitializer initializer =
			{
				.m_name = name,
				.m_type = TypeInfo::GetStaticTypeInfo<T>(),
				.m_handler = handler,
				.m_typeCategory = m_propertyTypeCategory,
				.m_ownerType = TypeInfo::GetStaticTypeInfo<TClass>(),
				.m_isShowGui = isShowGui
			};

			// Property 생성자 호출
			static Property property(typeInfo, initializer);
		}
	}

	PropertyTypeCategory m_propertyTypeCategory = PropertyTypeCategory::None;
};

/// <summary>
/// Property 클래스의 초기화를 담당하는 구조체
/// </summary>
struct PropertyInitializer
{
	const char* m_name = nullptr;
	const TypeInfo& m_type;
	const PropertyHandlerBase& m_handler;
	PropertyTypeCategory m_typeCategory;
	const TypeInfo& m_ownerType;
	bool m_isShowGui;
};

/// <summary>
/// 멤버 변수에 대한 정보를 담는 클래스
/// 작성자 : 김동일
/// </summary>
class Property
{
public:
	/// <summary>
	/// 반환되는 값을 한 번 포장하는 구조체
	/// 
	/// 값을 읽거나 변경하는 동작에 대한 제어를 향상 시킬 수 있고, (특정 조건을 검사하여 예외처리를 한다거나?)
	/// 반환되는 타입에 따라서 Get 함수의 반환 타입이 변경되어야 했지만
	/// 포장함으로써 항상 일반화된 반환 값 (ReturnValueWrapper<T> 타입)이
	/// 유지되므로, 이에 대한 일반화된 코드를 작성할 수 있다. 
	/// 그렇기에 이 인터페이스를 사용하는 코드는 일관화된 방식으로 속성 값을 다룰 수 있다.
	/// </summary>
	/// <typeparam name="T">최종적으로 반환되는 멤버 변수 값의 타입</typeparam>
	template <typename T>
	struct ReturnValueWrapper
	{
	public:
		explicit ReturnValueWrapper(T& value) : m_value(&value) {}
		ReturnValueWrapper() = default;

		ReturnValueWrapper& operator=(const T& value)
		{
			*m_value = value;
			return *this;
		}

		operator T& () { return *m_value; }
		T& GetValue() { return *m_value; }
		T* GetPointer() { return m_value; }

		/*friend bool operator==(const ReturnValueWrapper& lhs, const ReturnValueWrapper& rhs)
		{
			return *lhs.m_value == *rhs.m_value;
		}

		friend bool operator!=(const ReturnValueWrapper& lhs, const ReturnValueWrapper& rhs)
		{
			return !(lhs == rhs);
		}*/

	private:
		T* m_value = nullptr;
	};

	// 생성자
	Property(TypeInfo& owner, const PropertyInitializer& initializer);

	// 값을 래핑한 구조체로 반환
	template<typename T>
	ReturnValueWrapper<T> Get(void* object) const
	{
		// handler 클래스의 타입 정보를 호출하여 IPropertyHandler<T> 타입을 상속받고 있는지 체크
		if (m_handler.GetTypeInfo().IsChildOf<IPropertyHandler<T>>())
		{
			// 다형적 동작을 하기위해서 인터페이스 클래스로 다운캐스팅
			const IPropertyHandler<T>* concreteHandler = static_cast<const IPropertyHandler<T>*>(&m_handler);
			return ReturnValueWrapper(concreteHandler->Get(object));
		}
		else
		{
			assert(false && "Property::Get<T> - 핸들러 타입 확인 (상속 관계 오류)");
			return {};
		}
	}

	// 멤버 변수를 value 값으로 설정
	template <typename T>
	void Set(void* object, const T& value) const
	{
		// handler 클래스의 타입 정보를 호출하여 IPropertyHandler<T> 타입을 상속받고 있는지 체크
		if (m_handler.GetTypeInfo().IsChildOf<IPropertyHandler<T>>())
		{
			// 다형적 동작을 하기위해서 인터페이스 클래스로 다운캐스팅
			auto concreteHandler = static_cast<const IPropertyHandler<T>*>(&m_handler);
			concreteHandler->Set(object, value);
		}
		else
		{
			assert(false && "Property::Set<T> - 핸들러 타입 확인 (상속 관계 오류)");
		}
	}

	// 변수 타입 정보를 반환
	const TypeInfo& GetTypeInfo() const { return m_type; }
	const TypeInfo& GetOwnerTypeInfo() const { return m_ownerType; }

	// 변수 이름 반환
	const char* GetName() const { return m_name; }

	bool IsShowGui() const { return m_isShowGui; }

	const PropertyTypeCategory GetPropertyTypeCategory() const { return m_propertyTypeCategory; }

private:
	const char* m_name = nullptr;
	const TypeInfo& m_type;
	const PropertyHandlerBase& m_handler;
	const PropertyTypeCategory m_propertyTypeCategory;
	const TypeInfo& m_ownerType;
	const bool m_isShowGui = true;
};
