#include "GameEngine_pch.h"
#include "TypeIndex.h"

std::type_index GetTypeIndex(std::shared_ptr<Component> component)
{
	return std::type_index(typeid(component));
}

std::type_index MakeTypeIndexEmpty()
{
	return std::type_index(typeid(void));
}

bool IsTypeIndexEmpty(const std::type_index& typeIndex)
{
	// typeid(void)�� ����Ͽ� �� std::type_index ��ü�� ���Ͽ� ������ ����ִ� ����
	return typeIndex == std::type_index(typeid(void));
}

