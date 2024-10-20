#pragma once
typedef unsigned char BYTE;

class MaterialColor
{
public:
	MaterialColor();
	MaterialColor(unsigned int val);
	MaterialColor(BYTE r, BYTE g, BYTE b);
	MaterialColor(BYTE r, BYTE g, BYTE b, BYTE a);
	MaterialColor(const MaterialColor& src);

	MaterialColor& operator=(const MaterialColor& src);
	bool operator==(const MaterialColor& rhs) const;
	bool operator!=(const MaterialColor& rhs) const;

	constexpr BYTE GetR() const;
	void SetR(BYTE r);

	constexpr BYTE GetG() const;
	void SetG(BYTE g);

	constexpr BYTE GetB() const;
	void SetB(BYTE b);

	constexpr BYTE GetA() const;
	void SetA(BYTE a);

private:
	union
	{
		BYTE rgba[4];
		unsigned int color;
	};
};

namespace Colors
{
	const ::MaterialColor UnloadedTextureColor(100, 100, 100);
	const ::MaterialColor UnhandledTextureColor(250, 0, 0);
}
