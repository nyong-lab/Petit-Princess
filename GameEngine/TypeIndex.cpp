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
	// typeid(void)를 사용하여 빈 std::type_index 객체와 비교하여 같으면 비어있는 상태
	return typeIndex == std::type_index(typeid(void));
}

