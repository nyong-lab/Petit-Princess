#include "Reflection_pch.h"
#include "TypeInfoManager.h"

std::type_index TypeInfoManager::GetTypeIndex(const std::string& name) const
{
	auto iter = m_infomations.find(name);

	if (iter == m_infomations.end())
	{
		return std::type_index(typeid(void));
	}

	return iter->second.typeIndex;
}

TypeInfo* TypeInfoManager::GetTypeInfo(std::string name) const
{
	auto iter = m_infomations.find(name);

	if (iter == m_infomations.end())
	{
		return nullptr;
	}

	return iter->second.typeInfo;
}

TypeInfo* TypeInfoManager::GetTypeInfo(std::type_index index) const
{
	for(auto& information : m_infomations)
	{
		if (information.second.typeIndex == index)
		{
			return information.second.typeInfo;
		}
	}

	return nullptr;
}

const std::vector<TypeInfo*> TypeInfoManager::GetComponentTypeInfo() const
{
	std::vector<TypeInfo*> componentTypeInfo;
	for (auto& information : m_infomations)
	{
		if (information.second.typeInfo->GetTypeTag() == TypeTag::Component)
		{
			componentTypeInfo.push_back(information.second.typeInfo);
			return componentTypeInfo;
		}
	}
}

void TypeInfoManager::AddTypeInformation(const std::string& name, const TypeInfomation& info)
{
	auto iter = m_infomations.find(name);

	// 이미 추가한 타입을 추가함
	if (iter != m_infomations.end())
	{
		assert(!"이미 추가한 타입입니다. 리플렉션 오류");
		return;
	}
	
	m_infomations.insert({ name,info });
}
