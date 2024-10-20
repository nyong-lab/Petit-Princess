#pragma once

/// <summary>
/// 타입에 해당하는 TypeIndex를 반환
/// </summary>
template <typename T>
std::type_index GetTypeIndex()
{
	return std::type_index(typeid(T));
}

std::type_index MakeTypeIndexEmpty();
bool IsTypeIndexEmpty(const std::type_index& typeIndex);