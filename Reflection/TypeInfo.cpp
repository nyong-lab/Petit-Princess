#include "Reflection_pch.h"
#include "TypeInfo.h"


/// <summary>
/// 멤버 변수에 대한 정보를 가지고 있는 Property 클래스 추가
/// </summary>
void TypeInfo::AddProperty(Property* property)
{
	m_properties.emplace_back(property);
	m_propertyMap.emplace(property->GetName(), property);
}

/// <summary>
/// 키 값인 name을 매개변수로 받아서 propertyMap에 저장되어 있는지 찾고
/// 있으면 해당 property를 반환하고 없으면 nullptr 반환
/// </summary>
const Property* TypeInfo::GetProperty(const char* name) const
{
	auto iter = m_propertyMap.find(name);
	return (iter != std::end(m_propertyMap)) ? iter->second : nullptr;
}

/// <summary>
/// 현재 TypeInfo와 매개변수로 들어온 TypeInfo가 동일한 타입인지 검사
/// TypeInfo는 static 변수이기 때문에 주소검사로 확인이 가능하지만
/// 다른 dll의 같으 타입의 경우 처리가 안되기 때문에 typeid로 얻은 
/// 해시 값으로 한번 더 검사한다.
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
/// 현재 TypeInfo가 매개변수로 들어온 TypeInfo를 상속받는지 런타임에 검사
/// TypeInfo 생성시 얻어온 부모 클래스의 TypeInfo를 순회하면서
/// IsA 함수로 동일한 타입인지 확인한다.
/// 즉 내 부모 == other TypeInfo인 경우 Ture 반환
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
