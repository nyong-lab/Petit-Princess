#include "renderpch.h"
#include "MaterialColor.h"

MaterialColor::MaterialColor()
	:color(0)
{}

MaterialColor::MaterialColor(unsigned int val)
	: color(val)
{}

MaterialColor::MaterialColor(BYTE r, BYTE g, BYTE b)
	: MaterialColor(r, g, b, 255)
{
}

MaterialColor::MaterialColor(BYTE r, BYTE g, BYTE b, BYTE a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

MaterialColor::MaterialColor(const MaterialColor& src)
	:color(src.color)
{}

MaterialColor& MaterialColor::operator=(const MaterialColor& src)
{
	color = src.color;
	return *this;
}

bool MaterialColor::operator==(const MaterialColor& rhs) const
{
	return (color == rhs.color);
}

bool MaterialColor::operator!=(const MaterialColor& rhs) const
{
	return !(*this == rhs);
}

constexpr BYTE MaterialColor::GetR() const
{
	return rgba[0];
}
void MaterialColor::SetR(BYTE r)
{
	rgba[0] = r;
}

constexpr BYTE MaterialColor::GetG() const
{
	return rgba[1];
}
void MaterialColor::SetG(BYTE g)
{
	rgba[1] = g;
}

constexpr BYTE MaterialColor::GetB() const
{
	return rgba[2];
}
void MaterialColor::SetB(BYTE b)
{
	rgba[2] = b;
}

constexpr BYTE MaterialColor::GetA() const
{
	return rgba[3];
}
void MaterialColor::SetA(BYTE a)
{
	rgba[3] = a;
}
