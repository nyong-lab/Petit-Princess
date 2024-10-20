#pragma once

#include "IPropertyHandler.h"
#include "PropertyHandler.h"
#include "StaticPropertyHandler.h"
#include "TypeEnumDefinitions.h"


struct PropertyInitializer;

/// <summary>
/// Property Ŭ������ ����ϴ� Ŭ����
/// Property Ŭ������ �˸´� Handler�� ��󳻰� 
/// ������ �ʿ��� �ʱ�ȭ ����ü�� ����� ������Ų��.
/// �ۼ��� : �赿��
/// </summary>
/// <typeparam name="TClass">�����ϰ� �ִ� ��ü�� Ÿ��</typeparam>
/// <typeparam name="T">���� Ÿ��</typeparam>
/// <typeparam name="TPtr">���� ������ Ÿ��</typeparam>
/// <typeparam name="ptr">���� �ּ�</typeparam>
template<typename TClass, typename T, typename TPtr, TPtr ptr, bool isShowGui = true>
class PropertyRegister
{
public:
	PropertyRegister(const char* name, TypeInfo& typeInfo)
	{
		/// TODO::���� - ���� ���� ��� 1
		// ���� Ÿ�Կ� ���� �з��Ѵ�.
		// std::vector�� map ������� Ŭ�����̱� ������ if�� ������ ��������. 
		// ������ Ÿ�Կ� ���ؼ��� false�� ��ȯ�ϱ⿡ �����͸� ���ܳ��� �۾��� �ʿ��ϴ�.

		// int, float, bool, double ��
		if constexpr (std::is_fundamental_v<std::remove_pointer_t<T>>)
		{
			m_propertyTypeCategory = PropertyTypeCategory::Primitive;
		}
		// ����� ���� ����ü Vector4
		else if constexpr (IsVector4<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::Primitive;
		}
		// ����� ���� ����ü Vector3
		else if constexpr (IsVector3<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::Primitive;
		}
		// ����� ���� ����ü Vector2
		else if constexpr (IsVector2<std::remove_pointer_t<T>>())
		{
			m_propertyTypeCategory = PropertyTypeCategory::Primitive;
		}

		// ����� ���� ����ü Quaternion
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



		// Ŭ���� ���� static �������� �ƴ��� üũ
		// (Ŭ���� �ȿ� �ִ� static ������ ������ ��� ������� �θ� ���� ������ ������ �ٸ���)
		if constexpr (std::is_member_pointer_v<TPtr>)
		{
			// true : ��� ���� �ڵ鷯
			static PropertyHandler<TClass, T> handler(ptr);

			// Property ������ �ʱ�ȭ ���ִ� ����ü
			static PropertyInitializer initializer =
			{
				.m_name = name,
				.m_type = TypeInfo::GetStaticTypeInfo<T>(),
				.m_handler = handler,
				.m_typeCategory = m_propertyTypeCategory,
				.m_ownerType = TypeInfo::GetStaticTypeInfo<TClass>(),
				.m_isShowGui = isShowGui
			};

			// Property ������ ȣ��
			static Property property(typeInfo, initializer);
		}
		else
		{
			// false : ���� ���� �ڵ鷯 (static)
			static StaticPropertyHandler<TClass, T> handler(ptr);

			// Property ������ �ʱ�ȭ ���ִ� ����ü
			static PropertyInitializer initializer =
			{
				.m_name = name,
				.m_type = TypeInfo::GetStaticTypeInfo<T>(),
				.m_handler = handler,
				.m_typeCategory = m_propertyTypeCategory,
				.m_ownerType = TypeInfo::GetStaticTypeInfo<TClass>(),
				.m_isShowGui = isShowGui
			};

			// Property ������ ȣ��
			static Property property(typeInfo, initializer);
		}
	}

	PropertyTypeCategory m_propertyTypeCategory = PropertyTypeCategory::None;
};

/// <summary>
/// Property Ŭ������ �ʱ�ȭ�� ����ϴ� ����ü
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
/// ��� ������ ���� ������ ��� Ŭ����
/// �ۼ��� : �赿��
/// </summary>
class Property
{
public:
	/// <summary>
	/// ��ȯ�Ǵ� ���� �� �� �����ϴ� ����ü
	/// 
	/// ���� �аų� �����ϴ� ���ۿ� ���� ��� ��� ��ų �� �ְ�, (Ư�� ������ �˻��Ͽ� ����ó���� �Ѵٰų�?)
	/// ��ȯ�Ǵ� Ÿ�Կ� ���� Get �Լ��� ��ȯ Ÿ���� ����Ǿ�� ������
	/// ���������ν� �׻� �Ϲ�ȭ�� ��ȯ �� (ReturnValueWrapper<T> Ÿ��)��
	/// �����ǹǷ�, �̿� ���� �Ϲ�ȭ�� �ڵ带 �ۼ��� �� �ִ�. 
	/// �׷��⿡ �� �������̽��� ����ϴ� �ڵ�� �ϰ�ȭ�� ������� �Ӽ� ���� �ٷ� �� �ִ�.
	/// </summary>
	/// <typeparam name="T">���������� ��ȯ�Ǵ� ��� ���� ���� Ÿ��</typeparam>
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

	// ������
	Property(TypeInfo& owner, const PropertyInitializer& initializer);

	// ���� ������ ����ü�� ��ȯ
	template<typename T>
	ReturnValueWrapper<T> Get(void* object) const
	{
		// handler Ŭ������ Ÿ�� ������ ȣ���Ͽ� IPropertyHandler<T> Ÿ���� ��ӹް� �ִ��� üũ
		if (m_handler.GetTypeInfo().IsChildOf<IPropertyHandler<T>>())
		{
			// ������ ������ �ϱ����ؼ� �������̽� Ŭ������ �ٿ�ĳ����
			const IPropertyHandler<T>* concreteHandler = static_cast<const IPropertyHandler<T>*>(&m_handler);
			return ReturnValueWrapper(concreteHandler->Get(object));
		}
		else
		{
			assert(false && "Property::Get<T> - �ڵ鷯 Ÿ�� Ȯ�� (��� ���� ����)");
			return {};
		}
	}

	// ��� ������ value ������ ����
	template <typename T>
	void Set(void* object, const T& value) const
	{
		// handler Ŭ������ Ÿ�� ������ ȣ���Ͽ� IPropertyHandler<T> Ÿ���� ��ӹް� �ִ��� üũ
		if (m_handler.GetTypeInfo().IsChildOf<IPropertyHandler<T>>())
		{
			// ������ ������ �ϱ����ؼ� �������̽� Ŭ������ �ٿ�ĳ����
			auto concreteHandler = static_cast<const IPropertyHandler<T>*>(&m_handler);
			concreteHandler->Set(object, value);
		}
		else
		{
			assert(false && "Property::Set<T> - �ڵ鷯 Ÿ�� Ȯ�� (��� ���� ����)");
		}
	}

	// ���� Ÿ�� ������ ��ȯ
	const TypeInfo& GetTypeInfo() const { return m_type; }
	const TypeInfo& GetOwnerTypeInfo() const { return m_ownerType; }

	// ���� �̸� ��ȯ
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
