#include "GameEngine_pch.h"
#include "TextRender.h"
#include "Entity.h"
#include "IGraphics.h"

#include <codecvt>

void TextRender::Serialize(nlohmann::json& json)
{
	to_json(json, *this);

	json["m_leftTop"] =
	{
		{"x", m_leftTop.x},
		{"y", m_leftTop.y},
	};

	json["m_boxSize"] =
	{
		{"x", m_boxSize.x},
		{"y", m_boxSize.y},
	};

	json["m_color"] =
	{
		{"x", m_color.x},
		{"y", m_color.y},
		{"z", m_color.z},
		{"w", m_color.w},
	};

	// json에 wstring 저장 이상하게 되는거 이제 알았네..
	// string에 한글 들어가면 오류나네? 
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, m_text.c_str(), static_cast<int>(m_text.size()), nullptr, 0, nullptr, nullptr);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, m_text.c_str(), static_cast<int>(m_text.size()), &str[0], size_needed, nullptr, nullptr);

	json["m_text"] = str;
}

void TextRender::Deserialize(const nlohmann::json& json)
{
	if (!json.is_null())
	{
		from_json(json, *this);

		if (json.contains("m_leftTop"))
		{
			m_leftTop.x = json["m_leftTop"]["x"];
			m_leftTop.y = json["m_leftTop"]["y"];
		}

		if (json.contains("m_color"))
		{
			m_color.x = json["m_color"]["x"];
			m_color.y = json["m_color"]["y"];
			m_color.z = json["m_color"]["z"];
			m_color.w = json["m_color"]["w"];
		}

		if (json.contains("m_boxSize"))
		{
			m_boxSize.x = json["m_boxSize"]["x"];
			m_boxSize.y = json["m_boxSize"]["y"];
		}

		std::string utf8Str = json["m_text"];


		int size_needed_utf16 = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), static_cast<int>(utf8Str.size()), nullptr, 0);
		std::wstring wStr(size_needed_utf16, 0);
		MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), static_cast<int>(utf8Str.size()), &wStr[0], size_needed_utf16);

		m_text = wStr;
	}
}

TextRender::TextRender()
	:m_leftTop(Vector2(800,500)), m_boxSize(Vector2(300,50)), m_fontSize(20), m_color(Vector4(1,1,1,1)),
	m_text(L"텍스트")
{

}

TextRender::~TextRender()
{

}

void TextRender::Update(double dt)
{

}

void TextRender::Finalize()
{

}

void TextRender::Render(double dt)
{
	auto graphics = m_owner.lock()->GetGraphics();
	
	graphics.lock()->DrawChar(
		m_text,
		m_leftTop,
		m_fontSize,
		m_color,
		m_boxSize);
}

void TextRender::DebugRender()
{

}

std::shared_ptr<Component> TextRender::Clone() const
{
	auto clone = std::make_shared<TextRender>();
	clone->m_text = m_text;
	clone->m_leftTop = m_leftTop;
	clone->m_boxSize = m_boxSize;
	clone->m_fontSize = m_fontSize;
	clone->m_color = m_color;

	return clone;
}
