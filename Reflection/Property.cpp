#include "Reflection_pch.h"
#include "Property.h"

#include "TypeInfo.h"

Property::Property(TypeInfo& owner, const PropertyInitializer& initializer)
	: m_name(initializer.m_name)
	, m_type(initializer.m_type)
	, m_handler(initializer.m_handler)
	, m_propertyTypeCategory(initializer.m_typeCategory)
	, m_ownerType(initializer.m_ownerType)
{
	owner.AddProperty(this);
}
