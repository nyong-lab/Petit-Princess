#include "Reflection_pch.h"
#include "TypeInfo.h"


/// <summary>
/// ��� ������ ���� ������ ������ �ִ� Property Ŭ���� �߰�
/// </summary>
void TypeInfo::AddProperty(Property* property)
{
	m_properties.emplace_back(property);
	m_propertyMap.emplace(property->GetName(), property);
}

/// <summary>
/// Ű ���� name�� �Ű������� �޾Ƽ� propertyMap�� ����Ǿ� �ִ��� ã��
/// ������ �ش� property�� ��ȯ�ϰ� ������ nullptr ��ȯ
/// </summary>
const Property* TypeInfo::GetProperty(const char* name) const
{
	auto iter = m_propertyMap.find(name);
	return (iter != std::end(m_propertyMap)) ? iter->second : nullptr;
}

/// <summary>
/// ���� TypeInfo�� �Ű������� ���� TypeInfo�� ������ Ÿ������ �˻�
/// TypeInfo�� static �����̱� ������ �ּҰ˻�� Ȯ���� ����������
/// �ٸ� dll�� ���� Ÿ���� ��� ó���� �ȵǱ� ������ typeid�� ���� 
/// �ؽ� ������ �ѹ� �� �˻��Ѵ�.
/// </summary>
bool TypeInfo::IsSameTypeInfo(const TypeInfo& other) const
{
	if (this == &other)
	{
		return true;
	}

	return m_typeHash == other.m_typeHash;
}

/// <summary>
/// ���� TypeInfo�� �Ű������� ���� TypeInfo�� ��ӹ޴��� ��Ÿ�ӿ� �˻�
/// TypeInfo ������ ���� �θ� Ŭ������ TypeInfo�� ��ȸ�ϸ鼭
/// IsA �Լ��� ������ Ÿ������ Ȯ���Ѵ�.
/// �� �� �θ� == other TypeInfo�� ��� Ture ��ȯ
/// </summary>
bool TypeInfo::IsChildOf(const TypeInfo& other) const
{
	for (const TypeInfo* super = m_super; super != nullptr; super = super->GetSuper())
	{
		if (super->IsSameTypeInfo(other))
		{
			return true;
		}
	}

	return false;
}
