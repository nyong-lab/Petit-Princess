#pragma once

// Ŭ���� ������ ��������鿡�� �־����� ī�װ�
enum class PropertyTypeCategory
{
	Class, // Struct, Class 
	Primitive, // int, float, bool �� �⺻ �ڷ���
	String,
	Vector, // std::vector
	Map, // std::map
	UnorderedMap, // std::unordered_map
	EnumClass, // enum class

	Owner,

	None // error
};

// T
enum class TypeTag
{
	Component,
	Default,
};