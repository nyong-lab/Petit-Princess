#pragma once

// 클래스 내부의 멤버변수들에게 주어지는 카테고리
enum class PropertyTypeCategory
{
	Class, // Struct, Class 
	Primitive, // int, float, bool 등 기본 자료형
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