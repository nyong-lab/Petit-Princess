#pragma once


#include "TypeInfoManager.h"
#include "TypeEnumDefinitions.h"

class Property;
class Component;
class Collider;
class CollisionManager;

/// <summary>
/// TypeInfo �ʱ�ȭ������ ���Ǵ� ����ü
/// �θ� �ִٸ� �θ� TypeInfo�� ��������, �θ� ���ٸ� nullptr
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

		// �θ� Ÿ���� �ִ��� üũ
		if constexpr (HasSuper<T>)
		{
			// �ִٸ� �θ� Ÿ�� ���� ��ü�� ����
			m_super = &(typename T::Super::StaticTypeInfo());

			/// TODO::���� - ���� ���� ��� 2 
			/// ���� �ڵ�
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

				/// TODO::���� - ���� ���� ��� 3
				// �߻�Ŭ�������� üũ�ؼ� �ƴ϶�� �⺻ �����ڸ� ����� �Լ������͸� �����.
				// ������Ʈ �߰��� �� ����� �Լ������� propertyHandler ����� �⺻�����ڰ� ���� Ŭ������ ���ؼ��� ��������ٰų� �ϴ� ���ʿ��� �۵��� �ϱ⿡ �����ؾ��Ѵ�.
				// (�߻�Ŭ������ �ƴϸ鼭 �⺻�����ڰ� ���ٸ� �� ������ ������ ������ ����Ű�⿡ �⺻ �����ڸ� ���������Ѵ�.)
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
/// ���� Ÿ�� ������ ��� TypeInfo Ŭ����
/// �θ��� TypeInfo�� ���� �ּҸ� ������ �ִ� �κ��� �߿��ϴ�.
/// �̸� ���ؼ� TypeInfo�� Ÿ���� ��� ���踦 ǥ��. (���� ��� x)
/// �ۼ��� : �赿��
/// </summary>
class TypeInfo
{
public:
	// ������
	template <typename T>
	explicit TypeInfo(const TypeInfoInitializer<T>& initializer);

	// ������ ����
	friend class Property;

	// ��� ������ ���� ������ ������ �ִ� Property Ŭ������ �߰�
	void AddProperty(Property* property);

	// m_properties ��ȯ
	const std::vector<const Property*> GetProperties() const { return m_properties; }

	// Ű���� Ȱ���Ͽ� �ش� ��� ������ ��ȯ
	const Property* GetProperty(const char* name) const;

	// �θ� Ÿ������ ��ü�� ��ȯ
	const TypeInfo* GetSuper() const { return m_super; }

	// Ŭ���� �Ǵ� ���� �̸� ��ȯ
	const char* GetName() const { return m_name; }

	// Ÿ�Ա��� �پ��ִ� ��ü �̸� ��ȯ
	std::string GetFullName() const { return m_fullName; }

	// �� �ڽŰ� �Ű����� TypeInfo�� ������ Ÿ������ �˻�
	bool IsSameTypeInfo(const TypeInfo& other) const;

	// �� �ڽ��� �Ű����� TypeInfo�� ��ӹ޴� Ÿ������ �˻�
	bool IsChildOf(const TypeInfo& other) const;

	// �±� ��ȯ
	TypeTag GetTypeTag() const { return m_tag; }

	// ī�װ� ��ȯ
	PropertyTypeCategory GetCategory() const { return m_typeCategory; }

	std::shared_ptr<Component> InvokdDefaultConstructor() const { return m_defaultConstructor(); }

	template <typename T>
	bool IsSameTypeInfo() const { return IsSameTypeInfo(GetStaticTypeInfo<T>()); }

	template <typename T>
	bool IsChildOf() const { return IsChildOf(GetStaticTypeInfo<T>()); }

	// TŸ���� ���� Ÿ�� ���� ��ü�� ������ �ִٸ� ��ȯ 
	template <typename T> requires HasStaticTypeInfo<T>
	static const TypeInfo& GetStaticTypeInfo()
	{
		return T::StaticTypeInfo();
	}

	// T�� ������ Ÿ���̰� && �����͸� ���� T�� ���� Ÿ�� ���� ��ü�� ������ �ִٸ� ��ȯ
	template <typename T> requires std::is_pointer_v<T>&& HasStaticTypeInfo<std::remove_pointer_t<T>>
	static const TypeInfo& GetStaticTypeInfo()
	{
		return std::remove_pointer_t<T>::StaticTypeInfo();
	}

	// T�� Ÿ�� ���� ��ü�� ���ٸ� UnReflectedType ��ü ���� �� ��ȯ
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

	// �⺻�����ڸ� ȣ������ִ� ���� �Լ��� ������ �Լ� ������
	std::shared_ptr<Component>(*m_defaultConstructor)() = nullptr;

	// Ÿ�� ��ü�� ���� Ŭ������ �������� ������ �ƴ϶� Ŭ���� ��� ��  
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
