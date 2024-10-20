#pragma once

/// <summary>
/// Ÿ�Կ� �ش��ϴ� TypeIndex�� ��ȯ
/// </summary>
template <typename T>
std::type_index GetTypeIndex()
{
	return std::type_index(typeid(T));
}

std::type_index MakeTypeIndexEmpty();
bool IsTypeIndexEmpty(const std::type_index& typeIndex);