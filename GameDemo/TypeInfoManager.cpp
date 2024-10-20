#include "GameDemo_pch.h"
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

void TypeInfoManager::AddTypeInformation(const std::string& name, const TypeInfomation& info)
{
	auto iter = m_infomations.find(name);

	// �̹� �߰��� Ÿ���� �߰���
	if (iter != m_infomations.end())
	{
		assert(!"�̹� �߰��� Ÿ���Դϴ�. ���÷��� ����");
		return;
	}
	
	m_infomations.insert({ name,info });
}
