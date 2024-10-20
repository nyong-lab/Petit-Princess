#pragma once
#include "Component.h"

class IGraphics;
class CameraModule;

/// <summary>
/// 텍스트
/// 손서희
/// </summary>
class TextRender : public Component
{
	// Reflection
	GENERATE_CLASS_TYPE_INFO(TextRender)
		PROPERTY(m_text)
		PROPERTY(m_leftTop)
		PROPERTY(m_boxSize)
		PROPERTY(m_fontSize)
		PROPERTY(m_color)

	// Serialize
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TextRender, m_name, m_UID, m_fontSize)
	virtual void Serialize(nlohmann::json& json) override;
	virtual void Deserialize(const nlohmann::json& json) override;

public:
	TextRender();
	~TextRender();

public:
	virtual void Update(double dt) override;
	virtual void Finalize() override;
	virtual void Render(double dt);
	virtual void DebugRender();
	virtual std::shared_ptr<Component> Clone() const override;

	//너도 그냥 퍼블릭 해라
public:
	std::wstring m_text;
	Vector2 m_leftTop;
	Vector2 m_boxSize;
	Vector4 m_color;
	UINT m_fontSize;
};

