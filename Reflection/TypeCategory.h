#pragma once

enum class TypeCategory
{
	Class, // Struct, Class 
	Primitive, // int, float, bool 등 기본 자료형
	String, // std::string
	Vector, // std::vector
	Map, // std::map
	UnorderedMap, // std::unordered_map
	EnumClass, // enum	class

	None // error
};