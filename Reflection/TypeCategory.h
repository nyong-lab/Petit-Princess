#pragma once

enum class TypeCategory
{
	Class, // Struct, Class 
	Primitive, // int, float, bool �� �⺻ �ڷ���
	String, // std::string
	Vector, // std::vector
	Map, // std::map
	UnorderedMap, // std::unordered_map
	EnumClass, // enum	class

	None // error
};